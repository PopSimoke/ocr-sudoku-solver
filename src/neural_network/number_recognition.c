#include "number_recognition.h"
#include "../image_processing/pixel_operations.h"
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

      Uint8 r, g, b;

      // Get the pixel data at the i position
      Uint32 pixel = ((Uint32 *)sudoku_case->pixels)[i];

      // Extract the RGBA values from the pixel
      SDL_GetRGB(pixel, sudoku_case->format, &r, &g, &b);

      if (r == 255 && g == 255 && b == 255) {
        inputs[i] = 1;
        printf("1");
      } else {
        printf("0");
      }
    }
    printf("\n");
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

    for (size_t i = 1; i < OUTPUT_LAYER_SIZE; i++) {
      if (outputs[max_index] < outputs[i])
        max_index = i;
      //      printf("%f\n", outputs[i]);
    }
    //    printf("\n");

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

void generate_target(char number, double res[OUTPUT_LAYER_SIZE]) {
  for (char i = 0; i < OUTPUT_LAYER_SIZE; i++)
    res[i] = 0;

  res[number - 1] = 1;
}

void cast_inputs(char inputs_c[INPUT_LAYER_SIZE],
                 double inputs_d[INPUT_LAYER_SIZE]) {
  for (size_t i = 0; i < INPUT_LAYER_SIZE; i++) {
    //    printf("%d", inputs_c[i]);
    inputs_d[i] = (double)inputs_c[i];
  }
}

void print_list(char *data, size_t size) { printf("|%-40s|\n", data); }

int ai_wrapper_train(char *model_path, size_t iteration_count,
                     char *dataset_path, size_t dataset_size) {

  Neural_Network *nn =
      load_neural_network(model_path, LEARNING_RATE, INPUT_LAYER_SIZE,
                          OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);

  // open the dataset
  FILE *fptr;

  fptr = fopen(dataset_path, "rb");

  if (fptr == NULL) {
    printf("Can't read from the dataset at: %s", dataset_path);
    return EXIT_FAILURE;
  }

  // train for each image
  size_t old_percent = 101;
  for (size_t i = 0; i < iteration_count; i++) {
    size_t new_percent = (double)((double)i / (double)iteration_count) * 100;
    if (old_percent != new_percent) {
      old_percent = new_percent;
      printf("%3lu%% / 100%%\n", new_percent);
    }
    for (size_t i = 0; i < dataset_size; i++) {
      char inputs[1025];

      fread(inputs, 1025, 1, fptr);
      double targets[OUTPUT_LAYER_SIZE] = {0};
      double d_inputs[INPUT_LAYER_SIZE] = {0};
      generate_target(inputs[0], targets);
      cast_inputs(inputs + 1, d_inputs);

      train(nn, d_inputs, targets, 1);
    }
    rewind(fptr);
  }

  if (old_percent != 100)
    printf("100%% / 100%%\n");

  save_neural_network(nn, model_path);

  neural_network_destructor(nn);

  return EXIT_SUCCESS;
}

int create_dataset_from_images(char *images_dir_path, char *dataset_path) {

  FILE *fptr;
  fptr = fopen(dataset_path, "wb");
  Color white = {255, 255, 255};

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(images_dir_path)) == NULL) {
    printf("Can't get the directory at: %s\n", images_dir_path);
    return EXIT_FAILURE;
  }

  // Get each case of the sudoku grid
  while ((ent = readdir(dir)) != NULL) {
    if (*ent->d_name == '.')
      continue;

    // Load the image
    //    char *image_path = concat_str(images_dir_path, ent->d_name);
    char *image_path = "../image_processing/saved_images/case_0.png";
    SDL_Surface *sudoku_case = IMG_Load(image_path);
    printf("%s\n", image_path);
    //    free(image_path);
    if (sudoku_case == NULL) {
      printf("Image could not be loaded! SDL Image Error: %s\n",
             IMG_GetError());
      return EXIT_FAILURE;
    }

    // Lock the surface to directly access its pixel information
    if (SDL_MUSTLOCK(sudoku_case)) {
      SDL_LockSurface(sudoku_case);
    }

    // get the number inside the case from the file name
    char case_index = ent->d_name[0] - '0';

    char inputs[1025];

    for (size_t x = 0; x < 32; x++) {
      for (size_t y = 0; y < 32; y++) {
        if (isSameColor(sudoku_case, x, y, white)) {
          printf("coucou\n");
        }
      }
    }

    fwrite(inputs, sizeof(char), 1025, fptr);
    // Unlock the surface
    if (SDL_MUSTLOCK(sudoku_case)) {
      SDL_UnlockSurface(sudoku_case);
    }

    SDL_FreeSurface(sudoku_case);
  }

  closedir(dir);

  return EXIT_SUCCESS;
}

void ask_questions() {
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

    ai_wrapper_train(model_path, iteration_count, dataset_path, 30);
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
}

int main(int argc, char **argv) {

  create_dataset_from_images("../image_processing/trainingset/", "./dataset");
  //  ask_questions();

  return 0;
}
