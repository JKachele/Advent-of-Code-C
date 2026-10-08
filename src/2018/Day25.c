/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 08, 2026 13:30:43 EDT
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

#define INPUT_BUFFER_SIZE 1024

typedef tal(int) talint;
typedef tal(ivec4) talivec4;

struct point {
        ivec4 pos;
        int constellation;
};

struct input {
        talivec4 coords;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPoints(int numPoints, struct point points[]) {
        for (int i = 0; i < numPoints; i++) {
                ivec4 pos = points[i].pos;
                printf("%d: ", points[i].constellation);
                printf("(%d, %d, %d, %d)\n", pos.x, pos.y, pos.z, pos.w);
        }
}

int manhattanDist(ivec4 a, ivec4 b) {
        return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z) + abs(a.w - b.w);
}

void part1(struct input *input) {
        talivec4 coords = input->coords;
        int numPoints = coords.length;

        struct point points[numPoints];
        tal_for(coords, i) {
                points[i].pos = coords.array[i];
                points[i].constellation = 0;
        }

        int lastConstellation = 0;
        int numConstellations = 0;
        for (int i = 0; i < numPoints; i++) {
                // List of constelations point can join
                talint constellations = tal_init();
                for (int j = 0; j < i; j++) {
                        if (manhattanDist(points[i].pos, points[j].pos) <= 3) {
                                tal_add_unique(constellations, points[j].constellation);
                        }
                }

                if (constellations.length == 0) {
                        // New constelation
                        points[i].constellation = ++lastConstellation;
                        numConstellations++;
                        continue;
                } else if (constellations.length == 1) {
                        points[i].constellation = constellations.array[0];
                        continue;;
                }

                // If point can connect multiple constelations,
                // points in the other constelations will join the first
                int constellation = constellations.array[0];
                points[i].constellation = constellation;
                for (size_t j = 1; j < constellations.length; j++) {
                        numConstellations--;
                        for (int k = 0; k < i; k++) {
                                if (points[k].constellation == constellations.array[j]) {
                                        points[k].constellation = constellation;
                                }
                        }
                }
        }
        // printPoints(numPoints, points);

        printf("Part 1: Number of constellations = %d\n\n", numConstellations);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                ivec4 coord;
                coord.x = strtol(strtok(str, ","), NULL, 10);
                coord.y = strtol(strtok(NULL, ","), NULL, 10);
                coord.z = strtol(strtok(NULL, ","), NULL, 10);
                coord.w = strtol(strtok(NULL, ""), NULL, 10);
                tal_add(input.coords, coord);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day25.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

