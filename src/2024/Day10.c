/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Dec 24, 2024 11:56:55 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"

int SIZE_X = 0;
int SIZE_Y = 0;

typedef enum {
        UP,
        RIGHT,
        DOWN,
        LEFT
} direction;

void printMap(int map[SIZE_Y][SIZE_X]) {
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        printf("%d", map[y][x]);
                }
                printf("\n");
        }
}

bool isValidPos(vector2 pos) {
        return (pos.x >=0 && pos.x < SIZE_X
                        && pos.y >= 0 && pos.y < SIZE_Y);
}

int getNextPos(vector2 cur, vector2 *next, direction dir) {
        switch (dir) {
        case UP:
                next->x = cur.x;
                next->y = cur.y - 1;
                if (isValidPos(*next)) {
                        return 0;
                } else {
                        next->x = -1;
                        next->y = -1;
                        return 1;
                }
        case RIGHT:
        case DOWN:
        case LEFT:
        default:
                return -1;
        }
}

int findTrails(int map[SIZE_Y][SIZE_X], vector2 pos) {
        if (map[pos.y][pos.x] == 9)
                return 1;

        int next = map[pos.y][pos.x] + 1;
        vector2 nextpos = {-1, -1};
        // Up

        return 0;
}

void part1(llist *ll) {
        SIZE_Y = ll->length;
        SIZE_X = getLongestLine(ll);
        int map[SIZE_Y][SIZE_X];

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < SIZE_X; x++) {
                        map[line][x] = str[x] - '0';
                }
                current = current->next;
                line++;
        }
        // printMap(map);

        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {

                }
        }

        printf("Part 1: \n\n");
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test.txt");
        else
                ll = getInputFile("assets/2024/Day10.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
