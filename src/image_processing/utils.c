#include "utils.h"
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operations.h"
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

int arrayMaxIndexAfter(int *array, int size, int currentMaxIndex) {
    int max = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[max] && i != currentMaxIndex) {
            max = i;
        }
    }

    return max;
}
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
         __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })
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