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
CWD := $(shell powershell (Get-Location).path)
ROOT_DIR := $(shell powershell (Get-Item -Path "'$(CWD)'").BaseName)
else
ROOT_DIR := $(shell pwd | rev | cut -d'/' -f1 | rev)
endif

## Directories
BIN_NAME  = $(ROOT_DIR)
SRC_DIR   = src
BIN_DIR   = bin
BUILD_DIR = build

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
#WINDOWS_FLAGS=-mwindows

## OS Flags
## Notes: https://stackoverflow.com/questions/20673370/why-do-we-write-d-reentrant-while-compiling-c-code-using-threads
OS_SPECIFIC_FLAGS=
ifneq ($(OS), Windows_NT)
OS_SPECIFIC_FLAGS=\
	-pthread
endif

## SDL Flags
SDL_FLAGS=\
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-Iexternal-dep/SDL2/inc
ifeq ($(OS), Windows_NT)
SDL_FLAGS+=\
	-Lexternal-dep/SDL2/lib
## Note: Commented this out since I want to make sure the same SDL2 deps are used for both WIN/Linux
#else
#SDL_FLAGS+=\
#	$(shell sdl2-config --cflags)
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
## Note: The "Og" g++ option enables optimizations that do not interfere with debugging
##       But "em++" doesn't seem to like it :(
OPTIMIZATION=-Og
#OPTIMIZATION=-O2

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
	$(OPTIMIZATION) \
	$(OS_SPECIFIC_FLAGS) \
	$(CXX_STD) \
	$(SDL_FLAGS) \
	$(CXX_COMPILER_FLAGS)

WASM_OPTS=\
	$(OPTIMIZATION) \
	$(OS_SPECIFIC_FLAGS) \
	$(CXX_STD) \
	$(WASM_SDL_FLAGS) \
	$(CXX_COMPILER_FLAGS)

######################################################################################################
######################################################################################################

DIRS      := $(SRC_DIR) $(SRC_DIR)/util
OBJS_SRC  := $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.cpp))
OBJS_O    := $(addprefix $(BUILD_DIR)/, $(OBJS_SRC:%.cpp=%.o))
DIRS_O    := $(addprefix $(BUILD_DIR)/, $(DIRS))

######################################################################################################
######################################################################################################

## Top-level rule to create build directory structure and compile the basic program
MAKEFLAGS += -j$(NPROCS)
all: $(DIRS_O)
	$(MAKE) $(BIN_NAME)

## Generate build directory structure
## Note: The '$$output_sink' variable is - as the name suggests - a 'sink' to contain the output of running 'mkdir' in powershell, which
##       returns a large string that we want to ignore.
$(DIRS_O):
ifeq ($(OS), Windows_NT)
	powershell if (-not (Test-Path -Path '$@' -PathType Container)) { $$output_sink = mkdir '$@' }
else
	mkdir -p '$@'
endif

## Compile CPP object files
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(OPTS)

## Compile final binary
$(BIN_NAME): $(OBJS_O)
	$(CXX) -o $(BIN_DIR)/$@ $(OBJS_O) $(OPTS)

## Compile web-assembly version using docker container
## Note: Disabling compile pipeline experiments for now
#wasm:
#	docker run \
#		--rm \
#		--volume $(ROOT_DIR):/$(BUILD_DIR) \
#		emscripten/emsdk /bin/bash -c \
#			"$(PIP) install requests && $(EMXX) /$(BUILD_DIR)/$(SRC_DIR)/*.cpp -o /$(BUILD_DIR)/$(BIN_DIR)/$(BIN_NAME).html $(WASM_OPTS)"

## Clean build artifacts
## Note: WIN compile adds the '.exe' file extension, so need to manually add it here when cleaning up
clean:
ifeq ($(OS),Windows_NT)
	powershell if (Test-Path -Path '$(BUILD_DIR)' -PathType Container) { Remove-Item -Path '$(BUILD_DIR)' -recurse }
	powershell if (Test-Path -Path '$(BIN_DIR)/$(BIN_NAME).exe' -PathType Leaf) { Remove-Item -Path '$(BIN_DIR)/$(BIN_NAME).exe' }
else
	$(RM) -r $(BUILD_DIR)
	$(RM) $(BIN_DIR)/$(BIN_NAME)
endif
