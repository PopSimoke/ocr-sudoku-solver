#include "preprocess.h"

void drawLine(SDL_Surface *image, HoughLine line)
{
    int width = image->w;
    int height = image->h;
    Uint32 redColor = SDL_MapRGB(image->format, 255, 0, 0);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            double rho = line.rho;
            double theta = line.theta;
            double distance = x * cos(theta) + y * sin(theta);
            Uint8 r, g, b;
            SDL_GetRGB(getPixel(image, x, y), image->format, &r, &g, &b);
            if (fabs(distance - rho) < 1.0 && r == 255 && g == 255 && b == 255)
            {                                    // Tolérance pour dessiner la ligne
                setPixel(image, x, y, redColor); // Fonction pour définir la couleur du pixel en rouge
            }
        }
    }
}

void drawSquare(SDL_Surface *image, Point *p, int squareSize)
{
    int width = image->w;
    int height = image->h;
    Uint32 redColor = SDL_MapRGB(image->format, 255, 255, 0);

    for (int x = p->x - (int)(squareSize / 2); x < p->x + (int)(squareSize / 2); x++)
    {
        for (int y = p->y - (int)(squareSize / 2); y < p->y + (int)(squareSize / 2); y++)
        {
            if (x > 0 && y > 0 && x < width && y < height)
            {
                setPixel(image, x, y, redColor);
            }
        }
    }
}

SDL_Surface *preprocessImage(SDL_Surface *image, Color *mostFrequentColor)
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
    Color currentMostFrequentColor = colors[maxIndex];

    // découpe la grille

    Point *corners = findCorners(grayImage, currentMostFrequentColor);

    while (corners[1].x - corners[0].x < w / 3 || corners[3].x - corners[2].x < w / 3 ||
        corners[2].y - corners[0].y < h / 3 || corners[3].y - corners[1].y < h / 3)
    {
        printf("Changing grid\n");
        currentMostFrequentColor = colors[arrayMaxIndexAfter(intensities, w * h, maxIndex)];
        free(corners);
        corners = findCorners(grayImage, currentMostFrequentColor);
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

//    double angle = findRotationAngle(corners);
    // printf("Angle: %f\n", angle);

//    SDL_Surface *rotatedImage = rotateImage(angle, grayImage);
//    if (rotatedImage == NULL)
//    {
//        fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
//        return;
//    }
//    resizeImage(rotatedImage, w, h);

    // remove the perspective
    SDL_Surface *no_perspective = remove_perspective(grayImage, (SDL_Point *) (corners));

// JE SUIS OBLIGÉ DE LAISSER CETTE MERDE, SINON Y'A TOUT QUI PLANTE
    Point *cornersPostRotate = findCorners(grayImage, currentMostFrequentColor);
    // for (int i = 0; i < 4; i++)
    // {
    //     drawSquare(rotatedImage, &cornersPostRotate[i], 20);
    //     printf("Corner %d: (%d, %d)\n", i, cornersPostRotate[i].x, cornersPostRotate[i].y);
    // }

// JE SUIS OBLIGÉ DE LAISSER CETTE MERDE, SINON Y'A TOUT QUI PLANTE
    SDL_Surface *newImage = copySurface(no_perspective, cornersPostRotate, currentMostFrequentColor);

    mostFrequentColor->r = currentMostFrequentColor.r;
    mostFrequentColor->g = currentMostFrequentColor.g;
    mostFrequentColor->b = currentMostFrequentColor.b;

    SDL_BlitSurface(no_perspective, NULL, image, NULL);

    SDL_FreeSurface(no_perspective);

    free(corners);
    free(colors);
    free(intensities);

    return newImage;
}