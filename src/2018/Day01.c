/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 16, 2026 19:32:33 UTC
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
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 16384

typedef tll(int) tllint;

struct input {
        tllint changes;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input *input) {
        int freq = 0;
        tll_foreach(input->changes, it)
                freq += it->item;

        printf("Part 1: %d\n\n", freq);
}

void part2(struct input *input) {
        const int BIAS = 0x80000;
        bool *seen = calloc(0x100000, sizeof(bool));
        seen[BIAS] = true;
        int freq = 0;
        for (;;) {
                bool found = false;
                tll_foreach(input->changes, it) {
                        freq += it->item;
                        if (freq > BIAS) {
                                printf("ERROR: Overflow!\n");
                                found = true;
                                break;
                        }
                        if (seen[freq + BIAS]) {
                                found = true;
                                break;
                        }
                        seen[freq + BIAS] = true;
                }
                if (found) break;
        }

        printf("Part 2: %d\n", freq);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                bool pos = str[0] == '+';
                int num = strtol(str+1, NULL, 10);
                tll_push_back(input.changes, pos ? num : -num);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day01.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day01.txt";
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

