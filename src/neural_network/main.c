#include "number_recognition.h"
#include <stdio.h>
#include <string.h>

int ask_questions() {
  char default_mode = 1;
  char *default_model_path = "./model";
  char *default_input_path = "../image_processing/saved_images/";
  char *default_output_path = "./output.txt";
  char *default_dataset_path = "./dataset";
  char *default_training_folder_path = "../image_processing/trainingset/";
  size_t default_iteration_count = 100;

  printf("\n   ╔══════════════════════════╗\n   ║      Neural Network      "
         "║\n   "
         "╚══════════════════════════╝\n⑀ By Optical Crampté Recognition "
         "⑀\n\n");

  char mode_str[128];
  int mode = default_mode;
  printf("Mode \n  1: reading\n  2: training\n  3: dataset \n[%d]: ",
         default_mode);
  fgets(mode_str, 127, stdin);

  if (mode_str[0] >= '0' && mode_str[0] <= '9')
    mode = mode_str[0] - '0';

  if (mode == 3) {
    // Dataset creation

    char training_folder_path[128];
    printf("Training folder path [%s]: ", default_training_folder_path);
    fgets(training_folder_path, 127, stdin);

    if (training_folder_path[0] == '\n') {
      memmove(training_folder_path, default_training_folder_path,
              strlen(default_training_folder_path));
    } else
      training_folder_path[strlen(training_folder_path) - 1] = 0;

    char dataset_path[128];
    printf("Dataset path [%s]: ", default_dataset_path);
    fgets(dataset_path, 127, stdin);

    if (dataset_path[0] == '\n') {
      memmove(dataset_path, default_dataset_path, strlen(default_dataset_path));
    } else
      dataset_path[strlen(dataset_path) - 1] = 0;

    int dataset_size = 0;
    int err = create_dataset_from_images(training_folder_path, dataset_path,
                                         &dataset_size);
    printf("Dataset size: %d\n", dataset_size);
    return err;
  }

  char model_path[128];
  printf("Model path [%s]: ", default_model_path);
  fgets(model_path, 127, stdin);

  if (model_path[0] == '\n') {
    memmove(model_path, default_model_path, strlen(default_model_path));
  } else
    model_path[strlen(model_path) - 1] = 0;

  if (mode == 2) {
    char dataset_path[128];
    printf("Dataset path [%s]: ", default_dataset_path);
    fgets(dataset_path, 127, stdin);

    if (dataset_path[0] == '\n') {
      memmove(dataset_path, default_dataset_path, strlen(default_dataset_path));
    } else
      dataset_path[strlen(dataset_path) - 1] = 0;

    char iteration_count_str[128];
    printf("Iteration count [%lu]: ", default_iteration_count);
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

    ai_wrapper_train(model_path, iteration_count, dataset_path, 64);
  } else {
    char input_path[128];
    printf("Input path [%s]: ", default_input_path);
    fgets(input_path, 127, stdin);

    if (input_path[0] == '\n') {
      memmove(input_path, default_input_path, strlen(default_input_path));
    } else
      input_path[strlen(input_path) - 1] = 0;

    char output_path[128];
    printf("Output path [%s]: ", default_output_path);
    fgets(output_path, 127, stdin);

    if (output_path[0] == '\n') {
      memmove(output_path, default_output_path, strlen(default_output_path));
    } else
      output_path[strlen(output_path) - 1] = 0;

    ai_wrapper_with_model(model_path, input_path, output_path);
  }

  return 0;
}

int main() {

  int err = ask_questions();

  return err;
}