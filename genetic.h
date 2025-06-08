#ifndef GENETIC_H
#define GENETIC_H

typedef struct {
    int** grid;
    int fitness;
}Individual;

Individual compareFitness(Individual a, Individual b);

void initializePopulation(Individual population[], int size, int **original);

void calculateFitness(Individual *individual, int size);

void evaluatePopulation(Individual population[], int size);

Individual selectParentRoulette(Individual population[]);

Individual selectParentTournament(Individual population[]);

Individual selectParentRanking(Individual population[]);

void crossoverSingle(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size);

void crossoverMulti(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size);

void crossoverUni(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size);

void mutateRow(Individual *individual, int size, int **original);

void mutateCol(Individual *individual, int size, int **original);

void mutateBox(Individual *individual, int size, int **original);

void geneticAlgorithm(int **grid, int size);

#endif //GENETIC_H
