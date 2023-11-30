#include "utils.h"

int arrayMaxIndex(int *array, int size) {
    int max = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[max]) {
            max = i;
        }
    }

    return max;
}

int arrayMaxIndexAfter(int *array, int size, int currentMaxIndex) {
    int max = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[max] && i != currentMaxIndex) {
            max = i;
        }
    }

    return max;
}