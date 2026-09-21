/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 18, 2026 13:45:58 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
// #include "../util/talist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024
#define GRID_SIZE 300

struct input {
        int serial;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int setPower(int x, int y, int serial) {
        int id = x + 10;
        int power = id * y;
        power += serial;
        power *= id;
        power = (power / 100) % 10;
        power -= 5;
        return power;
}

int findAreaPower(int fuel[GRID_SIZE][GRID_SIZE], int x, int y) {
        int power = 0;
        for (int yi = y; yi < y+3; yi++) {
                for (int xi = x; xi < x+3; xi++) {
                        power += fuel[yi][xi];
                }
        }

        return power;
}

int findSumArea(int sumAreaTable[GRID_SIZE][GRID_SIZE], ivec2 pos, int size) {
        ivec2 bot = {{pos.x + size - 1, pos.y + size - 1}};
        if (bot.x >= GRID_SIZE || bot.y >= GRID_SIZE)
                return 0;

        int sum = sumAreaTable[bot.y][bot.x];

        if (pos.x > 0)
                sum -= sumAreaTable[bot.y][pos.x-1];
        if (pos.y > 0)
                sum -= sumAreaTable[pos.y-1][bot.x];
        if (pos.x > 0 && pos.y > 0)
                sum += sumAreaTable[pos.y-1][pos.x-1];

        return sum;
}

void part1(struct input *input) {
        int serial = input->serial;

        int fuel[GRID_SIZE][GRID_SIZE];
        for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                        fuel[y][x] = setPower(x+1, y+1, serial);
                }
        }

        int maxPower = 0;
        int maxX = 0;
        int maxY = 0;
        for (int y = 0; y < GRID_SIZE-3; y++) {
                for (int x = 0; x < GRID_SIZE-3; x++) {
                        int power = findAreaPower(fuel, x, y);
                        if (power > maxPower) {
                                maxPower = power;
                                maxX = x + 1;
                                maxY = y + 1;
                        }
                }
        }

        printf("Part 1: Max power of %d at (%d, %d)\n\n", maxPower, maxX, maxY);
}

void part2(struct input *input) {
        int serial = input->serial;

        int fuel[GRID_SIZE][GRID_SIZE];
        for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                        fuel[y][x] = setPower(x+1, y+1, serial);
                }
        }

        int sumAreaTable[GRID_SIZE][GRID_SIZE];
        for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                        int sum = fuel[y][x];
                        if (x > 0)
                                sum += sumAreaTable[y][x-1];
                        if (y > 0)
                                sum += sumAreaTable[y-1][x];
                        if (x > 0 && y > 0)
                                sum -= sumAreaTable[y-1][x-1];
                        sumAreaTable[y][x] = sum;
                }
        }

        int maxPower = 0;
        int maxX = 0;
        int maxY = 0;
        int maxSize = 3;
        for (int y = 0; y < GRID_SIZE-3; y++) {
                for (int x = 0; x < GRID_SIZE-3; x++) {
                        for (int i = 1; i < 300; i++) {
                                int power = findSumArea(sumAreaTable, (ivec2){{x, y}}, i);
                                if (power > maxPower) {
                                        maxPower = power;
                                        maxX = x + 1;
                                        maxY = y + 1;
                                        maxSize = i;
                                }
                        }
                }
        }

        printf("Part 2: Max power of %d at (%d, %d) Size of %d\n", maxPower, maxX, maxY, maxSize);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.serial = strtol(str, NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day11.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day11.txt";
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

