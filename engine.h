#ifndef ENGINE_H
#define ENGINE_H

int generateRandomNumber(int min, int max);

void printCongratulations();

void printSudokuInstructions();

int unUsedInBox(int **grid, int size, int rowStart, int colStart, int num);

int unUsedInRow(int **grid, int size, int i, int num);

int unUsedInCol (int **grid,int size, int j, int num);

int checkIfSafe(int **grid, int size, int i, int j, int num);

void fillBox(int **grid, int size, int row, int col);

void fillDiagonal(int **grid, int size);

int fillRemaining(int **grid, int size, int i, int j);

void removeKDigits(int **grid, int size,int k);

void freeGrid(int **grid, int size);

int countSolution(int **grid, int size);

int** sudokuGenerator(int size, int k);

void saveGridToFile(const char *filename, int **grid, int size, int k);

int** loadGridFromFile(const char *filename, int *size, int *k);

void printSudokuAsciiArt();

void printGrid(int **grid, int size);

void playSudoku(int **grid,int size, int k);

void newGame();

void menuScreen();

#endif //ENGINE_H
