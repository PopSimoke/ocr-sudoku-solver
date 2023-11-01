#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <SDL2/SDL.h>

// Fonction pour obtenir la couleur d'un pixel dans une image
Uint32 getPixel(SDL_Surface *image, int x, int y);

// Fonction pour set la couleur d'un pixel dans une image
void setPixel(SDL_Surface *image, int x, int y, Uint32 newPixel);

#endif