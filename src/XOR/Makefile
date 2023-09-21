# Makefile

CC = gcc -fsanitize=address
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror -O1
LDFLAGS =
LDLIBS =

SRC = main.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

main: ${SRC}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
