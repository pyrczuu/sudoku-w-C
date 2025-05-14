#ifndef ENGINE_H
#define ENGINE_H
#include <math.h>
#include <stdlib.h>
//#include <conio.h>
#include <ctype.h>

int generateRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void printCongratulations() {
    printf(" _____                             _         _       _   _                 _ \n");
    printf("/  __ \\                           | |       | |     | | (_)               | |\n");
    printf("| /  \\/ ___  _ __   __ _ _ __ __ _| |_ _   _| | __ _| |_ _  ___  _ __  ___| |\n");
    printf("| |    / _ \\| '_ \\ / _` | '__/ _` | __| | | | |/ _` | __| |/ _ \\| '_ \\/ __| |\n");
    printf("| \\__/\\ (_) | | | | (_| | | | (_| | |_| |_| | | (_| | |_| | (_) | | | \\__ \\_|\n");
    printf(" \\____/\\___/|_| |_|\\__, |_|  \\__,_|\\__|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_|___(_)\n");
    printf("                    __/ |                                                    \n");
    printf("                   |___/                                                     \n");
}

void printSudokuInstructions() {
    printf("Objective:\n");
    printf("Fill the 9x9 grid with digits so that each row, each column, and each 3x3 subgrid (also called a \"box\") contains all of the digits from 1 to 9, without repetition.\n");
    printf("\n");
    printf("How to Play:\n");
    printf("1. Initial Setup:\n");
    printf("   The Sudoku puzzle begins with some cells already filled in. These are your clues.\n");
    printf("\n");
    printf("2. Rules:\n");
    printf("   - Each row must contain the numbers 1 to 9, with no repetition.\n");
    printf("   - Each column must contain the numbers 1 to 9, with no repetition.\n");
    printf("   - Each 3x3 subgrid must also contain the numbers 1 to 9, with no repetition.\n");
    printf("\n");
    printf("3. Filling in the Grid:\n");
    printf("   Choose an empty cell and fill it with a number from 1 to 9. Make sure the number does not already appear in the same row, column, or subgrid.\n");
    printf("\n");
    printf("4. Hints and Help:\n");
    printf("   If you're stuck, try using logical strategies such as:\n");
    printf("   - Look for rows, columns, or boxes with only one possible place for a number.\n");
    printf("   - Use the process of elimination to rule out possible numbers.\n");
    printf("\n");
    printf("5. Winning the Game:\n");
    printf("   The game is complete when all cells are filled correctly, following the Sudoku rules.\n");
    printf("\n");
    printf("6. Controls:\n");
    printf("    The rows are marked with numbers and the columns with uppercase letters, to choose your cell, simply type in the coordinates following the instructions on screen.\n");
    printf("\n");
    printf("Good Luck!\n");
}

int unUsedInBox(int **grid, int size, int rowStart, int colStart, int num) {            // generacja planszy jest po prostu kopią rozwiązania z geeksforgeeks
    int boxSize = (int)sqrt(size);
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            if ((rowStart + i) < size && (colStart + j) < size) {
                if (grid[rowStart + i][colStart + j] == num) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int unUsedInRow(int **grid, int size, int i, int num) {
    if (i >= size) return 0;
    for (int j = 0; j < size; j++) {
        if (grid[i][j] == num) {
            return 0;
        }
    }
    return 1;
}

int unUsedInCol (int **grid,int size, int j, int num) {
    if (j >= size) return 0;
    for (int i = 0; i < size; i++) {
        if (grid[i][j] == num) {
            return 0;
        }
    }
    return 1;
}

int checkIfSafe(int **grid, int size, int i, int j, int num) {
    int boxSize = (int)sqrt(size);
    return (unUsedInRow(grid,size, i, num) &&
            unUsedInCol(grid,size, j, num) &&
            unUsedInBox(grid, size, i - i % boxSize, j - j % boxSize, num ));
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

void fillDiagonal(int **grid, int size) {
    int boxSize = sqrt(size);
    for (int i = 0; i < size; i = i + boxSize) {
        fillBox(grid, size, i, i);
    }
}

int fillRemaining(int **grid, int size, int i, int j) {

    if (i == size) {
        return 1;
    }

    if (j == size) {
        return fillRemaining(grid, size, i+1, 0);
    }

    if (grid[i][j] != 0) {
        return fillRemaining(grid, size, i, j+1);
    }

    for (int num = 1; num <= size; num++) {
        if (checkIfSafe(grid, size, i, j, num)) {
            grid[i][j] = num;
            if (fillRemaining(grid, size, i, j + 1)) {
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

void freeGrid(int **grid, int size) {
    for (int i = 0; i < size; i++) {
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
    if (!fillRemaining(grid, size, 0, 0)) {                     // fillRemaining zwraca true/false więc jeśli nie wyjdzie to abordaż
        printf("Failed to generate valid Sudoku grid.\n");
        freeGrid(grid, size);
        return NULL;
    }
    removeKDigits(grid, size, k);

    return grid;
}

void saveGridToFile(const char *filename, int **grid, int size, int k) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%d\n", size);                // zapisujemy w pliku te zmienne bo są potrzebne do gry
    fprintf(file, "%d\n", k);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d ", grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


int** loadGridFromFile(const char *filename, int *size, int *k) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fscanf(file, "%d", size);
    fscanf(file, "%d", k);

    int **grid = (int**) malloc(*size * sizeof(int*));
    for (int i = 0; i < *size; i++) {
        grid[i] = (int*) malloc(*size * sizeof(int));
    }

    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            fscanf(file, "%d", &grid[i][j]);
        }
    }

    fclose(file);
    return grid;
}



void printSudokuAsciiArt() {
    printf("   ,-,--.                             _,.---._    ,--.-.,-.               \n");
    printf(" ,-.'-  _\\ .--.-. .-.-. _,..---._   ,-.' , -  `. /==/- |\\  \\ .--.-. .-.-. \n");
    printf("/==/_ ,_.'/==/ -|/=/  /==/,   -  \\ /==/_,  ,  - \\|==|_ `/_ //==/ -|/=/  | \n");
    printf("\\==\\  \\   |==| ,||=| -|==|   _   _\\==|   .=.     |==| ,   / |==| ,||=| -| \n");
    printf(" \\==\\ -\\  |==|- | =/  |==|  .=.   |==|_ : ;=:  - |==|-  .|  |==|- | =/  | \n");
    printf(" _\\==\\ ,\\ |==|,  \\/ - |==|,|   | -|==| , '='     |==| _ , \\ |==|,  \\/ - | \n");
    printf("/==/\\/ _ ||==|-   ,   /==|  '='   /\\==\\ -    ,_ //==/  '\\  ||==|-   ,   / \n");
    printf("\\==\\ - , //==/ , _  .'|==|-,   _`/  '.='. -   .' \\==\\ /\\=\\.'/==/ , _  .'  \n");
    printf(" `--`---' `--`..---'  `-.`.____.'     `--`--''    `--`      `--`..---'    \n");
}

void printGrid(int **grid, int size) {
    int boxSize = sqrt(size);
    int lineLength = size*4 + boxSize+1+3; /* obliczanie jak długa ma być linia pozioma, size*4 bo każda liczba zajmuje 4 znaki,
                                            boxSize+1+3, bo kolumn jest boxSize+1 i 3 znaki na oznaczenie wiersza */
    char colName = 'A';
    printf("\n");
    printf("   ");
    for (int i = 0; i < boxSize; i++) {

        for (int j = 0; j < boxSize; j++) {             // zrobione na charach
            printf("%4c", colName);
            colName++;
        }
        printf(" ");
    }
    printf("\n");
    for (int i = 0; i < lineLength; i++) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("%2d ", i+1);        //numery wierszy
        printf("|");
        for (int j = 0; j < size; j++) {
            printf(" %2d ", grid[i][j]);
            if ((j+1) % boxSize == 0) {
                printf("|");
            }
        }
        printf("\n");
        if ((i+1) % boxSize == 0) {
            for (int _ = 0; _ < lineLength; _++) {
                printf("-");
            }
            printf("\n");
        }
    }
}

void playSudoku(int **grid,int size, int k) {
    int row;
    int column;
    int number;
    printGrid(grid, size);
    printf("Enter -1 at any time to save and exit.\n");
    while (k > 0) {
        while (1) {
            do {
                printf("Row: ");
                scanf(" %d", &row);
                if (row == -1) {
                    saveGridToFile("savedGame.txt", grid, size, k);
                    exit(0);
                }
                if (row <= 0 || row > size) {
                    printf("Row out of range.\n");
                }
            } while (row <= 0 || row > size);
            do {
                char colChar;
                printf("Column: ");
                scanf(" %c", &colChar);
                colChar = tolower(colChar);
                column = colChar - 'a' + 1;
                if (column <= 0 || column > size) {
                    printf("Column out of range.\n");               // troche bez sensu po zmianie oznaczenia kolumn z liczb na litery ale mecz się zaczyna
                }
                else if (row == -1) {
                    saveGridToFile("savedGame.txt", grid, size, k);
                    exit(0);
                }
            } while (column <= 0 || column > size);
            if (grid[row-1][column-1] != 0) {
                printf("Already filled!\n");
                continue;
            }
            do {
                printf("Number: ");
                scanf(" %d", &number);
                if (row == -1) {
                    saveGridToFile("savedGame.txt", grid, size, k);
                    exit(0);
                }
                if (number <= 0 || number > size) {
                    printf("Number out of range.\n");
                }
                else if (!checkIfSafe(grid, size, row-1, column-1, number)) {
                    printf("Invalid number.\n");
                }
            }while (number <= 0 || number > size || !checkIfSafe(grid, size, row-1, column-1, number));
            break;
        }
        grid[row-1][column-1] = number;
        k--;
        printGrid(grid, size);
    }
    printCongratulations();
}

void newGame() {
    char choice = 'X';
    int size = 0;
    float diffMultiplier = 0.0;
    int k = 0;
    printf("Choose your grid: \n");
    printf("[1] - 4 x 4\n");
    printf("[2] - 9 x 9\n");
    printf("[3] - 16 x 16\n");
    scanf(" %c", &choice);
    switch (choice) {
        case '1':
            size = 4;
            break;
        case '2':
            size = 9;
            break;
        case '3':
            size = 16;
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
    if (size != 0) {
        printf("Choose your difficulty:\n");
        printf("[1] - Easy\n");
        printf("[2] - Medium\n");
        printf("[3] - Hard\n");
        scanf(" %c", &choice);
        switch (choice) {
            case '1':
                diffMultiplier = 0.4;                   // nie wiem ile te wartości mają wspólnego z rzeczywistością ale nie chciałem mięć jakichś stałych k zapisanych
                break;
            case '2':
                diffMultiplier = 0.5;
                break;
            case '3':
                diffMultiplier = 0.6;
                break;
            default:
                printf("Invalid choice\n");
                return;
        }
        k = (int)(diffMultiplier * (size * size));
        int **grid = sudokuGenerator(size,k);
        playSudoku(grid,size,k);

        freeGrid(grid,size);
    }
    else {
        printf("Invalid choice\n");
        exit(0);
    }
}

void menuScreen() {
    char choice;
    int returnToMenu = 1;
    while (returnToMenu) {                                              // while żeby można się było cofnąć
        printSudokuAsciiArt();
        printf("\n");
        printf("Welcome to Sudoku, choose an option : \n");
        printf("[1] New game\n");
        printf("[2] Load game\n");
        printf("[3] How to play\n");
        printf("[4] Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                newGame();
            break;
            case '2':
                int size;
            int k;
            int **grid = loadGridFromFile("savedGame.txt", &size, &k);
            if (grid == NULL) {
                printf("Failed to load grid\n");
                return;
            }
            playSudoku(grid,size, k);
            break;
            case '3':
                printSudokuInstructions();
            printf("Enter -1 to exit:");
            int exitChoice;
            scanf(" %d", &exitChoice);
            if (exitChoice == -1) {
                returnToMenu = 1;
            }
            else {
                returnToMenu = 0;
            }
            break;
            case '4':
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
}

#endif //ENGINE_H
