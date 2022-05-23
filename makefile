######################################################################################################
######################################################################################################
## Simple makefile to compile SDL2 application
######################################################################################################
######################################################################################################

## Tools
CXX=g++
EMXX=em++
PIP=pip

## C++ standard
CXX_STD=-std=c++17

## Need to determine absolute project directory path
ifeq ($(OS), Windows_NT)
CWD := $(shell powershell '(Get-Location).path')
ROOT_DIR := $(shell powershell '(Get-Item -Path "'$(CWD)'").BaseName')
else
ROOT_DIR := $(shell pwd | rev | cut -d'/' -f1 | rev)
endif

## Directories
BIN_NAME  := $(ROOT_DIR)
SRC_DIR   = src
BIN_DIR   = bin
LOGS_DIR  = logs
BUILD_DIR = build
ASSET_DIR = assets

## Assets
#RESOURCES  = assets
#IMG_ASSETS = $(wildcard $(RESOURCES)/test-imgs/*)
#PRE_COMP_ASSETS = $(SRC_DIR)/pre-compiled-assets

## Tools
#IMG_TO_HPP=tools/img_to_hpp/bin/img_to_hpp
#TOOLS=$(wildcard tools/*)
#TOOLS=$(wildcard tools/img_to_hpp)

######################################################################################################
######################################################################################################

## Identify OS
## Note: https://stackoverflow.com/questions/714100/os-detecting-makefile
OS_SPECIFIC_FLAGS=
ifeq ($(OS),Windows_NT)
    OS_SPECIFIC_FLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        OS_SPECIFIC_FLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            OS_SPECIFIC_FLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            OS_SPECIFIC_FLAGS += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OS_SPECIFIC_FLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        OS_SPECIFIC_FLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        OS_SPECIFIC_FLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        OS_SPECIFIC_FLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        OS_SPECIFIC_FLAGS += -D ARM
    endif
endif

## OS Flags
## Notes: https://stackoverflow.com/questions/20673370/why-do-we-write-d-reentrant-while-compiling-c-code-using-threads
## Notes: https://gcc.gnu.org/onlinedocs/gcc/x86-Windows-Options.html
ifeq ($(OS), Windows_NT)
OS_SPECIFIC_FLAGS+=\
	-mwin32 \
	-mthreads \
	-mwindows
else
OS_SPECIFIC_FLAGS+=\
	-pthread
endif

## Make sure we compile for 64-bit
OS_SPECIFIC_FLAGS+=\
	-m64

## If manually set up SDL includes (.h files) and SDL libraries (.a & .la files), we grab those for windows
## Else for linux we get the flags from `pkg-config` (using `sdl-config` also works)
## Note: Grabbing 'release' builds from https://github.com/libsdl-org
## ** https://github.com/libsdl-org/SDL/releases
## ** https://github.com/libsdl-org/SDL_image/releases
## ** https://github.com/libsdl-org/SDL_ttf/releases
PLATFORM_VERSION = x86_64
EXTERNAL_SDL2_DEP=../MINGW-SDL2
ifeq ($(OS), Windows_NT)
	ifeq ("$(wildcard $(EXTERNAL_SDL2_DEP))","")
$(error "Failed to locate expected '$(EXTERNAL_SDL2_DEP)' directory containing SDL2 dependencies!")
	else
SDL_INCLUDES := $(shell powershell 'Get-ChildItem "include" -Directory -Path "$(EXTERNAL_SDL2_DEP)" -Recurse | Where {$$_.FullName -match "$(PLATFORM_VERSION)"} | Resolve-Path -Relative')
SDL_INCLUDES := $(subst \,/,$(addprefix -I, $(addsuffix /SDL2, $(SDL_INCLUDES))))
SDL_LIBRARIES := $(shell powershell 'Get-ChildItem "lib" -Directory -Path "$(EXTERNAL_SDL2_DEP)" -Recurse | Where {$$_.FullName -match "$(PLATFORM_VERSION)"} | Resolve-Path -Relative')
SDL_LIBRARIES := $(subst \,/,$(addprefix -L, $(SDL_LIBRARIES)))
SDL_FLAGS := $(SDL_INCLUDES) $(SDL_LIBRARIES)
	endif
else
SDL_FLAGS := $(shell pkg-config --cflags sdl2)
endif

## SDL Flags
SDL_LINKER_FLAGS=\
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf
ifeq ($(OS), Windows_NT)
SDL_LINKER_FLAGS += $(SDL_LIBRARIES)
else
SDL_LINKER_FLAGS := $(shell pkg-config --cflags --libs sdl2) $(SDL_LINKER_FLAGS)
endif

WASM_SDL_FLAGS=\
	-sUSE_SDL=2 \
	-sUSE_SDL_IMAGE=2 \
	-sUSE_SDL_TTF=2 \
	-sSDL2_IMAGE_FORMATS="[png,bmp,jpg]" \
	-sLLD_REPORT_UNDEFINED \
	-sWASM=1 \
	-v \
	--closure 1 \
	--minify 0 \
	--bind

## OPTIMIZATION
OPTIMIZATION_DEBUG=\
	-O0 \
	-g
OPTIMIZATION_RELEASE=\
	-O3

## Compiler flags to check "almost everything" because g++ doesn't have a "-Weverything-i-want" flag :P
## Notes: https://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings
CXX_COMPILER_FLAGS=\
	-Wall -Wextra -Werror -Wmaybe-uninitialized \
	-Wempty-body -Warray-bounds \
	-pedantic -pedantic-errors -Wcast-align \
	-Wcast-qual -Wconversion \
	-Wdisabled-optimization \
	-Wfloat-equal -Wlong-long \
	-Wimport  -Winit-self  -Winline \
	-Wmissing-field-initializers -Wmissing-format-attribute \
	-Wmissing-include-dirs -Wmissing-noreturn \
	-Wpacked -Wpointer-arith \
	-Wredundant-decls \
	-Wshadow -Wstack-protector \
	-Wstrict-aliasing=2 -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k \
	-Wswitch-enum \
	-Wvariadic-macros \
	-Wwrite-strings \
	-Wunreachable-code

ifeq ($(UNAME_S),Darwin)
	CXX_COMPILER_FLAGS := $(filter-out -Wmaybe-uninitialized,$(CXX_COMPILER_FLAGS))
	CXX_COMPILER_FLAGS += -Wuninitialized
endif

######################################################################################################
######################################################################################################

## Aggregated compiler flags
OPTS=\
	$(CXX_STD) \
	$(OS_SPECIFIC_FLAGS) \
	$(SDL_FLAGS) \
	$(CXX_COMPILER_FLAGS)

WASM_OPTS=\
	$(CXX_STD) \
	$(OS_SPECIFIC_FLAGS) \
	$(WASM_SDL_FLAGS) \
	$(CXX_COMPILER_FLAGS)

######################################################################################################
######################################################################################################

## Source files and output location of compiled object files
## Note: OBJS_O_DEBUG and OBJS_O_RELEASE are used for matching object file build targets
OBJS_SRC  := $(wildcard src/**/*.cpp src/*.cpp)
OBJS_O_DEBUG    := $(addprefix $(BUILD_DIR)/, $(OBJS_SRC:%.cpp=%.debug.o))
OBJS_O_RELEASE  := $(addprefix $(BUILD_DIR)/, $(OBJS_SRC:%.cpp=%.release.o))

## Object file output directories
## Note: 'DIRS_O' is calculated this way to get a unique list of sub-directories whereas doing $(dir $(OBJS_O)) would generate duplicates
DIRS_O = build/src
ifeq ($(OS), Windows_NT)
DIRS_O    += $(addprefix $(BUILD_DIR)/src, $(shell powershell 'Get-ChildItem -Path "$(SRC_DIR)" -Directory -Recurse | Resolve-Path -Relative'))
DIRS_O    := $(subst \,/,$(DIRS_O))
else
DIRS_O    += $(addprefix $(BUILD_DIR)/src, $(shell find $(SRC_DIR)/* -type d))
endif

######################################################################################################
######################################################################################################

ifeq ($(OS), Windows_NT)
WINDRES = windres
ICO_RC  := $(shell powershell 'Get-ChildItem -Path "$(ASSET_DIR)" -Filter "ico.rc" -Recurse | Resolve-Path -Relative')
ICO_RC  := $(subst \,/,$(ICO_RC))
ICO_O   := $(addprefix $(BUILD_DIR)/,$(subst .rc,.o,$(ICO_RC)))
ICO_DIR := $(dir $(ICO_O))
DIRS_O  += $(ICO_DIR)
endif

######################################################################################################
######################################################################################################

## Set up final targets
## Note: https://semver.org/
BUILD_VERSION ?= 0.0.0
BIN_DEBUG   := $(BIN_DIR)/debug/$(BIN_NAME)-$(BUILD_VERSION)-debug
BIN_RELEASE := $(BIN_DIR)/release/$(BIN_NAME)-$(BUILD_VERSION)-release

## Set up final binary name
ifeq ($(OS), Windows_NT)
BIN_DEBUG   := $(BIN_DEBUG).exe
BIN_RELEASE := $(BIN_RELEASE).exe
endif

## Bin directory and log directory paths
DIRS_BIN_DEBUG   := $(dir $(BIN_DEBUG))
DIRS_BIN_RELEASE := $(dir $(BIN_RELEASE))

## DLL files and output location where they will be copied
ifeq ($(OS), Windows_NT)
DLLS_SRC  := $(shell powershell 'Get-ChildItem "*.dll" -Path "$(EXTERNAL_SDL2_DEP)" -Recurse | Where {$$_.DirectoryName -match "$(PLATFORM_VERSION)"} | Resolve-Path -Relative')
DLLS_SRC  := $(subst \,/,$(DLLS_SRC))
DLLS_DEBUG   := $(addprefix $(DIRS_BIN_DEBUG)/, $(notdir $(DLLS_SRC)))
DLLS_RELEASE := $(addprefix $(DIRS_BIN_RELEASE)/, $(notdir $(DLLS_SRC)))
endif

######################################################################################################
######################################################################################################

## Generate build directory structure
## Note: The '$$output_sink' variable is - as the name suggests - a 'sink' to contain the output of running 'mkdir' in powershell, which
##       returns a large string that we want to ignore.
ALL_REQUIRED_DIRS = $(LOGS_DIR) $(DIRS_O) $(BIN_DIR) $(DIRS_BIN_DEBUG) $(DIRS_BIN_RELEASE)
ifeq ($(OS), Windows_NT)
$(foreach dir, $(ALL_REQUIRED_DIRS), $(powershell 'if (-not (Test-Path -Path "$(dir)" -PathType Container)) { $$output_sink = New-Item -Path "$(dir)" -ItemType Directory }'))
else
$(foreach dir, $(ALL_REQUIRED_DIRS), $(shell mkdir -p '$(dir)'))
endif

## Copy all the DLLs for Windows build
ifeq ($(OS), Windows_NT)
$(foreach dll,$(DLLS_SRC),$(shell powershell 'Copy-Item -Path "$(dll)" -Destination "$(dir $(BIN_DEBUG))"'))
$(foreach dll,$(DLLS_SRC),$(shell powershell 'Copy-Item -Path "$(dll)" -Destination "$(dir $(BIN_RELEASE))"'))
endif

######################################################################################################
######################################################################################################

MAKEFLAGS += -j$(NPROCS)

## User targets
debug-build: $(BIN_DEBUG)
release-build: $(BIN_RELEASE)
all: $(BIN_DEBUG) $(BIN_RELEASE)

## Compile final binary
$(BIN_DEBUG): $(OBJS_O_DEBUG) $(ICO_O)
ifeq ($(OS), Windows_NT)
  ## Set Windows executable thumbnail icon
	@$(WINDRES) $(ICO_RC) $(ICO_O)
endif
	$(CXX) -o "$@" $(ICO_O) $(OBJS_O_DEBUG) $(OPTIMIZATION_DEBUG) $(SDL_LINKER_FLAGS)

$(BIN_RELEASE): $(OBJS_O_RELEASE) $(ICO_O)
ifeq ($(OS), Windows_NT)
  ## Set Windows executable thumbnail icon
	@$(WINDRES) $(ICO_RC) $(ICO_O)
endif
	$(CXX) -o "$@" $(ICO_O) $(OBJS_O_RELEASE) $(OPTIMIZATION_RELEASE) $(SDL_LINKER_FLAGS)

## Compile individual CPP object files (debug & release)
$(BUILD_DIR)/%.debug.o: %.cpp
	$(CXX) -c -o '$@' '$<' $(OPTIMIZATION_DEBUG) $(OPTS)

$(BUILD_DIR)/%.release.o: %.cpp
	$(CXX) -c -o '$@' '$<' $(OPTIMIZATION_RELEASE) $(OPTS)

## Compile web-assembly version using docker container
## Note: Disabling compile pipeline experiments for now
#wasm:
#	docker run \
#		--rm \
#		--volume $(ROOT_DIR):/$(BUILD_DIR) \
#		emscripten/emsdk /bin/bash -c \
#			"$(PIP) install requests && $(EMXX) /$(BUILD_DIR)/$(SRC_DIR)/*.cpp -o /$(BUILD_DIR)/$(BIN_DIR)/$(BIN_NAME).html $(WASM_OPTS)"

## Clean build artifacts
## Note: Will need to rerun 'make init' to recreate directories and dependencies
PHONY += clean
clean:
ifeq ($(OS),Windows_NT)
	@powershell 'if (Test-Path -Path "$(BIN_DIR)" -PathType Container) { Remove-Item -Path "$(BIN_DIR)" -Recurse }'
	@powershell 'if (Test-Path -Path "$(BUILD_DIR)" -PathType Container) { Remove-Item -Path "$(BUILD_DIR)" -Recurse }'
	@powershell 'if (Test-Path -Path "$(LOGS_DIR)" -PathType Container) { Remove-Item -Path "$(LOGS_DIR)" -Recurse }'
else
	@$(RM) -r '$(BIN_DIR)'
	@$(RM) -r '$(BUILD_DIR)'
	@$(RM) -r '$(LOGS_DIR)'
endif

## Set up .PHONY
.PHONY := $(PHONY)
