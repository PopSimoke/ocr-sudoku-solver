#ifndef SOLVER_DEC_H
#define SOLVER_DEC_H
#define N 9
#include <SDL2/SDL.h>
/**
 * @brief check if the grid doesn't contain any errors
 *
 * @param grid //sudoku grid
 * @param row  //line
 * @param col  //column
 * @param num //number
 * @return int
 */
int Sudoku_Erreur(int grid[N][N], int row, int col, int num);

/**
 * @brief this function is a solver function with backtracking
 *
 * @param grid //the sudoku grid
 * @param row
 * @param col
 * @return int
 */
int solver(int grid[N][N], int row, int col);

SDL_Surface* createSudokuImage(int grid[N][N],int solvedGrid[N][N],unsigned int dimension);


#endif
