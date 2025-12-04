/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Dec 04, 2025 14:13:08 UTC
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
        u8 **grid;      // 255: empty, 0-8: paper(numAdjacent)
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(ivec2 s, u8 **grid) {
        for (int y = 0; y < s.y; y++) {
                for (int x = 0; x < s.x; x++) {
                        debugp("%c", grid[y][x] == 255 ? '.' : grid[y][x]+'0');
                }
                debugp("\n");
        }
        debugp("\n");
}

bool isValidPos(int x, int y, ivec2 size) {
        return x >= 0 && x < size.x && y >= 0 && y < size.y;
}

u8 getAdjacent(int xPos, int yPos, ivec2 size, u8 **grid) {
        int adjacent = 0;
        for (int y = yPos-1; y <= yPos+1; y++) {
                for (int x = xPos-1; x <= xPos+1; x++) {
                        if (y == yPos && x == xPos)
                                continue;
                        if (isValidPos(x, y, size) && grid[y][x] != 255)
                                adjacent++;
                }
        }
        return adjacent;
}

void updateAdjacent(int xPos, int yPos, ivec2 size, u8 **grid) {
        for (int y = yPos-1; y <= yPos+1; y++) {
                for (int x = xPos-1; x <= xPos+1; x++) {
                        if (isValidPos(x, y, size) && grid[y][x] != 255)
                                grid[y][x]--;
                }
        }
}

int32 removeRolls(ivec2 size, u8 **grid) {
        // Loop over grid and remove rolls that are less than 4 adjacent
        // Update surounding rolls
        int32 rollsRemoved = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x] < 4) {
                                grid[y][x] = 255;
                                updateAdjacent(x, y, size, grid);
                                rollsRemoved++;
                        }
                }
        }
        return rollsRemoved;
}

void part1(struct input input) {
        ivec2 size = input.size;
        u8 **grid = input.grid;

        // Set each position to number of adjacent rolls
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x] != 255) {
                                u8 adjacnt = getAdjacent(x, y, size, grid);
                                grid[y][x] = adjacnt;
                        }
                }
        }
        printGrid(size, grid);

        int32 numReachable = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x] < 4) {
                                numReachable++;
                                debugp("[%d, %d] Reachable\n", x, y);
                        }
                }
        }

        printf("Part 1: %d\n\n", numReachable);
}

void part2(struct input input) {
        ivec2 size = input.size;
        u8 **grid = input.grid;

        // Grid still populated from part 1
        // Remove rolls until no rolls can be removed
        int32 removed;
        int32 totalRemoved = 0;
        do {
                removed = removeRolls(size, grid);
                totalRemoved += removed;
                // printGrid(size, grid);
        } while (removed > 0);
        printGrid(size, grid);

        printf("Part 2: %d\n", totalRemoved);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        u8 **grid = malloc(size.y * sizeof(u8*));
        input.size = size;
        input.grid = grid;

        llNode *current = ll->head;
        int y = 0;
        while(current != NULL) {
                char *str = (char*)current->data;
                grid[y] = malloc(size.x * sizeof(u8));

                for (int x = 0; x < size.x; x++) {
                        grid[y][x] = (str[x] == '@') ? 0 : 255;
                }

                current = current->next;
                y++;
        }
        printGrid(size, grid);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day04.txt";
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

