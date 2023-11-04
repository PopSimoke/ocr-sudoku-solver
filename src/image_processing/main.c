#include "hough_transform.h"
#include "image_processing.h"
#include "pixel_operations.h"

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
            if (fabs(distance - rho) < 1.0)
            {                                    // Tolérance pour dessiner la ligne
                setPixel(image, x, y, redColor); // Fonction pour définir la couleur du pixel en rouge
            }
        }
    }
}

void preprocessImage(SDL_Surface *image)
{    
    SDL_Surface *grayImage = convertToGrayscale(image);
    if (grayImage == NULL)
    {
        fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
        return;
    }

    invertColors(grayImage);

    applyCannyFilter(grayImage, 50, 150);
    // applyDilation(grayImage, 3, 1);
    // applyErosion(grayImage, 5, 1);

    HoughLine *lines;
    int numLines;
    lines = HoughTransform(grayImage, &numLines);
    printf("Number of lines detected: %d\n", numLines);
    MergeSimilarLines(lines, &numLines, 10.0, 0.1);
    printf("Number of lines after merging: %d\n", numLines);

    for (int i = 0; i < numLines; i++)
    {
        drawLine(grayImage, lines[i]);
    }

    free(lines);

    SDL_BlitSurface(grayImage, NULL, image, NULL);

    SDL_FreeSurface(grayImage);
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
        // Étape 1 : Prétraitement de l'image
        // mais en fait cette étape elle sert plutôt à tester tout le code quoi lol

        preprocessImage(imageSurface);
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