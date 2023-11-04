#ifndef HOUGH_TRANSFORM_H
#define HOUGH_TRANSFORM_H

#include "pixel_operations.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    double rho;
    double theta;
} HoughLine;


/**
 * @brief This function applies the Hough Transform to an image and returns all the straight lines detected
 * 
 * @param image the image to process
 * @param numLines the number of lines detected
 * @return HoughLine* an array of HoughLine, all the straight lines detected in the image
 */
HoughLine *HoughTransform(SDL_Surface *image, int *numLines);

/**
 * @brief Merges similar lines in an array of HoughLine.
 * 
 * @param lines Array of HoughLine structures.
 * @param numLines Number of lines in the array.
 * @param rhoTolerance Tolerance for rho differences.
 * @param thetaTolerance Tolerance for theta differences.
 */
void MergeSimilarLines(HoughLine *lines, int *numLines, double rhoTolerance, double thetaTolerance);

#endif