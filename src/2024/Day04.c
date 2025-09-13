/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 09, 2024 17:01:25 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

const int DIR_COUNT = 8;

int SIZE_X = 0;
int SIZE_Y = 0;

typedef enum {
        N,
        NE,
        E,
        SE,
        S,
        SW,
        W,
        NW
} dir_t;

bool isValidPos(int x, int y) {
        return ((x >= 0) && (x < SIZE_X)
                        && (y >= 0) && (y < SIZE_Y));
}

// Get next space based on current position and direction.
// nextPos is of length 2 and is the output. -1 for invalis pos
int getNextSpace(int nextPos[], int x, int y, dir_t dir) {
        nextPos[0] = -1;
        switch (dir) {
        case N: // North
                if (!isValidPos(x, y - 1))
                        break;
                nextPos[0] = x;
                nextPos[1] = y - 1;
                break;
        case NE: // North-East
                if (!isValidPos(x + 1, y - 1))
                        break;
                nextPos[0] = x + 1;
                nextPos[1] = y - 1;
                break;
        case E: // East
                if (!isValidPos(x + 1, y))
                        break;
                nextPos[0] = x + 1;
                nextPos[1] = y;
                break;
        case SE: // South-East
                if (!isValidPos(x + 1, y + 1))
                        break;
                nextPos[0] = x + 1;
                nextPos[1] = y + 1;
                break;
        case S: // South
                if (!isValidPos(x, y + 1))
                        break;
                nextPos[0] = x;
                nextPos[1] = y + 1;
                break;
        case SW: // South-West
                if (!isValidPos(x - 1, y + 1))
                        break;
                nextPos[0] = x - 1;
                nextPos[1] = y + 1;
                break;
        case W: // West
                if (!isValidPos(x - 1, y))
                        break;
                nextPos[0] = x - 1;
                nextPos[1] = y;
                break;
        case NW: // North-West
                if (!isValidPos(x - 1, y - 1))
                        break;
                nextPos[0] = x - 1;
                nextPos[1] = y - 1;
                break;
        default:
                return 2;
        }
        return 0;
}

// dir: 0:N, 1:NE, 2:E, 3:SE, 4:S, 5:SW, 6:W, 7:NW
int countXmas(char arr[][SIZE_X], int charNum, int x, int y, int dir) {
        // Count instances of "XMAS" usind recursive calls looking for the
        // next character in the word
        int nextPos[2];
        switch (charNum) {
        case 0: // X
                if (arr[y][x] != 'X')
                        break;
                int count = 0;
                // Loop through all 8 directions
                for (int d = 0; d < DIR_COUNT; d++) {
                        getNextSpace(nextPos, x, y, d);
                        // Check if the next position is valid
                        if (nextPos[0] == -1)
                                continue;
                        // recurse function with charNum = 1 to look for 'M'
                        count += countXmas(arr, 1, nextPos[0], nextPos[1], d);
                }
                return count;
        case 1: // M
                if (arr[y][x] != 'M')
                        break;
                getNextSpace(nextPos, x, y, dir);
                if (nextPos[0] == -1)
                        break;
                return countXmas(arr, 2, nextPos[0], nextPos[1], dir);
        case 2: // A
                if (arr[y][x] != 'A')
                        break;
                getNextSpace(nextPos, x, y, dir);
                if (nextPos[0] == -1)
                        break;
                return countXmas(arr, 3, nextPos[0], nextPos[1], dir);
        case 3: // S
                if (arr[y][x] == 'S') {
                        return 1;
                } else {
                        return 0;
                }
        default:
                fprintf(stderr, "Invalid charNum!");
                break;
        }
        return 0;
}

int countMAS(char arr[][SIZE_X], int x, int y) {
        if (arr[y][x] != 'A')
                return 0;

        int nw[2], se[2];
        getNextSpace(nw, x, y, NW);
        getNextSpace(se, x, y, SE);

        if (nw[0] == -1 || se[0] == -1)
                return 0;
        
        if (arr[nw[1]][nw[0]] == 'M') {
                if (arr[se[1]][se[0]] != 'S')
                        return 0;
        } else if (arr[nw[1]][nw[0]] == 'S') {
                if (arr[se[1]][se[0]] != 'M')
                        return 0;
        } else {
                return 0;
        }

        int ne[2], sw[2];
        getNextSpace(ne, x, y, NE);
        getNextSpace(sw, x, y, SW);

        if (ne[0] == -1 || sw[0] == -1)
                return 0;
        
        if (arr[ne[1]][ne[0]] == 'M') {
                if (arr[sw[1]][sw[0]] != 'S')
                        return 0;
        } else if (arr[ne[1]][ne[0]] == 'S') {
                if (arr[sw[1]][sw[0]] != 'M')
                        return 0;
        } else {
                return 0;
        }

        return 1;
}

void part1(llist *ll) {
        llNode *current = ll->head;
        SIZE_X = strlen((char*)current->data);
        SIZE_Y = ll->length;
        char searchGrid[SIZE_Y][SIZE_X];
        int lineNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int i = 0; i < SIZE_X; i++) {
                        searchGrid[lineNum][i] = str[i];
                }
                current = current->next;
                lineNum++;
        }

        int xmasCount = 0;
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        xmasCount += countXmas(searchGrid, 0, x, y, 0);
                }
        }

        printf("\nPart 1: XMAS Count: %d\n", xmasCount);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        SIZE_X = strlen((char*)current->data);
        SIZE_Y = ll->length;
        char searchGrid[SIZE_Y][SIZE_X];
        int lineNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int i = 0; i < SIZE_X; i++) {
                        searchGrid[lineNum][i] = str[i];
                }
                current = current->next;
                lineNum++;
        }

        int xmasCount = 0;
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        xmasCount += countMAS(searchGrid, x, y);
                }
        }

        printf("\nPart 2: X-MAS Count: %d\n", xmasCount);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day04.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day04.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


