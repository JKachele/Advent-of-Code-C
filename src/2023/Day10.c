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
#include <uchar.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

typedef struct {
    int x;
    int y;
} vector2;

bool vectorEquals(vector2 vect1, vector2 vect2) {
    return (vect1.x == vect2.x) && (vect1.y == vect2.y);
}

vector2 getStartDir(int rows, int cols, char tiles[cols][rows], vector2 startPos) {
    vector2 nextPos = {-1, -1};
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
    return nextPos;
}

char16_t getMoveDir(vector2 start, vector2 end) {
    if (start.y == end.y) {
        if (start.x < end.x) {
            return u'';
        } else {
            return u'';
        }
    } else {
        if (start.y < end.y) {
            return u'';
        } else {
            return u'';
        }
    }
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
    //         printf("%c", tiles[y][x]);
    //     }
    //     printf("\n");
    // }
    // printf("Starting at (%d, %d)\n", startPos.x, startPos.y);

    int length = 1;
    vector2 curPos = getStartDir(rows, cols, tiles, startPos);
    vector2 prevPos = startPos;
    // printf("Continuing at (%d, %d)\n", curPos.x, curPos.y);
    while (!vectorEquals(curPos, startPos)) {
        length++;
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
                if (curPos.y - 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x++;
                } else {
                    prevPos = curPos;
                    curPos.y--;
                }
                break;
            case 'J':
                if (curPos.y - 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x--;
                } else {
                    prevPos = curPos;
                    curPos.y--;
                }
                break;
            case '7':
                if (curPos.y + 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x--;
                } else {
                    prevPos = curPos;
                    curPos.y++;
                }
                break;
            case 'F':
                if (curPos.y + 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x++;
                } else {
                    prevPos = curPos;
                    curPos.y++;
                }
                break;
            default:
                printf("ERROR: Off Path. Last position: (%d, %d)\n", prevPos.x, prevPos.y);
                return;
        }
        // printf("Next Node at (%d, %d)\n", curPos.x, curPos.y);
    }
    printf("Length: %d\n", length);
    int farthestPoint = length / 2;

    printf("Part 1: Farthest point is %d steps from the start\n", farthestPoint);
}

void part2(llist *ll) {
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

    // 1=up, 2=right, 3=down, 4=left
    char16_t map[cols][rows];
    for (int y = 0; y < cols; y++) {
        for (int x = 0; x < rows; x++) {
            map[y][x] = ' ';
        }
    }

    vector2 curPos = getStartDir(rows, cols, tiles, startPos);
    vector2 prevPos = startPos;
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
                if (curPos.y - 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x++;
                } else {
                    prevPos = curPos;
                    curPos.y--;
                }
                break;
            case 'J':
                if (curPos.y - 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x--;
                } else {
                    prevPos = curPos;
                    curPos.y--;
                }
                break;
            case '7':
                if (curPos.y + 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x--;
                } else {
                    prevPos = curPos;
                    curPos.y++;
                }
                break;
            case 'F':
                if (curPos.y + 1 == prevPos.y) {
                    prevPos = curPos;
                    curPos.x++;
                } else {
                    prevPos = curPos;
                    curPos.y++;
                }
                break;
            default:
                printf("ERROR: Off Path. Last position: (%d, %d)\n", prevPos.x, prevPos.y);
                return;
        }
        // printf("Next Node at (%d, %d)\n", curPos.x, curPos.y);
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


