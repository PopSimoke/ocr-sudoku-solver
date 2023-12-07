
#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

struct Matrix {
  size_t line_count;
  size_t column_count;
  double *data;
};

typedef struct Matrix Matrix;

/**
 * @brief Creates a new matrix with the specified number of rows and columns.
 *
 * @param line_count The number of rows in the matrix.
 * @param column_count The number of columns in the matrix.
 * @return A pointer to the newly allocated matrix.
 */
Matrix *create_new_matrix(size_t line_count, size_t column_count);

/**
 * @brief Creates a new matrix with the specified number of rows and columns,
 * filled with random values.
 *
 * @param line_count The number of rows in the matrix.
 * @param column_count The number of columns in the matrix.
 * @return A pointer to the newly allocated matrix.
 */
Matrix *create_new_random_matrix(size_t line_count, size_t column_count);

/**
 * @brief Creates a new matrix with the specified number of rows and columns,
 * filled with zeros.
 *
 * @param line_count The number of rows in the matrix.
 * @param column_count The number of columns in the matrix.
 * @return A pointer to the newly allocated matrix.
 */
Matrix *create_new_zeros_matrix(size_t line_count, size_t column_count);

/**
 * @brief Creates a new matrix from a specified array of values.
 *
 * @param list The array of values to use to fill the matrix.
 * @param length The number of elements in the array.
 * @return A pointer to the newly allocated matrix.
 */
Matrix *create_new_matrix_from_array(const double list[], size_t length);

/**
 * @brief Retrieves the element from the matrix at the specified row and column.
 *
 * @param matrix The matrix from which to extract the element.
 * @param line The row of the element.
 * @param column The column of the element.
 * @return A pointer to the matrix element.
 */
double *get_matrix_element(Matrix *matrix, size_t line, size_t column);

/**
 * @brief Sets the element of the matrix at the specified row and column with
 * the given value.
 *
 * @param matrix The matrix in which to set the element.
 * @param line The row of the element.
 * @param column The column of the element.
 * @param element The value to assign to the element.
 */
void set_matrix_element(Matrix *matrix, size_t line, size_t column,
                        double element);

/**
 * @brief Multiplies two matrices and stores the result in a third matrix.
 *
 * @param m1 The first matrix to multiply.
 * @param m2 The second matrix to multiply.
 * @param result The matrix in which to store the result of the multiplication.
 */
void matrix_dot_multiply(Matrix *m1, Matrix *m2, Matrix *result);

/**
 * @brief Multiplies two matrices and stores the result in a third matrix.
 *
 * @param m1 The first matrix to multiply.
 * @param m2 The second matrix to multiply.
 * @param result The matrix in which to store the result of the multiplication.
 */
void matrix_multiply(Matrix *m1, Matrix *m2, Matrix *result);

/**
 * @brief Multiplies a matrix by a double-precision factor and stores the result
 * in another matrix.
 *
 * @param m1 The matrix to multiply.
 * @param factor The multiplication factor.
 * @param result The matrix in which to store the result of the multiplication.
 */
void matrix_multiply_by_double(Matrix *m1, double factor, Matrix *result);

/**
 * @brief Adds two matrices and stores the result in a third matrix.
 *
 * @param m1 The first matrix to add.
 * @param m2 The second matrix to add.
 * @param result The matrix in which to store the result of the addition.
 */
void matrix_add(Matrix *m1, Matrix *m2, Matrix *result);

/**
 * @brief Subtracts two matrices and stores the result in a third matrix.
 *
 * @param m1 The first matrix to subtract.
 * @param m2 The second matrix to subtract.
 * @param result The matrix in which to store the result of the subtraction.
 */
void matrix_subtract(Matrix *m1, Matrix *m2, Matrix *result);

/**
 * @brief Transposes a matrix and stores the result in another matrix.
 *
 * @param m The matrix to transpose.
 * @param result The matrix in which to store the result of the transposition.
 */
void matrix_transpose(Matrix *m, Matrix *result);

/**
 * @brief Copies the contents of one matrix into another matrix.
 *
 * @param m1 The source matrix.
 * @param m2 The destination matrix.
 */
void matrix_copy(Matrix *m1, Matrix *m2);

/**
 * @brief Set the identity matrix to a given Matrix
 *
 * @param m The source matrix.
 */
void set_identity_matrix(Matrix *m);

/**
 * @brief Releases the memory allocated for a matrix.
 *
 * @param matrix The matrix to destroy.
 */
void matrix_destructor(Matrix *matrix);

#endif // MATRIX_H
