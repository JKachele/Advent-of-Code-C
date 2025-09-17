/*************************************************
 *File----------Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 17, 2025 15:29:53 UTC
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

typedef tll(u8) tllu8;

struct input {
        tllu8 fish;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void updateAges(int64 ages[9]) {
        int64 newFish = 0;
        for (int i=0; i<9; i++) {
                int64 numFish = ages[i];
                if (i == 0) {
                        newFish = numFish;
                } else {
                        ages[i-1] = ages[i];
                }
        }
        ages[6] += newFish;
        ages[8] = newFish;
}

void part1(struct input input) {
        const int32 NumDays = 80;

        int64 ages[9] = {0};
        tll_foreach(input.fish, it)
                ages[it->item]++;

        for (int i=0; i<NumDays; i++) {
                updateAges(ages);
        }

        int64 totalFish = 0;
        for (int i=0; i<9; i++) {
                totalFish += ages[i];
        }

        printf("Part 1: %ld\n\n", totalFish);
}

void part2(struct input input) {
        const int32 NumDays = 256;

        int64 ages[9] = {0};
        tll_foreach(input.fish, it)
                ages[it->item]++;

        for (int i=0; i<NumDays; i++) {
                updateAges(ages);
        }

        int64 totalFish = 0;
        for (int i=0; i<9; i++) {
                totalFish += ages[i];
        }

        printf("Part 2: %ld\n\n", totalFish);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tllu8 fish = tll_init();

        char *str = (char*)ll->head->data;
        char *token = strtok(str, ",");
        while (token != NULL) {
                u8 fishNum = strtol(token, NULL, 10);
                tll_push_back(fish, fishNum);
                token = strtok(NULL, ",");
        }
        input.fish = fish;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day06.txt";
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

