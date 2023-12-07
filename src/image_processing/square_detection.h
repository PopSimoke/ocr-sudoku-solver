#ifndef SQUARE_DETECTION_H
#define SQUARE_DETECTION_H

#include "image_processing.h"
#include "pixel_operations.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * @brief Save individual cells of a Sudoku image in a separate directory.
 *
 * This function divides the input Sudoku image into individual cells and saves
 * each cell as a separate PNG file in a directory called "saved_images."
 *
 * @param sudokuImage The input Sudoku image.
 */
void saveSquares(SDL_Surface *sudokuImage, Color mostFrequentColor);
int editSquare(SDL_Surface *square, Color mostFrequentColor);

/**
 * @brief Resize a given image.
 *
 * This function taks an image and resize it to the given with and height
 *
 * @param source The source image to resize.
 * @param newWidth The new width of the image.
 * @param newHeight The new height of the image.
 *
 * @return Return a new resized SDL_Surface.
 */
SDL_Surface *resizeImage(SDL_Surface *source, int newWidth, int newHeight);

#endif
