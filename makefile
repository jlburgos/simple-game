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

## Note: Add the '-mwindows' option to remove the terminal pop-up when double-clicking the game.exe file
##       https://gcc.gnu.org/onlinedocs/gcc/x86-Windows-Options.html
WINDOWS_RELEASE_FLAGS=-mwindows

## OS Flags
## Notes: https://stackoverflow.com/questions/20673370/why-do-we-write-d-reentrant-while-compiling-c-code-using-threads
ifeq ($(OS), Windows_NT)
OS_SPECIFIC_FLAGS=\
	-mwin32 \
	-mthreads
else
OS_SPECIFIC_FLAGS=\
	-pthread
endif

## Make sure we compile for 64-bit
OS_SPECIFIC_FLAGS+=\
	-m64

## SDL Flags
SDL_FLAGS=\
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf

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
INCLUDES := $(shell powershell 'Get-ChildItem "include" -Directory -Path "$(EXTERNAL_SDL2_DEP)" -Recurse | Where {$$_.FullName -match "$(PLATFORM_VERSION)"} | Resolve-Path -Relative')
INCLUDES := $(subst \,/,$(addprefix -I, $(addsuffix /SDL2, $(INCLUDES))))
LIBRARIES := $(shell powershell 'Get-ChildItem "lib" -Directory -Path "$(EXTERNAL_SDL2_DEP)" -Recurse | Where {$$_.FullName -match "$(PLATFORM_VERSION)"} | Resolve-Path -Relative')
LIBRARIES := $(subst \,/,$(addprefix -L, $(LIBRARIES)))
SDL_FLAGS+=$(INCLUDES) $(LIBRARIES)
	endif
else
SDL_FLAGS+=$(shell pkg-config --cflags --libs sdl2)
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
	-O3 \
	-s

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

MAKEFLAGS += -j$(NPROCS)

## User targets
debug: $(BIN_DEBUG)
release: $(BIN_RELEASE)
all: $(BIN_DEBUG) $(BIN_RELEASE)

## Compile final binary
$(BIN_DEBUG): $(LOGS_DIR) $(DIRS_O) $(DLLS_DEBUG) $(OBJS_O_DEBUG) $(ICO_O)
	$(info ------------------------------------------------------)
	$(info Building final executable $(BIN_DEBUG) ...)
	$(info ------------------------------------------------------)
	$(CXX) -o "$@" $(ICO_O) $(OBJS_O_DEBUG) $(OPTIMIZATION_DEBUG) $(OPTS)
$(BIN_RELEASE): $(LOGS_DIR) $(DIRS_O) $(DLLS_RELEASE) $(OBJS_O_RELEASE) $(ICO_O)
	$(info ------------------------------------------------------)
	$(info Building final executable $(BIN_RELEASE) ...)
	$(info ------------------------------------------------------)
	$(CXX) -o "$@" $(ICO_O) $(OBJS_O_RELEASE) $(OPTIMIZATION_RELEASE) $(OPTS)

## Generate build directory structure
## Note: The '$$output_sink' variable is - as the name suggests - a 'sink' to contain the output of running 'mkdir' in powershell, which
##       returns a large string that we want to ignore.
$(LOGS_DIR) $(DIRS_O) $(DIRS_BIN_DEBUG) $(DIRS_BIN_RELEASE):
ifeq ($(OS), Windows_NT)
	powershell 'if (-not (Test-Path -Path "$@" -PathType Container)) { $$output_sink = New-Item -Path "$@" -ItemType Directory }'
else
	mkdir -p '$@'
endif

## Set Windows executable thumbnail icon
$(ICO_O): $(ICO_DIR)
	$(WINDRES) $(ICO_RC) $(ICO_O)

## Compile CPP object files
$(BUILD_DIR)/%.debug.o: %.cpp
	$(CXX) -c -o '$@' '$<' $(OPTIMIZATION_DEBUG) $(OPTS)
$(BUILD_DIR)/%.release.o: %.cpp
	$(CXX) -c -o '$@' '$<' $(OPTIMIZATION_RELEASE) $(OPTS)

## Copy all the DLLs for Windows build
$(DLLS_DEBUG): $(DIRS_BIN_DEBUG)
ifeq ($(OS), Windows_NT)
	$(foreach dll,$(DLLS_SRC),$(shell powershell 'Copy-Item -Path "$(dll)" -Destination "$(dir $(BIN_DEBUG))"'))
endif
$(DLLS_RELEASE): $(DIRS_BIN_RELEASE)
ifeq ($(OS), Windows_NT)
	$(foreach dll,$(DLLS_SRC),$(shell powershell 'Copy-Item -Path "$(dll)" -Destination "$(dir $(BIN_RELEASE))"'))
endif

## Compile web-assembly version using docker container
## Note: Disabling compile pipeline experiments for now
#wasm:
#	docker run \
#		--rm \
#		--volume $(ROOT_DIR):/$(BUILD_DIR) \
#		emscripten/emsdk /bin/bash -c \
#			"$(PIP) install requests && $(EMXX) /$(BUILD_DIR)/$(SRC_DIR)/*.cpp -o /$(BUILD_DIR)/$(BIN_DIR)/$(BIN_NAME).html $(WASM_OPTS)"

## Clean build artifacts
PHONY += clean
clean:
ifeq ($(OS),Windows_NT)
	powershell 'if (Test-Path -Path "$(BIN_DIR)" -PathType Container) { Remove-Item -Path "$(BIN_DIR)" -Recurse }'
	powershell 'if (Test-Path -Path "$(BUILD_DIR)" -PathType Container) { Remove-Item -Path "$(BUILD_DIR)" -Recurse }'
	powershell 'if (Test-Path -Path "$(LOGS_DIR)" -PathType Container) { Remove-Item -Path "$(LOGS_DIR)" -Recurse }'
else
	$(RM) -r '$(BIN_DIR)'
	$(RM) -r '$(BUILD_DIR)'
	$(RM) -r '$(LOGS_DIR)'
endif

## Set up .PHONY
.PHONY := $(PHONY)
