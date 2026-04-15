/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 15, 2026 13:15:40 UTC
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

#define INPUT_BUFFER_SIZE 16384

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

bool addToList(talint64 *list, int64 val) {
        for (int i = 0; i < (int)list->length; i++)
                if (list->array[i] == val)
                        return false;
        tal_add(*list, val);
        return true;
}

void part1(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        // Create 50 computers;
        talint64 inputs[50] = {0};
        talint64 outputs[50] = {0};
        talint64 code[50] = {0};
        haltmode state[50] = {0};
        for (int i = 0; i < 50; i++) {
                tal_add(inputs[i], i);
                copyIntcodePtr(&code[i], intcode);
        }

        bool halt = false;
        int64 outY = -1;
        while (!halt) {
                for (int i = 0; i < 50; i++) {
                        // Check for empty input
                        if (inputs[i].length == 0)
                                tal_add(inputs[i], -1);

                        // Run computer
                        state[i] = runIntcode(&code[i], state[i], &inputs[i], &outputs[i]);

                        // Send packets to computers
                        for (int j = 0; j < (int)outputs[i].length; j += 3) {
                                int64 dest = outputs[i].array[j];
                                int64 x = outputs[i].array[j+1];
                                int64 y = outputs[i].array[j+2];
                                if (dest == 255) {
                                        halt = true;
                                        outY = y;
                                        break;
                                }
                                tal_add(inputs[dest], x);
                                tal_add(inputs[dest], y);
                        }
                        if (halt) break;
                        
                        // Clear Outputs
                        tal_destroy(outputs[i]);
                }
        }

        printf("Part 1: %ld\n\n", outY);
        tal_destroy(intcode);
        for (int i = 0; i < 50; i++)
                tal_destroy(inputs[i]);
}

void part2(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        // Create 50 computers;
        talint64 inputs[50] = {0};
        talint64 outputs[50] = {0};
        talint64 code[50] = {0};
        haltmode state[50] = {0};
        for (int i = 0; i < 50; i++) {
                tal_add(inputs[i], i);
                copyIntcodePtr(&code[i], intcode);
        }

        int64 natX = 0;
        int64 natY = 0;
        talint64 sentY = tal_init();

        for (;;) {
                bool allIdle = true;
                for (int i = 0; i < 50; i++) {
                        // Check for empty input
                        if (inputs[i].length == 0)
                                tal_add(inputs[i], -1);
                        else
                                allIdle = false;

                        // Run computer
                        state[i] = runIntcode(&code[i], state[i], &inputs[i], &outputs[i]);

                        // Send packets to computers
                        for (int j = 0; j < (int)outputs[i].length; j += 3) {
                                int64 dest = outputs[i].array[j];
                                int64 x = outputs[i].array[j+1];
                                int64 y = outputs[i].array[j+2];
                                if (dest == 255) {
                                        natX = x;
                                        natY = y;
                                }
                                tal_add(inputs[dest], x);
                                tal_add(inputs[dest], y);
                        }
                        
                        // Clear Outputs
                        tal_destroy(outputs[i]);
                }
                if (allIdle) {
                        if (addToList(&sentY, natY)) {
                                tal_add(inputs[0], natX);
                                tal_add(inputs[0], natY);
                        } else {
                                break;
                        }
                }
        }

        printf("Part 2: %ld\n", natY);
        tal_destroy(intcode);
        for (int i = 0; i < 50; i++)
                tal_destroy(inputs[i]);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                int64 num = strtoll(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day23.txt";
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

