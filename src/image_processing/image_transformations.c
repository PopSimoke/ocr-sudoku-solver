#include "image_transformations.h"
#include "../neural_network/matrix.h"
#include "pixel_operations.h"
#include <math.h>
#include <stdlib.h>
#define SIZE 9
#define POINTS_COUNT 4

#include <math.h>
#include <stdio.h>

#define EPSILON 0.000001

int min_int(int a, int b) {
  return a < b ? a : b;
}

int max_int(int a, int b) {
  return a > b ? a : b;
}

Matrix *powerMatrixMethod(Matrix *A) {

  // Create the 1 column matrix
  double x_array[SIZE] = {1};
  Matrix *X = create_new_matrix_from_array(x_array, SIZE);
  Matrix *Y = create_new_matrix(SIZE, 1);

  double lambda = 0;

  while (true) {
    matrix_dot_multiply(A, X, Y);

    // Find the max value
    double max = 0;
    for (int i = 0; i < SIZE; i++) {
      if (fabs(Y->data[i]) > max)
        max = fabs(Y->data[i]);
    }

    // divide each value from Y by lambda and store them into X
    double lambda1 = max;
    for (int i = 0; i < SIZE; i++) {
      X->data[i] = Y->data[i] / lambda1;
    }

    // Get the delta lambda and compare it to EPSION
    if (fabs(lambda1 - lambda) <= EPSILON)
      break;

    // Assign the old lambda to the new one
    lambda = lambda1;
  }

  matrix_destructor(Y);
  return X;
}

void invertMatrix(double C[SIZE][SIZE], double A[SIZE][SIZE]) {

  // C=I (identity matrix)

  int i, j, k;
  double norm, oper, constant;

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      if (i == j) {
        C[i][j] = 1;
      } else {
        C[i][j] = 0;
      }
    }
  }

  for (i = 0; i < SIZE; i++) {
    // multiply row i in matrix A and I by 1/A[i][i]
    norm = A[i][i];
    for (j = 0; j < SIZE; j++) {
      A[i][j] /= norm;
      C[i][j] /= norm;
    }

    // row operation to make principal diagonal to 1
    for (j = 0; j < SIZE; j++) {
      if (j != i) {
        // Add -A[j][i]*rowi to rowj in matrix A and I
        constant = -A[j][i];
        for (k = 0; k < SIZE; k++) {
          A[j][k] += constant * A[i][k];
          C[j][k] += constant * C[i][k];
        }
      }
    }
  }
}

SDL_Surface *remove_perspective(SDL_Surface *src_image, SDL_Point corners[4]) {
  
  // Find the new sudoku corners
  SDL_Point upper_left_corner = { min_int(corners[0].x, corners[2].x), min_int(corners[0].y, corners[1].y) };
  SDL_Point bottom_right_corner = { max_int(corners[1].x, corners[3].x), max_int(corners[2].y, corners[3].y) };
  
  if (bottom_right_corner.x - upper_left_corner.x > bottom_right_corner.y - upper_left_corner.y) {
    // width > height
    // increase the height
    bottom_right_corner.y = upper_left_corner.y + bottom_right_corner.x - upper_left_corner.x;
  } else {
    // height > width
    // increase the width
    bottom_right_corner.x = upper_left_corner.x + bottom_right_corner.y - upper_left_corner.y;
  }
  
  SDL_Point dst_points[4] = {
    upper_left_corner,
    { bottom_right_corner.x, upper_left_corner.y },
    { upper_left_corner.x, bottom_right_corner.y },
    bottom_right_corner
  };
  
  size_t sudoku_width = bottom_right_corner.y - upper_left_corner.y;
  int x_translation = -upper_left_corner.x;
  int y_translation = -upper_left_corner.y;
  
  for (size_t i = 0; i < 4; i++) {
    dst_points[i].x += x_translation;
    dst_points[i].y += y_translation;
  }

  // Init the matrix
  Matrix *A = create_new_zeros_matrix(POINTS_COUNT * 2, SIZE);
  Matrix *A_t = create_new_zeros_matrix(SIZE, POINTS_COUNT * 2);
  Matrix *A_txA = create_new_zeros_matrix(SIZE, SIZE);

  // fill the Matrix A with the coordinates of the source and destination points
  for (size_t point_index = 0; point_index < POINTS_COUNT * 2;
       point_index += 2) {
    double x_s = corners[point_index / 2].x;
    double y_s = corners[point_index / 2].y;
    double x_d = dst_points[point_index / 2].x;
    double y_d = dst_points[point_index / 2].y;

    set_matrix_element(A, point_index, 0, -x_s);
    set_matrix_element(A, point_index, 1, -y_s);
    set_matrix_element(A, point_index, 2, -1);
    set_matrix_element(A, point_index, 3, 0);
    set_matrix_element(A, point_index, 4, 0);
    set_matrix_element(A, point_index, 5, 0);
    set_matrix_element(A, point_index, 6, x_d * x_s);
    set_matrix_element(A, point_index, 7, x_d * y_s);
    set_matrix_element(A, point_index, 8, x_d);

    set_matrix_element(A, point_index + 1, 0, 0);
    set_matrix_element(A, point_index + 1, 1, 0);
    set_matrix_element(A, point_index + 1, 2, 0);
    set_matrix_element(A, point_index + 1, 3, -x_s);
    set_matrix_element(A, point_index + 1, 4, -y_s);
    set_matrix_element(A, point_index + 1, 5, -1);
    set_matrix_element(A, point_index + 1, 6, y_d * x_s);
    set_matrix_element(A, point_index + 1, 7, y_d * y_s);
    set_matrix_element(A, point_index + 1, 8, y_d);
  }

  matrix_transpose(A, A_t);
  matrix_dot_multiply(A_t, A, A_txA);

  // Convert the matrix A_txA into a list of list
  double A_txA_as_list[SIZE][SIZE] = {0};

  for (size_t row = 0; row < SIZE; row++) {
    for (size_t col = 0; col < SIZE; col++) {
      A_txA_as_list[row][col] = A_txA->data[row * SIZE + col];
    }
  }

  // Create the matrix that will store the inverse of A_txA and initializa it
  // with the identity matrix
  double M_as_list[SIZE][SIZE] = {0};

  for (size_t row = 0; row < SIZE; row++) {
    for (size_t col = 0; col < SIZE; col++) {
      M_as_list[row][col] = row == col ? 1 : 0;
    }
  }

  invertMatrix(M_as_list, A_txA_as_list);

  // Convert M to a Matrix struct
  Matrix *M = create_new_zeros_matrix(SIZE, SIZE);

  for (size_t row = 0; row < SIZE; row++) {
    for (size_t col = 0; col < SIZE; col++) {
      M->data[row * SIZE + col] = M_as_list[row][col];
    }
  }

  // Compute the eigenvector of the smallest eigenvalue with the inverse power
  // algorithm
  Matrix *H = powerMatrixMethod(M);

  SDL_Surface *dst_image =
      SDL_CreateRGBSurface(0, sudoku_width, sudoku_width, 32, 0, 0, 0, 0);

  for (size_t x = 0; x < src_image->w; x++) {
    for (size_t y = 0; y < src_image->h; y++) {

      Uint32 *pixels = (Uint32 *)dst_image->pixels;

      double divisor = H->data[6] * x + H->data[7] * y + H->data[8];
      int x_d = ((H->data[0] * x + H->data[1] * y + H->data[2]) / divisor);
      int y_d = ((H->data[3] * x + H->data[4] * y + H->data[5]) / divisor);

      if (x_d < 0 || x_d >= dst_image->w || y_d < 0 || y_d >= dst_image->h)
        continue;

      Uint32 *pixel = src_image->pixels + y * src_image->pitch +
                      x * src_image->format->BytesPerPixel;

      Uint32 *target_pixel = dst_image->pixels + y_d * dst_image->pitch +
                             x_d * dst_image->format->BytesPerPixel;

      *target_pixel = *pixel;
    }
  }

  matrix_destructor(M);
  matrix_destructor(H);
  matrix_destructor(A_t);
  matrix_destructor(A_txA);
  matrix_destructor(A);

  return dst_image;
}