#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 16

int Sudoku_Erreur(int grid[N][N], int row, int col, int num) {
    // Checking in row
    for (int x = 0; x < N; x++)
        if (grid[row][x] == num)
            return 0;

    // Checking in column
    for (int x = 0; x < N; x++)
        if (grid[x][col] == num)
            return 0;

    // Checking subgrid
    int startRow = row - row % 4, startCol = col - col % 4;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (grid[i + startRow][j + startCol] == num)
                return 0;

    return 1;
}

int solver(int grid[N][N], int row, int col) {
    if (row == N - 1 && col == N)
        return 1;

    if (col == N) {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solver(grid, row, col + 1);

    for (int num = 1; num <= N; num++) {
        if (Sudoku_Erreur(grid, row, col, num) == 1) {
            grid[row][col] = num;

            if (solver(grid, row, col + 1) == 1)
                return 1;
        }

        grid[row][col] = 0;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }
    if (argc != 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    int grid[N][N];
    int col = 0;
    int row = 0;
    char tempvalue;
    while (!feof(inputFile)) {
        char oui =fscanf(inputFile, "%c", &tempvalue);
        oui+=1;

        if (tempvalue >= '1' && tempvalue <= '9') {
            grid[row][col] = tempvalue - '0';
            col++;
            if (col == N) {
                col = 0;
                row++;
            }
        } else if (tempvalue >= 'A' && tempvalue <= 'G') {
            grid[row][col] = tempvalue - 'A' + 10;
            col++;
            if (col == N) {
                col = 0;
                row++;
            }
        } else if (tempvalue == '.') {
            grid[row][col] = 0;
            col++;
            if (col == N) {
                col = 0;
                row++;
            }
        }
    }

    fclose(inputFile);

    if (solver(grid, 0, 0) == 1) {
        char outputFileName[strlen(argv[1]) + 8];
        strcpy(outputFileName, argv[1]);
        strcat(outputFileName, "_result");
        FILE *output;
        output = fopen(outputFileName, "w");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] <= 9)
                    fprintf(output, "%d", grid[i][j]);
                else
                    fprintf(output, "%c", grid[i][j] + 'A' - 10);
                if (j % 4 == 3)
                    fprintf(output, " ");
            }
            if (i % 4 == 3)
                fprintf(output, "\n");
            fprintf(output, "\n");
        }
        fclose(output);
    } else
        printf("No solution exists");

    return 0;
}