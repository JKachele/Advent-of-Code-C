/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 03, 2026 16:54:12 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/quicksort.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

struct input {
        int len;
        int *adapters;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int64 getNumPaths(int64 hashTable[], int max, int cur) {
        if (cur == max)
                return 1;
        if (hashTable[cur] > 0)
                return hashTable[cur];

        int64 paths = 0;
        for (int i = 1; i <= 3; i++) {
                int index = cur + i;
                if (index > max) break;
                if (hashTable[index] == -1)
                        continue;
                paths += getNumPaths(hashTable, max, index);
        }
        hashTable[cur] = paths;
        return paths;
}

void part1(struct input input) {
        int len = input.len;
        int *adapters = input.adapters;

        // Sort the adapters
        quicksort(adapters, 0, len - 1);

        // Get num of 1 and 3 differences
        int oneDiff = 0;
        int threeDiff = 0;
        for (int i = 1; i < len; i++) {
                if (adapters[i] - adapters[i - 1] == 1)
                        oneDiff++;
                if (adapters[i] - adapters[i - 1] == 3)
                        threeDiff++;
        }

        int ans = oneDiff * threeDiff;
        printf("Part 1: %d\n\n", ans);
}

void part2(struct input input) {
        int len = input.len;
        int *adapters = input.adapters;
        int max = adapters[len - 1];

        // create a hash table of adapters
        int64 hashTable[max + 1];
        // Set all enteies of adapters we have to 0 and everything else to -1
        for (int i = 0; i <= max; i++)
                hashTable[i] = -1;
        for (int i = 0; i < len; i++)
                hashTable[adapters[i]] = 0;

        int64 numPaths = getNumPaths(hashTable, max, 0);

        printf("Part 2: %ld\n", numPaths);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int len = ll->length + 2;
        int *adapters = malloc(sizeof(int) * len);

        adapters[0] = 0; // Add the outlet adapter
        int i = 1;
        int max = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                int num = strtol(str, NULL, 10);
                adapters[i] = num;
                if (num > max)
                        max = num;

                i++;
        }
        adapters[len - 1] = max + 3; // Add the built-in adapter

        input.len = len;
        input.adapters = adapters;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day10.txt";
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

