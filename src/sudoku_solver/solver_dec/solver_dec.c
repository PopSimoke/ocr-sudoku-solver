#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define N 9

int Sudoku_Erreur(int grid[N][N], int row,
                  int col, int num)
{
    // Checking in row
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;

    // Checking in column
    for (int x = 0; x <= 8; x++)
    {
        if (grid[x][col] == num)
            return 0;
    }

    // Checking subgrid
    int startRow = row - row % 3,
        startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j +
                                   startCol] == num)
                return 0;

    return 1;
}


int erreur(int grid[N][N])
{
    for (int i = 1 ; i <= 9; i++)
    {
        int cpt = 0;
        for (int j = 0 ; j<N; j++)
        {
            cpt = 0;
            for (int k = 0 ; k< N ; k++)
            {
                if (grid[j][k]== i)
                    cpt++;
            }
            if (cpt>1)
                return 1;
        }
    }
    for (int i = 1 ; i <= 9; i++)
    {
        int cpt = 0;
        for (int j = 0 ; j<N; j++)
        {
            cpt = 0;
            for (int k = 0 ; k< N ; k++)
            {
                if (grid[k][j]== i)
                    cpt++;
            }
            if (cpt>1)
                return 1;
        }

    }
    return 0;
}
// Function to display the sudoku
int solver(int grid[N][N], int row, int col)
{
    if (erreur(grid)==1)
    {
        return 0;
    }
    if (row == N - 1 && col == N)
        return 1;

    if (col == N)
    {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0 && grid[row][col] <= 9)
        return solver(grid, row, col + 1);

    for (int num = 1; num <= N; num++)
    {

        if (Sudoku_Erreur(grid, row, col, num) == 1)
        {

            grid[row][col] = num;

            if (solver(grid, row, col + 1) == 1)
                return 1;
        }

        grid[row][col] = 0;
    }
    return 0;
}

SDL_Surface *createSudokuImage(int grid[N][N], int solvedGrid[N][N], unsigned int dimension, char *path)
{
    SDL_Surface *image = SDL_CreateRGBSurface(0, dimension + 20, dimension + 20, 32, 0, 0, 0, 0);
    SDL_Surface *cell_image = NULL;
    SDL_Rect dest;

    char filename[128];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (solvedGrid[i][j] != grid[i][j])

                sprintf(filename, "%sdigit-%d.bmp", path, solvedGrid[i][j]);
            else
                sprintf(filename, "%sdigit-%d-f.bmp", path, solvedGrid[i][j]);
            cell_image = SDL_LoadBMP(filename);
            if (cell_image == NULL)
            {
                printf("Unable to load image %s! SDL Error: %s\n", filename, SDL_GetError());
                return NULL;
            }
            if (i == 1)
            {
                dest.x = 94 + 15;
            }
            if (i == 2)
            {
                dest.x = 94 * 2 + 15;
            }
            if (i == 3)
            {
                dest.x = 94 * 3 + 5 + 15;
            }
            if (i == 4)
            {
                dest.x = 94 * 4 + 5 + 15;
            }
            if (i == 5)
            {
                dest.x = 94 * 5 + 5 + 15;
            }
            if (i == 6)
            {
                dest.x = 94 * 6 + 10 + 15;
            }
            if (i == 7)
            {
                dest.x = 94 * 7 + 10 + 15;
            }
            if (i == 8)
            {
                dest.x = 94 * 8 + 10 + 15;
            }
            if (i == 0)
            {
                dest.x = 0 + 15;
            }

            if (j == 1)
            {
                dest.y = 94 + 15;
            }
            if (j == 2)
            {
                dest.y = 94 * 2 + 15;
            }
            if (j == 3)
            {
                dest.y = 94 * 3 + 5 + 15;
            }
            if (j == 4)
            {
                dest.y = 94 * 4 + 5 + 15;
            }
            if (j == 5)
            {
                dest.y = 94 * 5 + 5 + 15;
            }
            if (j == 6)
            {
                dest.y = 94 * 6 + 10 + 15;
            }
            if (j == 7)
            {
                dest.y = 94 * 7 + 10 + 15;
            }
            if (j == 8)
            {
                dest.y = 94 * 8 + 10 + 15;
            }
            if (j == 0)
            {
                dest.y = 0 + 15;
            }

            SDL_BlitSurface(cell_image, NULL, image, &dest);
            SDL_FreeSurface(cell_image);
        }
    }

    return image;
}

int **readGridFromFile(char *filename)
{
    FILE *inputFile = fopen(filename, "r");
    if (inputFile == NULL)
    {
        perror("Error opening input file");
        return NULL;
    }

    int **grid = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        grid[i] = malloc(N * sizeof(int));
    }

    int col = 0;
    int row = 0;
    char tempvalue;
    while (!feof(inputFile)) // while not end of file keep reading the file and store the values in the 2D array if . then store 0
    {
        if (row == N)
        {
            break;
        }
        int err =fscanf(inputFile, "%c", &tempvalue);
        err+=1;

        if (tempvalue >= '1' && tempvalue <= '9')
        {
            grid[row][col] = tempvalue - '0';
            col += 1;
            if (col == N)
            {
                col = 0;
                row += 1;
            }
        }
        else if (tempvalue == '.')
        {
            grid[row][col] = 0;
            col += 1;
            if (col == N)
            {
                col = 0;
                row += 1;
            }
        }
    }

    fclose(inputFile);
    return grid;
}


// Main function
// int main(int argc, char *argv[])
// {

//     int** zizigrid= readGridFromFile("test.txt");
//     for (int i = 0 ; i<N;i++){
//         for (int j = 0 ; j<N;j++){
//             printf("%d",zizigrid[i][j]);
//         }
//         printf("\n");
//     }
//     // Check if the input file is valid, if it's valid, read the file and store the values in a 2D array
//     FILE *inputFile = fopen(argv[1], "r");
//     if (inputFile == NULL)
//     {
//         perror("Error opening input file");
//         return 1;
//     }
//     if (argc != 2)
//     {
//         printf("Usage: %s <input file>\n", argv[0]);
//         return 1;
//     }

//     int grid[N][N];
//     int solvedGrid[N][N];
//     int col = 0;
//     int row = 0;
//     char tempvalue;
//     while (!feof(inputFile)) // while not end of file keep reading the file and store the values in the 2D array if . then store 0
//     {
//         if (row==9){
//             break;
//         }
//         char oui = fscanf(inputFile, "%c", &tempvalue);
//         oui += 1;

//         if (tempvalue == '1')
//         {
//             grid[row][col] = 1;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '2')
//         {
//             grid[row][col] = 2;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '3')
//         {
//             grid[row][col] = 3;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '4')
//         {
//             grid[row][col] = 4;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '5')
//         {
//             grid[row][col] = 5;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '6')
//         {
//             grid[row][col] = 6;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '7')
//         {
//             grid[row][col] = 7;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '8')
//         {
//             grid[row][col] = 8;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '9')
//         {
//             grid[row][col] = 9;
//             solvedGrid[row][col] = 1;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == '.')
//         {
//             grid[row][col] = 0;
//             solvedGrid[row][col] = 0;
//             col += 1;
//             if (col == 9)
//             {
//                 col = 0;
//                 row += 1;
//             }
//         }
//         if (tempvalue == ' ')
//             continue;
//     }

//     fclose(inputFile);

//     if (solver(grid, 0, 0) == 1) // if the sudoku is solved, write the solution in a file
//     {
//         char outputFileName[strlen(argv[1]) + 8];
//         strcpy(outputFileName, argv[1]);
//         strcat(outputFileName, "_result");
//         FILE *output;
//         output = fopen(outputFileName, "w");
//         for (int i = 0; i < N; i++) // write the solution in the file
//         {
//             for (int j = 0; j < N; j++)
//             {
//                 fprintf(output, "%d", grid[i][j]);
//                 if (j == 2 || j == 5 || j == 8)
//                     fprintf(output, " ");
//             }
//             if (i == 2 || i == 5 || i == 8)
//                 fprintf(output, "\n");
//             fprintf(output, "\n");
//         }
//         fclose(output);
//     }
//     else
//         printf("No solution exists");
//     // Après avoir créé l'image du sudoku

// SDL_Surface* sudokuImage = createSudokuImage(grid,solvedGrid, 96*9);

// // Sauvegarder l'image dans le même répertoire que le fichier d'entrée
// char save_path[256];
// sprintf(save_path, "%s_sudoku.bmp", argv[1]);
// if(SDL_SaveBMP(sudokuImage, save_path) != 0) {
//     printf("Unable to save image! SDL Error: %s\n", SDL_GetError());
// } else {
//     printf("Image saved to %s\n", save_path);
// }

// // Libérer la surface de l'image
// SDL_FreeSurface(sudokuImage);
//     return 0;

// }
