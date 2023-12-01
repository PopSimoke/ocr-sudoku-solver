#include "pixel_operations.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

void setPixelFromColor(SDL_Surface *image, int x, int y, Color color)
{
    Uint32 newPixel = SDL_MapRGB(image->format, color.r, color.g, color.b);
    setPixel(image, x, y, newPixel);
}


int compareIntensity(const void *a, const void *b)
{
    return (*(Uint8 *)a - *(Uint8 *)b);
}

Uint8 getIntensity(SDL_Surface *image, int x, int y)
{
    Uint8 intensity;
    if (x >= 0 && x < image->w && y >= 0 && y < image->h)
    {
        SDL_GetRGB(getPixel(image, x, y), image->format, &intensity, &intensity, &intensity);
    }
    else
    {
        intensity = 0; // Valeur par défaut en dehors de l'image.
    }
    return intensity;
}

Color fromPixel(Uint32 pixel, SDL_PixelFormat *format) {
    Color color;
    SDL_GetRGB(pixel, format, &color.r, &color.g, &color.b);
    return color;
}