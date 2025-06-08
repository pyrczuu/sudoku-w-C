#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "engine.h"
#include "anealing.h"
#include "genetic.h"

int main(void) {
    srand ( time(NULL) );

    //menuScreen();

    int **sudoku = sudokuGenerator(9,20);
    printGrid(sudoku,9);
    //simulated_anealing(sudoku,9 , 10.0, 0.001, 0.995, 100000 );
    geneticAlgorithm(sudoku,9);
    freeGrid(sudoku,9);
    return 0;
}

