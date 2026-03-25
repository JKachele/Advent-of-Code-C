/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 17:23:42 UTC
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
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

struct input {
        int keyPublic;
        int doorPublic;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int transformStep(int value, int subject) {
        int64 new = (int64)value * subject;
        return new % 20201227;
}

void part1(struct input input) {
        int keyPublic = input.keyPublic;
        int doorPublic = input.doorPublic;

        int keyLoop = 0;
        int doorLoop = 0;

        int value = 1;
        int subject = 7;
        int i = 0;
        for (;;) {
                if (value == keyPublic) {
                        keyLoop = i;
                        if (doorLoop != 0)
                                break;
                }
                if (value == doorPublic) {
                        doorLoop = i;
                        if (keyLoop != 0)
                                break;
                }
                value = transformStep(value, subject);
                i++;
        }
        // printf("Key: %d, Door: %d\n", keyLoop, doorLoop);

        value = 1;
        subject = doorPublic;
        for (int i = 0; i < keyLoop; i++) {
                value = transformStep(value, subject);
        }

        printf("Part 1: %d\n\n", value);
}

void part2(struct input input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        input.keyPublic = strtol(ll->head->data, NULL, 10);;
        input.doorPublic = strtol(ll->tail->data, NULL, 10);;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

