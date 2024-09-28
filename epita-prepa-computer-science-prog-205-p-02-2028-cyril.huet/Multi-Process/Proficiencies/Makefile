# Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

EXE = chg_case manager

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)

# END