#include "neural_network.h"
#include "neural_network_utils.h"
#include "activations.h"
#include "matrix.h"
#include <stdlib.h>

// The neural network constructor with only one hidden layer
Neural_Network *create_new_neural_network() {
    struct Neural_Network *nn = malloc(sizeof(struct Neural_Network));

    // create the matrix of weights, biases and activations with the correct dimensions and initial values
    nn->weights1 = create_new_random_matrix(HIDDEN_LAYER_SIZE, INPUT_LAYER_SIZE);
    nn->weights2 = create_new_random_matrix(OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);
    nn->biases1 = create_new_random_matrix(HIDDEN_LAYER_SIZE, 1);
    nn->biases2 = create_new_random_matrix(OUTPUT_LAYER_SIZE, 1);
    nn->activations1 = create_new_zeros_matrix(HIDDEN_LAYER_SIZE, 1);
    nn->activations2 = create_new_zeros_matrix(OUTPUT_LAYER_SIZE, 1);

    return nn;
}

// get the outputs of some inputs throw the current neural network
void forward_propagation(Neural_Network *nn, Matrix *inputs, Matrix *outputs) {
    // Propagate to the hidden layer
    matrix_dot_multiply(nn->weights1, inputs, nn->activations1);
    matrix_add(nn->activations1, nn->biases1, nn->activations1);
    // apply the activation function
    for (size_t i = 0; i < HIDDEN_LAYER_SIZE; i++)
        *(nn->activations1->data + i) = sigmoid(*(nn->activations1->data + i));

    // Propagate to the output layer
    matrix_dot_multiply(nn->weights2, nn->activations1, nn->activations2);
    matrix_add(nn->activations2, nn->biases2, nn->activations2);
    // apply the activation function
    for (size_t i = 0; i < OUTPUT_LAYER_SIZE; i++)
        *(nn->activations2->data + i) = sigmoid(*(nn->activations2->data + i));

    // copy the activations2 matrix into the output
    for (size_t i = 0; i < outputs->line_count * outputs->column_count; i++)
        *(outputs->data + i) = *(nn->activations2->data + i);
}

//void backward_propagation(Neural_Network *nn, const double targets[OUTPUT_LAYER_SIZE]) {
//    double dZ2[OUTPUT_LAYER_SIZE] = {0};
//    double dW2[OUTPUT_LAYER_SIZE] = {0};
//    double db2[OUTPUT_LAYER_SIZE] = {0};
//
//    for (size_t i = 0; i < OUTPUT_LAYER_SIZE; i++)
//        dZ2[i] = nn->activations2[i] - targets[i];
//
//}

// Free the neural network resources
void neural_network_destructor(Neural_Network *nn) {
    matrix_destructor(nn->weights1);
    matrix_destructor(nn->weights2);
    matrix_destructor(nn->biases1);
    matrix_destructor(nn->biases2);
    matrix_destructor(nn->activations1);
    matrix_destructor(nn->activations2);
    free(nn);
}
