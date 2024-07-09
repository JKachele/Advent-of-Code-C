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

typedef struct {
    int x;
    int y;
} vector2;

int manhattanDist(vector2 a, vector2 b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void part1(llist *ll) {
    // Get image into 2D Array
    llNode *current = ll->head;
    int rows = strlen((char*)current->data);
    int cols = ll->length;
    char image[cols][rows];
    int numGalaxies = 0;
    for (int y = 0; y < cols; y++) {
        char *str = (char*)current->data;
        for (int x = 0; x < rows; x++) {
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
    for (int y = 0; y < cols; y++) { // Count Rows (There are "Cols" # of rows)
        bool isEmpty = true;
        for (int x = 0; x < rows; x++) {
            if (image[y][x] != '.') {
                isEmpty = false;
                break;
            }
        }
        emptyRows[y] = isEmpty;
        if (isEmpty) numEmptyRows++;
    }
    for (int x = 0; x < rows; x++) { // Count Cols (There are "Rows" # of cols)
        bool isEmpty = true;
        for (int y = 0; y < cols; y++) {
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
    int rowsExp = rows + numEmptyCols;
    int colsExp = cols + numEmptyRows;
    char imageExp[colsExp][rowsExp];
    memset(imageExp, '.', sizeof(imageExp));
    vector2 galaxies[numGalaxies];
    int galaxyIndex = 0;
    int yExp = 0; // Y value of expanded array
    for (int y = 0; y < cols; y++) {
        int xExp = 0; // Xvalue of expanded array
        for (int x = 0; x < rows; x++) {
            imageExp[yExp][xExp] = image[y][x];
            if (image[y][x] == '#') {
                vector2 galaxy = {.x = xExp, .y = yExp};
                galaxies[galaxyIndex] = galaxy;
                galaxyIndex++;
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

    for (int y = 0; y < colsExp; y++) {
        for (int x = 0; x < rowsExp; x++) {
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
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
    llist *ll = getInputFile("assets/2023/Day11.txt");
    // llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}


