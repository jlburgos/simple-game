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

## Binary
BIN_NAME  = $(ROOT_DIR)
BIN_DIR   = bin
BUILD_DIR = build

## Assets
#RESOURCES  = assets
#IMG_ASSETS = $(wildcard $(RESOURCES)/test-imgs/*)
#PRE_COMP_ASSETS = src/pre-compiled-assets

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
	-lpthread \
	-D_REENTRANT
endif

## SDL Flags
SDL_FLAGS=\
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf
ifeq ($(OS), Windows_NT)
SDL_FLAGS+=\
	-Iexternal-dep/SDL2/inc \
	-Lexternal-dep/SDL2/lib
else
SDL_FLAGS+=\
	$(shell sdl2-config --cflags)
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
	-Wall \
	-pedantic \
	-pedantic-errors -Wextra -Wcast-align \
	-Wcast-qual -Wconversion \
	-Wdisabled-optimization \
	-Werror -Wfloat-equal -Wformat=2 \
	-Wformat-nonliteral -Wformat-security  \
	-Wformat-y2k \
	-Wimport  -Winit-self  -Winline \
	-Wlong-long \
	-Wmissing-field-initializers -Wmissing-format-attribute \
	-Wmissing-include-dirs -Wmissing-noreturn \
	-Wpacked -Wpointer-arith \
	-Wredundant-decls \
	-Wshadow -Wstack-protector \
	-Wstrict-aliasing=2 \
	-Wswitch-enum \
	-Wvariadic-macros \
	-Wwrite-strings \
	-Wunreachable-code -Wunused -Wunused-parameter

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

## Setting this when in windows creates race condition with the `powershell (... { mkdir })` portion
ifneq ($(OS), Windows_NT)
MAKEFLAGS += -j$(NPROCS)
endif

OBJS_SRC  := $(wildcard src/util/*.cpp src/*.cpp)
OBJS_O    += $(addprefix $(BUILD_DIR)/, $(OBJS_SRC:%.cpp=%.o))

######################################################################################################
######################################################################################################

## top-level rule to create the basic program
all: $(BIN_NAME)

clean:
ifeq ($(OS),Windows_NT)
	powershell Remove-Item -path $(BUILD_DIR) -recurse
else
	$(RM) -r $(BUILD_DIR)
endif

$(BIN_NAME): $(OBJS_O)
	$(CXX) -o $(BIN_DIR)/$@ $(OBJS_O) $(OPTS)

$(BUILD_DIR)/%.o: %.cpp
ifeq ($(OS), Windows_NT)
	powershell if (-not (Test-Path -Path '$(@D)' -PathType Container)) { $$output_sink = mkdir -p '$(@D)' }
else
	mkdir -p '$(@D)'
endif
	$(CXX) $(OPTS) -c -o $@ $<

## Note: Disabling compile pipeline experiments for now
#wasm: #tools assets
#	docker run \
#		--rm \
#		--volume $(ROOT_DIR):/$(ROOT_DIR) \
#		emscripten/emsdk /bin/bash -c \
#			"$(PIP) install requests && $(EMXX) /$(ROOT_DIR)/src/*.cpp -o /$(ROOT_DIR)/$(BIN_DIR)/$(BIN_NAME).html $(WASM_OPTS)"
