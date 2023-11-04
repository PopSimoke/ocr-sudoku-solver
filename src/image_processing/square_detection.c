#include "square_detection.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void saveSquares(SDL_Surface *sudokuImage) {
    // If the "saved_images" directory does not exist, create it
    const char *outputDir = "saved_images";
    if (mkdir(outputDir, 0777) == -1) {
        printf("Error creating directory %s\n", outputDir);
        return;
    }

    int cellWidth = sudokuImage->w / 9;
    int cellHeight = sudokuImage->h / 9;

    int cellIndex = 0;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            // create a surface to store the cell
            SDL_Surface *cell = SDL_CreateRGBSurface(0, cellWidth, cellHeight, 32, 0, 0, 0, 0);

            // copy the cell from the original image
            SDL_Rect srcRect = {col * cellWidth, row * cellHeight, cellWidth, cellHeight};
            SDL_BlitSurface(sudokuImage, &srcRect, cell, NULL);

            // sve the cell in the "saved_images" directory
            char filename[128];
            snprintf(filename, sizeof(filename), "saved_images/case_%d.png", cellIndex);
            IMG_SavePNG(cell, filename);

            SDL_FreeSurface(cell);

            cellIndex++;
        }
    }
}
