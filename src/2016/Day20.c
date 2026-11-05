/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Nov 03, 2026 09:23:30 EST
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
#include "../util/util.h"
#include "../util/vector.h"
#include "../util/talist.h"
#include "../util/quicksort.h"

#define INPUT_BUFFER_SIZE 1024

struct range {
        u32 start;
        u32 end;
};
typedef tal(struct range) talrange;

struct input {
        talrange ranges;
};

int cmp(void *a, void *b) {
        struct range r1 = *(struct range*)a;
        struct range r2 = *(struct range*)b;
        if (r1.start > r2.start) {
                return 1;
        } else if (r1.start == r2.start) {
                if (r1.end > r2.end) {
                        return 1;
                } else if (r1.end == r2.end) {
                        return 0;
                } else {
                        return -1;
                }
        } else {
                return -1;
        }
}

void part1(struct input *input) {
        talrange *ranges = &input->ranges;

        quicksort(ranges->array, sizeof(struct range), 0, ranges->length - 1, cmp);

        // Merge all ranges that overlap
        for (int i = 0; i < (int)ranges->length - 1; i++) {
                struct range r1 = ranges->array[i];
                struct range r2 = ranges->array[i+1];
                // +1 because the end is inclusive
                if (r1.end >= r2.start - 1) {
                        tal_remove(*ranges, i+1);
                        if (r1.end < r2.end) {
                                ranges->array[i].end = r2.end;
                        }
                        i--;
                }
        }

        u32 lowestAllowed = 0;
        if (ranges->array[0].start == 0) {
                lowestAllowed = ranges->array[0].end + 1;
        }

        printf("Part 1: First Allowed IP = %u\n\n", lowestAllowed);
}

void part2(struct input *input) {
        talrange ranges = input->ranges;

        // Check if ranges cover first and last IPs
        u32 numAllowed = ranges.array[0].start;
        numAllowed += UINT32_MAX - ranges.array[ranges.length-1].end;

        // Add gaps between ranges
        for (int i = 0; i < (int)ranges.length - 1; i++) {
                struct range r1 = ranges.array[i];
                struct range r2 = ranges.array[i+1];
                numAllowed += r2.start - (r1.end + 1);
        }

        printf("Part 2: Allowed IPs = %u\n", numAllowed);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct range range;
                range.start = strtol(strtok(str, "-"), NULL, 10);
                range.end = strtol(strtok(NULL, ""), NULL, 10);
                tal_add(input.ranges, range);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day20.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

