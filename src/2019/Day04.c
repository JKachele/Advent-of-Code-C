/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 30, 2026 12:59:16 UTC
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
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

struct input {
        int min;
        int max;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void numToArr(int num, u8 arr[6]) {
        for (int i = 5; i >= 0; i--) {
                arr[i] = num % 10;
                num /= 10;
        }
}

bool adjacentDigits(u8 num[6]) {
        for (int i = 0; i < 5; i++) {
                if (num[i] == num[i + 1])
                        return true;
        }
        return false;
}

bool increasingDigits(u8 num[6]) {
        for (int i = 0; i < 5; i++) {
                if (num[i] > num [i+1])
                        return false;
        }
        return true;
}

bool adjacentDigitsStrict(u8 num[6]) {
        for (int i = 0; i < 5; i++) {
                if (num[i] == num[i+1]) {
                        if (i > 0 && num[i] == num[i-1])
                                continue;
                        if (i < 4 && num[i] == num[i+2])
                                continue;
                        return true;
                }
        }
        return false;
}

void part1(struct input *input) {
        int numValid = 0;
        for (int i = input->min; i <= input->max; i++) {
                u8 num[6];
                numToArr(i, num);

                bool valid = true;
                if (!adjacentDigits(num))
                        valid = false;
                if (!increasingDigits(num))
                        valid = false;

                if (valid)
                        numValid++;
        }

        printf("Part 1: %d\n\n", numValid);
}

void part2(struct input *input) {
        int numValid = 0;
        for (int i = input->min; i <= input->max; i++) {
                u8 num[6];
                numToArr(i, num);

                bool valid = true;
                if (!adjacentDigitsStrict(num))
                        valid = false;
                if (!increasingDigits(num))
                        valid = false;

                if (valid)
                        numValid++;
        }

        printf("Part 2: %d\n", numValid);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.min = strtol(strtok(str, "-"), NULL, 10);
        input.max = strtol(strtok(NULL, ""), NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

