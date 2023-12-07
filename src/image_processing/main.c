#include "preprocess.h"

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
