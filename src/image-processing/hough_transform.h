#ifndef HOUGH_TRANSFORM_H
#define HOUGH_TRANSFORM_H

#include "pixel_operations.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    double rho;
    double theta;
} HoughLine;

HoughLine *HoughTransform(SDL_Surface *image, int *numLines);

#endif