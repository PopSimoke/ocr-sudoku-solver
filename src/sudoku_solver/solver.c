#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "solver.h"



int ask_questions() {
  
  

  printf("\n   ╔══════════════════════════╗\n   ║       Sudoku Solver      "
         "║\n   "
         "╚══════════════════════════╝\n By Optical Cramptés Recognition "
         "\n\n");

  char mode_str[128];
  
  printf("What is the Sudoku type ? \n  1: Sudoku 4x4\n  2: Sudoku 3x3\nChoice:");
  char* r3 = fgets(mode_str, 127, stdin);
    if (r3 == 0){
        r3+=1;
    }

  
  
  if (mode_str[0] == '1' ){
    char path[256];
    printf("Hexa mode 4x4\nPlease enter the path\n");
    char* r2 = fgets(path, 255, stdin);
    if (r2 == 0){
        r2+=1;
    }
    path[strcspn(path, "\n")] = 0;

    if (access(path, F_OK) != -1) {
        printf("Processing ...\n");
        if (main_hexa(path) != 0) {
            printf("Error in main_hexa.\n");
            return 1;
        }
    } 
    else {
        printf("File does not exist.\n");
    }
    printf("Done\n");
}
    // Remove newline character at the end
    
    

  if (mode_str[0] == '2' ){
    char path[256];
    printf("Classic mode 3x3\nPlease enter the path\n");
    char* r1 = fgets(path, 255, stdin);
    if (r1 == 0){
        r1+=1;
    }
 
    path[strcspn(path, "\n")] = 0;

    if (access(path, F_OK) != -1) {
        printf("Processing ...\n");
        if (main_dec(path) != 0) {
            printf("Error in main_dec.\n");
            return 1;
        }
        
    } 
    else {
        printf("File does not exist.\n");
    }
  }
  printf("Done\n");
  return 0;
}

int Sudoku_Erreur_Hexa(int grid[NHEX][NHEX], int row, int col, int num) {
    // Checking in row
    for (int x = 0; x < NHEX; x++)
        if (grid[row][x] == num)
            return 0;

    // Checking in column
    for (int x = 0; x < NHEX; x++)
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

int solver_Hexa(int grid[NHEX][NHEX], int row, int col) {
    if (row == NHEX - 1 && col == NHEX)
        return 1;

    if (col == NHEX) {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solver_Hexa(grid, row, col + 1);

    for (int num = 1; num <= NHEX; num++) {
        if (Sudoku_Erreur_Hexa(grid, row, col, num) == 1) {
            grid[row][col] = num;

            if (solver_Hexa(grid, row, col + 1) == 1)
                return 1;
        }

        grid[row][col] = 0;
    }
    return 0;
}

int main_hexa(char *path) {
    FILE *inputFile = fopen(path, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }
    
    

    int grid[16][16];
    int col = 0;
    int row = 0;
    char tempvalue;
    
   
    while (!feof(inputFile)) {

    char oui =fscanf(inputFile, "%c", &tempvalue);
    
    
    oui+=1;

    if (tempvalue >= '1' && tempvalue <= '9') {
        grid[row][col] = tempvalue - '0';
        col++;
    } else if (tempvalue >= 'A' && tempvalue <=  'G') {
        grid[row][col] = tempvalue - 'A' + 10;
       
        col++;
    } 
    else if (tempvalue == '.') {
            grid[row][col] = 0;
            col++;
            if (col == NHEX) {
                col = 0;
                row++;
            }
        }
    
    
    else if (tempvalue == ' '){
        continue;
    }
    

    if (col == NHEX) {
        col = 0;
        row++;
    }
    }
    
    
    

    fclose(inputFile);

    if (solver_Hexa(grid, 0, 0) == 1) {
        char outputFileName[strlen(path) + 8];
        strcpy(outputFileName, path);
        strcat(outputFileName, "_result");
        FILE *output;
        output = fopen(outputFileName, "w");
        for (int i = 0; i < NHEX; i++) {
            for (int j = 0; j < NHEX; j++) {
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

int Sudoku_Erreur(int grid[N][N], int row,
                  int col, int num)
{
    // Checking in row
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;

    // Checking in column
    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;

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

    if (grid[row][col] > 0)
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
// Main function
int main_dec(char *path)
{

    // Check if the input file is valid, if it's valid, read the file and store the values in a 2D array
    FILE *inputFile = fopen(path, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    int grid[9][9];
    int col = 0;
    int row = 0;
    char tempvalue;
    while (!feof(inputFile)) // while not end of file keep reading the file and store the values in the 2D array if . then store 0
    {
        if(row==9){
            break;
        }
        
        char oui = fscanf(inputFile, "%c", &tempvalue);
        oui += 1;

        if (tempvalue == '1')
        {
            grid[row][col] = 1;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '2')
        {
            grid[row][col] = 2;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '3')
        {
            grid[row][col] = 3;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '4')
        {
            grid[row][col] = 4;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '5')
        {
            grid[row][col] = 5;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '6')
        {
            grid[row][col] = 6;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '7')
        {
            grid[row][col] = 7;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '8')
        {
            grid[row][col] = 8;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '9')
        {
            grid[row][col] = 9;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == '.')
        {
            grid[row][col] = 0;
            col += 1;
            if (col == 9)
            {
                col = 0;
                row += 1;
            }
        }
        if (tempvalue == ' ')
            continue;
    }

    fclose(inputFile);

    if (solver(grid, 0, 0) == 1) // if the sudoku is solved, write the solution in a file
    {
        char outputFileName[strlen(path) + 8];
        strcpy(outputFileName, path);
        strcat(outputFileName, "_result");
        FILE *output;
        output = fopen(outputFileName, "w");

        for (int i = 0; i < N; i++) // write the solution in the file
        {
            for (int j = 0; j < N; j++)
            {
                fprintf(output, "%d", grid[i][j]);
                if (j == 2 || j == 5 || j == 8)
                    fprintf(output, " ");
            }
            if (i == 2 || i == 5 || i == 8)
                fprintf(output, "\n");
            fprintf(output, "\n");
        }
        fclose(output);
    }
    else
        printf("No solution exists\n");

    return 0;
}



int main() {
  int err = ask_questions();
  
  return err;
}

