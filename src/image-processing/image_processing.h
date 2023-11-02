#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "pixel_operations.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface *convertToGrayscale(SDL_Surface *image);
void applyMedianFilter(SDL_Surface *image, int kernelSize);
void binarizeImage(SDL_Surface *grayImage, Uint8 threshold);
void invertColors(SDL_Surface *image);
void applyAverageFilter(SDL_Surface *image, int kernelSize);
void applySobelFilter(SDL_Surface *image);
void applyCannyFilter(SDL_Surface *image, Uint8 lowThreshold, Uint8 highThreshold);
void applyDilation(SDL_Surface *image, int kernelSize, int iterations);
void applyErosion(SDL_Surface *image, int kernelSize, int iterations);


#endif