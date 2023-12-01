#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

/**
 * @brief Get the color of a pixel in an image.
 *
 * @param image The input image.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @return Uint32 The color value of the pixel.
 */
Uint32 getPixel(SDL_Surface *image, int x, int y);

/**
 * @brief Set the color of a pixel in an image.
 *
 * @param image The input image.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param newPixel The new color value to set for the pixel.
 */
void setPixel(SDL_Surface *image, int x, int y, Uint32 newPixel);

/** (NOT USED (YET ?))
 * @brief Compare two intensity values for sorting purposes.
 *
 * @param a A pointer to the first intensity value.
 * @param b A pointer to the second intensity value.
 * @return int A value less than 0 if a < b, 0 if a = b, or a value greater than 0 if a > b.
 */
int compareIntensity(const void *a, const void *b);

/**
 * @brief Get the intensity value at the point (x, y) of the image.
 *
 * @param image The input image.
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @return Uint8 The intensity value at the specified point.
 */
Uint8 getIntensity(SDL_Surface *image, int x, int y);

Color fromPixel(Uint32 pixel, SDL_PixelFormat *format);

void setPixelFromColor(SDL_Surface *image, int x, int y, Color color);

#endif