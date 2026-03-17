/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 17, 2026 15:29:52 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096
#define NUM_TURNS1 2020
#define NUM_TURNS2 30000000

typedef tll(int) tllint;

struct input {
        tllint startingNums;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input input) {
        // Hash table of numbers initalized to 0
        // 0 indicates number was never said before
        int lastSaid[NUM_TURNS1] = {0};

        // Set starting numbers
        int turn = 1;
        int lastNum = 0;
        tll_foreach(input.startingNums, it) {
                int num = it->item;
                lastSaid[num] = turn;
                lastNum = num;
                turn++;
        }

        // Start number game
        for (; turn <= NUM_TURNS1; turn++) {
                // Get last time the last number was said
                int numLastSaid = lastSaid[lastNum];

                // if last said was 0, number said for first time, say 0
                if (numLastSaid == 0) {
                        lastSaid[lastNum] = turn - 1;
                        lastNum = 0;
                }
                // else, say difference between last turn and last time number was said
                else {
                        int diff = (turn - 1) - numLastSaid;
                        lastSaid[lastNum] = turn - 1;
                        lastNum = diff;
                }
        }

        printf("Part 1: %d\n\n", lastNum);
}

void part2(struct input input) {
        // Hash table of numbers initalized to 0
        // 0 indicates number was never said before
        // int lastSaid[NUM_TURNS1] = {0};
        int *lastSaid = calloc(NUM_TURNS2, sizeof(int));

        // Set starting numbers
        int turn = 1;
        int lastNum = 0;
        tll_foreach(input.startingNums, it) {
                int num = it->item;
                lastSaid[num] = turn;
                lastNum = num;
                turn++;
        }

        // Start number game
        for (; turn <= NUM_TURNS2; turn++) {
                // Get last time the last number was said
                int numLastSaid = lastSaid[lastNum];

                // if last said was 0, number said for first time, say 0
                if (numLastSaid == 0) {
                        lastSaid[lastNum] = turn - 1;
                        lastNum = 0;
                }
                // else, say difference between last turn and last time number was said
                else {
                        int diff = (turn - 1) - numLastSaid;
                        lastSaid[lastNum] = turn - 1;
                        lastNum = diff;
                }
        }

        printf("Part 2: %d\n", lastNum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *numStr = strtok(ll->head->data, ",");
        while (numStr != NULL) {
                int num = strtol(numStr, NULL, 10);
                tll_push_back(input.startingNums, num);
                numStr = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day15.txt";
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

