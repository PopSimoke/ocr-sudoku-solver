
#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

struct Matrix {
    size_t line_count;
    size_t column_count;
    double *data;
};

typedef struct Matrix Matrix;

Matrix *create_new_matrix(size_t line_count, size_t column_count);

Matrix *create_new_random_matrix(size_t line_count, size_t column_count);

Matrix *create_new_zeros_matrix(size_t line_count, size_t column_count);

Matrix *create_new_matrix_from_array(const double list[], size_t length);

double *get_matrix_element(Matrix *matrix, size_t line, size_t column);

void set_matrix_element(Matrix *matrix, size_t line, size_t column, double element);

void matrix_dot_multiply(Matrix *m1, Matrix *m2, Matrix *result);

void matrix_add(Matrix *m1, Matrix *m2, Matrix *result);

void matrix_destructor(Matrix *matrix);

#endif //MATRIX_H
