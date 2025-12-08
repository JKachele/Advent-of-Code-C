/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 08, 2025 20:50:48 UTC
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

typedef struct cell {
        bool split;
        bool seen;
        u64 paths;
} cell;

struct input {
        ivec2 size;
        cell **grid;
        ivec2 start;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(ivec2 size, cell **grid) {
        if (!Debug) return;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        printf("%c", grid[y][x].split? '^' : '.');
                }
                printf("\n");
        }
        printf("\n");
}

void printGridPath1(ivec2 size, cell **grid) {
        if (!Debug) return;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        char c = '.';
                        if (grid[y][x].seen)
                                c = '|';
                        if (grid[y][x].split)
                                c = '^';
                        printf("%c", c);
                }
                printf("\n");
        }
        printf("\n");
}

void printGridPath2(ivec2 size, cell **grid) {
        if (!Debug) return;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x].split)
                                printf("^");
                        else if (grid[y][x].paths == 0)
                                printf(".");
                        else
                                printf("%lx", grid[y][x].paths);
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(ivec2 size, ivec2 pos) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

int moveTachyon(ivec2 size, cell **grid, ivec2 pos, int splits) {
        // Move down until a splitter
        while (validPos(size, pos) && !grid[pos.y][pos.x].split) {
                grid[pos.y][pos.x].seen = true;
                pos.y++;
        }
        if (!validPos(size, pos)) return splits;

        // Split tachyon
        ivec2 left = (ivec2){{pos.x - 1, pos.y}};
        ivec2 right = (ivec2){{pos.x + 1, pos.y}};

        if (validPos(size, left) && !grid[left.y][left.x].seen) {
                int newSplits = moveTachyon(size, grid, left, splits + 1);
                if (validPos(size, right) && !grid[right.y][right.x].seen) {
                        newSplits += moveTachyon(size, grid, right, 0);
                }
                return newSplits;
        }
        if (validPos(size, right) && !grid[right.y][right.x].seen) {
                int newSplits = moveTachyon(size, grid, right, splits + 1);
                return newSplits;
        }

        return splits;
}

void countPaths(ivec2 size, cell **grid) {
        for (int y = 1; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        // If empty, add path from cell above
                        if (!grid[y][x].split) {
                                grid[y][x].paths += grid[y-1][x].paths;
                        }

                        // if splitter, add path from cell above to either side
                        if (grid[y][x].split) {
                                if (!grid[y][x-1].split)
                                        grid[y][x-1].paths += grid[y-1][x].paths;
                                if (!grid[y][x+1].split)
                                        grid[y][x+1].paths += grid[y-1][x].paths;
                        }
                }
        }
}

void part1(struct input input) {
        ivec2 size = input.size;
        cell **grid = input.grid;
        ivec2 start = input.start;
        printGrid(size, grid);

        int splits = moveTachyon(size, grid, start, 0);
        printGridPath1(size, grid);

        printf("Part 1: %d\n\n", splits);
}

void part2(struct input input) {
        ivec2 size = input.size;
        cell **grid = input.grid;
        ivec2 start = input.start;

        grid[start.y][start.x].paths = 1;
        countPaths(size, grid);
        printGridPath2(size, grid);

        // Count paths on bottom row
        u64 numPaths = 0;
        int y = size.y - 1;
        for (int x = 0; x < size.x; x++) {
                numPaths += grid[y][x].paths;
        }

        printf("Part 2: %lu\n", numPaths);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        cell **grid = malloc(size.y * sizeof(cell*));

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                grid[y] = malloc(size.x * sizeof(cell));
                for (int x = 0; x < size.x; x++) {
                        grid[y][x].seen = false;
                        grid[y][x].paths = 0;
                        grid[y][x].split = (str[x] == '^');
                        if (str[x] == 'S')
                                input.start = (ivec2){{x, y}};
                }
                y++;
        }

        input.size = size;
        input.grid = grid;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day07.txt";
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

