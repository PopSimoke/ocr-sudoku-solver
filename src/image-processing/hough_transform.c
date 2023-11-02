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

    int numThetas = 180;
    
    int *accumulator = (int *)calloc(diagonalLength * numThetas, sizeof(int));

    // Parcourir tous les pixels de l'image
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 intensity;
            SDL_GetRGB(getPixel(image, x, y), image->format, &intensity, &intensity, &intensity);

            // Si le pixel est un bord blanc, appliquer la transformée de Hough
            if (intensity == 255)
            {
                for (int thetaIndex = 0; thetaIndex < numThetas; thetaIndex++)
                {
                    double theta = (double)thetaIndex * M_PI / numThetas;
                    double rho = x * cos(theta) + y * sin(theta);
                    int rhoIndex = (int)(rho + diagonalLength / 2 + 0.5);

                    // Vérifier les limites du tableau
                    if (rhoIndex >= 0 && rhoIndex < diagonalLength)
                    {
                        accumulator[rhoIndex * numThetas + thetaIndex]++;
                    }
                }
            }
        }
    }

    // Trouver les lignes avec le plus grand nombre de votes
    int maxVotes = 0;
    for (int i = 0; i < diagonalLength * numThetas; i++)
    {
        if (accumulator[i] > maxVotes)
        {
            maxVotes = accumulator[i];
        }
    }

    int threshold = (int)(maxVotes * NOISE_TRESHOLD);

    // Créer une liste de lignes détectées
    HoughLine *lines = (HoughLine *)malloc(sizeof(HoughLine) * diagonalLength * numThetas);
    int numLinesFound = 0;

    // Parcourir tous les indices de l'accumulateur
    for (int rhoIndex = 0; rhoIndex < diagonalLength; rhoIndex++)
    {
        for (int thetaIndex = 0; thetaIndex < numThetas; thetaIndex++)
        {
            int votes = accumulator[rhoIndex * numThetas + thetaIndex];

            // Si le nombre de votes est supérieur au treshold, ajouter la ligne à la liste
            if (votes > threshold)
            {
                HoughLine line;
                line.rho = (double)(rhoIndex - diagonalLength / 2);
                line.theta = (double)thetaIndex * M_PI / numThetas;
                lines[numLinesFound] = line;
                numLinesFound++;
            }
        }
    }

    // Libérer la mémoire de l'accumulateur
    free(accumulator);

    // Définir le nombre de lignes détectées pour print après pour debug
    *numLines = numLinesFound;

    return lines;
}