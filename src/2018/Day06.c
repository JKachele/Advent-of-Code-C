/*************************************************
 *File----------Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 17, 2026 11:16:56 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/talist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

struct cell {
        int *dists;
        int closest;
        int distSum;
};

struct coord {
        ivec2 coord;
        int numCells;
        bool edge;
};
typedef tal(struct coord) talcoord;

struct input {
        talcoord coords;
        ivec2 size;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printCoords(talcoord coords) {
        for (int i = 0; i < (int)coords.length; i++) {
                struct coord coord = coords.array[i];
                printf("(%d, %d): %d, %s\n", coord.coord.x, coord.coord.y, coord.numCells, 
                                coord.edge ? "Edge" : "Inner");
        }
}

void printGrid(ivec2 size, struct cell grid[size.y][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        struct cell cell = grid[y][x];
                        if (cell.closest < 0)
                                printf(".");
                        else
                                printf("%c", cell.closest + 'a');
                }
                printf("\n");
        }
}

void printGridDist(ivec2 size, struct cell grid[size.y][size.x], const int MaxDist) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        struct cell cell = grid[y][x];
                        if (cell.distSum < MaxDist)
                                printf("#");
                        else
                                printf(".");
                }
                printf("\n");
        }
}

bool isEdge(ivec2 size, int x, int y) {
        return (x == 0) || (y == 0) || (x == size.x - 1) || (y == size.y - 1);
}

int manhattanDist(ivec2 a, ivec2 b) {
        return abs(b.x - a.x) + abs(b.y - a.y);
}

void getDists(struct cell *cell, ivec2 pos, talcoord coords) {
        cell->distSum = 0;
        for (int i = 0; i < (int)coords.length; i++) {
                struct coord coord = coords.array[i];
                cell->dists[i] = manhattanDist(pos, coord.coord);
                cell->distSum += cell->dists[i];
        }
}

void getClosest(struct cell *cell, int numCoords) {
        int minDist = INT32_MAX;
        int closest = 0;
        for (int i = 0; i < numCoords; i++) {
                if (cell->dists[i] < minDist) {
                        minDist = cell->dists[i];
                        closest = i;
                }
        }

        // If multiple coords are tied for closest, set to -1
        int numClosest = 0;
        for (int i = 0; i < numCoords; i++) {
                if (cell->dists[i] == minDist)
                        numClosest++;
        }

        if (numClosest > 1)
                cell->closest = -1;
        else
                cell->closest = closest;
}

void part1(struct input *input) {
        talcoord coords = input->coords;

        // Get bounds of coords (coords are positive)
        ivec2 size = {0};
        for (int i = 0; i < (int)coords.length; i++) {
                struct coord coord = coords.array[i];
                if (coord.coord.x >= size.x) size.x = coord.coord.x + 1;
                if (coord.coord.y >= size.y) size.y = coord.coord.y + 1;
        }
        input->size = size;

        // make coord grid
        struct cell grid[size.y][size.x];
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        struct cell *cell = &grid[y][x];
                        cell->dists = malloc(sizeof(int) * coords.length);
                        getDists(cell, (ivec2){{x, y}}, coords);
                        getClosest(cell, coords.length);

                        if (cell->closest >= 0) {
                                coords.array[cell->closest].numCells++;
                                if (isEdge(size, x, y))
                                        coords.array[cell->closest].edge = true;
                        }
                }
        }
        // printGrid(size, grid);
        // printCoords(coords);

        // Get largest non-edge coord
        int maxSize = 0;
        for (int i = 0; i < (int)coords.length; i++) {
                struct coord coord = coords.array[i];
                if (coord.edge) continue;
                if (coord.numCells > maxSize)
                        maxSize = coord.numCells;
        }

        printf("Part 1: Largest Area = %d\n\n", maxSize);
}

void part2(struct input *input) {
        talcoord coords = input->coords;
        ivec2 size = input->size;

        const int MaxDist = Debug ? 32 : 10000;

        // make coord grid
        int regionSize = 0;
        struct cell grid[size.y][size.x];
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        struct cell *cell = &grid[y][x];
                        cell->dists = malloc(sizeof(int) * coords.length);
                        getDists(cell, (ivec2){{x, y}}, coords);
                        if (cell->distSum < MaxDist)
                                regionSize++;
                }
        }
        // printGridDist(size, grid, MaxDist);
        // printCoords(coords);

        printf("Part 2: Region Size = %d\n", regionSize);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct coord coord = {0};
                coord.coord.x = strtol(strtok(str, ", "), NULL, 10);
                coord.coord.y = strtol(strtok(NULL, ""), NULL, 10);
                tal_add(input.coords, coord);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day06.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

