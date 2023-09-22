#include "neural_network.h"
#include <stdio.h>

int main() {
    Neural_Network *nn = create_new_neural_network();
    double inputs[INPUT_LAYER_SIZE] = {0, 1};
    double outputs[OUTPUT_LAYER_SIZE];

    forward_propagation(nn, inputs, outputs);

    printf("Inputs:  ");
    for (size_t i = 0; i < INPUT_LAYER_SIZE; i++)
        printf("%f ", inputs[i]);

    printf("\nOutputs: ");
    for (size_t i = 0; i < OUTPUT_LAYER_SIZE; i++)
        printf("%f ", outputs[i]);

    printf("\n");

    neural_network_destructor(nn);
    return 0;
}
