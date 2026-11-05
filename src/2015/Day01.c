/*************************************************
 *File----------src/2015/Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Nov 05, 2026 10:08:05 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 8192

struct input {
        char *instrs;
};

void part1(struct input *input) {
        char *instrs = input->instrs;

        int floor = 0;
        for (int i = 0; i < (int)strlen(instrs); i++) {
                if (instrs[i] == '(') {
                        floor++;
                }
                if (instrs[i] == ')') {
                        floor--;
                }
        }

        printf("Part 1: Floor = %d\n\n", floor);
}

void part2(struct input *input) {
        char *instrs = input->instrs;

        int floor = 0;
        int charToBasement = 0;
        for (int i = 0; i < (int)strlen(instrs); i++) {
                if (instrs[i] == '(') {
                        floor++;
                } else {
                        floor--;
                }

                if (floor < 0) {
                        charToBasement = i + 1;
                        break;
                }
        }

        printf("Part 2: Basement at %d\n", charToBasement);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.instrs = malloc(strlen(str) + 1);
        strcpy(input.instrs, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2015/Day01.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2015/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2015/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

