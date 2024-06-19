/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday May 13, 2024 08:48:40 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

typedef struct {
    int x;
    int y;
} vector2;

bool vectorEquals(vector2 vect1, vector2 vect2) {
    return (vect1.x == vect2.x) && (vect1.y == vect2.y);
}

vector2 getStartDir(int rows, int cols, char tiles[rows][cols], vector2 startPos) {
    vector2 nextPos = {-1, -1};
    if (startPos.x > 0) {
        char pipe = tiles[startPos.y][startPos.x-1];
        if (pipe == '-' || pipe == 'L' || pipe == 'F') {
            nextPos.x = startPos.x - 1;
            nextPos.y = startPos.y;
            return nextPos;
        }
    }
    if (startPos.y > 0) {
        char pipe = tiles[startPos.y-1][startPos.x];
        if (pipe == '|' || pipe == '7' || pipe == 'F') {
            nextPos.x = startPos.x;
            nextPos.y = startPos.y - 1;
            return nextPos;
        }
    }
    if (startPos.x < rows - 1) {
        char pipe = tiles[startPos.y][startPos.x+1];
        if (pipe == '-' || pipe == '7' || pipe == 'J') {
            nextPos.x = startPos.x + 1;
            nextPos.y = startPos.y;
            return nextPos;
        }
    }
    if (startPos.y < cols - 1) {
        char pipe = tiles[startPos.y+1][startPos.x];
        if (pipe == '|' || pipe == 'L' || pipe == 'J') {
            nextPos.x = startPos.x;
            nextPos.y = startPos.y + 1;
            return nextPos;
        }
    }
    return nextPos;
}

void part1(llist *ll) {
    llNode *current = ll->head;
    int rows = strlen((char*)current->data);
    int cols = ll->length;
    char tiles[cols][rows];
    vector2 startPos = {-1, -1};
    for (int y = 0; y < cols; y++) {
        char *str = (char*)current->data;
        for (int x = 0; x < rows; x++) {
            tiles[y][x] = str[x];
            if (str[x] == 'S') {
                startPos.x = x;
                startPos.y = y;
            }
        }
        current = current->next;
    }
    if (startPos.x == -1 || startPos.y == -1) {
        printf("ERROR: NO START POSITION FOUND\n");
        return;
    }

    // for (int y = 0; y < cols; y++) {
    //     for (int x = 0; x < rows; x++) {
    //         printf("[%c]", tiles[y][x]);
    //     }
    //     printf("\n");
    // }
    // printf("Starting at (%d, %d)\n", startPos.x, startPos.y);

    vector2 curPos = getStartDir(rows, cols, tiles, startPos);
    vector2 prevPos = startPos;
    return;
    while (!vectorEquals(curPos, startPos)) {
        switch (tiles[curPos.y][curPos.x]) {
            case '-':
                if (curPos.x - 1 == prevPos.x) {
                    prevPos = curPos;
                    curPos.x++;
                } else {
                    prevPos = curPos;
                    curPos.x--;
                }
                break;
            case '|':
                if (curPos.y - 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.y++;
                } else {
                    prevPos = curPos;
                    curPos.y--;
                }
                break;
            case 'L':
                break;
            case 'J':
                break;
            case '7':
                break;
            case 'F':
                break;
            default:
                break;
        }
    }

    printf("Part 1: \n");
}

void part2(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
    // llist *ll = getInputFile("assets/2023/Day10.txt");
    llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}


