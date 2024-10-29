CC = gcc
CFLAGS = -Wall -Werror -g

separtion: separtion.c
	$(CC) $(CFLAGS) -o separtion separtion.c -lSDL2 -lSDL2_image -lm

.PHONY: clean
clean:
	rm -f separtion

