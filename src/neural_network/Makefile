# Makefile

CC = gcc -fsanitize=address -g
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror -O3 -std=c99
LDFLAGS = -lm
LDLIBS =

SRC = xor.c neural_network.c neural_network_utils.c activations.c matrix.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

xor: ${SRC}

all: xor

.PHONY: clean

-include: ${DEP}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${EXE}

# END
