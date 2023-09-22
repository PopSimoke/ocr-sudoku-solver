#include "neural_network.h"
#include "matrix.h"
#include <stdio.h>

int main() {
    Neural_Network *nn = create_new_neural_network();
    double inputs_array[INPUT_LAYER_SIZE] = {0, 1};
    Matrix *inputs = create_new_matrix_from_array(inputs_array, INPUT_LAYER_SIZE);

    Matrix *outputs = create_new_matrix(OUTPUT_LAYER_SIZE, 1);

    forward_propagation(nn, inputs, outputs);

    printf("Inputs:  ");
    for (size_t i = 0; i < INPUT_LAYER_SIZE; i++)
        printf("%f ", *get_matrix_element(inputs, i, 0));

    printf("\nOutputs: ");
    for (size_t i = 0; i < OUTPUT_LAYER_SIZE; i++)
        printf("%f ", *get_matrix_element(outputs, i, 0));

    printf("\n");

    // free the allocated resources
    matrix_destructor(inputs);
    matrix_destructor(outputs);
    neural_network_destructor(nn);
    return 0;
}
