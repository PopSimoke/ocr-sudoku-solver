
#include "activations.h"
#include <math.h>

double sigmoid(double input) {
    return 1 / (1 + exp(-input));
}