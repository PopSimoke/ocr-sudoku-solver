#include "preprocess.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *imagePath = NULL;
    int option;
    int grayscale = 0, contrast = 0, gamma = 0, median = 0, otsu = 0, cramp = 0, gridonly = 0, all = 0, save = 0, invert = 0;

    while ((option = getopt(argc, argv, "gclmoatysh")) != -1)
    {
        switch (option)
        {
        case 'h':
            printf("Usage: %s -[gclmoatysh] <image_path>\n", argv[0]);
            printf("Options:\n");
            printf("\t-g: Convert image to grayscale\n");
            printf("\t-c: Apply auto contrast\n");
            printf("\t-l: Apply gamma correction\n");
            printf("\t-m: Apply median filter\n");
            printf("\t-o: Apply otsu thresholding\n");
            printf("\t-a: Apply all previous options\n");
            printf("\t-t: Apply crampthresholding\n");
            printf("\t-y: Keep only the grid\n");
            printf("\t-s: Save the squares\n");
            printf("\t-h: Display this help\n");

        case 'g':
            grayscale = 1;
            break;
        case 'c':
            contrast = 1;
            break;
        case 'l':
            gamma = 1;
            break;
        case 'm':
            median = 1;
            break;
        case 'o':
            otsu = 1;
            break;
        case 'a':
            all = 1;
            break;
        case 't':
            cramp = 1;
            break;
        case 'y':
            gridonly = 1;
            break;
        case 's':
            save = 1;
            break;
        case '?':
            fprintf(stderr, "Unknown option: %c\n", optopt);
            return 1;
        }
    }

    if (optind < argc)
    {
        imagePath = argv[optind];
    }
    else
    {
        fprintf(stderr, "Usage: %s -[gclmoabs] <image_path>\n", argv[0]);
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
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
        Color *mostFrequentColor = malloc(sizeof(Color));
        SDL_Surface *postTreatmentImage = imageSurface;
        Color *colors = (Color *)malloc(imageSurface->w * imageSurface->h * sizeof(Color));
        int *intensities = (int *)calloc(sizeof(int), imageSurface->w * imageSurface->h);

        if (grayscale)
        {
            postTreatmentImage = convertToGrayscale(imageSurface);
            if (postTreatmentImage == NULL)
            {
                fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
                errx(1, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
            }
        }
        if (contrast)
        {
            postTreatmentImage = convertToGrayscale(imageSurface);
            if (postTreatmentImage == NULL)
            {
                fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
                errx(1, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
            }
            autoContrast(postTreatmentImage);
        }
        if (gamma)
        {
            gammaCorrection(postTreatmentImage, 0.2);
        }
        if (median)
        {
            applyMedianFilter(postTreatmentImage, 3);
        }
        if (invert)
        {
            invertColors(postTreatmentImage);
        }
        if (otsu)
        {
            if (invert == 0)
            {
                invertColors(postTreatmentImage);
            }
            otsuTresholding(postTreatmentImage);
        }
        if (cramp)
        {
            crampthresholding(postTreatmentImage, colors, intensities);
        }
        if (gridonly)
        {
            int w = postTreatmentImage->w;
            int h = postTreatmentImage->h;

            int maxIndex = arrayMaxIndex(intensities, w * h);
            Color currentMostFrequentColor = colors[maxIndex];

            Point *corners = findCorners(postTreatmentImage, currentMostFrequentColor);

            while (corners[1].x - corners[0].x < w / 3 || corners[3].x - corners[2].x < w / 3 ||
                   corners[2].y - corners[0].y < h / 3 || corners[3].y - corners[1].y < h / 3)
            {
                // printf("Changing grid\n");
                currentMostFrequentColor = colors[arrayMaxIndexAfter(intensities, w * h, maxIndex)];
                free(corners);
                corners = findCorners(postTreatmentImage, currentMostFrequentColor);
            }

            for (int x = 0; x < postTreatmentImage->w; x++)
            {
                for (int y = 0; y < postTreatmentImage->h; y++)
                {
                    if (!isSameColor(postTreatmentImage, x, y, currentMostFrequentColor))
                    {
                        setPixel(postTreatmentImage, x, y, SDL_MapRGB(postTreatmentImage->format, 0, 0, 0));
                    }
                }
            }
        }

        if (all)
        {
            postTreatmentImage = preprocessImage(imageSurface, mostFrequentColor);
        }

        if (save)
        {
            saveSquares(postTreatmentImage, *mostFrequentColor);
        }

        free(mostFrequentColor);
        free(colors);
        free(intensities);

        // Convertir l'image prétraitée à afficher en texture
        imageTexture = SDL_CreateTextureFromSurface(renderer, postTreatmentImage);
        SDL_FreeSurface(imageSurface); // Libérer la mémoire de l'image prétraitée
        SDL_FreeSurface(postTreatmentImage);
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