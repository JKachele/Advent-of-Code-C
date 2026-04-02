/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 30, 2026 17:03:51 UTC
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
#include "../util/Intcode2019.h"
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

struct input {
        talint64 intcode;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

talint64 copyList(talint64 list) {
        talint64 newList = tal_init();
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(newList, list.array[i]);
        }
        return newList;
}

void copyList2(talint64 list, talint64 *newList) {
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(*newList, list.array[i]);
        }
}

void MoveListItems(talint64 *dest, talint64 *src) {
        while (src->length > 0) {
                tal_add(*dest, tal_pop_front(*src));
        }
}

void swap(int *a, int *b) {
        int tmp = *a;
        *a = *b;
        *b = tmp;
}

u64 getThrust(talint64 intcode, int phaseSettings[5]) {
        u32 currentInput = 0;
        for (int i = 0; i < 5; i++) {
                talint64 inputs = tal_init();
                talint64 outputs = tal_init();
                tal_add(inputs, phaseSettings[i]);
                tal_add(inputs, currentInput);

                talint64 copy = copyList(intcode);
                runIntcode(&copy, (haltmode){0}, &inputs, &outputs);
                tal_destroy(copy);
                currentInput = tal_back(outputs);

                tal_destroy(inputs);
                tal_destroy(outputs);
        }
        return currentInput;
}

u64 getThrustLoop(talint64 intcode, int phaseSettings[5]) {
        talint64 memory[5] = {0};
        talint64 inputs[5] = {0};
        talint64 outputs[5] = {0};
        haltmode halts[5] = {0};
        for (int i = 0; i < 5; i++) {
                tal_add(inputs[i], phaseSettings[i]);
                copyList2(intcode, &memory[i]);
        }
        tal_add(inputs[0], 0);

        for (;;) {
                bool allHalt = true;
                for (int i = 0; i < 5; i++) {
                        debugp("**************** AMP %d ****************\n", i);
                        halts[i] = runIntcode(&memory[i], halts[i], &inputs[i], &outputs[i]);
                        MoveListItems(&inputs[(i+1)%5], &outputs[i]);
                        if (!halts[i].halt)
                                allHalt = false;
                }
                if (allHalt)
                        break;
        }

        return tal_back(inputs[0]);
}

u64 getMaxThrust(talint64 intcode, int phases[], int l, int r, bool part1) {
        static u64 maxThrust = 0;
        if (l == r) {
                u64 thrust = part1 ? getThrust(intcode, phases) : getThrustLoop(intcode, phases);
                if (thrust > maxThrust)
                        maxThrust = thrust;
        } else {
                for (int i = l; i <= r; i++) {
                        swap(&phases[l], &phases[i]);
                        u64 thrust = getMaxThrust(intcode, phases, l + 1, r, part1);
                        if (thrust > maxThrust)
                                maxThrust = thrust;

                        // Backtrack
                        swap(&phases[l], &phases[i]);
                }
        }

        return maxThrust;
}

void part1(struct input *input) {
        int phases[] = {0, 1, 2, 3, 4};
        u64 maxThrust = getMaxThrust(input->intcode, phases, 0, 4, true);

        printf("Part 1: %lu\n\n", maxThrust);
}

void part2(struct input *input) {
        int phases[] = {5,6,7,8,9};
        u64 maxThrust = getMaxThrust(input->intcode, phases, 0, 4, false);

        printf("Part 2: %lu\n", maxThrust);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                int64 num = strtoul(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
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

