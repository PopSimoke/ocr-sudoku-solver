# Makefile

CC = gcc -fsanitize=address -g
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror -O3 -std=c99
LDFLAGS = -lm
LDLIBS =

SRC = number_recognition.c xor.c neural_network.c neural_network_utils.c activations.c matrix.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

all: xor

xor: xor.o neural_network.o neural_network_utils.o activations.o matrix.o

number_recognition: number_recognition.o neural_network.o neural_network_utils.o activations.o matrix.o

.PHONY: clean

-include: ${DEP}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${EXE}

# END
