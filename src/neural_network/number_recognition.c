#include <SDL2/SDL_surface.h>
#define INPUT_LAYER_SIZE 1024
#define HIDDEN_LAYER_SIZE 100
#define OUTPUT_LAYER_SIZE 9

#include "neural_network.h"
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Concat two string (useful for path concatenation)
char *concat_str(char *s1, char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

// Convert the grid case images into exploitable data for the ai
int compute_grid(Neural_Network *nn, char *input_path, char *output_path) {
  char *numbers = malloc(81 * sizeof(char));
  memset(numbers, '.', 81);
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(input_path)) == NULL) {
    printf("Can't get the directory at: %s\n", input_path);
    return EXIT_FAILURE;
  }

  // Get each case of the sudoku grid
  while ((ent = readdir(dir)) != NULL) {
    if (*ent->d_name == '.')
      continue;

    // Load the image
    char *image_path = concat_str(input_path, ent->d_name);
    SDL_Surface *sudoku_case = IMG_Load(image_path);
    free(image_path);
    if (sudoku_case == NULL) {
      printf("Image could not be loaded! SDL Image Error: %s\n",
             IMG_GetError());
      return EXIT_FAILURE;
    }

    // Lock the surface to directly access its pixel information
    if (SDL_MUSTLOCK(sudoku_case)) {
      SDL_LockSurface(sudoku_case);
    }

    double inputs[INPUT_LAYER_SIZE] = {0};

    // get the index of the case from the file name
    size_t case_index = 0;
    size_t c_i = 5;
    while (ent->d_name[c_i] >= '0' && ent->d_name[c_i] <= '9') {
      case_index = case_index * 10 + ent->d_name[c_i] - '0';
      c_i += 1;
    }

    for (size_t i = 0; i < INPUT_LAYER_SIZE; i++) {

      Uint8 r, g, b, a;

      // Get the pixel data at the i position
      Uint32 pixel = ((Uint32 *)sudoku_case->pixels)[i];

      // Extract the RGBA values from the pixel
      SDL_GetRGBA(pixel, sudoku_case->format, &r, &g, &b, &a);

      if (r == 255 && g == 255 && b == 255) {
        inputs[i] = 1;
      }
    }
    // Unlock the surface
    if (SDL_MUSTLOCK(sudoku_case)) {
      SDL_UnlockSurface(sudoku_case);
    }

    SDL_FreeSurface(sudoku_case);

    // get the output from the ai
    double outputs[OUTPUT_LAYER_SIZE] = {0};
    predict(nn, inputs, outputs);

    // get the max from the output
    size_t max_index = 0;

    for (size_t i = 1; i < OUTPUT_LAYER_SIZE; i++)
      if (outputs[max_index] < outputs[i])
        max_index = i;

    // Store the index (aka identified number) into the numbers list
    numbers[case_index] = max_index + '1';
  }

  closedir(dir);

  // Save the reconstructed grid into a new file
  FILE *fptr;
  fptr = fopen(output_path, "w");

  for (size_t y = 0; y < 9; y++) {
    for (size_t x = 0; x < 9; x++) {
      if (x == 3 || x == 6)
        fprintf(fptr, " %c", numbers[y * 9 + x]);
      else
        fprintf(fptr, "%c", numbers[y * 9 + x]);
    }
    if (y == 2 || y == 5)
      fprintf(fptr, "\n\n");
    else
      fprintf(fptr, "\n");
  }

  fclose(fptr);
  free(numbers);

  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {

  if (argc != 2) {
    errx(1, "You have to specify the iteration count for the training.");
  }

  double learning_rate = 0.5;
  size_t iteration_count = 0;
  for (size_t i = 0; i < strlen(argv[1]); i++) {
    iteration_count = iteration_count * 10 + (argv[1][i] - '0');
  }

  //    Neural_Network *nn = create_new_neural_network(learning_rate);

  char *path = "./model";
  Neural_Network *nn =
      load_neural_network(path, learning_rate, INPUT_LAYER_SIZE,
                          OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);

  char *input_path = "../image_processing/saved_images/";
  char *output_path = "./output.txt";
  compute_grid(nn, input_path, output_path);

  save_neural_network(nn, path);
  printf("Model saved at : %s\n", path);

  // free the allocated resources
  neural_network_destructor(nn);
  return 0;
}
