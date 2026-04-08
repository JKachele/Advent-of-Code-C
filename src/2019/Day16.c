/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 07, 2026 21:14:43 UTC
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

#define INPUT_BUFFER_SIZE 16384
#define NUM_PHASES 100

struct input {
        int len;
        u8 *list;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printList(int len, u8 *list) {
        for (int i = 0; i < len; i++) {
                printf("%d", list[i]);
        }
        printf("\n");
}

void getIndexPattern(int len, int *indexPattern, int index) {
        const int pattern[4] = {0, 1, 0, -1};

        int i = -1;
        while (i < len) {
                for (int j = 0; j < 4; j++) {
                        if (i >= len)
                                break;
                        for (int k = 0; k < index + 1; k++) {
                                if (i >= len)
                                        break;
                                if (i >= 0) 
                                        indexPattern[i] = pattern[j];
                                i++;
                        }
                }
        }
}

void phase(int len, u8 *list) {
        u8 *copy = malloc(len * sizeof(u8));
        memcpy(copy, list, len * sizeof(u8));

        for (int i = 0; i < len; i++) {
                int indexPattern[len];
                getIndexPattern(len, indexPattern, i);

                int num = 0;
                for (int j = 0; j < len; j++) {
                        num += copy[j] * indexPattern[j];
                }

                // Get last digit of the resulting number
                list[i] = abs(num % 10);
        }

        free(copy);
}

void phase2(int64 len, u8 *list) {
        u8 *copy = malloc(len * sizeof(u8));
        memcpy(copy, list, len * sizeof(u8));

        // For each phase, the nth digit is the sum of digits from n to len-1 of the last phase
        // This only works for the last half of the list, but because the offset is guaranteed to
        // be in the 2nd half, this works
        // can mod 10 each time as we only care about the last digit
        int runningSum = 0;
        for (int64 i = len-1; i >= 0; i--) {
                runningSum = (runningSum + copy[i]) % 10;
                list[i] = runningSum;
        }
        free(copy);
}

int firstNDigits(int64 len, u8 *list, int n) {
        if (len < n) return -1;
        int num = 0;
        for (int64 i = 0; i < n; i++) {
                num *= 10;
                num += list[i];
        }
        return num;
}

void part1(struct input *input) {
        int64 len = input->len;
        u8 *list = malloc(len * sizeof(u8));
        memcpy(list, input->list, len * sizeof(u8));

        for (int i = 0; i < NUM_PHASES; i++) {
                phase(len, list);
        }
        // printList(len, list);
        int ans = firstNDigits(len, list, 8);

        printf("Part 1: %d\n\n", ans);
}

void part2(struct input *input) {
        int len = input->len;
        u8 *list = input->list;

        // Get offset from first 7 digits of input
        int offset = firstNDigits(len, list, 7);
        // Create a new list that starts at the offset. New list has 10000 copies of input
        int64 newLen = (len * 10000) - offset;
        u8 *newList = malloc(newLen * sizeof(u8));
        int initIndex = offset % len;
        for (int64 i = 0; i < newLen; i++) {
                newList[i] = list[(initIndex + i) % len];
        }

        for (int i = 0; i < NUM_PHASES; i++) {
                phase2(newLen, newList);
        }

        // Answer is first 8 digits of new list
        int ans = firstNDigits(newLen, newList, 8);

        printf("Part 2: %d\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        int len = strlen(str);
        input.len = len;
        input.list = malloc(len * sizeof(u8));
        for (int i = 0; i < len; i++) {
                input.list[i] = str[i] - '0';
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
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

