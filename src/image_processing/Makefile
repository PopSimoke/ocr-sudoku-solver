# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -O1 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = main.c preprocess.c hough_transform.c image_processing.c pixel_operations.c square_detection.c utils.c image_transformations.c ../neural_network/matrix.c ../neural_network/neural_network_utils.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

main: ${SRC}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
