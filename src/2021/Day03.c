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

typedef tll(bool) tllbool;
typedef tll(tllbool) tlltllbool;

struct input {
        int32 logLen;
        int32 logWidth;
        bool **log;
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

int32 bintoi(int32 numBits, bool bin[]) {
        int32 num = 0;
        for (int i=0; i<numBits; i++) {
                num = num << 1;
                if (bin[i])
                        num += 1;
        }
        return num;
}

void part1(struct input input) {
        bool (*log)[input.logWidth] = (bool(*)[input.logWidth])input.log;
        // printLog(input.logLen, input.logWidth, log);

        int32 *numOnes = calloc(input.logWidth, sizeof(int32));
        for (int i=0; i<input.logLen; i++) {
                for (int j=0; j<input.logWidth; j++) {
                        if (log[i][j])
                                numOnes[j]++;
                }
        }

        bool binGamma[input.logWidth];
        bool binEpsilon[input.logWidth];
        for (int i=0; i<input.logWidth; i++) {
                if (numOnes[i] > input.logLen - numOnes[i]) {
                        binGamma[i] = true;
                        binEpsilon[i] = false;
                } else {
                        binGamma[i] = false;
                        binEpsilon[i] = true;
                }
        }

        int32 gamma = bintoi(input.logWidth, binGamma);
        int32 epsilon = bintoi(input.logWidth, binEpsilon);
        int64 power = gamma * epsilon;

        printf("Part 1: %ld\n\n", power);
}

void part2(struct input input) {

        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input;
        input.logLen = ll->length;
        input.logWidth = getLongestLine(ll);
        input.log = calloc(input.logLen * input.logWidth, sizeof(bool));
        bool (*log)[input.logWidth] = (bool(*)[input.logWidth])input.log;

        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int j=0; j<input.logWidth; j++) {
                        if (str[j] == '0')
                                log[i][j] = false;
                        else
                                log[i][j] = true;
                }

                current = current->next;
                i++;
        }
        // printLog(input.logLen, input.logWidth, log);

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

