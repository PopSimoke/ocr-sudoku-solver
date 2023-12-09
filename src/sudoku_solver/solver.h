#ifndef SOLVER_H
#define SOLVER_H
#define NHEX 16
#define N 9

int ask_questions();

int Sudoku_Erreur_Hexa(int grid[NHEX][NHEX], int row, int col, int num);

int solver_Hexa(int grid[NHEX][NHEX], int row, int col) ;

int main_hexa(char *path);

int Sudoku_Erreur(int grid[N][N], int row, int col, int num);

int solver(int grid[N][N], int row, int col) ;

int main_dec(char *path);

#endif