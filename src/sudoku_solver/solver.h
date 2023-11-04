#ifndef TEST_H
#define TEST_H
#define N 9
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

#endif
