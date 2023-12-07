#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "hough_transform.h"
#include "image_processing.h"
#include "pixel_operations.h"
#include "square_detection.h"
#include "utils.h"
#include "image_transformations.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Surface *preprocessImage(SDL_Surface *image, Color *mostFrequentColor);

#endif