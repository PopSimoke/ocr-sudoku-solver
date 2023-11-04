#include "hough_transform.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define M_PI 3.14159265358979323846
#define NOISE_TRESHOLD 0.4

HoughLine *HoughTransform(SDL_Surface *image, int *numLines)
{
    int width = image->w;
    int height = image->h;

    int diagonalLength = (int)sqrt(width * width + height * height);
    diagonalLength += diagonalLength;

    int numThetas = 180;

    int *accumulator = (int *)calloc(diagonalLength * numThetas * 2, sizeof(int));

    // go through all pixels
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            Uint8 intensity;
            SDL_GetRGB(getPixel(image, x, y), image->format, &intensity, &intensity, &intensity);

            // if pixel is white (edge)
            if (intensity == 255)
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