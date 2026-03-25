/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 18:34:11 UTC
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
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tal(u32) talu32;

struct input {
        talu32 intcode;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

talu32 copyList(talu32 list) {
        talu32 newList = tal_init();
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(newList, list.array[i]);
        }
        return newList;
}

u32 runIntcode(talu32 input) {
        talu32 intcode = copyList(input);
        int index = 0;
        while (intcode.array[index] != 99) {
                if (intcode.array[index] == 1) {
                        u32 op1 = intcode.array[index + 1];
                        u32 op2 = intcode.array[index + 2];
                        u32 dest = intcode.array[index + 3];

                        u32 sum = intcode.array[op1] + intcode.array[op2];
                        intcode.array[dest] = sum;
                } else if (intcode.array[index] == 2) {
                        u32 op1 = intcode.array[index + 1];
                        u32 op2 = intcode.array[index + 2];
                        u32 dest = intcode.array[index + 3];

                        u32 prod = intcode.array[op1] * intcode.array[op2];
                        intcode.array[dest] = prod;
                } else {
                        printf("Invalid opcode at index %d: %d\n", index, intcode.array[index]);
                }
                index += 4;
        }
        u32 output = intcode.array[0];
        tal_destroy(intcode);
        return output;
}

void part1(struct input input) {
        talu32 intcode = copyList(input.intcode);

        // Initial replacements
        intcode.array[1] = 12;
        intcode.array[2] = 2;

        u32 output = runIntcode(intcode);
        tal_destroy(intcode);

        printf("Part 1: %u\n\n", output);
}

void part2(struct input input) {
        const u32 ExpectedOutput = 19690720;
        talu32 intcode = input.intcode;

        u32 ans = 0;
        for (int noun = 0; noun <= 99; noun++) {
                for (int verb = 0; verb <= 99; verb++) {
                        intcode.array[1] = noun;
                        intcode.array[2] = verb;
                        if (runIntcode(intcode) == ExpectedOutput) {
                                ans = 100 * noun + verb;
                                goto END;
                        }
                }
        }
END:

        printf("Part 2: %u\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                u32 num = strtoul(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day02.txt";
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

