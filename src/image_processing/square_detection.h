#ifndef SQUARE_DETECTION_H
#define SQUARE_DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "pixel_operations.h"

/**
 * @brief Save individual cells of a Sudoku image in a separate directory.
 *
 * This function divides the input Sudoku image into individual cells and saves each cell
 * as a separate PNG file in a directory called "saved_images."
 *
 * @param sudokuImage The input Sudoku image.
 */
void saveSquares(SDL_Surface *sudokuImage, Color mostFrequentColor);

#endif
