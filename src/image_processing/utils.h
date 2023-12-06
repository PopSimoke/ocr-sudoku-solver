#ifndef UTILS_H
#define UTILS_H

#include "pixel_operations.h"
#include "image_processing.h"
#include <SDL2/SDL.h>

int arrayMaxIndex(int *array, int size);
int arrayMaxIndexAfter(int *array, int size, int currentIndex);
SDL_Surface* rotateImage(double angle, SDL_Surface* image);
SDL_Surface *copySurface(SDL_Surface *surface, Point *corners, Color mostFrequentColor);

#endif