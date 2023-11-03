#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Fonction pour obtenir la couleur d'un pixel dans une image
Uint32 getPixel(SDL_Surface *image, int x, int y);

// Fonction pour set la couleur d'un pixel dans une image
void setPixel(SDL_Surface *image, int x, int y, Uint32 newPixel);

// Compare deux valeurs d'intensité.
int compareIntensity(const void *a, const void *b);

// Renvoie la valeur d'intensité au point (x, y) de l'image.
Uint8 getIntensity(SDL_Surface *image, int x, int y);

#endif