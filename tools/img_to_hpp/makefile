######################################################################################################
######################################################################################################
## Simple makefile
######################################################################################################
######################################################################################################

## Compiler + Package Manager
CXX=g++

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

######################################################################################################
######################################################################################################

## OPTIMIZATION
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
	$(CXX_COMPILER_FLAGS)

######################################################################################################
######################################################################################################

default:
	$(CXX) src/*.cpp -o $(BIN_DIR)/$(BIN_NAME) $(OPTS)
