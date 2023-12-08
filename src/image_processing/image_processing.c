#include "image_processing.h"

// Convert an image to grayscale
SDL_Surface *convertToGrayscale(SDL_Surface *image)
{
    SDL_Surface *grayImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGB888, 0);
    // SDL_Surface *grayImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
    if (grayImage == NULL)
    {
        fprintf(stderr, "SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
        return NULL;
    }

    int w = grayImage->w;
    int h = grayImage->h;

    // Iterate through each pixel
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Uint32 pixel = getPixel(grayImage, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, grayImage->format, &r, &g, &b);
            Uint8 grayValue = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);

            Uint32 newPixel = SDL_MapRGB(grayImage->format, grayValue, grayValue, grayValue);
            // Uint32 newPixel = SDL_MapRGBA(grayImage->format, grayValue, grayValue, grayValue, 255);
            *((Uint32 *)grayImage->pixels + y * grayImage->w + x) = newPixel;
        }
    }

    return grayImage;
}

// Apply a median filter to the image
void applyMedianFilter(SDL_Surface *image, int kernelSize)
{
    if (kernelSize % 2 == 0)
    {
        fprintf(stderr, "Kernel size should be an odd number.\n");
        return;
    }

    int w = image->w;
    int h = image->h;

    // Iterate through each pixel
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Uint8 rValues[kernelSize * kernelSize];
            Uint8 gValues[kernelSize * kernelSize];
            Uint8 bValues[kernelSize * kernelSize];
            int index = 0;

            // Iterate through the kernel
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

            // Sort the color values
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

            int medianIndex = index / 2;
            Uint32 medianPixel = SDL_MapRGB(image->format, rValues[medianIndex], gValues[medianIndex], bValues[medianIndex]);
            setPixel(image, x, y, medianPixel);
        }
    }
}

// Binarize the grayscale image
void binarizeImage(SDL_Surface *grayImage, Uint8 threshold)
{
    int width = grayImage->w;
    int height = grayImage->h;

    // Iterate through each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 grayValue;
            SDL_GetRGB(getPixel(grayImage, x, y), grayImage->format, &grayValue, &grayValue, &grayValue);

            Uint8 binarizedValue = (grayValue < threshold) ? 0 : 255;
            Uint32 newPixel = SDL_MapRGB(grayImage->format, binarizedValue, binarizedValue, binarizedValue);
            // Uint32 newPixel = SDL_MapRGBA(grayImage->format, binarizedValue, binarizedValue, binarizedValue, 255);
            setPixel(grayImage, x, y, newPixel);
        }
    }
}

// Invert the colors of the image
void invertColors(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;

    // Iterate through each pixel
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

// Apply a simple average filter (not used)
void applyAverageFilter(SDL_Surface *image, int kernelSize)
{
    int width = image->w;
    int height = image->h;
    int halfKernel = kernelSize / 2;

    SDL_Surface *tempImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    // Iterate through each pixel
    for (int y = halfKernel; y < height - halfKernel; y++)
    {
        for (int x = halfKernel; x < width - halfKernel; x++)
        {
            int totalR = 0;
            int totalG = 0;
            int totalB = 0;

            // Iterate through the kernel
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

    SDL_BlitSurface(tempImage, NULL, image, NULL);

    SDL_FreeSurface(tempImage);
}

// Apply the Sobel filter
void applySobelFilter(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;

    SDL_Surface *tempImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    if (tempImage == NULL)
    {
        fprintf(stderr, "SDL_CreateRGBSurface Error: %s\n", SDL_GetError());
        return;
    }

    // Iterate through each pixel
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            int gx = 0;
            int gy = 0;

            int kernelX[3][3] = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};

            int kernelY[3][3] = {
                {-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}};

            // Convolve with Sobel kernel
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

            if (gradient > 255)
            {
                gradient = 255;
            }

            Uint32 newPixel = SDL_MapRGB(tempImage->format, gradient, gradient, gradient);
            // Uint32 newPixel = SDL_MapRGBA(tempImage->format, gradient, gradient, gradient, 255);
            *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
        }
    }

    SDL_BlitSurface(tempImage, NULL, image, NULL);

    SDL_FreeSurface(tempImage);
}

void applyCannyFilter(SDL_Surface *image, Uint8 lowThreshold, Uint8 highThreshold)
{
    applySobelFilter(image);

    int width = image->w;
    int height = image->h;

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

            if (intensity < lowThreshold)
            {
                intensity = 0;
            }
            else if (intensity >= highThreshold)
            {
                intensity = 255;
            }
            else
            {
                Uint8 neighbors[8];
                neighbors[0] = getIntensity(image, x - 1, y - 1);
                neighbors[1] = getIntensity(image, x, y - 1);
                neighbors[2] = getIntensity(image, x + 1, y - 1);
                neighbors[3] = getIntensity(image, x - 1, y);
                neighbors[4] = getIntensity(image, x + 1, y);
                neighbors[5] = getIntensity(image, x - 1, y + 1);
                neighbors[6] = getIntensity(image, x, y + 1);
                neighbors[7] = getIntensity(image, x + 1, y + 1);

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

            Uint32 newPixel = SDL_MapRGB(tempImage->format, intensity, intensity, intensity);
            // Uint32 newPixel = SDL_MapRGBA(tempImage->format, intensity, intensity, intensity, 255);
            *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
        }
    }

    SDL_BlitSurface(tempImage, NULL, image, NULL);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 intensity;
            SDL_GetRGB(getPixel(image, x, y), image->format, &intensity, &intensity, &intensity);

            if (intensity == 255)
            {
                for (int ky = -1; ky <= 1; ky++)
                {
                    for (int kx = -1; kx <= 1; kx++)
                    {
                        if (getIntensity(image, x + kx, y + ky) >= lowThreshold && intensity != 255)
                        {
                            intensity = 255;
                            break;
                        }
                    }
                }

                Uint32 newPixel = SDL_MapRGB(tempImage->format, intensity, intensity, intensity);
                // Uint32 newPixel = SDL_MapRGBA(tempImage->format, intensity, intensity, intensity, 255);
                *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
            }
        }
    }

    SDL_BlitSurface(tempImage, NULL, image, NULL);

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

                Uint32 newPixel = SDL_MapRGB(image->format, maxIntensity, maxIntensity, maxIntensity);
                // Uint32 newPixel = SDL_MapRGBA(image->format, maxIntensity, maxIntensity, maxIntensity, 255);
                *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
            }
        }

        SDL_BlitSurface(tempImage, NULL, image, NULL);

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

                Uint32 newPixel = SDL_MapRGB(image->format, minIntensity, minIntensity, minIntensity);
                // Uint32 newPixel = SDL_MapRGBA(image->format, minIntensity, minIntensity, minIntensity, 255);
                *((Uint32 *)tempImage->pixels + y * width + x) = newPixel;
            }
        }

        SDL_BlitSurface(tempImage, NULL, image, NULL);

        SDL_FreeSurface(tempImage);
    }
}

void gammaCorrection(SDL_Surface *image, double gamma)
{
    for (int i = 0; i < image->w * image->h; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(getPixel(image, i % image->w, i / image->w), image->format, &r, &g, &b);
        r = (int)(255 * pow((double)r / 255, gamma));
        g = (int)(255 * pow((double)g / 255, gamma));
        b = (int)(255 * pow((double)b / 255, gamma));
        setPixel(image, i % image->w, i / image->w, SDL_MapRGB(image->format, r, g, b));
    }
}

void autoResize(SDL_Surface *image, Point *corners)
{
    int w = image->w;
    int h = image->h;

    // resize an image with the four points in corners
    int newWidth = (int)sqrt(pow(corners[0].x - corners[1].x, 2) + pow(corners[0].y - corners[1].y, 2));
    printf("newWidth: %d\n", newWidth);
    int newHeight = (int)sqrt(pow(corners[0].x - corners[2].x, 2) + pow(corners[0].y - corners[2].y, 2));
    printf("newHeight: %d\n", newHeight);
    SDL_Surface *tempImage = SDL_CreateRGBSurface(0, newWidth, newHeight, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);

    // Iterate through each pixel
    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            int srcX = (int)((double)x / newWidth * w);
            int srcY = (int)((double)y / newHeight * h);
            Uint32 pixel = getPixel(image, srcX, srcY);
            *((Uint32 *)tempImage->pixels + y * newWidth + x) = pixel;
        }
    }

    SDL_BlitSurface(tempImage, NULL, image, NULL);

    SDL_FreeSurface(tempImage);
}

void contrastCorrection(SDL_Surface *image, double contrast)
{
    for (int i = 0; i < image->w * image->h; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(getPixel(image, i % image->w, i / image->w), image->format, &r, &g, &b);
        double newR = contrast * (r - 128) + 128;
        double newG = contrast * (g - 128) + 128;
        double newB = contrast * (b - 128) + 128;
        if (newR > 255)
        {
            newR = 255;
        }
        else if (newR < 0)
        {
            newR = 0;
        }
        if (newG > 255)
        {
            newG = 255;
        }
        else if (newG < 0)
        {
            newG = 0;
        }
        if (newB > 255)
        {
            newB = 255;
        }
        else if (newB < 0)
        {
            newB = 0;
        }
        setPixel(image, i % image->w, i / image->w, SDL_MapRGB(image->format, newR, newG, newB));
    }
}

void otsuTresholding(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;

    int histogram[256];
    memset(histogram, 0, 256 * sizeof(int));

    // Compute the histogram
    for (int i = 0; i < width * height; i++)
    {
        Uint8 intensity;
        SDL_GetRGB(getPixel(image, i % width, i / width), image->format, &intensity, &intensity, &intensity);
        histogram[intensity]++;
    }

    // Compute the total number of pixels
    int total = width * height;

    double sum = 0;
    for (int i = 0; i < 256; i++)
    {
        sum += i * histogram[i];
    }

    double sumB = 0;
    int wB = 0;
    int wF = 0;

    double varMax = 0;
    int threshold = 0;

    for (int i = 0; i < 256; i++)
    {
        wB += histogram[i];
        if (wB == 0)
        {
            continue;
        }

        wF = total - wB;
        if (wF == 0)
        {
            break;
        }

        sumB += (double)(i * histogram[i]);

        double mB = sumB / wB;
        double mF = (sum - sumB) / wF;

        double varBetween = (double)wB * (double)wF * (mB - mF) * (mB - mF);

        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = i;
        }
    }

    binarizeImage(image, threshold);
}

Stack *createStack()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->size = 0;
    stack->next = NULL;
    return stack;
}

bool isEmpty(Stack *stack)
{
    return stack->size == 0;
}

void push(Stack **stack, Point *point)
{
    Stack *newStack = (Stack *)malloc(sizeof(Stack));
    newStack->point.x = point->x;
    newStack->point.y = point->y;
    newStack->size = (*stack)->size + 1;
    newStack->next = *stack;
    *stack = newStack;
}

Point *pop(Stack **stack)
{
    if (isEmpty(*stack))
    {
        return NULL;
    }

    Point *point = (Point *)malloc(sizeof(Point));
    point->x = (*stack)->point.x;
    point->y = (*stack)->point.y;

    Stack *newStack = (*stack)->next;
    free(*stack);
    *stack = newStack;

    return point;
}

Point *peek(Stack *stack)
{
    if (isEmpty(stack))
    {
        return NULL;
    }

    Point *point = (Point *)malloc(sizeof(Point));
    point->x = stack->point.x;
    point->y = stack->point.y;

    return point;
}

void freeStack(Stack *stack)
{
    while (!isEmpty(stack))
    {
        pop(&stack);
    }
    free(stack);
}

void floodFill(SDL_Surface *image, int x, int y, Color targetColor, Color newColor, int *intensity)
{
    int w = image->w;
    int h = image->h;

    Stack *stack = createStack();
    Point *point = (Point *)malloc(sizeof(Point));
    point->x = x;
    point->y = y;
    push(&stack, point);
    free(point);
    while (!isEmpty(stack))
    {
        Point *current = pop(&stack);
        if (current->x < 0 || current->x >= w || current->y < 0 || current->y >= h)
        {
            free(current);
            continue;
        }
        Uint8 r, g, b;
        SDL_GetRGB(getPixel(image, current->x, current->y), image->format, &r, &g, &b);
        if (r != targetColor.r || g != targetColor.g || b != targetColor.b)
        {
            free(current);
            continue;
        }
        (*intensity)++;
        setPixel(image, current->x, current->y, SDL_MapRGB(image->format, newColor.r, newColor.g, newColor.b));
        Point *up = (Point *)malloc(sizeof(Point));
        up->x = current->x;
        up->y = current->y - 1;
        push(&stack, up);

        Point *down = (Point *)malloc(sizeof(Point));
        down->x = current->x;
        down->y = current->y + 1;
        push(&stack, down);

        Point *left = (Point *)malloc(sizeof(Point));
        left->x = current->x - 1;
        left->y = current->y;
        push(&stack, left);

        Point *right = (Point *)malloc(sizeof(Point));
        right->x = current->x + 1;
        right->y = current->y;
        push(&stack, right);

        free(current);
        free(up);
        free(down);
        free(left);
        free(right);
    }
    freeStack(stack);
}

void crampthresholding(SDL_Surface *image, Color *colors, int *intensities)
{
    int w = image->w;
    int h = image->h;

    for (int i = 0; i < w * h; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(getPixel(image, i % w, i / w), image->format, &r, &g, &b);
        if (r == 255 && g == 255 && b == 255)
        {
            Color white = {255, 255, 255};
            Color color;
            color.r = (rand() % 254) + 1;
            color.g = (rand() % 254) + 1;
            color.b = (rand() % 254) + 1;
            colors[i] = color;
            int intensity = 0;
            floodFill(image, i % w, i / w, white, color, &intensity);
            intensities[i] = intensity;
        }
    }
}

Point *findCorners(SDL_Surface *image, Color mostFrequentColor)
{
    int w = image->w;
    int h = image->h;

    Point *corners = (Point *)malloc(4 * sizeof(Point));

    Point *topLeft = (Point *)malloc(sizeof(Point));
    topLeft->x = w;
    topLeft->y = h;

    Point *topRight = (Point *)malloc(sizeof(Point));
    topRight->x = 0;
    topRight->y = h;

    Point *bottomLeft = (Point *)malloc(sizeof(Point));
    bottomLeft->x = w;
    bottomLeft->y = 0;

    Point *bottomRight = (Point *)malloc(sizeof(Point));
    bottomRight->x = 0;
    bottomRight->y = 0;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            if (!isSameColor(image, x, y, mostFrequentColor))
            {
                continue;
            }

            // find the pixel with the lowest x and y
            if (x + y < topLeft->x + topLeft->y)
            {
                topLeft->x = x;
                topLeft->y = y;
            }

            // find the pixel with the highest x and lowest y
            if (x - y > topRight->x - topRight->y)
            {
                topRight->x = x;
                topRight->y = y;
            }

            // find the pixel with the lowest x and highest y
            if (x - y < bottomLeft->x - bottomLeft->y)
            {
                bottomLeft->x = x;
                bottomLeft->y = y;
            }

            // find the pixel with the highest x and y
            if (x + y > bottomRight->x + bottomRight->y)
            {
                bottomRight->x = x;
                bottomRight->y = y;
            }
        }
    }

    corners[0] = *topLeft;
    corners[1] = *topRight;
    corners[2] = *bottomLeft;
    corners[3] = *bottomRight;

    free(topLeft);
    free(topRight);
    free(bottomLeft);
    free(bottomRight);

    return corners;
}

Point findCenter(Point *corners)
{
    Point center;

    center.x = (corners[0].x + corners[1].x + corners[2].x + corners[3].x) / 4;
    center.y = (corners[0].y + corners[1].y + corners[2].y + corners[3].y) / 4;

    return center;
}

double bilinearly_interpolate(unsigned int top, unsigned int bottom,
                              unsigned int left, unsigned int right,
                              double horizontal_position,
                              double vertical_position, Uint32 *pixels)
{
    int height = bottom - top;
    int width = right - left;
    // Determine the values of the corners.
    double top_left = pixels[top * width + left];
    double top_right = pixels[top * width + right];
    double bottom_left = pixels[bottom * width + left];
    double bottom_right = pixels[bottom * width + right];

    // Figure out "how far" the output pixel being considered is
    // between *_left and *_right.
    double horizontal_progress = horizontal_position - (double)left;
    double vertical_progress = vertical_position - (double)top;

    // Combine top_left and top_right into one large, horizontal
    // block.
    double top_block = top_left + horizontal_progress * (top_right - top_left);

    // Combine bottom_left and bottom_right into one large, horizontal
    // block.
    double bottom_block =
        bottom_left + horizontal_progress * (bottom_right - bottom_left);

    // Combine the top_block and bottom_block using vertical
    // interpolation and return as the resulting pixel.
    return top_block + vertical_progress * (bottom_block - top_block);
}

double findRotationAngle(Point *corners)
{
    Point topLeft = corners[0];
    Point topRight = corners[1];
    Point bottomLeft = corners[2];
    Point bottomRight = corners[3];

    double angleTop = atan2(topRight.y - topLeft.y, topRight.x - topLeft.x) * 180.0f / M_PI;
    double angleBottom = atan2(bottomRight.y - bottomLeft.y, bottomRight.x - bottomLeft.x) * 180.0f / M_PI;

    double angle = (angleTop + angleBottom) / 2.0f;

    if (360.0f - angle > 360.0f)
    {
        return 360.0f - angle - 360.0f;
    }

    return 360.0f - angle;
}

void autoContrast(SDL_Surface *image)
{
    double bestContrastValue = 1.8;

    int maxGrayValue = 0;
    int minGrayValue = 255;

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(getPixel(image, x, y), image->format, &r, &g, &b);
            int grayValue = (r + g + b) / 3;
            if (grayValue > maxGrayValue)
            {
                maxGrayValue = grayValue;
            }
            if (grayValue < minGrayValue)
            {
                minGrayValue = grayValue;
            }
        }
    }

    bestContrastValue = ((double)maxGrayValue + (double)minGrayValue) / 2.0 / 128.0;

    contrastCorrection(image, bestContrastValue);
}