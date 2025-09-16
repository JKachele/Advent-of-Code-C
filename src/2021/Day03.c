/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 15, 2025 17:18:29 UTC
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

typedef tll(u16) tllu16;

struct input {
        int numBits;
        tllu16 log;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printLog(int len, int width, bool log[][width]) {
        if (!Debug) return;
        for (int i=0; i<len; i++) {
                for (int j=0; j<width; j++) {
                        printf("%d", log[i][j] ? 1 : 0);
                }
                printf("\n");
        }
        printf("\n");
}

tllu16 copyTll(tllu16 tll) {
        tllu16 cpy = tll_init();
        tll_foreach(tll, it) {
                tll_push_back(cpy, it->item);
        }
        return cpy;
}

void reduceLog(tllu16 *log, int bit, bool ox) {
        // Find most common bit in position
        int numOnes = 0;
        tll_foreach(*log, it) {
                u16 mask = 1 << bit;
                if ((it->item & mask) != 0) {
                        numOnes++;
                }
        }
        uint16 keepBit = 0;
        if (ox && numOnes >= (int)tll_length(*log) - numOnes)
                keepBit = 1;
        else if (!ox && numOnes < (int)tll_length(*log) - numOnes)
                keepBit = 1;
        keepBit = keepBit << bit;

        // Remove entrys without the keep bit
        tll_foreach(*log, it) {
                u16 mask = 1 << bit;
                if ((it->item & mask) != keepBit) {
                        tll_remove(*log, it);
                }
        }
}

void part1(struct input input) {
        tllu16 log = input.log;
        int numBits = input.numBits;

        int logOnes[numBits];
        memset(logOnes, 0, numBits * sizeof(int));
        tll_foreach(log, it) {
                u16 num = it->item;
                for (int i=0; i<numBits; i++) {
                        u16 mask = 1 << i;
                        if ((num & mask) != 0) {
                                logOnes[(numBits-i)-1]++;
                        }
                }
        }

        u16 gamma = 0;
        u16 epsilon = 0;
        for (int i=0; i<numBits; i++) {
                gamma = gamma << 1;
                epsilon = epsilon << 1;
                if (logOnes[i] > (int)tll_length(log) - logOnes[i]) {
                        gamma++;
                } else {
                        epsilon++;
                }
        }
        int64 power = gamma * epsilon;

        printf("Part 1: %ld\n\n", power);
}

void part2(struct input input) {
        int numBits = input.numBits;
        tllu16 oxLog = input.log;
        tllu16 co2Log = copyTll(oxLog);

        int i = numBits - 1;
        while (tll_length(oxLog) > 1 && i >= 0) {
                reduceLog(&oxLog, i, true);
                i--;
        }
        u16 oxRating = tll_front(oxLog);

        i = numBits - 1;
        while (tll_length(co2Log) > 1 && i >= 0) {
                reduceLog(&co2Log, i, false);
                i--;
        }
        u16 co2Rating = tll_front(co2Log);

        u32 lifeSupportRating = oxRating * co2Rating;

        printf("Part 2: %d\n", lifeSupportRating);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numBits = getLongestLine(ll);

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;

                u16 logNum = strtol(str, NULL, 2);
                tll_push_back(input.log, logNum);

                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day03.txt";
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

