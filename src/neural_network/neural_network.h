#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stddef.h>
#include "matrix.h"

struct Neural_Network {
    Matrix *weights1;
    Matrix *weights2;
    Matrix *biases1;
    Matrix *biases2;
    Matrix *activations1;
    Matrix *activations2;
    double learning_rate;
    size_t inputLayerSize;
    size_t outputLayerSize;
    size_t hiddenLayerSize;
};

typedef struct Neural_Network Neural_Network;

Neural_Network *create_new_neural_network(double learning_rate, size_t inputLayerSize, size_t outputLayerSize, size_t hiddenLayerSize);

void forward_propagation(Neural_Network *nn, Matrix *inputs);

void backward_propagation(Neural_Network *nn, Matrix *inputs, Matrix *targets);

void train(Neural_Network *nn, const double *inputs, const double *targets,
           size_t iteration_count);

void predict(Neural_Network *nn, const double *inputs, double *outputs);

void save_neural_network(Neural_Network *nn, char *path);

Neural_Network *load_neural_network(char *path, double learning_rate, size_t inputLayerSize, size_t outputLayerSize, size_t hiddenLayerSize);

void neural_network_destructor(Neural_Network *neural_network);

#endif
