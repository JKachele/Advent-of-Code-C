/*************************************************
 *File----------src/2015/Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Nov 05, 2026 12:41:28 EST
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
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024
#define GRID_SIZE 1000

enum opperation {
        ON,
        OFF,
        TOGGLE
};

struct instruction {
        enum opperation opp;
        ivec2 min;
        ivec2 max;
};

struct input {
        int numInstrs;
        struct instruction *instrs;
};

void part1(struct input *input) {
        int numInstrs = input->numInstrs;
        struct instruction *instrs = input->instrs;

        bool lights[GRID_SIZE][GRID_SIZE] = {0};

        for (int i = 0; i < numInstrs; i++) {
                struct instruction in = instrs[i];
                for (int y = in.min.y; y <= in.max.y; y++) {
                        for (int x = in.min.x; x <= in.max.x; x++) {
                                if (in.opp == ON) {
                                        lights[y][x] = true;
                                } else if (in.opp == OFF) {
                                        lights[y][x] = false;
                                } else {
                                        lights[y][x] = !lights[y][x];
                                }
                        }
                }
        }

        int numLit = 0;
        for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                        if (lights[y][x]) {
                                numLit++;
                        }
                }
        }

        printf("Part 1: Lit Lights = %d\n\n", numLit);
}

void part2(struct input *input) {
        int numInstrs = input->numInstrs;
        struct instruction *instrs = input->instrs;

        u32 lights[GRID_SIZE][GRID_SIZE] = {0};

        for (int i = 0; i < numInstrs; i++) {
                struct instruction in = instrs[i];
                for (int y = in.min.y; y <= in.max.y; y++) {
                        for (int x = in.min.x; x <= in.max.x; x++) {
                                if (in.opp == ON) {
                                        lights[y][x]++;
                                } else if (in.opp == OFF) {
                                        if (lights[y][x] > 0) {
                                                lights[y][x]--;
                                        }
                                } else {
                                        lights[y][x] += 2;
                                }
                        }
                }
        }

        u64 brightness = 0;
        for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                        brightness += lights[y][x];
                }
        }

        printf("Part 2: Total Brightness = %lu\n", brightness);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstrs = ll->length;
        input.instrs = malloc(ll->length * sizeof(struct instruction));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct instruction instr;

                char *tok = strtok(str, " ");
                if (tok[1] == 'u') {
                        tok = strtok(NULL, " ");
                        if (tok[1] == 'n') {
                                instr.opp = ON;
                        } else {
                                instr.opp = OFF;
                        }
                } else {
                        instr.opp = TOGGLE;
                }

                instr.min.x = strtol(strtok(NULL, ","), NULL, 10);
                instr.min.y = strtol(strtok(NULL, " "), NULL, 10);
                strtok(NULL, " ");
                instr.max.x = strtol(strtok(NULL, ","), NULL, 10);
                instr.max.y = strtol(strtok(NULL, " "), NULL, 10);

                input.instrs[i++] = instr;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2015/Day06.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2015/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2015/Day06.txt";
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

