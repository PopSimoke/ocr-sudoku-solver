# Makefile

CC = gcc -fsanitize=address -g
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror -O1 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = main.c hough_transform.c image_processing.c pixel_operations.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

main: ${SRC}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
