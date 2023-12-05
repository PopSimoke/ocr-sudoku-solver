#ifndef UTILS_H
#define UTILS_H
#include <SDL2/SDL.h>
int arrayMaxIndex(int *array, int size);
int arrayMaxIndexAfter(int *array, int size, int currentIndex);
SDL_Surface* rotateImage(double angle, SDL_Surface* image);
#endif