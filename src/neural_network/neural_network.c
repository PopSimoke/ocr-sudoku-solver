#include "neural_network.h"
#include "neural_network_utils.h"
#include "activations.h"
#include <stdlib.h>

// The neural network constructor with only one hidden layer
Neural_Network *create_new_neural_network() {
    struct Neural_Network *nn = malloc(sizeof(struct Neural_Network));

    for (size_t i = 0; i < HIDDEN_LAYER_SIZE; i++) {
        nn->weights1[i] = generate_random_double();
        nn->biases1[i] = generate_random_double();
    }

    for (size_t i = 0; i < OUTPUT_LAYER_SIZE; i++) {
        nn->weights2[i] = generate_random_double();
        nn->biases2[i] = generate_random_double();
    }

    return nn;
}

void forward_propagation(Neural_Network *nn, const double inputs[INPUT_LAYER_SIZE], double outputs[OUTPUT_LAYER_SIZE]) {
    // propagate to the hidden layer
    double hidden_layer_output[HIDDEN_LAYER_SIZE] = {0};

    for (size_t hidden_layer_index = 0; hidden_layer_index < HIDDEN_LAYER_SIZE; hidden_layer_index++) {
        for (size_t input_layer_index = 0; input_layer_index < INPUT_LAYER_SIZE; input_layer_index++) {
            hidden_layer_output[hidden_layer_index] += inputs[input_layer_index] * nn->weights1[hidden_layer_index];
        }
        hidden_layer_output[hidden_layer_index] += nn->biases1[hidden_layer_index];
        hidden_layer_output[hidden_layer_index] = sigmoid(hidden_layer_output[hidden_layer_index]);
    }

    // propagate to the output layer
    for (size_t output_layer_index = 0; output_layer_index < OUTPUT_LAYER_SIZE; output_layer_index++) {
        for (size_t hidden_layer_index = 0; hidden_layer_index < HIDDEN_LAYER_SIZE; hidden_layer_index++) {
            outputs[output_layer_index] += hidden_layer_output[hidden_layer_index] * nn->weights2[output_layer_index];
        }
        outputs[output_layer_index] += nn->biases2[output_layer_index];
        outputs[output_layer_index] = sigmoid(outputs[output_layer_index]);
    }
}

// Free the neural network resources
void neural_network_destructor(Neural_Network *neural_network) {
    free(neural_network);
}
