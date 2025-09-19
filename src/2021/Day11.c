/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 18, 2025 16:06:28 UTC
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
        u8 **grid;
};

static const ivec2 DIRS[] = {{{1, 0}}, {{1, 1}}, {{0, 1}}, {{-1, 1}},
                            {{-1, 0}}, {{-1, -1}}, {{0, -1}}, {{1, -1}}};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(ivec2 size, u8 grid[][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (grid[y][x] == 0)
                                printf("*");
                        else
                                printf("%d", grid[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(ivec2 size, ivec2 pos) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

int32 flashOctopus(ivec2 size, u8 grid[][size.x]) {
        int32 numFlashes = 0;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (grid[y][x] <= 9)
                                continue;
                        numFlashes++;
                        grid[y][x] = 0;
                        ivec2 cur = {{x, y}};
                        for (int i=0; i<8; i++) {
                                ivec2 new = ivec2Add(cur, DIRS[i]);
                                if (!validPos(size, new))
                                        continue;
                                if (grid[new.y][new.x] == 0)
                                        continue;
                                grid[new.y][new.x]++;
                        }
                }
        }
        return numFlashes;
}

int32 stepOctopus(ivec2 size, u8 grid[][size.x]) {
        // Increment all by 1
        for (int i=0; i<size.x*size.y; i++)
                grid[i/size.x][i%size.x]++;

        // For every octopus over 9, reset to 0 and increment adjacent cells
        // Do this until no octopus flashes
        int32 totalFlashes = 0;
        int32 numFlashes = 0;
        do {
                numFlashes = flashOctopus(size, grid);
                totalFlashes += numFlashes;
        } while (numFlashes > 0);

        return totalFlashes;
}

void part1(struct input input) {
        const int NumSteps = 100;

        ivec2 size = input.size;
        u8 grid[size.y][size.x];
        memcpy(grid, input.grid, size.x * size.y * sizeof(u8));

        int32 totalFlashes = 0;
        for (int i=0; i<NumSteps; i++) {
                int32 flashes = stepOctopus(size, grid);
                totalFlashes += flashes;
                // printGrid(size, grid);
        }

        printf("Part 1: %d\n\n", totalFlashes);
}

void part2(struct input input) {
        ivec2 size = input.size;
        u8 grid[size.y][size.x];
        memcpy(grid, input.grid, size.x * size.y * sizeof(u8));
        int32 numOctous = size.x * size.y;

        int32 steps = 0;
        int numFlashed = 0;
        while (numFlashed != numOctous) {
                numFlashed = stepOctopus(size, grid);
                steps++;
        }

        printf("Part 2: %d\n", steps);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.grid = calloc(size.x * size.y, sizeof(u8));
        u8 (*grid)[size.x] = (u8(*)[size.x])input.grid;

        llNode *current = ll->head;
        int y = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x = 0; x < size.x; x++) {
                        grid[y][x] = str[x] - '0';
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
                char *file = "assets/tests/2021/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();
        part2(input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

