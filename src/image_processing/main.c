#include "hough_transform.h"
#include "image_processing.h"
#include "pixel_operations.h"
#include "square_detection.h"
#include "utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

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

    SDL_BlitSurface(grayImage, NULL, image, NULL);

    SDL_FreeSurface(grayImage);
}

void gridDetection(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;

    Color *colors = (Color *)malloc(w * h * sizeof(Color));
    int *intensities = (int *)calloc(sizeof(int), w * h);
    crampthresholding(image, colors, intensities);

    int maxIndex = arrayMaxIndex(intensities, w * h);
    Color mostFrequentColor = colors[maxIndex];

    // découpe la grille

    Point *corners = findCorners(image, mostFrequentColor);

    while (corners[1].x - corners[0].x < w / 3 || corners[3].x - corners[2].x < w / 3 ||
        corners[2].y - corners[0].y < h / 3 || corners[3].y - corners[1].y < h / 3)
    {
        printf("Changing grid\n");
        mostFrequentColor = colors[arrayMaxIndexAfter(intensities, w * h, maxIndex)];
        free(corners);
        corners = findCorners(image, mostFrequentColor);
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

    SDL_Surface *rotatedImage = rotateImage(angle, image);
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <image_path>\n", argv[0]);
        return 1;
    }

    const char *imagePath = argv[1];

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Sudoku Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *imageTexture = NULL;
    SDL_Surface *imageSurface = IMG_Load(imagePath);
    if (imageSurface == NULL)
    {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
    }
    else
    {

        preprocessImage(imageSurface);

        gridDetection(imageSurface);

        // Convertir l'image prétraitée à afficher en texture
        imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface); // Libérer la mémoire de l'image prétraitée

        if (imageTexture == NULL)
        {
            fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
    }

    SDL_Event e;
    int quit = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        SDL_RenderClear(renderer);
        if (imageTexture != NULL)
        {
            SDL_RenderCopy(renderer, imageTexture, NULL, NULL);
        }
        SDL_RenderPresent(renderer);
    }

    // Libérer la mémoire de la texture
    SDL_DestroyTexture(imageTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
