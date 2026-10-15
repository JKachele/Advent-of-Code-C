/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 15, 2026 11:35:55 EDT
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
#include "../util/talist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 32768

enum direction {
        NORTH,
        NORTHEAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        NORTHWEST
};
typedef tal(enum direction) taldir;

struct input {
        taldir directions;
};

// Using cubic coordinates described here: https://www.redblobgames.com/grids/hexagons/
static const ivec3 DIRS[6] = {{{0, -1, 1}}, {{1, -1, 0}}, {{1, 0, -1}},
                              {{0, 1, -1}}, {{-1, 1, 0}}, {{-1, 0, 1}}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printDirs(taldir dirs) {
        const char *DIR_NAMES[6] = {"North", "NorthEast", "SouthEast",
                                    "South", "SouthWest", "NorthWest"};
        tal_for(dirs, i) {
                printf("%s ", DIR_NAMES[dirs.array[i]]);
        }
        printf("\n");
}

void part1_2(struct input *input) {
        taldir dirs = input->directions;
        // printDirs(dirs);

        ivec3 pos = {0};
        int maxDist = 0;
        tal_for(dirs, i) {
                pos = ivec3Add(pos, DIRS[dirs.array[i]]);
                int dist = (abs(pos.x) + abs(pos.y) + abs(pos.z)) / 2;
                if (dist > maxDist) { maxDist = dist; }
                // printf("(%d, %d, %d)\n", pos.x, pos.y, pos.z);
        }

        // Distance with cubic coordinates is 3d manhattan distance divided by 2
        int finalDist = (abs(pos.x) + abs(pos.y) + abs(pos.z)) / 2;

        printf("Part 1: Distance = %d\n\n", finalDist);
        printf("Part 1: Max Distance = %d\n\n", maxDist);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *tok = strtok(str, ",");
        while (tok != NULL) {
                enum direction dir;
                if (strlen(tok) == 1) {
                        if (tok[0] == 'n') {
                                dir = NORTH;
                        } else { // 's'
                                dir = SOUTH;
                        }
                } else {
                        if (tok[0] == 'n') {
                                if (tok[1] == 'e') {
                                        dir = NORTHEAST;
                                } else {
                                        dir = NORTHWEST;
                                }
                        } else {
                                if (tok[1] == 'e') {
                                        dir = SOUTHEAST;
                                } else {
                                        dir = SOUTHWEST;
                                }
                        }
                }
                tal_add(input.directions, dir);
                tok = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day11.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

