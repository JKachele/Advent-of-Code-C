/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 16:32:56 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

struct input {
        ivec2 size;
        u8 **map; // 0: Empty, 1: Right, 2: Down
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, u8 map[][size.x]) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (map[y][x] == 0)
                                printf(".");
                        else if (map[y][x] == 1)
                                printf(">");
                        else
                                printf("v");
                }
                printf("\n");
        }
        printf("\n");
}

int stepMapRight(ivec2 size, u8 map[][size.x]) {
        // Find all cells that can move;
        bool canMove[size.y][size.x];
        int numMoved = 0;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        int nextX = (x + 1) % size.x;
                        if (map[y][x] == 1 && map[y][nextX] == 0) {
                                canMove[y][x] = true;
                                numMoved++;
                        } else {
                                canMove[y][x] = false;
                        }
                }
        }

        // Move all cells that can move
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        int nextX = (x + 1) % size.x;
                        if (canMove[y][x]) {
                                map[y][x] = 0;
                                map[y][nextX] = 1;
                        }
                }
        }

        return numMoved;
}

int stepMapDown(ivec2 size, u8 map[][size.x]) {
        // Find all cells that can move;
        bool canMove[size.y][size.x];
        int numMoved = 0;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        int nextY = (y + 1) % size.y;
                        if (map[y][x] == 2 && map[nextY][x] == 0) {
                                canMove[y][x] = true;
                                numMoved++;
                        } else {
                                canMove[y][x] = false;
                        }
                }
        }

        // Move all cells that can move
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        int nextY = (y + 1) % size.y;
                        if (canMove[y][x]) {
                                map[y][x] = 0;
                                map[nextY][x] = 2;
                        }
                }
        }

        return numMoved;
}

void part1(struct input input) {
        ivec2 size = input.size;
        u8 map[size.y][size.x];
        memcpy(map, input.map, size.x * size.y * sizeof(u8));
        // printMap(size, map);

        int numSteps = 0;
        int numMoved = 0;
        do {
                numMoved = stepMapRight(size, map);
                numMoved += stepMapDown(size, map);
                numSteps++;
        } while (numMoved > 0);

        printf("Part 1: %d\n\n", numSteps);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.map = calloc(size.x * size.y, sizeof(u8));
        u8 (*map)[size.x] = (u8(*)[size.x])input.map;

        llNode *current = ll->head;
        int y = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x = 0; x < size.x; x++) {
                        char c = str[x];
                        if (c == '.')
                                map[y][x] = 0;
                        else if (c == '>')
                                map[y][x] = 1;
                        else
                                map[y][x] = 2;
                }

                current = current->next;
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

