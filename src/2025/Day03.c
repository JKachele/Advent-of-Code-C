/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Dec 03, 2025 12:51:12 UTC
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
        ivec2 numBat;
        u8 **batteries;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int32 getMaxJoltage2(int numBat, u8 bank[numBat]) {
        int32 maxJoltage = 0;
        // Loop through each battery and find the highest battery after that
        for (int i = 0; i < numBat - 1; i++) {
                debugp("%d", bank[i]);
                int max = 0;
                for (int j = i + 1; j < numBat; j++) {
                        if (bank[j] > max)
                                max = bank[j];
                }
                int joltage = (bank[i] * 10) + max;
                if (joltage > maxJoltage)
                        maxJoltage = joltage;
        }
        return maxJoltage;
}

int getHighestInRange(u8 *bank, int start, int end) {
        int highestIndex = start;
        for (int i = start + 1; i <= end; i++) {
                if (bank[i] > bank[highestIndex])
                        highestIndex = i;
        }
        return highestIndex;
}

int64 getMaxJoltage(int numBat, u8 bank[numBat], int numOn) {
        // To get the nth digit of max, look through window starting after the
        // last chosen battery up to the numOn-nth battery from the end and pick
        // the highest battery in the window, ties goes to the first battery
        int64 joltage = 0;
        int winStart = 0;
        for (int i = numOn; i > 0; i--) {
                int winEnd = numBat - i;
                int highestIndex = getHighestInRange(bank, winStart, winEnd);
                winStart = highestIndex + 1;
                joltage = (joltage * 10) + bank[highestIndex];
        }
        return joltage;
}

void part1(struct input input) {
        ivec2 numBat = input.numBat;

        int64 totalJoltage = 0;
        for (int i = 0; i < numBat.x; i++) {
                int32 maxJoltage = getMaxJoltage2(numBat.y, input.batteries[i]);
                totalJoltage += maxJoltage;
                debugp(": %d\n", maxJoltage);
        }

        printf("Part 1: %lu\n\n", totalJoltage);
}

void part2(struct input input) {
        ivec2 numBat = input.numBat;

        int64 totalJoltage = 0;
        for (int i = 0; i < numBat.x; i++) {
                int64 joltage = getMaxJoltage(numBat.y, input.batteries[i], 12);
                totalJoltage += joltage;
                debugp("%ld\n", joltage);
        }

        printf("Part 2: %lu\n\n", totalJoltage);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 numBat = {{ll->length, getLongestLine(ll)}};
        input.batteries = malloc(numBat.x * sizeof(u8*));
        input.numBat = numBat;

        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                input.batteries[i] = malloc(numBat.y * sizeof(u8));
                for (int j = 0; j < numBat.y; j++)
                        input.batteries[i][j] = str[j] - '0';

                current = current->next;
                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day03.txt";
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

