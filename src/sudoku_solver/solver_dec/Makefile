# Makefile

CC = gcc -fsanitize=address
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror -O1
LDFLAGS =
LDLIBS =

SRC = solver_dec.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

solver: ${SRC}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END

