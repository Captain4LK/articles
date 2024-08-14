#!/bin/sh

gcc $1 -Wall -Wextra -Wshadow -std=c99 -Wpedantic -Wconversion -Wno-sign-conversion -Og -g3 -fsanitize=undefined,address -lSDL2 -DRCG_EXAMPLE -DRCG_IMPLEMENTATION
