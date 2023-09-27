# Makefile

CC = gcc -fsanitize=address -g
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror -O3 -std=c99
LDFLAGS = -lm
LDLIBS =

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
