/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 15, 2025 15:56:01 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

struct input {
        int32 numDepths;
        int32 *depths;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input input) {
        int32 prev = input.depths[0];
        int32 numIncreases = 0;
        for (int i=1; i<input.numDepths; i++) {
                if (input.depths[i] > prev)
                        numIncreases++;
                prev = input.depths[i];
        }

        printf("Part 1: %d\n\n", numIncreases);
}

void part2(struct input input) {
        int32 *depths = input.depths;

        int32 numIncreases = 0;
        int prev = depths[0] + depths[1] + depths[2];
        for (int i=3; i<input.numDepths; i++) {
                int cur = depths[i] + depths[i-1] + depths[i-2];
                if (cur > prev)
                        numIncreases++;
                prev -= depths[i-3];
                prev += depths[i];
        }

        printf("Part 2: %d\n\n", numIncreases);
}

struct input parseInput(llist *ll) {
        struct input input;
        input.numDepths = ll->length;
        input.depths = calloc(ll->length, sizeof(int32));

        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                input.depths[i] = strtol(str, NULL, 10);

                current = current->next;
                i++;
        }
        // for (int i=0; i<input.numDepths; i++)
        //         debugP("Depth: %d\n", input.depths[i]);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day01.txt";
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

