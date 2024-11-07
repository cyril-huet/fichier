CC = gcc
CFLAGS = -Wall -Wextra

all: test

test: test.o malloc.o
test.o: test.c malloc.h
malloc.o: malloc.c malloc.h

.PHONY: clean

clean: