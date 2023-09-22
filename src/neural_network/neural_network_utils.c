#include "neural_network_utils.h"
#include <stdlib.h>

// Generates a random float number between -1 and 1
double generate_random_double() {
    return (double) rand() / (double) (RAND_MAX / 2) - 1;
}
