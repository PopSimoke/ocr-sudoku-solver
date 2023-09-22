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
};

typedef struct Neural_Network Neural_Network;

Neural_Network *create_new_neural_network();

void forward_propagation(Neural_Network *nn, Matrix *inputs, Matrix *outputs);

void backward_propagation(Neural_Network *nn, const double targets[OUTPUT_LAYER_SIZE]);

void neural_network_destructor(Neural_Network *neural_network);

#endif
