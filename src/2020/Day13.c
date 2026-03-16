/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 16, 2026 17:56:19 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(int) tllint;

struct input {
        int earliestDeparture;
        tllint busIds;
        int numRunningBusses;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBusses(tllint busIds) {
        tll_foreach(busIds, it) {
                int id = it->item;
                if (id == -1)
                        printf("x ");
                else
                        printf("%d ", id);
        }
        printf("\n");
}

// Return modular multiplicative inverse of a with respect to m
// Uses Extended Euclid Algorithm
int64 inv(int64 a, int64 m) {
        int64 m0 = m;
        int64 t;
        int64 q;
        int64 x0 = 0;
        int64 x1 = 1;

        if (m == 1) return 0;

        while (a > 1) {
                q = a / m;
                t = m;
                m = a % m;
                a = t;
                t = x0;
                x0 = x1 - q * x0;
                x1 = t;
        }

        if (x1 < 0) x1 += m0;

        return x1;
}

// Chineese Remainder Theorem (CRT)
// Finds the smallest x such that:
// x % num[0] == rem[0]
// x % num[1] == rem[1]
// ...
// x % num[len-1] == rem[len-1]
int64 findMinX(int64 num[], int64 rem[], int len) {
        // Get prod of all nums
        int64 prod = 1;
        for (int i = 0; i < len; i++) 
                prod *= num[i];

        int64 result = 0;

        for (int i = 0; i < len; i++) {
                int64 pp = prod / num[i];
                result += rem[i] * pp * inv(pp, num[i]);
        }

        return result % prod;
}

void part1(struct input input) {
        int earliestDeparture = input.earliestDeparture;
        tllint busIds = input.busIds;
        // printBusses(busIds);

        // Wait time is the id minus the modulus of the earliest departure time and the id
        int minWait = INT_MAX;
        int minBusId = -1;
        tll_foreach(busIds, it) {
                int id = it->item;
                if (id == -1) continue;

                int wait = id - (earliestDeparture % id);
                if (wait < minWait) {
                        minWait = wait;
                        minBusId = id;
                }
        }

        // Multiply the bus ID with the wait time
        int ans = minWait * minBusId;

        printf("Part 1: %d\n\n", ans);
}

// Used chineese remainder theorem
// Based on answer by reddit user u/mcpower_
void part2(struct input input) {
        // Transform bus Ids to the form used by the CRT: x % num[i] == rem[i]
        // x % busId[i] == (-offset[i]) % busId[i]
        // num[i] = busId[i], rem[i] = (offset[i] * -1) % busId[i]

        int64 num[input.numRunningBusses];
        int64 rem[input.numRunningBusses];
        
        int offset = 1;
        int i = 0;
        tll_foreach(input.busIds, it) {
                int id = it->item;
                offset--;
                if (id == -1) continue;

                num[i] = id;
                rem[i] = ((offset % id) + id) % id;
                i++;
        }

        // Input num and rem to CRT to get smallest answer
        int64 ans = findMinX(num, rem, input.numRunningBusses);

        printf("Part 2: %ld\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        input.earliestDeparture = strtol(ll->head->data, NULL, 10);

        char *busStr = strtok(ll->tail->data, ",");
        while (busStr != NULL) {
                int id = -1;
                if (busStr[0] != 'x') {
                        id = strtol(busStr, NULL, 10);
                        input.numRunningBusses++;
                }
                tll_push_back(input.busIds, id);
                busStr = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day13.txt";
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

