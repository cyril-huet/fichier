# To use this Makefile, simply execute the 'make' command with the name of the file to compile.
# For example: 'make ingredients'
# DO NOT modify this Makefile, it will be used to test your code!

# Compilation flags
CFLAGS = -Wall -Wextra -std=c99 -fsanitize=address -g

.PHONY: clean help

help:
	@printf "usage:   make <filename>\nexample: make ingredients\n"

all: ingredients swamp_packing donkey_echo dragon_treasure

clean:
	rm ingredients swamp_packing donkey_echo dragon_treasure
