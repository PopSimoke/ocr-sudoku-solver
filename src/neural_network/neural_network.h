#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stddef.h>

#define INPUT_LAYER_SIZE 2
#define HIDDEN_LAYER_SIZE 10
#define OUTPUT_LAYER_SIZE 2

struct Neural_Network {
    double weights1[HIDDEN_LAYER_SIZE];
    double weights2[OUTPUT_LAYER_SIZE];
    double biases1[HIDDEN_LAYER_SIZE];
    double biases2[OUTPUT_LAYER_SIZE];
};

typedef struct Neural_Network Neural_Network;

Neural_Network *create_new_neural_network();

void forward_propagation(Neural_Network *nn, const double inputs[INPUT_LAYER_SIZE], double outputs[OUTPUT_LAYER_SIZE]);

void neural_network_destructor(Neural_Network *neural_network);

#endif
