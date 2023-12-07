# Makefile

CC = gcc -fsanitize=address -g
CPPFLAGS =
CFLAGS = -O1 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = solver_dec.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

solver_dec: ${SRC}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END

