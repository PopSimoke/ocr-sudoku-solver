# Makefile

CC = gcc -fsanitize=address -g

CPPFLAGS = `pkg-config --cflags gtk+-3.0` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl2 SDL2_image` 
LDLFLAGS = -lm -lpthread 
LDLIBS = `pkg-config --libs gtk+-3.0` -rdynamic `pkg-config --libs sdl2 SDL2_image`

SRC = ocr_gui.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

ocr_gui: ${SRC}

all: ocr_gui

.PHONY: clean

-include: ${DEP}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${EXE}

# END
