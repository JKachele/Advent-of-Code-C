/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 16, 2026 13:17:35 EDT
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
// #include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024
#define FACTOR_A 16807
#define FACTOR_B 48271
#define MODULUS 0x7FFFFFFF

struct input {
        int startA;
        int startB;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

u64 updateNum(u64 num, u64 factor) {
        num *= factor;
        num %= MODULUS;
        return num;
}

void part1(struct input *input) {
        const int NumPairs = 40000000;
        const u64 Mask = 0xFFFF;

        u64 genA = input->startA;
        u64 genB = input->startB;

        int judge = 0;
        for (int i = 0; i < NumPairs; i++) {
                genA = updateNum(genA, FACTOR_A);
                genB = updateNum(genB, FACTOR_B);
                
                if ((genA & Mask) == (genB & Mask)) {
                        judge++;
                }
        }

        printf("Part 1: Judge Count = %d\n\n", judge);
}

void part2(struct input *input) {
        const int NumPairs = 5000000;
        const u64 Mask = 0xFFFF;

        u64 genA = input->startA;
        u64 genB = input->startB;

        int judge = 0;
        int numJudged = 0;
        bool readyA = false;
        bool readyB = false;
        while (numJudged < NumPairs) {
                if (!readyA) {
                        genA = updateNum(genA, FACTOR_A);

                        // Gen A must be multiple of 4
                        if ((genA & 3l) == 0) {
                                readyA = true;
                        }
                }
                if (!readyB) {
                        genB = updateNum(genB, FACTOR_B);

                        // Gen B must be multiple of 8
                        if ((genB & 7l) == 0) {
                                readyB = true;
                        }
                }

                if (readyA && readyB) {
                        numJudged++;
                        readyA = false;
                        readyB = false;
                        if ((genA & Mask) == (genB & Mask)) {
                                judge++;
                        }
                }
        }

        printf("Part 2: Judge Count = %d\n", judge);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.startA = strtol(str + 24, NULL, 10);
        str = (char*)ll->head->next->data;
        input.startB = strtol(str + 24, NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day15.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day15.txt";
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
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
        //                 parseTime, pt1Time);

        return 0;
}

