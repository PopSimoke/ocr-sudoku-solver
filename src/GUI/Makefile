# Makefile

CC = gcc  

CPPFLAGS = `pkg-config --cflags gtk+-3.0` -MMD -D__NO_INLINE__
CFLAGS = -std=c99 -O1  `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl2 SDL2_image` 
LDLIBS = `pkg-config --libs gtk+-3.0` -rdynamic `pkg-config --libs sdl2 SDL2_image` -lm

SRCGUI = ocr_gui.c 
SRCIMAGE = ../image_processing/preprocess.c  ../image_processing/image_processing.c ../image_processing/pixel_operations.c ../image_processing/square_detection.c ../image_processing/utils.c ../image_processing/image_transformations.c 
SRCNN = ../neural_network/number_recognition.c ../neural_network/neural_network.c ../neural_network/neural_network_utils.c ../neural_network/activations.c ../neural_network/matrix.c 
SRCSOLVER = ../sudoku_solver/solver_dec/solver_dec.c
SRC = ${SRCGUI} ${SRCIMAGE} ${SRCNN} ${SRCSOLVER}
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