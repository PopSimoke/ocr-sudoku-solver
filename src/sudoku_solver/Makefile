# Makefile

CC = gcc -fsanitize=address
CPPFLAGS =
CFLAGS = -Wall -Wextra  -O1
LDFLAGS =
LDLIBS =

SRC = solver.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

solver: ${SRC}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END