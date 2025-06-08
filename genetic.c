#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "genetic.h"
#include "anealing.h"

#define POPULATION_SIZE 100
#define NUM_GENERATIONS 200
#define MUTATION_RATE 0.01
#define CROSSOVER_RATE 0.7
#define UNIFORM_RATE 0.5

Individual compareFitness(Individual a, Individual b) {
    if (a.fitness > b.fitness) {
        return a;
    }
    return b;
}

void initializePopulation(Individual population[], int size, int **original) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i].grid = copyGrid(original, size);
        population[i].fitness = 0;

        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (population[i].grid[row][col] == 0) {
                    int num;
                    do {
                        num = generateRandomNumber(1,size);
                    } while (!unUsedInRow(population[i].grid, size, row, num));
                    population[i].grid[row][col] = num;
                }
            }
        }
    }
}

void calculateFitness(Individual *individual, int size) {
    individual->fitness = -1 * calculateCost(individual->grid, size);
}

void evaluatePopulation(Individual population[], int size) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        calculateFitness(&population[i], size);
    }
}

Individual selectParentRoulette(Individual population[]) {
    int min_fitness = population[0].fitness;
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (population[i].fitness < min_fitness) {
            min_fitness = population[i].fitness;
        }
    }

    int offset = -min_fitness;

    int fitness_sum = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness_sum += (population[i].fitness + offset);
    }

    int r = generateRandomNumber(0, fitness_sum);

    int current_sum = 0;
    int i = 0;
    while (current_sum < r && i < POPULATION_SIZE) {
        current_sum += (population[i].fitness + offset);
        i++;
    }
    return population[i - 1];
}

Individual selectParentTournament(Individual population[]) {
    int tournament_size = 4;
    Individual best_in_tournament = population[generateRandomNumber(0, POPULATION_SIZE - 1)];

    for (int i = 1; i < tournament_size; i++) {
        Individual current = population[generateRandomNumber(0, POPULATION_SIZE - 1)];
        if (current.fitness > best_in_tournament.fitness) {
            best_in_tournament = current;
        }
    }
    return best_in_tournament;
}

Individual selectParentRanking(Individual population[]) {
    int n = sizeof(population) / sizeof(population[0]);

    qsort(population, n, sizeof(population[0]), compareFitness);

    return population[0];

}

void crossoverSingle(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size) {
    child1->grid = copyGrid(parent1.grid, size);
    child2->grid = copyGrid(parent2.grid, size);

    if ((double)rand() / RAND_MAX < CROSSOVER_RATE) {
        int crossover_row = generateRandomNumber(0, (size) - 1);
        int crossover_col = generateRandomNumber(0, (size) - 1);

        for (int i = crossover_row; i < size; i++) {
            for (int j = crossover_col; j < size; j++) {
                int temp = child1->grid[i][j];
                child1->grid[i][j] = child2->grid[i][j];
                child2->grid[i][j] = temp;
            }
        }
    }

    child1->fitness = 0;
    child2->fitness = 0;
}

void crossoverMulti(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size) {
    child1->grid = copyGrid(parent1.grid, size);
    child2->grid = copyGrid(parent2.grid, size);

    if (((double)rand() / RAND_MAX) < CROSSOVER_RATE) {

        int p1 = generateRandomNumber(0, (size) - 1);
        int p2 = generateRandomNumber(0, (size) - 1);

        if (p1 > p2) {
            int temp = p1;
            p1 = p2;
            p2 = temp;
        }

        for (int i = p1; i <= p2; i++) {
            for (int j = 0; j < size; j++) {
                int temp = child1->grid[i][j];
                child1->grid[i][j] = child2->grid[i][j];
                child2->grid[i][j] = temp;
            }
        }
    }

    child1->fitness = 0;
    child2->fitness = 0;
}

void crossoverUni(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size) {

    child1->grid = copyGrid(parent1.grid, size);
    child2->grid = copyGrid(parent2.grid, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (child1->grid[i][j] == 0) {
                if (((double)rand() / RAND_MAX) < UNIFORM_RATE) {
                    child1->grid[i][j] = parent1.grid[i][j];
                    child2->grid[i][j] = parent2.grid[i][j];
                } else {
                    child1->grid[i][j] = parent2.grid[i][j];
                    child2->grid[i][j] = parent1.grid[i][j];
                }
            }
        }
    }

    child1->fitness = 0;
    child2->fitness = 0;
}

void mutateRow(Individual *individual, int size, int **original) {
    int row_is_viable[size];
    int viable_count = 0;
    for (int row = 0; row < size; row++) {
        int empty_cells = 0;
        for (int col = 0; col < size; col++) {
            if (original[row][col] == 0) {
                empty_cells++;
            }
        }
        if (empty_cells >= 2) {
            row_is_viable[row] = 1;
            viable_count++;
        }
    }
    if (viable_count == 0){
        return;
    }

    int row;
    do {
        row = generateRandomNumber(0, (size) - 1);
    }while (row_is_viable[row] != 1);

    int col1;
    do {
        col1 = generateRandomNumber(0, (size) - 1);
    }while (original[row][col1] != 0);

    int col2;
    do {
        col2 = generateRandomNumber(0, (size) - 1);
    }while (original[row][col2] != 0 || col2 == col1);

    int temp = individual->grid[row][col1];
    individual->grid[row][col1] = individual->grid[row][col2];
    individual->grid[row][col2] = temp;
}

void mutateCol(Individual *individual, int size, int **original) {
    int col_is_viable[size];
    int viable_count = 0;
    for (int col = 0; col < size; col++) {
        int empty_cells = 0;
        for (int row = 0; row < size; row++) {
            if (original[row][col] == 0) {
                empty_cells++;
            }
        }
        if (empty_cells >= 2) {
            col_is_viable[col] = 1;
            viable_count++;
        }
    }
    if (viable_count == 0){
        return;
    }

    int col;
    do {
        col = generateRandomNumber(0, (size) - 1);
    }while (col_is_viable[col] != 1);

    int row1;
    do {
        row1 = generateRandomNumber(0, (size) - 1);
    }while (original[row1][col] != 0);

    int row2;
    do {
        row2 = generateRandomNumber(0, (size) - 1);
    }while (original[row2][col] != 0 || row2 == row1);

    int temp = individual->grid[row1][col];
    individual->grid[row1][col] = individual->grid[row2][col];
    individual->grid[row2][col] = temp;
}

void mutateBox(Individual *individual, int size, int **original); //todo

void geneticAlgorithm(int **grid, int size) {
    Individual population[POPULATION_SIZE];
    Individual next_population[POPULATION_SIZE];

    initializePopulation(population, size, grid);

    Individual best_solution_ever;
    best_solution_ever.fitness = -100;

    for (int generation = 0; generation < NUM_GENERATIONS; generation++) {
        evaluatePopulation(population, size);

        Individual current_best = population[0];
        for (int i = 1; i < POPULATION_SIZE; i++) {
            if (population[i].fitness > current_best.fitness) {
                current_best = population[i];
            }
        }

        if (current_best.fitness > best_solution_ever.fitness) {
            best_solution_ever = current_best;
            printf("Pokolenie %d: Znaleziono lepsze rozwiazanie - fitness = %d",
                   generation, best_solution_ever.fitness);
            printf("\n");
        } else {
            if (generation % 10 == 0) {
                printf("Pokolenie %d: Najlepsza wartosc = %d\n", generation, current_best.fitness);
            }
        }

        int current_population_idx = 0;

        while (current_population_idx < POPULATION_SIZE) {

            Individual parent1 = selectParentTournament(population);
            Individual parent2 = selectParentTournament(population);

            Individual child1, child2;

            crossoverSingle(parent1, parent2, &child1, &child2, size);

            mutateRow(&child1, size, grid);
            mutateRow(&child2, size, grid);

            next_population[current_population_idx++] = child1;
            if (current_population_idx < POPULATION_SIZE) {
                next_population[current_population_idx++] = child2;
            }
        }

        for(int i = 0; i < POPULATION_SIZE; i++){
            population[i] = next_population[i];
        }
    }

    printf("\n--- Koniec Algorytmu Genetycznego ---\n");
    printf("Najlepsze znalezione rozwiazanie:\n");
    printf("Fitness: %d",best_solution_ever.fitness);
    printGrid(best_solution_ever.grid,size);
    printf("\n");
}
