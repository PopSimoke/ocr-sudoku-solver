# Makefile

CC = gcc -fsanitize=address
CPPFLAGS =
CFLAGS = -Wall -Wextra  -O1
LDFLAGS =
LDLIBS =

SRC = solver_hexa.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

solver_hexa: ${SRC}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
