#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "engine.h"

int main(void) {
    srand ( time(NULL) );

    int k = 20;
    int size = 16;
    int **tablica = malloc(size*sizeof(int*));
    for (int i = 0; i < size; i++) {
        tablica[i] = malloc(size*sizeof(int));
    }
    tablica = sudokuGenerator(size,k);

    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%d ", sudoku[i][j]);
    //     }
    //     printf("\n");
    // }

    printGrid(tablica,size);

    for (int j = 0; j < size; j++) {
        free(tablica[j]);
    }
    free(tablica);
    return 0;
}

