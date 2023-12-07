// #include "square_detection.h"

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/stat.h>
// #include <sys/types.h>

// void centerImage(SDL_Surface *image)
// {
//     // iterates over a black and white image, and center the white pixels

//     int w = image->w;
//     int h = image->h;

//     Point maxPoint = {0, 0};
//     Point minPoint = {w, h};

//     // find the max and min points
//     for (int x = 0; x < w; x++)
//     {
//         for (int y = 0; y < h; y++)
//         {
//             Uint8 r, g, b;
//             SDL_GetRGB(getPixel(image, x, y), image->format, &r, &g, &b);
//             if (r == 255 && g == 255 && b == 255)
//             {
//                 if (x > maxPoint.x)
//                 {
//                     maxPoint.x = x;
//                 }
//                 if (y > maxPoint.y)
//                 {
//                     maxPoint.y = y;
//                 }
//                 if (x < minPoint.x)
//                 {
//                     minPoint.x = x;
//                 }
//                 if (y < minPoint.y)
//                 {
//                     minPoint.y = y;
//                 }
//             }
//         }
//     }

//     // calculate the center point
//     Point centerPoint = {(maxPoint.x + minPoint.x) / 2, (maxPoint.y + minPoint.y) / 2};

//     // create a new image with the same size as the original one
//     SDL_Surface *centeredImage = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

//     // copy the original image to the new one, but centered
//     for (int x = 0; x < w; x++)
//     {
//         for (int y = 0; y < h; y++)
//         {
//             Uint8 r, g, b;
//             SDL_GetRGB(getPixel(image, x, y), image->format, &r, &g, &b);
//             if (r == 255 && g == 255 && b == 255)
//             {
//                 setPixel(centeredImage, x - centerPoint.x + (int)(w / 2), y - centerPoint.y + (int)(h / 2), SDL_MapRGB(image->format, 255, 255, 255));
//             }
//         }
//     }

//     // copy the centered image to the original one
//     SDL_BlitSurface(centeredImage, NULL, image, NULL);
//     SDL_FreeSurface(centeredImage);
// }

// int editSquare(SDL_Surface *square, Color mostFrequentColor)
// {

//     int w = square->w;
//     int h = square->h;

//     Color white = {255, 255, 255};
//     Color black = {0, 0, 0};

//     int edited = -1;

//     for (int x = 0; x < w; x++)
//     {
//         for (int y = 0; y < h; y++)
//         {
//             if (isSameColor(square, x, y, mostFrequentColor))
//             {
//                 setPixelFromColor(square, x, y, black);
//             }
//             else if (isSameColor(square, x, y, black))
//             {
//                 continue;
//             }
//             else
//             {
//                 setPixelFromColor(square, x, y, white);
//                 edited += 1;
//             }
//         }
//     }

//     return edited;
// }

// SDL_Surface *resizeImage(SDL_Surface *source, int newWidth, int newHeight)
// {
//     SDL_Surface *temp =
//         SDL_CreateRGBSurface(0, newWidth, newHeight, source->format->BitsPerPixel,
//                              source->format->Rmask, source->format->Gmask,
//                              source->format->Bmask, source->format->Amask);
//     SDL_BlitScaled(source, NULL, temp, NULL);
//     return temp;
// }

// void saveSquares(SDL_Surface *sudokuImage, Color mostFrequentColor)
// {
//     // If the "saved_images" directory does not exist, create it
//     // And if it already exists, prompt "Would you like to delete squares and create a new set ? [Y/n]"
//     struct stat st = {0};
//     const char *outputDir = "saved_images";
//     if (stat(outputDir, &st) == -1)
//     {
//         mkdir(outputDir, 0700);
//     }
//     else
//     {
//         printf("Would you like to delete squares and create a new set ? [Y/n] ");
//         char answer;
//         scanf("%c", &answer);
//         if (answer == 'Y' || answer == 'y')
//         {
//             system("rm -rf saved_images");
//             mkdir(outputDir, 0700);
//         }
//     }

//     int cellWidth = sudokuImage->w / 9;
//     int cellHeight = sudokuImage->h / 9;

//     int cellIndex = 0;
//     for (int row = 0; row < 9; row++)
//     {
//         for (int col = 0; col < 9; col++)
//         {
//             // create a surface to store the cell
//             SDL_Surface *cell =
//                 SDL_CreateRGBSurface(0, cellWidth, cellHeight, 32, 0, 0, 0, 0);

//             // copy the cell from the original image
//             SDL_Rect srcRect = {col * cellWidth, row * cellHeight, cellWidth,
//                                 cellHeight};
//             SDL_BlitSurface(sudokuImage, &srcRect, cell, NULL);

//             int result = editSquare(cell, mostFrequentColor);

//             if (result < 100)
//             {
//                 cellIndex++;
//                 continue;
//             }
//             // sve the cell in the "saved_images" directory
//             char filename[128];
//             snprintf(filename, sizeof(filename), "saved_images/case_%d.png",
//                      cellIndex);

//             centerImage(cell);
//             SDL_Surface *resized_cell = resizeImage(cell, 32, 32);
//             IMG_SavePNG(resized_cell, filename);
//             SDL_FreeSurface(cell);
//             SDL_FreeSurface(resized_cell);

//             cellIndex++;
//         }
//     }
// }


#include "square_detection.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void centerImage(SDL_Surface *image)
{
    // iterates over a black and white image, and center the white pixels

    int w = image->w;
    int h = image->h;

    Point maxPoint = {0, 0};
    Point minPoint = {w, h};

    // find the max and min points
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(getPixel(image, x, y), image->format, &r, &g, &b);
            if (r == 255 && g == 255 && b == 255)
            {
                if (x > maxPoint.x)
                {
                    maxPoint.x = x;
                }
                if (y > maxPoint.y)
                {
                    maxPoint.y = y;
                }
                if (x < minPoint.x)
                {
                    minPoint.x = x;
                }
                if (y < minPoint.y)
                {
                    minPoint.y = y;
                }
            }
        }
    }

    Point centerPoint = {(maxPoint.x + minPoint.x) / 2, (maxPoint.y + minPoint.y) / 2};

    // create a new image with the same size as the original one
    SDL_Surface *centeredImage = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

    // copy the original image to the new one, but centered
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(getPixel(image, x, y), image->format, &r, &g, &b);
            if (r == 255 && g == 255 && b == 255)
            {
                setPixel(centeredImage, x - centerPoint.x + (int)(w / 2), y - centerPoint.y + (int)(h / 2), SDL_MapRGB(image->format, 255, 255, 255));
            }
        }
    }

    // copy the centered image to the original one
    SDL_BlitSurface(centeredImage, NULL, image, NULL);
    SDL_FreeSurface(centeredImage);
}

int editSquare(SDL_Surface *square, Color mostFrequentColor)
{

    int w = square->w;
    int h = square->h;

    Color white = {255, 255, 255};
    Color black = {0, 0, 0};

    int edited = 0;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            if (isSameColor(square, x, y, mostFrequentColor))
            {
                setPixelFromColor(square, x, y, black);
            }
            else if (isSameColor(square, x, y, black))
            {
                continue;
            }
            else
            {
                // Color c = fromPixel(getPixel(square, x, y), square->format);
                // printf("color : (%d, %d, %d)\n", c.r, c.g, c.b);
                setPixelFromColor(square, x, y, white);
                edited += 1;
            }
        }
    }

    return edited;
}

SDL_Surface *resizeImage(SDL_Surface *source, int newWidth, int newHeight)
{
    SDL_Surface *temp =
        SDL_CreateRGBSurface(0, newWidth, newHeight, source->format->BitsPerPixel,
                             source->format->Rmask, source->format->Gmask,
                             source->format->Bmask, source->format->Amask);
    SDL_BlitScaled(source, NULL, temp, NULL);
    return temp;
}

void saveSquares(SDL_Surface *sudokuImage, Color mostFrequentColor)
{
    // If the "saved_images" directory does not exist, create it
    const char *outputDir = "saved_images";
    if (mkdir(outputDir, 0777) == -1)
    {
        // if directory already exists, delete it and create a new one
        system("rm -rf saved_images");
        mkdir(outputDir, 0777);
    }

    int cellWidth = sudokuImage->w / 9;
    int cellHeight = sudokuImage->h / 9;

    int cellIndex = 0;
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            // create a surface to store the cell
            SDL_Surface *cell =
                SDL_CreateRGBSurface(0, cellWidth, cellHeight, 32, 0, 0, 0, 0);

            // copy the cell from the original image
            SDL_Rect srcRect = {col * cellWidth, row * cellHeight, cellWidth,
                                cellHeight};
            SDL_BlitSurface(sudokuImage, &srcRect, cell, NULL);

            int result = editSquare(cell, mostFrequentColor);

            if (result < 100)
            {
                cellIndex++;
                continue;
            }
            // sve the cell in the "saved_images" directory
            char filename[128];
            snprintf(filename, sizeof(filename), "saved_images/case_%d.png",
                     cellIndex);

            centerImage(cell);
            SDL_Surface *resized_cell = resizeImage(cell, 32, 32);
            IMG_SavePNG(resized_cell, filename);
            SDL_FreeSurface(cell);
            SDL_FreeSurface(resized_cell);

            cellIndex++;
        }
    }
}