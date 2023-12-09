#ifndef NUMBER_RECOGNITION_H
#define NUMBER_RECOGNITION_H

#define INPUT_LAYER_SIZE 1024
#define HIDDEN_LAYER_SIZE 500
#define OUTPUT_LAYER_SIZE 9
#define LEARNING_RATE 0.005

#include "neural_network.h"

/**
 * The function recognize sudoku case and store the result as a text grid.
 *
 * @param nn Pointer to an instance of a Neural_Network struct.
 * @param input_path Path to the file from where the function reads input neural
 * network data.
 * @param output_path Path to the text file of the generated sudoku grid.
 * @return Integer indicating the success/failure of the function's execution.
 * Typically, returns 0 upon success and non-zero values upon failure.
 */
int generate_grid(Neural_Network *nn, char *input_path, char *output_path);

/**
 * A wrap of all the ai methods that generates a sudoku grid from a given
 * directory with sudoku cases inside.
 *
 * @param model_path Path to a model file.
 * @param input_path Path to the directory containing the sudoku cases.
 * @param output_path Path to the text file of the generated sudoku grid.
 *
 * @return Integer indicating the success/failure of the function's execution.
 * Typically, returns 0 upon success and non-zero values upon failure.
 */
int ai_wrapper_with_model(char *model_path, char *input_path,
                          char *output_path);

/**
 * A wrap of all the ai methods that generates a sudoku grid from a given
 * directory with sudoku cases inside.
 *
 * @param input_path Path to the directory containing the sudoku cases.
 * @param output_path Path to the text file of the generated sudoku grid.
 *
 * @return Integer indicating the success/failure of the function's execution.
 * Typically, returns 0 upon success and non-zero values upon failure.
 */
int ai_wrapper(char *input_path, char *output_path);

/**
 * A wrap of all the ai methods that trains a neural network and store it into a
 * model file
 *
 * @param model_path Path to a model file.
 * @param iteration_count Number of iteration the train method have to perform.
 * @param output_path Path of the datataset that trains the ai.
 *
 * @return Integer indicating the success/failure of the function's execution.
 * Typically, returns 0 upon success and non-zero values upon failure.
 */
int ai_wrapper_train(char *model_path, size_t iteration_count,
                     char *dataset_path);

/**
 * Creates a dataset from a folder of annoted images
 *
 * @param images_dir_path Path to the training folder containing the images.
 * @param dataset_path Path to the output dataset.
 * @param dataset_size The size of the dataset (The number of images inside it).
 *
 * @return Integer indicating the success/failure of the function's execution.
 * Typically, returns 0 upon success and non-zero values upon failure.
 */
int create_dataset_from_images(char *images_dir_path, char *dataset_path,
                               int *dataset_size);

#endif