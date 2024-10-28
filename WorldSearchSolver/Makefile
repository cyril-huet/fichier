CC = gcc
CFLAGS = `sdl2-config --cflags` -g
LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf

all: main

main: src/main.o src/grid.o src/image.o
	$(CC) -o main src/main.o src/grid.o src/image.o $(LIBS)

clean:
	rm -f src/*.o main

