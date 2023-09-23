#include <stdio.h>
#include <stdlib.h>
 
#define N 9
 

int Sudoku_Erreur(int grid[N][N], int row,
                       int col, int num)
{
     
    
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;
 
    
    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;
 
    
    int startRow = row - row % 3,
                 startCol = col - col % 3;
   
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j +
                          startCol] == num)
                return 0;
 
    return 1;
}
 
int solver(int grid[N][N], int row, int col)
{
     
    
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
         
        
        if (Sudoku_Erreur(grid, row, col, num)==1)
        {
            
            grid[row][col] = num;
           
            
            if (solver(grid, row, col + 1)==1)
                return 1;
        }
       
        
        grid[row][col] = 0;
    }
    return 0;
}


int main(int argc, char *argv[])
{

    
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }
    
    int grid[N][N];
    int col = 0;
    int row = 0;
    char tempvalue;
    while (!feof(inputFile))
        {
            fscanf(inputFile, "%c" , &tempvalue);

            if (tempvalue=='1')
                {
                    grid[row][col] = 1;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                    
                }
             if (tempvalue=='2')
                {
                    grid[row][col] = 2;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='3')
                {
                    grid[row][col] = 3;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='4')
                {
                    grid[row][col] = 4;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='5')
                {
                    grid[row][col] = 5;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='6')
                {
                    grid[row][col] = 6;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='7')
                {
                    grid[row][col] = 7;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='8')
                {
                    grid[row][col] = 8;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='9')
                {
                    grid[row][col] =9;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
             if (tempvalue=='.')
                {
                    grid[row][col] = 0;
                    col+=1;
                    if (col == 9)
                        {col = 0;
                        row+=1;}
                }
            if (tempvalue==' ')
                continue;
        }
     
    fclose(inputFile);

            

    
    
    
 
    if (solver(grid, 0, 0)==1)
    {
        FILE *output ;
        
        output = fopen("output_solver", "w");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                {
                fprintf(output, "%d" ,grid[i][j]);
                if(j==2 || j == 5 || j==8)
                fprintf(output, " "); }
            if(i==2 ||i == 5 || i == 8)
                fprintf(output,"\n");
            fprintf(output,"\n");

        }
        fclose(output);
        
    }
    else
        printf("No solution exists");
 
    return 0;
}
    