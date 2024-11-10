# To use this Makefile, simply execute the 'make' command with the name of the file to compile.
# For example: 'make magic_mirror'
# DO NOT modify this Makefile, it will be used to test your code!

# Compilation flags
CFLAGS = -Wall -Wextra -std=c99 -fsanitize=address -g

.PHONY: clean help

help:
	@printf "usage:   make <filename>\nexample: make magic_mirror\n"

all: magic_mirror secret_message

clean:
	rm magic_mirror secret_message
