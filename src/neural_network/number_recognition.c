#define INPUT_LAYER_SIZE 1024
#define HIDDEN_LAYER_SIZE 100
#define OUTPUT_LAYER_SIZE 10

#include "neural_network.h"
#include <stdio.h>
#include <err.h>
#include <string.h>

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
  Neural_Network *nn = load_neural_network(path, learning_rate,
                                           INPUT_LAYER_SIZE, OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);

  double inputs_array1[INPUT_LAYER_SIZE] = {0, 1};
  double targets_array1[OUTPUT_LAYER_SIZE] = {1, 0}; // true
  double inputs_array2[INPUT_LAYER_SIZE] = {1, 0};
  double targets_array2[OUTPUT_LAYER_SIZE] = {1, 0}; // true
  double inputs_array3[INPUT_LAYER_SIZE] = {1, 1};
  double targets_array3[OUTPUT_LAYER_SIZE] = {0, 1}; // false
  double inputs_array4[INPUT_LAYER_SIZE] = {0, 0};
  double targets_array4[OUTPUT_LAYER_SIZE] = {0, 1}; // false

  printf("Iteration count: %lu\n\n", iteration_count);

  size_t old_percent = 101;

  for (size_t i = 0; i < iteration_count; i++) {
    size_t new_percent = (double) ((double) i / (double) iteration_count) * 100;
    if (old_percent != new_percent) {
      old_percent = new_percent;
      printf("%3lu%% / 100%%\n", new_percent);
    }
    train(nn, inputs_array1, targets_array1, 1);
    train(nn, inputs_array2, targets_array2, 1);
    train(nn, inputs_array3, targets_array3, 1);
    train(nn, inputs_array4, targets_array4, 1);
  }

  // test the model with some predictions

  double outputs[OUTPUT_LAYER_SIZE] = {0, 0};
  predict(nn, inputs_array1, outputs);
  printf("Inputs:  %lf %lf\n", inputs_array1[0], inputs_array1[1]);
  printf("Outputs: %lf %lf\n", outputs[0], outputs[1]);
  printf("Targets: %lf %lf\n\n", targets_array1[0], targets_array1[1]);

  predict(nn, inputs_array2, outputs);
  printf("Inputs: %lf %lf\n", inputs_array2[0], inputs_array2[1]);
  printf("Outputs: %lf %lf\n", outputs[0], outputs[1]);
  printf("Targets: %lf %lf\n\n", targets_array2[0], targets_array2[1]);

  predict(nn, inputs_array3, outputs);
  printf("Inputs: %lf %lf\n", inputs_array3[0], inputs_array3[1]);
  printf("Outputs: %lf %lf\n", outputs[0], outputs[1]);
  printf("Targets: %lf %lf\n\n", targets_array3[0], targets_array3[1]);

  predict(nn, inputs_array4, outputs);
  printf("Inputs: %lf %lf\n", inputs_array4[0], inputs_array4[1]);
  printf("Outputs: %lf %lf\n", outputs[0], outputs[1]);
  printf("Targets: %lf %lf\n\n", targets_array4[0], targets_array4[1]);

  save_neural_network(nn, path);
  printf("Model saved at : %s\n", path);

  // free the allocated resources
  neural_network_destructor(nn);
  return 0;
}
