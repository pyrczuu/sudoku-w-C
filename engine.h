#ifndef ENGINE_H
#define ENGINE_H
#include <math.h>
#include <stdlib.h>
#include <time.h>

int generateRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int unUsedInBox(int **grid, int size, int rowStart, int colStart, int num) {
    int boxSize = sqrt(size);
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            if (grid[rowStart + i][colStart + j] == num) {
                return 0;
            }
        }
    }
    return 1;
}

void fillBox(int **grid, int size, int row, int col) {
    int num;
    int boxSize = sqrt(size);
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            do {
                num = generateRandomNumber(1, size);
            } while (!unUsedInBox(grid, size, row, col, num));
            grid[row + i][col + j] = num;
        }
    }
}

int unUsedInRow(int **grid, int i, int size, int num) {
    for (int j = 0; j < size; j++) {
        if (grid[i][j] == num) {
            return 0;
        }
    }
    return 1;
}

int unUsedInCol (int **grid, int j, int size, int num) {
    for (int i = 0; i < size; i++) {
        if (grid[i][j] == num) {
            return 0;
        }
    }
    return 1;
}

int checkIfSafe(int **grid, int i, int j, int size, int num) {
    int boxSize = sqrt(size);
    return (unUsedInRow(grid, i, size, num) && unUsedInCol(grid, j, size, num) &&
        unUsedInBox(grid, size, i - i % boxSize, j - j % boxSize, num ));
}

void fillDiagonal(int **grid, int size) {
    int boxSize = sqrt(size);
    for (int i = 0; i < size; i = i + boxSize) {
        fillBox(grid, size, i, i);
    }
}

int fillRemaining(int **grid, int i, int j, int size) {

    if (i == size) {
        return 1;
    }

    if (j == size) {
        return fillRemaining(grid, i+1, 0, size);
    }

    if (grid[i][j] != 0) {
        return fillRemaining(grid, i, j+1, size);
    }

    for (int num = 1; num <= size; num++) {
        if (checkIfSafe(grid, i, j, size, num)) {
            grid[i][j] = num;
            if (fillRemaining(grid, i, j + 1, size)) {
                return 1;
            }
            grid[i][j] = 0;
        }
    }
    return 0;
}

void removeKDigits(int **grid, int size,int k) {
    while (k > 0) {
        int cellId = generateRandomNumber(0, size * size - 1);
        int i = cellId / size;
        int j = cellId % size;
        if (grid[i][j] != 0) {
            grid[i][j] = 0;
            k--;
        }
    }
}

void printGrid(int **grid, int size) {
    int boxSize = sqrt(size);
    for (int _ = 0; _ < size*4; _++) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
       printf("|");
        for (int j = 0; j < size; j++) {
            printf(" %2d ", grid[i][j]);
            if ((j+1) % boxSize == 0) {
                printf("|");
            }
        }
        printf("\n");
        if ((i+1) % boxSize == 0) {
            for (int _ = 0; _ < size*4; _++) {
                printf("-");
            }
            printf("\n");
        }
    }
}

void freeGrid(int **grid, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            free(grid[i][j]);
        }
        free(grid[i]);
    }
    free(grid);
}

int** sudokuGenerator(int size, int k) {
    int **grid = (int**) malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        grid[i] = (int*) malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            grid[i][j] = 0;
        }
    }

    fillDiagonal(grid, size);
    fillRemaining(grid, 0, 0, size);
    removeKDigits(grid, size, k);

    return grid;
}



#endif //ENGINE_H
