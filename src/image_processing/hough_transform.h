#ifndef HOUGH_TRANSFORM_H
#define HOUGH_TRANSFORM_H

#include "pixel_operations.h"
#include "image_processing.h"

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


typedef struct Element {
    float theta;
    int votes;
    struct Element *next;
} Element;

/**
 * @brief This function applies the Hough Transform to an image and returns all the straight lines detected
 * 
 * @param image the image to process
 * @param numLines the number of lines detected
 * @return HoughLine* an array of HoughLine, all the straight lines detected in the image
 */
HoughLine *HoughTransform(SDL_Surface *image, int *numLines, Point *corners, Color mostFrequentColor, float *mostpresenttheta);

/**
 * @brief Merges similar lines in an array of HoughLine.
 * 
 * @param lines Array of HoughLine structures.
 * @param numLines Number of lines in the array.
 * @param rhoTolerance Tolerance for rho differences.
 * @param thetaTolerance Tolerance for theta differences.
 */
void MergeSimilarLines(HoughLine *lines, int *numLines, double rhoTolerance, double thetaTolerance);

/**
 * @brief This function returns the most present theta in the list
 * 
 * @param list the list of theta
 * @return float the most present theta
 */
float* getMostPresentTheta(Element *list);


/**
 * @brief This function returns the number of votes of the most present theta in the list
 * 
 * @param list the list of theta
 * @return int the number of votes of the most present theta
 */
int getMostPresentThetaVotes(Element *list);

/**
 * @brief This function returns the number of votes of the most present theta in the list
 * 
 * @param list the list of theta
 * @return int the number of votes of the most present theta
 */
float returntheta(SDL_Surface *image, Point *corners, Color mostFrequentColor);
#endif