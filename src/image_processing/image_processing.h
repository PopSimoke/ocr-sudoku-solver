#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "pixel_operations.h"
#include "utils.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define M_PI 3.14159265358979323846

typedef struct Point
{
    int x;
    int y;
} Point;

typedef struct Stack
{
    Point point;
    int size;
    struct Stack *next;
} Stack;

Stack *createStack();
void push(Stack **stack, Point *point);
Point *pop(Stack **stack);
Point *peek(Stack *stack);
bool isEmpty(Stack *stack);
void freeStack(Stack *stack);

/**
 * @brief Converts an image to grayscale.
 * 
 * @param image The input image.
 * @return SDL_Surface* The grayscale image.
 */
SDL_Surface *convertToGrayscale(SDL_Surface *image);

/**
 * @brief Applies a median filter to the image.
 * 
 * @param image The input image.
 * @param kernelSize The size of the filter kernel (should be an odd number).
 */
void applyMedianFilter(SDL_Surface *image, int kernelSize);

/**
 * @brief Binarizes a grayscale image based on a threshold.
 * 
 * @param grayImage The grayscale image.
 * @param threshold The threshold value for binarization.
 */
void binarizeImage(SDL_Surface *grayImage, Uint8 threshold);

/**
 * @brief Inverts the colors of an image.
 * 
 * @param image The input image.
 */
void invertColors(SDL_Surface *image);

/**
 * @brief Applies an average filter to the image (not used in the example).
 * 
 * @param image The input image.
 * @param kernelSize The size of the filter kernel.
 */
void applyAverageFilter(SDL_Surface *image, int kernelSize);

/**
 * @brief Applies the Sobel filter to detect edges in the image.
 * 
 * @param image The input image.
 */
void applySobelFilter(SDL_Surface *image);

/**
 * @brief Applies the Canny filter to the image for edge detection.
 * 
 * @param image The input image.
 * @param lowThreshold The lower threshold for edge detection.
 * @param highThreshold The higher threshold for edge detection.
 */
void applyCannyFilter(SDL_Surface *image, Uint8 lowThreshold, Uint8 highThreshold);

/**
 * @brief Applies dilation to the image.
 * 
 * @param image The input image.
 * @param kernelSize The size of the structuring element (should be an odd number).
 * @param iterations The number of dilation iterations.
 */
void applyDilation(SDL_Surface *image, int kernelSize, int iterations);

/**
 * @brief Applies erosion to the image.
 * 
 * @param image The input image.
 * @param kernelSize The size of the structuring element (should be an odd number).
 * @param iterations The number of erosion iterations.
 */
void applyErosion(SDL_Surface *image, int kernelSize, int iterations);

/**
 * @brief Corrects the gamma of the image.
 * 
 * @param image The input image.
 * @param gamma The gamma value.
 */
void gammaCorrection(SDL_Surface *image, double gamma);

/**
 * @brief Corrects the contrast of the image.
 * 
 * @param image The input image.
 * @param contrast The contrast value.
 */
void contrastCorrection(SDL_Surface *image, double contrast);

/**
 * @brief Converts the image to a binary image using the Otsu method.
 * 
 * @param image The input image.
 */
void otsuTresholding(SDL_Surface *image);

void floodFill(SDL_Surface *image, int x, int y, Color targetColor, Color newColor, int *intensity);
void crampthresholding(SDL_Surface *image, Color *colors, int *intensities);
Point *findCorners(SDL_Surface *image, Color mostFrequentColor);
void autoResize(SDL_Surface *image, Point *corners);
void rotate(SDL_Surface *surface, double angleDegree);
Point findCenter(Point *corners);
double findRotationAngle(Point *corners);
void autoContrast(SDL_Surface* image);

#endif