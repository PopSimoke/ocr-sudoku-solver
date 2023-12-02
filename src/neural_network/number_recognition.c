#include "number_recognition.h"
#include "neural_network.h"

#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

// Concat two string (useful for path concatenation)
char *concat_str(char *s1, char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

int generate_grid(Neural_Network *nn, char *input_path, char *output_path) {
  char numbers[81] = {0};
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

  return EXIT_SUCCESS;
}

int ai_wrapper_with_model(char *model_path, char *input_path,
                          char *output_path) {

  Neural_Network *nn =
      load_neural_network(model_path, LEARNING_RATE, INPUT_LAYER_SIZE,
                          OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);

  int err = generate_grid(nn, input_path, output_path);

  neural_network_destructor(nn);

  return err;
}

int ai_wrapper(char *model_path, char *input_path, char *output_path) {

  Neural_Network *nn = create_new_neural_network(
      LEARNING_RATE, INPUT_LAYER_SIZE, OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);

  int err = generate_grid(nn, input_path, output_path);

  neural_network_destructor(nn);

  return err;
}

int ai_wrapper_train(char *model_path, size_t iteration_count, char *dataset) {
  // TODO
}

int main(int argc, char **argv) {

  char *default_model_path = "./model";
  char *default_input_path = "../image_processing/saved_images/";
  char *default_output_path = "./output.txt";
  char *default_dataset_path = "./dataset";
  size_t default_iteration_count = 100;

  printf("\n   ╔══════════════════════════╗\n   ║      Neural Network      "
         "║\n   "
         "╚══════════════════════════╝\n⑀ By Optical Crampté Recognition "
         "⑀\n\n");

  char model_path[128];
  printf("Model Path [%s]: ", default_model_path);
  fgets(model_path, 127, stdin);

  if (model_path[0] == '\n') {
    memmove(model_path, default_model_path, strlen(default_model_path));
  } else
    model_path[strlen(model_path) - 1] = 0;

  char training_mode_str[128];
  printf("Training Mode [y/N]: ");
  fgets(training_mode_str, 127, stdin);

  int training_mode =
      training_mode_str[0] == 'y' || training_mode_str[0] == 'Y';

  if (training_mode) {
    char dataset_path[128];
    printf("Dataset Path [%s]: ", default_dataset_path);
    fgets(dataset_path, 127, stdin);

    if (dataset_path[0] == '\n') {
      memmove(dataset_path, default_dataset_path, strlen(default_dataset_path));
    } else
      dataset_path[strlen(dataset_path) - 1] = 0;

    char iteration_count_str[128];
    printf("Iteration Count [%lu]: ", default_iteration_count);
    fgets(iteration_count_str, 127, stdin);

    size_t iteration_count = 0;

    if (iteration_count_str[0] == '\n') {
      iteration_count = default_iteration_count;
    } else {
      iteration_count_str[strlen(iteration_count_str) - 1] = 0;
      for (size_t i = 0; i < strlen(iteration_count_str); i++) {
        iteration_count = iteration_count * 10 + (iteration_count_str[i] - '0');
      }
    }

    ai_wrapper_train(model_path, iteration_count, dataset_path);
  } else {
    char input_path[128];
    printf("Input Path [%s]: ", default_input_path);
    fgets(input_path, 127, stdin);

    if (input_path[0] == '\n') {
      memmove(input_path, default_input_path, strlen(default_input_path));
    } else
      input_path[strlen(input_path) - 1] = 0;

    char output_path[128];
    printf("Output Path [%s]: ", default_output_path);
    fgets(output_path, 127, stdin);

    if (output_path[0] == '\n') {
      memmove(output_path, default_output_path, strlen(default_output_path));
    } else
      output_path[strlen(output_path) - 1] = 0;

    ai_wrapper_with_model(model_path, input_path, output_path);
  }

  //  if (argc < 2) {
  //    errx(1, "You have to specify the iteration count for the
  //    training.");
  //  }
  //
  //  size_t iteration_count = 0;
  //  for (size_t i = 0; i < strlen(argv[1]); i++) {
  //    iteration_count = iteration_count * 10 + (argv[1][i] - '0');
  //  }
  //
  //  //    Neural_Network *nn =
  //  create_new_neural_network(learning_rate);
  //
  //  char *model_path = "./model";
  //  printf("Loading model from: %s\n", model_path);
  //  Neural_Network *nn =
  //      load_neural_network(model_path, LEARNING_RATE,
  //      INPUT_LAYER_SIZE,
  //                          OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);
  //
  //  char *input_path = "../image_processing/saved_images/";
  //  char *output_path = "./output.txt";
  //
  //  save_neural_network(nn, model_path);
  //  printf("Model saved at : %s\n", model_path);
  //
  //  // free the allocated resources
  //  neural_network_destructor(nn);
  return 0;
}
