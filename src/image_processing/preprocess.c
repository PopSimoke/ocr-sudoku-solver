#include "preprocess.h"

void preprocessImage(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;

    SDL_Surface *grayImage = convertToGrayscale(image);
    if (grayImage == NULL)
    {
        fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
        return;
    }

    contrastCorrection(grayImage, 1.8);
    gammaCorrection(grayImage, 0.5);
    applyMedianFilter(grayImage, 3);
    invertColors(grayImage);
    otsuTresholding(grayImage);

    Color *colors = (Color *)malloc(w * h * sizeof(Color));
    int *intensities = (int *)calloc(sizeof(int), w * h);
    crampthresholding(grayImage, colors, intensities);

    int maxIndex = arrayMaxIndex(intensities, w * h);
    Color mostFrequentColor = colors[maxIndex];

    // découpe la grille

    Point *corners = findCorners(grayImage, mostFrequentColor);

    while (corners[1].x - corners[0].x < w / 3 || corners[3].x - corners[2].x < w / 3 ||
        corners[2].y - corners[0].y < h / 3 || corners[3].y - corners[1].y < h / 3)
    {
        printf("Changing grid\n");
        mostFrequentColor = colors[arrayMaxIndexAfter(intensities, w * h, maxIndex)];
        free(corners);
        corners = findCorners(grayImage, mostFrequentColor);
    }

    // pour mettre tout en noir sauf la grille
    // for (int x = 0; x < grayImage->w; x++)
    // {
    //     for (int y = 0; y < grayImage->h; y++)
    //     {
    //         if (!isSameColor(grayImage, x, y, mostFrequentColor))
    //         {
    //             setPixel(grayImage, x, y, SDL_MapRGB(grayImage->format, 0, 0, 0));
    //         }
    //     }
    // }

    // pour dessiner les 4 coins
    // for (int i = 0; i < 4; i++)
    // {
    //     drawSquare(grayImage, &corners[i], 20);
    //     printf("Corner %d: (%d, %d)\n", i, corners[i].x, corners[i].y);
    // }

    // Automatic rotation

    double angle = findRotationAngle(corners);
    // printf("Angle: %f\n", angle);

    SDL_Surface *rotatedImage = rotateImage(angle, grayImage);
    if (rotatedImage == NULL)
    {
        fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
        return;
    }
    resizeImage(rotatedImage, w, h);

    Point *cornersPostRotate = findCorners(rotatedImage, mostFrequentColor);
    // for (int i = 0; i < 4; i++)
    // {
    //     drawSquare(rotatedImage, &cornersPostRotate[i], 20);
    //     printf("Corner %d: (%d, %d)\n", i, cornersPostRotate[i].x, cornersPostRotate[i].y);
    // }

    SDL_Surface *newImage = copySurface(rotatedImage, cornersPostRotate, mostFrequentColor);

    // saveSquares(newImage, mostFrequentColor);

    SDL_BlitSurface(newImage, NULL, image, NULL);

    SDL_FreeSurface(newImage);
    SDL_FreeSurface(rotatedImage);

    free(corners);
    free(colors);
    free(intensities);
}