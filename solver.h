#ifndef SOLVER_H
#define SOLVER_H

#include "engine.h"
#include <string.h>

typedef struct {
    int **grid;
    double cost; // liczba duplikatów
} SudokuState;

int **copyGrid(int **grid, int size);


// wypełnia zera losowymi wartościami które jeszcze nie wystąpiły w tym bloku
void generateBox(int **grid, int size, int row, int col);

// wywołuje powyższą funkcję dla wszystkich bloków
void generateBoard(int **grid, int size);

int countDuplicatesRow(int **grid, int size, int num, int row);

int countDuplicatesCol(int **grid, int size, int num, int col);

int calculateCost(int **grid, int size);


int** generateNeighbor(int **grid, int size, int **originalGrid);

void simulated_anealing(int **grid, int size, double T_start, double T_end, double alpha, int max_iterations);

#endif //SOLVER_H
