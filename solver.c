#include "engine.h"
#include "solver.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int **copyGrid(int **grid, int size) {
    int **copy = (int**) malloc(size * sizeof(int*));
    if (!copy) {
        perror("Memory allocation failed for rows");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        copy[i] = (int*) malloc(size * sizeof(int));
        if (!copy[i]) {
            perror("Memory allocation failed for columns");
            for (int j = 0; j < i; j++) {
                free(copy[j]);
            }
            free(copy);
            return NULL;
        }

        memcpy(copy[i], grid[i], size * sizeof(int));
    }

    return copy;
}


// wypełnia zera losowymi wartościami które jeszcze nie wystąpiły w tym bloku
void generateBox(int **grid, int size, int row, int col) {
    int num;
    int boxSize = sqrt(size);
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            if (grid[row + i][col + j] == 0) {
                do {
                    num = generateRandomNumber(1, size);
                } while (!unUsedInBox(grid, size, row, col, num));
                grid[row + i][col + j] = num;
            }
        }
    }
}

// wywołuje powyższą funkcję dla wszystkich bloków
void generateBoard(int **grid, int size) {
    int boxSize = sqrt(size);
    for (int i = 0; i < size; i += boxSize) {
        for (int j = 0; j < size; j += boxSize) {
            generateBox(grid, size, i, j);
        }
    }
}

int countDuplicatesRow(int **grid, int size, int num, int row) {
    int count = 0;
    for (int j = 0; j < size; j++) {
        if (grid[row][j] == num) {
            count++;
        }
    }
    return count;
}

int countDuplicatesCol(int **grid, int size, int num, int col) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (grid[i][col] == num) {
            count++;
        }
    }
    return count;
}

int calculateCost(int **grid, int size) {
    int cost = 0;

    // Liczenie duplikatów w wierszach
    for (int row = 0; row < size; row++) {
        for (int num = 1; num <= size; num++) {
            int count = 0;

            for (int col = 0; col < size; col++) {
                if (grid[row][col] == num) {
                    count++;
                }
            }

            // jeśli count == 1 to wcale nie duplikat
            if (count > 1) {
                cost += (count - 1);
            }
        }
    }

    // Liczenie duplikatów w kolumnach
    for (int col = 0; col < size; col++) {
        for (int num = 1; num <= size; num++) {
            int count = 0;

            for (int row = 0; row < size; row++) {
                if (grid[row][col] == num) {
                    count++;
                }
            }

            if (count > 1) {
                cost += (count - 1);
            }
        }
    }

    return cost;
}


int** generateNeighbor(int **grid, int size, int **originalGrid) {
    int **neighborGrid = copyGrid(grid, size);
    int blockSize = sqrt(size);
    int blockRow = generateRandomNumber(0,blockSize - 1 );   // wybieramy jeden z mniejszych bloków
    int blockCol = generateRandomNumber(0,blockSize - 1 );

    // tu przechowujemy komórki które możemy zmieniać, każdy blok może mieć ich maksymalnie tyle co size
    int canChange[size * size][2];
    int editableCount = 0;      // liczba pól które można zmienić, potrzeba przynajmniej 2

    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < blockSize; j++) {
            int row = blockRow * blockSize + i;         // 'blockRow * blockSize' -> odpowiedni blok
            int col = blockCol * blockSize + j;
            if (grid[row][col] != originalGrid[row][col]) {
                canChange[editableCount][0] = row;
                canChange[editableCount][1] = col;
                editableCount++;
            }
        }
    }

    if (editableCount >= 2) {
        int a = generateRandomNumber(0, editableCount - 1);
        int b;
        do {
            b = generateRandomNumber(0, editableCount - 1);
        } while (a == b);

        int row1 = canChange[a][0];
        int col1 = canChange[a][1];

        int row2 = canChange[b][0];
        int col2 = canChange[b][1];

        int temp = neighborGrid[row1][col1];
        neighborGrid[row1][col1] = neighborGrid[row2][col2];
        neighborGrid[row2][col2] = temp;
    }
    return neighborGrid;
}

void simulated_anealing(int **grid, int size, double T_start, double T_end, double alpha, int max_iterations) {
    srand(time(NULL));

    SudokuState currentState, neighborState, bestState;
    currentState.grid = copyGrid(grid, size);
    neighborState.grid = copyGrid(grid, size);
    bestState.grid = copyGrid(grid, size);

    int **originalGrid = copyGrid(grid, size);    // kopia oryginalnej tablicy żeby wiedzieć które pola są nienaruszalne

    if (!currentState.grid || !neighborState.grid || !bestState.grid) {
        perror("Memory allocation error");

        free(currentState.grid);
        free(neighborState.grid);
        free(bestState.grid);
        free(originalGrid);
        return;
    }

    generateBoard(currentState.grid, size);
    generateBoard(neighborState.grid, size);
    generateBoard(bestState.grid, size);

    currentState.cost = calculateCost(currentState.grid, size);
    bestState.cost = currentState.cost;

    printf("Initial cost calculation: %.0f\n", currentState.cost);
    if (currentState.cost == 0) {
        printf("Warning: Initial cost is zero - grid may already be solved!\n");
        printf("Original grid:\n");
        printGrid(grid, size);
        printf("Copied grid:\n");
        printGrid(currentState.grid, size);
    }


    double T = T_start;
    int iteration = 0;

    printf("Initial Cost: %.0f\n", currentState.cost);

    while (T > T_end && iteration < max_iterations && bestState.cost > 0) {
        for (int row  = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                neighborState.grid[row][col] = currentState.grid[row][col];
            }
        }
        neighborState.grid = generateNeighbor(neighborState.grid, size, originalGrid);
        neighborState.cost = calculateCost(neighborState.grid, size);
        double delta_E = neighborState.cost - currentState.cost;

        if (delta_E < 0 || ((double)rand() / (double)RAND_MAX) < exp(-delta_E / T)) {
            for (int row = 0; row < size; row++) {
                for (int col = 0; col < size; col++) {
                    currentState.grid[row][col] = neighborState.grid[row][col];
                }
            }
            currentState.cost = neighborState.cost;
        }

        if (currentState.cost < bestState.cost) {
            for (int row = 0; row < size; row++) {
                for (int col = 0; col < size; col++) {
                    bestState.grid[row][col] = currentState.grid[row][col];
                }
            }
            bestState.cost = currentState.cost;
        }

        T *= alpha;
        iteration++;

        if (iteration % 100 == 0 || bestState.cost == 0) {
            printf("Iteration: %d, T: %.4f, Current cost: %.0f, Best cost: %.0f\n",
                   iteration, T, currentState.cost, bestState.cost);
        }
    }

    printf("\nFinal cost: %.0f\n", bestState.cost);
    printf("Solved Sudoku:\n");
    printGrid(bestState.grid, size);

    freeGrid(bestState.grid, size);
    freeGrid(currentState.grid, size);
    freeGrid(neighborState.grid, size);
}