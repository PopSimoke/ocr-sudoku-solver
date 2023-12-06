#include "utils.h"
#include "pixel_operations.h"
#include "image_processing.h"

#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#define M_PI 3.14159265358979323846
int arrayMaxIndex(int *array, int size) {
    int max = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[max]) {
            max = i;
        }
    }

    return max;
}

SDL_Surface *copySurface(SDL_Surface *surface, Point *corners, Color mostFrequentColor)
{
    int w = corners[1].x - corners[0].x;
    int h = corners[2].y - corners[0].y;

    SDL_Surface *copy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

    /// iterate over all the pixels in the surface and copy them to the new surface if they are in the rectangle
    for (int x = 0; x < surface->w; x++)
    {
        for (int y = 0; y < surface->h; y++)
        {
            if (x >= corners[0].x && x <= corners[1].x && y >= corners[0].y && y <= corners[2].y)
            {
                Color currentPixelColor = fromPixel(getPixel(surface, x, y), surface->format);
                setPixelFromColor(copy, x - corners[0].x, y - corners[0].y, currentPixelColor);
            }
            else
            {
                Color black = {0, 0, 0};
                setPixelFromColor(copy, x - corners[0].x, y - corners[0].y, black);
            }
        }
    }

    return copy;
}

int arrayMaxIndexAfter(int *array, int size, int currentMaxIndex) {
    int max = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[max] && i != currentMaxIndex) {
            max = i;
        }
    }

    return max;
}
int max (int a, int b){
    if (a>b){
        return a;
    }
    return b;
}
int min (int a, int b){
    if (a<b){
        return a;
    }
    return b;
}

SDL_Surface* rotateImage(double angle, SDL_Surface* image) {
    
    int cX = image->w / 2;
    int cY = image->h / 2;
    
    float oui  = ((M_PI/180) * angle);
    //x' = (x - centerX) * cos(oui) - (y - centerY) * sin(oui) + centerX
    //y' = (x - centerX) * sin(oui) + (y - centerY) * cos(oui) + centerY
    
    int x1 = (0-cX) * cos(oui) - (0-cY) * sin(oui) + cX;
    
    int y1 = (0-cX) * sin(oui) + (0-cY) * cos(oui) + cY;
    
    int x2 = (image->w-cX) * cos(oui) - (0-cY) * sin(oui) + cX; 
    
    int y2 = (image->w-cX) * sin(oui) + (0-cY) * cos(oui) + cY;
   
    int x3 = (0-cX) * cos(oui) - (image->h-cY) * sin(oui) + cX;
    
    int y3 = (0-cX) * sin(oui) + (image->h-cY) * cos(oui) + cY;
    
    int x4 = (image->w-cX) * cos(oui) - (image->h-cY) * sin(oui) + cX;
    
    int y4 = (image->w-cX) * sin(oui) + (image->h-cY) * cos(oui) + cY;
    
    int minX = min(min(x1, x2), min(x3, x4));
    
    int maxX = max(max(x1, x2), max(x3, x4));
    
    int minY = min(min(y1, y2), min(y3, y4));
    
    int maxY = max(max(y1, y2), max(y3, y4));
    
    int newWidth = abs(maxX) - abs(minX);
    int newHeight = abs(maxY) - abs(minY);

    
    SDL_Surface* rotatedImage = SDL_CreateRGBSurface(0, newWidth, newHeight, 32, 0, 0, 0, 0);
    int x,y,newX,newY;

    

    for (y = 0; y < image->h; y++) {
        for (x = 0; x < image->w; x++) {
            //printf("x : %d , y: %d",x,y);
            newX = (x - cX) * cos(oui) - (y - cY) * sin(oui) + cX;
            
            newY = (x - cX) * sin(oui) + (y - cY) * cos(oui) + cY;
            //printf("  newX : %d , newY: %d\n",newX,newY);
            Color currentPixelColor = fromPixel(getPixel(image, x, y), image->format);
            setPixelFromColor(rotatedImage, newX, newY, currentPixelColor);
            
        }
    }

    return rotatedImage;
}