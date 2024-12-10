/*************************************************
 *File----------Day4.c
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

bool isValidPos(int x, int y) {
        return ((x >= 0) && (x < SIZE_X)
                        && (y >= 0) && (y < SIZE_Y));
}

// Get next space based on current position and direction.
// nextPos is of length 2 and is the output. -1 for invalis pos
void getNextSpace(int nextPos[], int x, int y, int dir) {
        nextPos[0] = -1;
        switch (dir) {
        case 0: // North
                if (!isValidPos(x, y - 1))
                        break;
                nextPos[0] = x;
                nextPos[1] = y - 1;
                break;
        case 1: // North-East
                if (!isValidPos(x + 1, y - 1))
                        break;
                nextPos[0] = x + 1;
                nextPos[1] = y - 1;
                break;
        case 2: // East
                if (!isValidPos(x + 1, y))
                        break;
                nextPos[0] = x + 1;
                nextPos[1] = y;
                break;
        case 3: // South-East
                if (!isValidPos(x + 1, y + 1))
                        break;
                nextPos[0] = x + 1;
                nextPos[1] = y + 1;
                break;
        case 4: // South
                if (!isValidPos(x, y + 1))
                        break;
                nextPos[0] = x;
                nextPos[1] = y + 1;
                break;
        case 5: // South-West
                if (!isValidPos(x - 1, y + 1))
                        break;
                nextPos[0] = x - 1;
                nextPos[1] = y + 1;
                break;
        case 6: // West
                if (!isValidPos(x - 1, y))
                        break;
                nextPos[0] = x - 1;
                nextPos[1] = y;
                break;
        case 7: // North-West
                if (!isValidPos(x - 1, y - 1))
                        break;
                nextPos[0] = x - 1;
                nextPos[1] = y - 1;
                break;
        default:
                break;
        }
}

// dir: 0:N, 1:NE, 2:E, 3:SE, 4:S, 5:SW, 6:W, 7:NW
int countXmas(char arr[][SIZE_X], int charNum, int x, int y, int dir) {
        switch (charNum) {
        case 0: // X
                if (arr[y][x] != 'X')
                        break;
                int count = 0;
                // Loop through all 8 directions
                for (int d = 0; d < DIR_COUNT; d++) {
                        int nextPos[2];
                        getNextSpace(nextPos, x, y, d);
                        // Check if the next position is valid
                        if (nextPos[0] == -1)
                                continue;
                        // recurse function with charNum = 1 to look for 'M'
                        count += countXmas(arr, 1, nextPos[0], nextPos[1], d);
                }
                break;
        case 1: // M
                break;
        case 2: // A
                break;
        case 3: // S
                break;
        default:
                break;
        }
        return 0;
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

        // Search through array with a scaning 4x4 block looking for "xmas"


        printf("\nPart 1: \n");
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("\nPart 2: \n");
}

int main(int argc, char *argv[]) {
        // llist *ll = getInputFile("assets/2024/Day4.txt");
        llist *ll = getInputFile("assets/test.txt");
        llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


