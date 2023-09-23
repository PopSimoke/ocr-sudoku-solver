#include "neural_network.h"
#include "activations.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

// The neural network constructor with only one hidden layer
Neural_Network *create_new_neural_network(double learning_rate) {
    struct Neural_Network *nn = malloc(sizeof(struct Neural_Network));

    // create the matrix of weights, biases and activations with the correct dimensions and initial values
    nn->weights1 = create_new_random_matrix(HIDDEN_LAYER_SIZE, INPUT_LAYER_SIZE);
    nn->weights2 = create_new_random_matrix(OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);
    nn->biases1 = create_new_random_matrix(HIDDEN_LAYER_SIZE, 1);
    nn->biases2 = create_new_random_matrix(OUTPUT_LAYER_SIZE, 1);
    nn->activations1 = create_new_zeros_matrix(HIDDEN_LAYER_SIZE, 1);
    nn->activations2 = create_new_zeros_matrix(OUTPUT_LAYER_SIZE, 1);
    nn->learning_rate = learning_rate;

    return nn;
}

// get the outputs of some inputs throw the current neural network
void forward_propagation(Neural_Network *nn, Matrix *inputs) {
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
}

void backward_propagation(Neural_Network *nn, Matrix *inputs, Matrix *targets) {
    // compute the gradients of the output layer
    Matrix *dZ2 = create_new_matrix(OUTPUT_LAYER_SIZE, 1);
    Matrix *dW2 = create_new_matrix(OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE);
    Matrix *db2 = create_new_matrix(OUTPUT_LAYER_SIZE, 1);

    matrix_subtract(nn->activations2, targets, dZ2);

    Matrix *A1T = create_new_matrix(nn->activations1->column_count, nn->activations1->line_count);

    matrix_transpose(nn->activations1, A1T);

    matrix_dot_multiply(dZ2, A1T, dW2);

    matrix_copy(dZ2, db2);

    // compute the gradients of the hidden layer
    Matrix *dZ1 = create_new_matrix(HIDDEN_LAYER_SIZE, 1);
    Matrix *dW1 = create_new_matrix(HIDDEN_LAYER_SIZE, INPUT_LAYER_SIZE);
    Matrix *db1 = create_new_matrix(HIDDEN_LAYER_SIZE, 1);

    Matrix *W2T = create_new_matrix(nn->weights2->column_count, nn->weights2->line_count);

    matrix_transpose(nn->weights2, W2T);

    matrix_dot_multiply(W2T, dZ2, dZ1);
    matrix_multiply(dZ1, nn->activations1, dZ1);

    for (size_t i = 0; i < HIDDEN_LAYER_SIZE; i++)
        *(dZ1->data + i) = *(dZ1->data + i) * (1 - *(nn->activations1->data + i));

    Matrix *XT = create_new_matrix(1, INPUT_LAYER_SIZE);

    matrix_transpose(inputs, XT);

    matrix_dot_multiply(dZ1, XT, dW1);

    matrix_copy(dZ1, db1);

    // apply the modification to the current parameters

    // W1
    matrix_multiply_by_double(dW1, nn->learning_rate, dW1);
    matrix_subtract(nn->weights1, dW1, nn->weights1);

    // b1
    matrix_multiply_by_double(db1, nn->learning_rate, db1);
    matrix_subtract(nn->biases1, db1, nn->biases1);

    // W2
    matrix_multiply_by_double(dW2, nn->learning_rate, dW2);
    matrix_subtract(nn->weights2, dW2, nn->weights2);

    // b2
    matrix_multiply_by_double(db2, nn->learning_rate, db2);
    matrix_subtract(nn->biases2, db2, nn->biases2);

    // free the resources
    matrix_destructor(dZ2);
    matrix_destructor(dW2);
    matrix_destructor(db2);
    matrix_destructor(A1T);
    matrix_destructor(dZ1);
    matrix_destructor(dW1);
    matrix_destructor(db1);
    matrix_destructor(W2T);
    matrix_destructor(XT);
}

// Train the neural network with successive forward and backward propagation
void train(Neural_Network *nn, const double inputs[INPUT_LAYER_SIZE], const double targets[OUTPUT_LAYER_SIZE],
           size_t iteration_count) {

    Matrix *inputs_matrix = create_new_matrix_from_array(inputs, INPUT_LAYER_SIZE);
    Matrix *targets_matrix = create_new_matrix_from_array(targets, OUTPUT_LAYER_SIZE);

    for (size_t i = 0; i < iteration_count; i++) {
        forward_propagation(nn, inputs_matrix);
        backward_propagation(nn, inputs_matrix, targets_matrix);
//        printf("%lf %lf\n", *get_matrix_element(nn->activations2, 0, 0), *get_matrix_element(nn->activations2, 1, 0));
    }

    matrix_destructor(inputs_matrix);
    matrix_destructor(targets_matrix);
}

// Predict the output of some inputs throw the current neural network
void predict(Neural_Network *nn, const double inputs[INPUT_LAYER_SIZE], double outputs[OUTPUT_LAYER_SIZE]) {
    Matrix *inputs_matrix = create_new_matrix_from_array(inputs, INPUT_LAYER_SIZE);

    forward_propagation(nn, inputs_matrix);

    for (size_t i = 0; i < OUTPUT_LAYER_SIZE; i++)
        outputs[i] = *(nn->activations2->data + i);

    matrix_destructor(inputs_matrix);
}

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
