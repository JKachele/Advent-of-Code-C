/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 31, 2026 13:57:11 UTC
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

void printIntcode(talint64 intcode) {
        for (int i = 0; i < (int)intcode.length; i++) {
                printf("%ld ", intcode.array[i]);
                if (i % 20 == 19)
                        printf("\n");
        }
        printf("\n");
}

talint64 copyList(talint64 list) {
        talint64 newList = tal_init();
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(newList, list.array[i]);
        }
        return newList;
}

void part1(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        talint64 inputs = tal_init();
        talint64 outputs = tal_init();

        tal_add(inputs, 1); // Provided input

        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);
        tal_destroy(intcode);

        int64 keycode = tal_back(outputs);

        printf("Part 1: %ld\n\n", keycode);
}

void part2(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        talint64 inputs = tal_init();
        talint64 outputs = tal_init();

        tal_add(inputs, 2); // Provided input

        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);

        int64 coordinates = tal_back(outputs);
        printf("Part 2: %ld\n", coordinates);
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
                char *file = "assets/tests/2019/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day09.txt";
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

