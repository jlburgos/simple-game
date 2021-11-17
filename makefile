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

## CWD
## Need to determine absolute project directory path
ifeq ($(OS), Windows_NT)
CWD=$(shell powershell (Get-Location).path)
ROOT_DIR=$(shell powershell (Get-Item -Path $(CWD)).BaseName)
else
CWD=$(shell pwd)
ROOT_DIR=$(shell pwd | rev | cut -d'/' -f1 | rev)
endif

## Binary
BIN_NAME=$(ROOT_DIR)
BIN_DIR=bin

## Assets
RESOURCES=assets
IMG_ASSETS=$(wildcard $(RESOURCES)/test-imgs/*)
PRE_COMP_ASSETS=src/pre-compiled-assets

## Tools
IMG_TO_HPP=tools/img_to_hpp/bin/img_to_hpp
#TOOLS=$(wildcard tools/*)
TOOLS=$(wildcard tools/img_to_hpp)

######################################################################################################
######################################################################################################

## SDL Flags
SDL_FLAGS=\
	-l SDL2main \
	-l SDL2 \
	-l SDL2_image \
	-l SDL2_ttf \
	-I external-dep/SDL2/inc
ifeq ($(OS), Windows_NT)
SDL_FLAGS+=\
	-L external-dep/SDL2/lib
endif


WASM_SDL_FLAGS=\
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-s USE_SDL_TTF=2 \
	-s SDL2_IMAGE_FORMATS="[png,bmp,jpg]" \
	-s LLD_REPORT_UNDEFINED \
	-s WASM=1 \
	-flto \
	-v \
	--closure 1 \
	--minify 0 \
	--bind


## Note: Add the '-mwindows' option to remove the terminal pop-up when double-clicking the game.exe file
##       https://gcc.gnu.org/onlinedocs/gcc/x86-Windows-Options.html
WINDOWS_FLAGS=-mwindows

## OPTIMIZATION
## Note: The "Og" g++ option enables optimizations that do not interfere with debugging
##       But "em++" doesn't seem to like it :(
#OPTIMIZATION=-Og
OPTIMIZATION=-O2

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
	$(CXX_STD) \
	$(SDL_FLAGS) \
	$(CXX_COMPILER_FLAGS)

WASM_OPTS=\
	$(OPTIMIZATION) \
	$(CXX_STD) \
	$(WASM_SDL_FLAGS) \
	$(CXX_COMPILER_FLAGS)

######################################################################################################
######################################################################################################

default: tools assets
	$(CXX) src/*.cpp -o $(BIN_DIR)/$(BIN_NAME) $(OPTS)

wasm: tools assets
	docker run \
		--rm \
		--volume $(CWD):/$(ROOT_DIR) \
		emscripten/emsdk /bin/bash -c \
			"$(PIP) install requests && $(EMXX) /$(ROOT_DIR)/src/*.cpp -o /$(ROOT_DIR)/$(BIN_DIR)/$(BIN_NAME).html $(WASM_OPTS)"

.PHONY: $(IMG_ASSETS)
assets: $(IMG_ASSETS)
$(IMG_ASSETS):
	$(IMG_TO_HPP) $@ src/pre-compiled-assets/$(notdir $@).hpp

.PHONY: $(TOOLS)
tools: $(TOOLS)
$(TOOLS):
	$(MAKE) -C $@

all: tools img_assets default wasm