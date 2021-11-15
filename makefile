######################################################################################################
######################################################################################################
## Simple makefile to compile SDL2 application
######################################################################################################
######################################################################################################

## Compiler
CXX=g++

## C++ standard
CXX_STD=-std=c++17

## Binary
BIN_NAME=game
BIN_DIR=bin

## Assets
RESOURCES=resources
IMG_ASSETS=$(RESOURCES)/imgs
PRE_COMP_ASSETS=src/pre-compiled-assets

######################################################################################################
######################################################################################################

## SDL Flags
SDL_FLAGS=\
	-I src/external-dep/SDL2/inc \
	-L src/external-dep/SDL2/lib \
	-l SDL2main \
	-l SDL2 \
	-l SDL2_image \
	-l SDL2_ttf

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

## Need to determine absolute project directory path
ifeq ($(OS), Windows_NT)
	CWD=$(shell powershell (pwd).path)
else
	CWD=$(shell pwd)
endif

######################################################################################################
######################################################################################################

default:
	$(CXX) src/*.cpp -o $(BIN_DIR)/$(BIN_NAME) $(OPTS)

assets:
	## TODO: Write loop that handles xxd.exe

wasm:
	docker run \
		--rm \
		--name simple-game-wasm-build \
		--volume $(CWD):/simple-game \
		emscripten/emsdk /bin/bash -c \
			'pip install requests && em++ $(WASM_OPTS) /simple-game/src/*.cpp -o /simple-game/bin/game.html'

clean:
	$(RM) $(BIN_DIR)
