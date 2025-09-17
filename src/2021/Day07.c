/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 17, 2025 15:41:24 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"
#include "../util/quicksort.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(int32) tllint;

struct input {
        tllint crabs;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int64 getCost(tllint crabs, int32 pos) {
        int64 cost = 0;
        tll_foreach(crabs, it) {
                cost += labs(it->item - pos);
        }
        return cost;
}

int64 getCost2(tllint crabs, int32 pos) {
        int64 cost = 0;
        tll_foreach(crabs, it) {
                int32 dist = abs(it->item - pos);
                double crabCost = dist * (((double)dist + 1) / 2);
                cost += (int64)crabCost;
        }
        return cost;
}

// The best position is the median of the set due to the optimality property of
// medians. The Median of a set has the lowest absolute distance to the data.
void part1(struct input input) {
        int32 numCrabs = tll_length(input.crabs);

        int32 crabs[numCrabs];
        int i = 0;
        tll_foreach(input.crabs, it) {
                crabs[i] = it->item;
                i++;
        }

        int32 median = 0;
        if (numCrabs % 2 == 1) {
                median = quickselect(crabs, 0, numCrabs-1, numCrabs/2);
        } else {
                int32 med1 = quickselect(crabs, 0, numCrabs-1, (numCrabs/2)-1);
                int32 med2 = quickselect(crabs, 0, numCrabs-1, numCrabs/2);
                median = (med1 + med2) / 2;
        }
        int64 cost = getCost(input.crabs, median);

        printf("Part 1: %ld\n\n", cost);
}

// Median won't work here because the costs are't linear
// Cost function contains n^2 so the mean will be close to the best pos
void part2(struct input input) {
        tllint crabs = input.crabs;
        int32 numCrabs = tll_length(crabs);

        int64 total = 0;
        tll_foreach(crabs, it)
                total += it->item;

        int64 avg = total / numCrabs;
        // Check either side of the average and take the min cost;
        int64 minCost = INT64_MAX;
        for (int i=-1; i<=1; i++) {
                int64 cost = getCost2(crabs, avg + i);
                if (cost < minCost)
                        minCost = cost;
        }

        printf("Part 2: %ld\n", minCost);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tllint crabs = tll_init();

        char *str = (char*)ll->head->data;
        char *token = strtok(str, ",");
        while (token != NULL) {
                int32 crab = strtol(token, NULL, 10);
                tll_push_back(crabs, crab);
                token = strtok(NULL, ",");
        }
        input.crabs = crabs;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day07.txt";
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

