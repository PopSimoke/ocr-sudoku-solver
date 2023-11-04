#ifndef TEST_H
#define TEST_H
#define N 9
int Sudoku_Erreur(int grid[N][N], int row,
                       int col, int num);
int solver(int grid[N][N], int row, int col);
#endif
