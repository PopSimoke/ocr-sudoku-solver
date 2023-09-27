#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stddef.h>
#include "matrix.h"

#define INPUT_LAYER_SIZE 2
#define HIDDEN_LAYER_SIZE 10
#define OUTPUT_LAYER_SIZE 2

struct Neural_Network {
    Matrix *weights1;
    Matrix *weights2;
    Matrix *biases1;
    Matrix *biases2;
    Matrix *activations1;
    Matrix *activations2;
    double learning_rate;
};

typedef struct Neural_Network Neural_Network;

Neural_Network *create_new_neural_network(double learning_rate);

void forward_propagation(Neural_Network *nn, Matrix *inputs);

void backward_propagation(Neural_Network *nn, Matrix *inputs, Matrix *targets);

void train(Neural_Network *nn, const double inputs[INPUT_LAYER_SIZE], const double targets[OUTPUT_LAYER_SIZE],
           size_t iteration_count);

void predict(Neural_Network *nn, const double inputs[INPUT_LAYER_SIZE], double outputs[OUTPUT_LAYER_SIZE]);

void save_neural_network(Neural_Network *nn, char *path);

Neural_Network *load_neural_network(char *path, double learning_rate);

void neural_network_destructor(Neural_Network *neural_network);

#endif
