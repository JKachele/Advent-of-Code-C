/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Jul 08, 2024 07:52:56 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

#define EXPANSION 1000000

typedef struct {
    int x;
    int y;
} vector2;

typedef struct {
    int x;
    int y;
    int emptyX; // Empty columns to the left
    int emptyY; // Empty rows above
} galaxy;

int manhattanDist(vector2 a, vector2 b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

long galaxyDist(galaxy a, galaxy b) {
    vector2 aVec = {.x = a.x, .y = a.y};
    vector2 bVec = {.x = b.x, .y = b.y};
    int dist = manhattanDist(aVec, bVec);
    vector2 aEmpty = {.x = a.emptyX, .y = a.emptyY};
    vector2 bEmpty = {.x = b.emptyX, .y = b.emptyY};
    int numEmptyCrossings = manhattanDist(aEmpty, bEmpty);
    return dist + (numEmptyCrossings * (EXPANSION - 1));
    // -1 to account for original empty row
}

void part1(llist *ll) {
    // Get image into 2D Array
    llNode *current = ll->head;
    int cols = strlen((char*)current->data);
    int rows = ll->length;
    char image[rows][cols];
    int numGalaxies = 0;
    for (int y = 0; y < rows; y++) {
        char *str = (char*)current->data;
        for (int x = 0; x < cols; x++) {
            image[y][x] = str[x];
            if (str[x] == '#') numGalaxies++;
        }
        current = current->next;
    }

    // Count empty rows and columns
    bool emptyRows[rows];
    bool emptyCols[cols];
    memset(emptyRows, 0, rows); // Initialize array to 0(false)
    memset(emptyCols, 0, cols); // Initialize array to 0(false)
    int numEmptyRows = 0;
    int numEmptyCols = 0;
    for (int y = 0; y < rows; y++) { // Count Rows
        bool isEmpty = true;
        for (int x = 0; x < cols; x++) {
            if (image[y][x] != '.') {
                isEmpty = false;
                break;
            }
        }
        emptyRows[y] = isEmpty;
        if (isEmpty) numEmptyRows++;
    }
    for (int x = 0; x < cols; x++) { // Count Cols
        bool isEmpty = true;
        for (int y = 0; y < rows; y++) {
            if (image[y][x] != '.') {
                isEmpty = false;
                break;
            }
        }
        emptyCols[x] = isEmpty;
        if (isEmpty) numEmptyCols++;
    }
    printf("Empty Rows: %d, Empty Cols: %d\n", numEmptyRows, numEmptyCols);

    // Expand Universe
    int rowsExp = rows + numEmptyRows;
    int colsExp = cols + numEmptyCols;
    char imageExp[rowsExp][colsExp];
    memset(imageExp, '.', sizeof(imageExp));
    vector2 galaxies[numGalaxies];
    int galaxyIndex = 0;
    int yExp = 0; // Y value of expanded array
    for (int y = 0; y < rows; y++) {
        int xExp = 0; // Xvalue of expanded array
        for (int x = 0; x < cols; x++) {
            imageExp[yExp][xExp] = image[y][x];
            if (image[y][x] == '#') {
                vector2 galaxy = {.x = xExp, .y = yExp};
                galaxies[galaxyIndex++] = galaxy;
            }
            if (emptyRows[y]) imageExp[yExp + 1][xExp] = '.';
            if (emptyCols[x]) {
                xExp++;
                imageExp[yExp][xExp] = '.';
            }
            xExp++;
        }
        if (emptyRows[y]) yExp++;
        yExp++;
    }

    for (int y = 0; y < rowsExp; y++) {
        for (int x = 0; x < colsExp; x++) {
            printf("%c", imageExp[y][x]);
        }
        printf("\n");
    }
    for (int i = 0; i < numGalaxies; i++) {
        printf("[%d, %d], ", galaxies[i].x, galaxies[i].y);
    }
    printf("\n");

    // Find Manhattan Distance between all points
    int distanceSum = 0;
    for (int i = 0; i < numGalaxies; i++) {
        for (int j = i + 1; j < numGalaxies; j++) {
            int dist = manhattanDist(galaxies[i], galaxies[j]);
            distanceSum += dist;
        }
    }

    printf("Part 1: Distance Sum: %d\n", distanceSum);
}

void part2(llist *ll) {
    // Get image into 2D Array
    llNode *current = ll->head;
    int cols = strlen((char*)current->data);
    int rows = ll->length;
    char image[rows][cols];
    int numGalaxies = 0;
    for (int y = 0; y < rows; y++) {
        char *str = (char*)current->data;
        for (int x = 0; x < cols; x++) {
            image[y][x] = str[x];
            if (str[x] == '#') numGalaxies++;
        }
        current = current->next;
    }

    // Count empty rows and columns
    bool emptyRows[rows];
    bool emptyCols[cols];
    memset(emptyRows, 0, rows); // Initialize array to 0(false)
    memset(emptyCols, 0, cols); // Initialize array to 0(false)
    galaxy galaxies[numGalaxies];
    int galaxyIndex = 0;
    for (int y = 0; y < rows; y++) { // Count Rows
        bool isEmpty = true;
        for (int x = 0; x < cols; x++) {
            if (image[y][x] != '.') {
                isEmpty = false;
                break;
            }
        }
        emptyRows[y] = isEmpty;
    }
    for (int x = 0; x < cols; x++) { // Count Cols
        bool isEmpty = true;
        for (int y = 0; y < rows; y++) {
            if (image[y][x] != '.') {
                isEmpty = false;
                break;
            }
        }
        emptyCols[x] = isEmpty;
    }

    // Get galaxy locations
    int emptyRowNum = 0;
    for (int y = 0; y < rows; y++) {
        if (emptyRows[y]) {
            emptyRowNum++;
            continue;
        }
        int emptyColNum = 0;
        for (int x = 0; x < cols; x++) {
            if (emptyCols[x]) {
                emptyColNum++;
                continue;
            }
            if (image[y][x] == '#') {
                galaxy galaxy = {.x = x, .y = y,
                    .emptyX = emptyColNum, .emptyY = emptyRowNum};
                galaxies[galaxyIndex++] = galaxy;
            }
        }
    }

    // Get distances
    long distanceSum = 0;
    for (int i = 0; i < numGalaxies; i++) {
        for (int j = i + 1; j < numGalaxies; j++) {
            distanceSum += galaxyDist(galaxies[i], galaxies[j]);
        }
    }

    printf("Part 2: Distance Sum: %ld\n", distanceSum);
}

int main(int argc, char *argv[]) {
    llist *ll = getInputFile("assets/2023/Day11.txt");
    // llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}


