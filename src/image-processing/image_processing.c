#include "image_processing.h"

SDL_Surface *convertToGrayscale(SDL_Surface *image)
{
    SDL_Surface *grayImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
    if (grayImage == NULL)
    {
        fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
        return NULL;
    }

    int w = grayImage->w;
    int h = grayImage->h;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Uint32 pixel = getPixel(grayImage, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, grayImage->format, &r, &g, &b);
            Uint8 grayValue = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);
            Uint32 newPixel = SDL_MapRGBA(grayImage->format, grayValue, grayValue, grayValue, 255);
            *((Uint32 *)grayImage->pixels + y * grayImage->w + x) = newPixel;
        }
    }

    return grayImage;
}

void applyMedianFilter(SDL_Surface *image, int kernelSize)
{
    if (kernelSize % 2 == 0)
    {
        fprintf(stderr, "Kernel size should be an odd number.\n");
        return;
    }

    int w = image->w;
    int h = image->h;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Uint8 rValues[kernelSize * kernelSize];
            Uint8 gValues[kernelSize * kernelSize];
            Uint8 bValues[kernelSize * kernelSize];
            int index = 0;

            for (int ky = -kernelSize / 2; ky <= kernelSize / 2; ky++)
            {
                for (int kx = -kernelSize / 2; kx <= kernelSize / 2; kx++)
                {
                    int srcX = x + kx;
                    int srcY = y + ky;

                    if (srcX >= 0 && srcX < w && srcY >= 0 && srcY < h)
                    {
                        Uint32 pixel = getPixel(image, srcX, srcY);
                        SDL_GetRGB(pixel, image->format, &rValues[index], &gValues[index], &bValues[index]);
                        index++;
                    }
                }
            }

            // Tri des valeurs RVB
            for (int i = 0; i < index - 1; i++)
            {
                for (int j = 0; j < index - i - 1; j++)
                {
                    if (rValues[j] > rValues[j + 1])
                    {
                        Uint8 tmp = rValues[j];
                        rValues[j] = rValues[j + 1];
                        rValues[j + 1] = tmp;
                    }
                    if (gValues[j] > gValues[j + 1])
                    {
                        Uint8 tmp = gValues[j];
                        gValues[j] = gValues[j + 1];
                        gValues[j + 1] = tmp;
                    }
                    if (bValues[j] > bValues[j + 1])
                    {
                        Uint8 tmp = bValues[j];
                        bValues[j] = bValues[j + 1];
                        bValues[j + 1] = tmp;
                    }
                }
            }

            // Utilisation de la valeur médiane
            int medianIndex = index / 2;
            Uint32 medianPixel = SDL_MapRGB(image->format, rValues[medianIndex], gValues[medianIndex], bValues[medianIndex]);
            setPixel(image, x, y, medianPixel);
        }
    }
}

void binarizeImage(SDL_Surface *grayImage, Uint8 threshold)
{
    int width = grayImage->w;
    int height = grayImage->h;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 grayValue;
            SDL_GetRGB(getPixel(grayImage, x, y), grayImage->format, &grayValue, &grayValue, &grayValue);

            // Binarisation : si la valeur du pixel est inférieure au seuil, le pixel devient noir, sinon il devient blanc
            Uint8 binarizedValue = (grayValue < threshold) ? 0 : 255;
            Uint32 newPixel = SDL_MapRGBA(grayImage->format, binarizedValue, binarizedValue, binarizedValue, 255);
            setPixel(grayImage, x, y, newPixel);
        }
    }
}

// Fonction pour inverser les couleurs d'une image
void invertColors(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint32 pixel = getPixel(image, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            Uint32 newPixel = SDL_MapRGB(image->format, r, g, b);
            setPixel(image, x, y, newPixel);
        }
    }
}

// le filtre flou pas fou
void applyAverageFilter(SDL_Surface *image, int kernelSize)
{
    int width = image->w;
    int height = image->h;
    int halfKernel = kernelSize / 2;

    SDL_Surface *tempImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    for (int y = halfKernel; y < height - halfKernel; y++)
    {
        for (int x = halfKernel; x < width - halfKernel; x++)
        {
            int totalR = 0;
            int totalG = 0;
            int totalB = 0;

            for (int ky = -halfKernel; ky <= halfKernel; ky++)
            {
                for (int kx = -halfKernel; kx <= halfKernel; kx++)
                {
                    Uint32 pixel = getPixel(image, x + kx, y + ky);
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, image->format, &r, &g, &b);
                    totalR += r;
                    totalG += g;
                    totalB += b;
                }
            }

            Uint8 avgR = (Uint8)(totalR / (kernelSize * kernelSize));
            Uint8 avgG = (Uint8)(totalG / (kernelSize * kernelSize));
            Uint8 avgB = (Uint8)(totalB / (kernelSize * kernelSize));

            Uint32 newPixel = SDL_MapRGB(tempImage->format, avgR, avgG, avgB);
            *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
        }
    }

    // Copier le résultat de l'image temporaire dans l'image d'origine
    SDL_BlitSurface(tempImage, NULL, image, NULL);

    // Libérer l'image temporaire
    SDL_FreeSurface(tempImage);
}

void applySobelFilter(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;

    // Création d'une image temporaire pour stocker les résultats du filtre de Sobel
    SDL_Surface *tempImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    if (tempImage == NULL)
    {
        fprintf(stderr, "SDL_CreateRGBSurface Error: %s\n", SDL_GetError());
        return;
    }

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            // Appliquer le filtre de Sobel
            int gx = 0;
            int gy = 0;

            // Noyau de Sobel pour la détection des contours horizontaux
            int kernelX[3][3] = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};

            // Noyau de Sobel pour la détection des contours verticaux
            int kernelY[3][3] = {
                {-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}};

            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    Uint8 grayValue;
                    SDL_GetRGB(getPixel(image, x + kx, y + ky), image->format, &grayValue, &grayValue, &grayValue);
                    gx += kernelX[ky + 1][kx + 1] * grayValue;
                    gy += kernelY[ky + 1][kx + 1] * grayValue;
                }
            }

            int gradient = (int)sqrt(gx * gx + gy * gy);

            // Limiter la valeur du gradient à 255 pour éviter un débordement
            if (gradient > 255)
            {
                gradient = 255;
            }

            Uint32 newPixel = SDL_MapRGBA(tempImage->format, gradient, gradient, gradient, 255);
            *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
        }
    }

    // Copier les résultats du filtre de Sobel dans l'image d'origine
    SDL_BlitSurface(tempImage, NULL, image, NULL);

    // Libérer l'image temporaire
    SDL_FreeSurface(tempImage);
}

void applyCannyFilter(SDL_Surface *image, Uint8 lowThreshold, Uint8 highThreshold)
{

    // Appliquer le filtre de Sobel pour détecter les contours
    applySobelFilter(image);

    int width = image->w;
    int height = image->h;

    // Création d'une image temporaire pour stocker les résultats
    SDL_Surface *tempImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    if (tempImage == NULL)
    {
        fprintf(stderr, "SDL_CreateRGBSurface Error: %s\n", SDL_GetError());
        return;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 intensity;
            SDL_GetRGB(getPixel(image, x, y), image->format, &intensity, &intensity, &intensity);

            // Si l'intensité est inférieure au seuil bas, considérez-le comme un pixel de fond
            if (intensity < lowThreshold)
            {
                intensity = 0;
            }
            // Si l'intensité est supérieure au seuil haut, considérez-le comme un pixel de contour
            else if (intensity >= highThreshold)
            {
                intensity = 255;
            }
            else
            {
                // Vérifiez les pixels voisins pour la suppression des non-maximums
                Uint8 neighbors[8];
                neighbors[0] = getIntensity(image, x - 1, y - 1);
                neighbors[1] = getIntensity(image, x, y - 1);
                neighbors[2] = getIntensity(image, x + 1, y - 1);
                neighbors[3] = getIntensity(image, x - 1, y);
                neighbors[4] = getIntensity(image, x + 1, y);
                neighbors[5] = getIntensity(image, x - 1, y + 1);
                neighbors[6] = getIntensity(image, x, y + 1);
                neighbors[7] = getIntensity(image, x + 1, y + 1);

                // Comparez l'intensité actuelle avec les voisins
                int isMaximum = 1;
                for (int i = 0; i < 8; i++)
                {
                    if (intensity < neighbors[i])
                    {
                        isMaximum = 0;
                        break;
                    }
                }

                if (isMaximum)
                {
                    intensity = 255;
                }
                else
                {
                    intensity = 0;
                }
            }

            Uint32 newPixel = SDL_MapRGBA(tempImage->format, intensity, intensity, intensity, 255);
            *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
        }
    }

    // Copier les résultats dans l'image d'origine
    SDL_BlitSurface(tempImage, NULL, image, NULL);

    // Suivi des contours (hystérésis)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 intensity;
            SDL_GetRGB(getPixel(image, x, y), image->format, &intensity, &intensity, &intensity);

            if (intensity == 255)
            {
                // Si un pixel est un contour potentiel, suivez-le
                for (int ky = -1; ky <= 1; ky++)
                {
                    for (int kx = -1; kx <= 1; kx++)
                    {
                        if (getIntensity(image, x + kx, y + ky) >= lowThreshold && intensity != 255)
                        {
                            // Marquer le pixel comme un contour
                            intensity = 255;
                            break;
                        }
                    }
                }

                Uint32 newPixel = SDL_MapRGBA(tempImage->format, intensity, intensity, intensity, 255);
                *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
            }
        }
    }

    // Copier les résultats finaux dans l'image d'origine
    SDL_BlitSurface(tempImage, NULL, image, NULL);

    // Libérer l'image temporaire
    SDL_FreeSurface(tempImage);
}

void applyDilation(SDL_Surface *image, int kernelSize, int iterations)
{
    if (kernelSize % 2 == 0)
    {
        fprintf(stderr, "Kernel size should be an odd number.\n");
        return;
    }

    int width = image->w;
    int height = image->h;

    for (int iteration = 0; iteration < iterations; iteration++)
    {
        SDL_Surface *tempImage = SDL_CreateRGBSurface(0, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                Uint8 maxIntensity = 0;

                for (int ky = -kernelSize / 2; ky <= kernelSize / 2; ky++)
                {
                    for (int kx = -kernelSize / 2; kx <= kernelSize / 2; kx++)
                    {
                        Uint8 intensity = getIntensity(image, x + kx, y + ky);
                        if (intensity > maxIntensity)
                        {
                            maxIntensity = intensity;
                        }
                    }
                }

                Uint32 newPixel = SDL_MapRGBA(image->format, maxIntensity, maxIntensity, maxIntensity, 255);
                *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
            }
        }

        // Copier les résultats dans l'image d'origine
        SDL_BlitSurface(tempImage, NULL, image, NULL);

        // Libérer l'image temporaire
        SDL_FreeSurface(tempImage);
    }
}

void applyErosion(SDL_Surface *image, int kernelSize, int iterations)
{
    if (kernelSize % 2 == 0)
    {
        fprintf(stderr, "Kernel size should be an odd number.\n");
        return;
    }

    int width = image->w;
    int height = image->h;

    for (int iteration = 0; iteration < iterations; iteration++)
    {
        SDL_Surface *tempImage = SDL_CreateRGBSurface(0, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                Uint8 minIntensity = 255;

                for (int ky = -kernelSize / 2; ky <= kernelSize / 2; ky++)
                {
                    for (int kx = -kernelSize / 2; kx <= kernelSize / 2; kx++)
                    {
                        Uint8 intensity = getIntensity(image, x + kx, y + ky);
                        if (intensity < minIntensity)
                        {
                            minIntensity = intensity;
                        }
                    }
                }

                Uint32 newPixel = SDL_MapRGBA(image->format, minIntensity, minIntensity, minIntensity, 255);
                *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
            }
        }

        // Copier les résultats dans l'image d'origine
        SDL_BlitSurface(tempImage, NULL, image, NULL);

        // Libérer l'image temporaire
        SDL_FreeSurface(tempImage);
    }
}