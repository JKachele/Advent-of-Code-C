/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Dec 02, 2025 13:38:31 UTC
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
        int32 numRanges;
        lvec2 *ranges;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool repeatsTwice(int64 n) {
        // Convert number to string
        char numStr[17];
        snprintf(numStr, 17, "%ld", n);
        int digits = strlen(numStr);

        // Check for even number of digits
        if (digits % 2 != 0) 
                return false;

        // Check for repeating digits
        for (int i = 0; i < digits/2; i++) {
                if (numStr[i] != numStr[i+digits/2])
                        return false;
        }
        return true;
}

bool hasRepeats(int64 n) {
        // Convert number to string
        char numStr[17];
        snprintf(numStr, 17, "%ld", n);
        int digits = strlen(numStr);

        // Concat string to itself and remove first and last digits
        char concatStr[digits*2 + 1];
        strcpy(concatStr, numStr+1);
        strcat(concatStr, numStr);
        concatStr[strlen(concatStr) - 1] = '\0';

        // Check if original string is in concatenated string
        if (strstr(concatStr, numStr) != NULL) {
                return true;
        }
        return false;
}

void part1(struct input input) {
        u64 invalidSum = 0;
        for (int i = 0; i < input.numRanges; i++) {
                lvec2 r = input.ranges[i];
                // printf("%ld - %ld\n", r.x, r.y);
                for (int64 j = r.x; j <= r.y; j++) {
                        if (repeatsTwice(j)) {
                                // printf("%ld\n", j);
                                invalidSum += j;
                        }
                }
        }

        printf("Part 1: %ld\n\n", invalidSum);
}

void part2(struct input input) {
        u64 invalidSum = 0;
        for (int i = 0; i < input.numRanges; i++) {
                lvec2 r = input.ranges[i];
                // printf("%ld - %ld\n", r.x, r.y);
                for (int64 j = r.x; j <= r.y; j++) {
                        if (hasRepeats(j)) {
                                // printf("%ld\n", j);
                                invalidSum += j;
                        }
                }
        }

        printf("Part 2: %ld\n\n", invalidSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        
        // Count number of ranges
        input.numRanges = 1;
        for (int i = 0; str[i] != '\0'; i++) {
                if (str[i] == ',')
                        input.numRanges++;
        }

        input.ranges = malloc(input.numRanges * sizeof(lvec2));
        char *tok = strtok(str, "-");
        int i = 0;
        while (tok != NULL) {
                input.ranges[i].x = strtol(tok, NULL, 10);
                tok = strtok(NULL, ",");
                input.ranges[i].y = strtol(tok, NULL, 10);
                if (tok != NULL) 
                        tok = strtok(NULL, "-");
                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day02.txt";
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

