#include "hough_transform.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image_processing.h"
#include "pixel_operations.h"
#define M_PI 3.14159265358979323846
#define NOISE_TRESHOLD 0.4

void addThetaToList(Element **list, float theta) {
    Element *current = *list;

    // Iterate over the list
    while (current != NULL) {
        if (current->theta == theta) {
            // If theta is found, increment the votes and return
            current->votes++;
            return;
        }
        current = current->next;
    }

    // If theta is not found, create a new element
    Element *newElement = (Element *)malloc(sizeof(Element));
    newElement->theta = theta;
    newElement->votes = 1;
    newElement->next = NULL;

    if (*list == NULL) {
        // If the list is empty, make the new element the head
        *list = newElement;
    } else {
        // Otherwise, add the new element to the end of the list
        current = *list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newElement;
    }
}
float* getMostPresentTheta(Element *list) {
    int maxVotes = 0;
    float* mostPresentTheta = (float*) malloc(sizeof(float));
    *mostPresentTheta = 0.0;
    Element *current = list;

    // Iterate over the list
    while (current != NULL) {
        if (current->votes > maxVotes) {
            // If current element's votes are greater than maxVotes, update maxVotes and mostPresentTheta
            maxVotes = current->votes;
            *mostPresentTheta = current->theta;
        }
        current = current->next;
    }
    *mostPresentTheta = fmod(*mostPresentTheta , 360);
    // Return the pointer to the theta with the most votes
    return mostPresentTheta;
}

HoughLine *HoughTransform(SDL_Surface *image, int *numLines, Point *corners, Color mostFrequentColor ,float* mostpresenttheta)
{
    int minx = 10000;
    int miny= 10000;
    int maxy = 0;
    int maxx = 0;
    
    for (int i = 0 ; i <4 ; i++)
    {
        Point oui  = corners[i];
        if ((oui.x)>maxx)
            maxx = oui.x;
        if ((oui.x)<minx)
            minx = oui.x;
        if ((oui.y)>maxy)
            maxy = oui.y;
        if ((oui.y)<miny)
            miny = oui.y;

    }
    int width = maxx-minx;
    int height = maxy-miny;

    int diagonalLength = (int)sqrt(width * width + height * height);
    diagonalLength += diagonalLength;

    int numThetas = 180;

    int *accumulator = (int *)calloc(diagonalLength * numThetas * 2, sizeof(int));
    
    // go through all pixels
    for (int x = minx; x < maxx; x++)
    {
        for (int y = miny; y < maxy; y++)
        {
            // Uint8 intensity;
            // SDL_GetRGB(getPixel(image, x, y), image->format, &intensity, &intensity, &intensity);

            // if pixel is white (edge)
            if (isSameColor(image, x, y, mostFrequentColor))
            {
                
                for (int thetaIndex = 0; thetaIndex < numThetas; thetaIndex++)
                {
                    double theta = (double)thetaIndex * M_PI / numThetas;
                    double rho = x * cos(theta) + y * sin(theta);
                    int rhoIndex = (int)(rho + diagonalLength / 2 + 0.5);

                    // check if rho is in bounds
                    if (rhoIndex >= 0 && rhoIndex < diagonalLength)
                    {
                        accumulator[(rhoIndex + diagonalLength) * numThetas + thetaIndex]++;
                    }

                    if (rhoIndex < 0 && rhoIndex > -diagonalLength)
                    {
                        accumulator[(rhoIndex + diagonalLength) * numThetas + thetaIndex]++;
                    }
                }
            }
        }
    }

    // find the maximum number of votes
    int maxVotes = 0;
    for (int i = 0; i < diagonalLength * numThetas * 2; i++)
    {
        if (accumulator[i] > maxVotes)
        {
            maxVotes = accumulator[i];
        }
    }

    int threshold = (int)(maxVotes * 0.5);

    // create array of lines
    HoughLine *lines = (HoughLine *)malloc(sizeof(HoughLine) * diagonalLength * numThetas);
    int numLinesFound = 0;

    // go through the accumulator and add lines that have enough votes to the array
    for (int rhoIndex = 0; rhoIndex < diagonalLength * 2; rhoIndex++)
    {
        for (int thetaIndex = 0; thetaIndex < numThetas; thetaIndex++)
        {
            int votes = accumulator[rhoIndex * numThetas + thetaIndex];

            // add line to array if it has enough votes
            if (votes > threshold)
            {
                HoughLine line;
                line.rho = (double)(rhoIndex - diagonalLength - diagonalLength / 2);
                line.theta = (double)thetaIndex * M_PI / numThetas;
                lines[numLinesFound] = line;
                numLinesFound++;
            }
        }
    }

    free(accumulator);

    // set the number of lines found
    *numLines = numLinesFound;
    Element *list = NULL;
    
    
    for (int i = 0; i < numLinesFound; i++)
    {
        addThetaToList(&list, lines[i].theta*180);
    }
    //printf("theta : %f\n",getMostPresentTheta(list));
    float* a = getMostPresentTheta(list);
    *mostpresenttheta = *a;
    
    
    return lines;
}

void MergeSimilarLines(HoughLine *lines, int *numLines, double rhoTolerance, double thetaTolerance)
{
    int numMergedLines = *numLines;

    for (int i = 0; i < numMergedLines; i++)
    {
        if (lines[i].rho == 0.0 && lines[i].theta == 0.0)
        {
            continue; // line already merged
        }

        for (int j = i + 1; j < numMergedLines; j++)
        {
            if (fabs(lines[i].rho - lines[j].rho) <= rhoTolerance &&
                fabs(lines[i].theta - lines[j].theta) <= thetaTolerance)
            {
                // merge lines
                lines[i].rho = (lines[i].rho + lines[j].rho) / 2.0;
                lines[i].theta = (lines[i].theta + lines[j].theta) / 2.0;
                // mark line as merged
                lines[j].rho = 0.0;
                lines[j].theta = 0.0;
            }
        }
    }

    // count the number of remaining lines
    int numRemainingLines = 0;
    for (int i = 0; i < numMergedLines; i++)
    {
        if (lines[i].rho != 0.0 || lines[i].theta != 0.0)
        {
            numRemainingLines++;
        }
    }

    // copy the remaining lines to the beginning of the array
    int currentIndex = 0;
    for (int i = 0; i < numMergedLines; i++)
    {
        if (lines[i].rho != 0.0 || lines[i].theta != 0.0)
        {
            if (currentIndex != i)
            {
                lines[currentIndex] = lines[i];
            }
            currentIndex++;
        }
    }

    // set the number of remaining lines
    *numLines = numRemainingLines;
}






