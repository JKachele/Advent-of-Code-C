/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Dec 05, 2025 09:12:27 EST
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

typedef tll(lvec2) tllvec2;

struct input {
        int numRanges;
        lvec2 *ranges;
        int numIds;
        int64 *ids;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool idIsFresh(int64 id, int numRanges, lvec2 *ranges) {
        for (int i = 0; i < numRanges; i++) {
                if (id >= ranges[i].x && id <= ranges[i].y)
                        return true;
        }
        return false;
}

// 0: no overlap, 1: overlap a-b, 2: overlap b-a, 3: b in a, 4: a in b
int rangeOverlap( lvec2 a, lvec2 b) {
        bool aXbX = a.x <= b.x;
        bool aXbY = a.x <= b.y;
        bool aYbX = a.y <= b.x;
        bool aYbY = a.y <= b.y;
        bool bXaX = b.x <= a.x;
        bool bXaY = b.x <= a.y;
        bool bYaX = b.y <= a.x;
        bool bYaY = b.y <= a.y;

        // a--------a
        //       b--------b
        if (aXbX && aYbY && bXaY)
                return 1;

        //       a--------a
        // b--------b
        if (bXaX && bYaY && aXbY)
                return 2;

        // a--------a
        //   b----b
        if (aXbX && bYaY)
                return 3;

        //   a----a
        // b--------b
        if (bXaX && aYbY)
                return 4;

        return 0;
}

int mergeRanges(tllvec2 *ranges, tllvec2 *newRanges) {
        int numMerges = 0;
        while (tll_length(*ranges) > 0) {
                lvec2 range = tll_pop_front(*ranges);

                tll_foreach(*ranges, it) {
                        int overlap = rangeOverlap(range, it->item);
                        if (overlap == 0) continue;
                        numMerges++;
                        lvec2 range2 = it->item;
                        tll_remove(*ranges, it);

                        switch (overlap) {
                                case 1:
                                        range.y = range2.y;
                                        break;
                                case 2:
                                        range.x = range2.x;
                                        break;
                                case 3:
                                        break;
                                case 4:
                                        range.x = range2.x;
                                        range.y = range2.y;
                                        break;
                        }
                }

                tll_push_back(*newRanges, range);
        }
        return numMerges;
}

tllvec2 mergeAllRanges(lvec2 *rangesList, int numRanges) {
        // Convert ranges to linked list
        tllvec2 ranges = tll_init();
        for (int i = 0; i < numRanges; i++) {
                tll_push_back(ranges, rangesList[i]);
        }

        int numMerges = 0;
        do {
                tllvec2 newRanges = tll_init();
                numMerges = mergeRanges(&ranges, &newRanges);

                tll_foreach(newRanges, it)
                        tll_push_back(ranges, it->item);

        } while (numMerges > 0);

        return ranges;
}

// Brute force :)
void part1(struct input input) {
        int numIds = input.numIds;
        int64 *ids = input.ids;

        int freshCount = 0;
        for (int i = 0; i < numIds; i++) {
                if (idIsFresh(ids[i], input.numRanges, input.ranges))
                        freshCount++;
        }

        printf("Part 1: %d\n\n", freshCount);
}

// Range-finding algorithm
void part2(struct input input) {
        tllvec2 range = mergeAllRanges(input.ranges, input.numRanges);

        // Add up ranges
        u64 numIds = 0;
        tll_foreach(range, it) {
                lvec2 range = it->item;
                numIds += (range.y - range.x) + 1;
        }

        printf("Part 2: %lu\n", numIds);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int numRanges = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                if (strlen((char*)cur->data) == 0)
                        break;
                numRanges++;
        }

        lvec2 *ranges = malloc(numRanges * sizeof(lvec2));
        int numIds = ll->length - numRanges - 1;;
        int64 *ids = malloc(numIds * sizeof(int64));

        llNode *cur = ll->head;
        for (int i = 0; i < numRanges; i++) {
                char *str = (char*)cur->data;
                if (strlen(str) == 0)
                        break;
                ranges[i].x = strtol(strtok(str, "-"), NULL, 10);
                ranges[i].y = strtol(strtok(NULL, ""), NULL, 10);
                // printf("%ld - %ld\n", ranges[i].x, ranges[i].y);
                cur = cur->next;
        }

        cur = cur->next;
        for (int i = 0; i < numIds; i++) {
                char *str = (char*)cur->data;
                if (strlen(str) == 0)
                        break;
                ids[i] = strtol(str, NULL, 10);
                // printf("%ld\n", ids[i]);
                cur = cur->next;
        }

        input.numRanges = numRanges;
        input.ranges = ranges;
        input.numIds = numIds;
        input.ids = ids;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day05.txt";
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

