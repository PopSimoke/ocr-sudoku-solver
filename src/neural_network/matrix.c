
#include "matrix.h"
#include "neural_network_utils.h"
#include <stdlib.h>
#include <err.h>

// Create a new matrix with the given lines and columns
Matrix *create_new_matrix(size_t line_count, size_t column_count) {
    Matrix *matrix = malloc(sizeof(struct Matrix));
    matrix->line_count = line_count;
    matrix->column_count = column_count;
    matrix->data = (double *) calloc(line_count * column_count, sizeof(double));
    return matrix;
}

// Create a new matrix filled with random doubles from -1 to 1
Matrix *create_new_random_matrix(size_t line_count, size_t column_count) {
    Matrix *matrix = create_new_matrix(line_count, column_count);

    for (size_t i = 0; i < line_count * column_count; i++) {
        *(matrix->data + i) = generate_random_double();
    }

    return matrix;
}

// Create a new matrix filled with zeros
Matrix *create_new_zeros_matrix(size_t line_count, size_t column_count) {
    Matrix *matrix = create_new_matrix(line_count, column_count);

    for (size_t i = 0; i < line_count * column_count; i++) {
        *(matrix->data + i) = 0;
    }

    return matrix;
}

// Create a new column matrix from an array
Matrix *create_new_matrix_from_array(const double list[], size_t length) {
    Matrix *matrix = create_new_matrix(length, 1);

    for (size_t i = 0; i < length; i++)
        *(matrix->data + i) = list[i];

    return matrix;
}

// Get an element from a matrix by its line and column
double *get_matrix_element(Matrix *matrix, size_t line, size_t column) {
    return matrix->data + line * matrix->column_count + column;
}

// Set an element from a matrix by its line and column
void set_matrix_element(Matrix *matrix, size_t line, size_t column, double element) {
    *(matrix->data + line * matrix->column_count + column) = element;
}

// Dot multiply two matrix with compatible dimensions
void matrix_dot_multiply(Matrix *m1, Matrix *m2, Matrix *result) {

    if (m1->column_count != m2->line_count)
        errx(1, "Can't dot multiply two matrix of incompatible dimension: m1(%lu,%lu) and m2(%lu,%lu)",
             m1->line_count, m1->column_count, m2->line_count, m2->column_count);

    for (size_t line = 0; line < m1->line_count; line++)
        for (size_t column = 0; column < m2->column_count; column++) {
            double sum = 0;
            for (size_t i = 0; i < m1->column_count; i++)
                sum += *get_matrix_element(m1, line, i) * *get_matrix_element(m2, i, column);
            set_matrix_element(result, line, column, sum);
        }
}

// Multiply two matrix with the same dimensions
void matrix_multiply(Matrix *m1, Matrix *m2, Matrix *result) {

    if (m1->line_count != m2->line_count || m1->column_count != m2->column_count)
        errx(1, "Can't multiply two matrix of different dimensions: m1(%lu,%lu) and m2(%lu,%lu)",
             m1->line_count, m1->column_count, m2->line_count, m2->column_count);

    for (size_t i = 0; i < m1->column_count * m1->line_count; i++)
        *(result->data + i) = *(m1->data + i) * *(m2->data + i);
}

// Multiply a matrix with a double
void matrix_multiply_by_double(Matrix *m1, double factor, Matrix *result) {

    for (size_t i = 0; i < m1->column_count * m1->line_count; i++)
        *(result->data + i) = *(m1->data + i) * factor;
}

// Add two matrix with the same dimension
void matrix_add(Matrix *m1, Matrix *m2, Matrix *result) {

    if (m1->line_count != m2->line_count || m1->column_count != m2->column_count)
        errx(1, "Can't add two matrix of different dimensions: m1(%lu,%lu) and m2(%lu,%lu)",
             m1->line_count, m1->column_count, m2->line_count, m2->column_count);

    for (size_t i = 0; i < m1->column_count * m1->line_count; i++)
        *(result->data + i) = *(m1->data + i) + *(m2->data + i);
}

// Subtract two matrix with the same dimension
void matrix_subtract(Matrix *m1, Matrix *m2, Matrix *result) {

    if (m1->line_count != m2->line_count || m1->column_count != m2->column_count)
        errx(1, "Can't subtract two matrix of different dimensions: m1(%lu,%lu) and m2(%lu,%lu)",
             m1->line_count, m1->column_count, m2->line_count, m2->column_count);

    for (size_t i = 0; i < m1->column_count * m1->line_count; i++)
        *(result->data + i) = *(m1->data + i) - *(m2->data + i);
}

// Transpose matrix
void matrix_transpose(Matrix *m, Matrix *result) {
    for (size_t col = 0; col < m->column_count; col++)
        for (size_t row = 0; row < m->line_count; row++)
            set_matrix_element(result, col, row, *get_matrix_element(m, row, col));
}

// Copy m1 to m2
void matrix_copy(Matrix *m1, Matrix *m2) {

    if (m1->line_count != m2->line_count || m1->column_count != m2->column_count)
        errx(1, "Can't copy a matrix to another one with different dimensions: m1(%lu,%lu) and m2(%lu,%lu)",
             m1->line_count, m1->column_count, m2->line_count, m2->column_count);

    for (size_t i = 0; i < m1->column_count * m1->line_count; i++)
        *(m2->data + i) = *(m1->data + i);
}

void matrix_destructor(Matrix *matrix) {
    free(matrix->data);
    free(matrix);
}