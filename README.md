
# OCR Sudoku Solver

School group project that recognize and solve sudoku grids using home made OCR and sudoku solver algorithm.

## How does it works ?

### Image processing
- Load image
- Remove colors (grayscale)
- Preprocessing

### Grid analyze
- Grid detection
- Detection of the grid boxes
- Split image to get the numbers

### Character recognition
- Send the images to a neural network that identify the number

### Sudoku solver
- Grid reconstruction
- Solve the Sudoku

### Results
- Display the solved grid
- Save the solved Sudoku grid

## Installation

### Compile from source

The project being built in C, you need [gcc](https://gcc.gnu.org) and [make](https://www.gnu.org/software/make).

You need to run `make` at the root of the project to compile all the required ressources of the OCR Sudoku Solver.

A new file called *ocr-sudoku-solver* should be now created.

Just run this file with the commande `./ocr-sudoku-solver <...args>`.

### With the binary file

To download the compiled binary file of the OCR Sudoku Solver, get on the [Releases](https://github.com/yqnk/ocr-sudoku-solver/releases/) page from this github repository.
## Authors

- [@yqnk](https://github.com/yqnk)
- [@matiix310](https://www.github.com/matiix310)
- [@OnicaS-AnyaL ](https://www.github.com/OnicaS-AnyaL )
- [@PopSimoke](https://www.github.com/PopSimoke)


