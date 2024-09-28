# Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

EXE = fork sort exec clock

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)

# END