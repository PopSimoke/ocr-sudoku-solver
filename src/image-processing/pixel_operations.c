#include "pixel_operations.h"

#include <SDL/SDL.h>

Uint32 getPixel(SDL_Surface *image, int x, int y)
{
    if (x >= 0 && x < image->w && y >= 0 && y < image->h) {
        Uint8 *pixel = (Uint8 *)image->pixels + y * image->pitch + x * image->format->BytesPerPixel;
        return *(Uint32 *)pixel;
    }
    return 0; // Pixel hors de l'image, retourne 0 par défaut.
}

void setPixel(SDL_Surface *image, int x, int y, Uint32 newPixel)
{
    int width = image->w;
    Uint32 *pixels = (Uint32 *)image->pixels;
    
    if (x >= 0 && x < width && y >= 0 && y < image->h)
    {
        pixels[y * width + x] = newPixel;
    }
}