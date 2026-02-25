/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 17:10:53 UTC
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
        int32 size;
        int32 *numbers;
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
        int32 size = input.size;
        int32 *nums = input.numbers;

        u64 ans = 0;
        for (int i = 0; i < size - 1; i++) {
                for (int j = i + 1; j < size; j++) {
                        if (nums[i] + nums[j] == 2020) {
                                ans = nums[i] * nums[j];
                                break;
                        }
                }
                if (ans != 0) break;
        }

        printf("Part 1: %lu\n\n", ans);
}

void part2(struct input input) {
        int32 size = input.size;
        int32 *nums = input.numbers;

        u64 ans = 0;
        for (int i = 0; i < size - 2; i++) {
                for (int j = i + 1; j < size - 1; j++) {
                        for (int k = j + 1; k < size; k++) {
                                if (nums[i] + nums[j] + nums[k] == 2020) {
                                        ans = nums[i] * nums[j] * nums[k];
                                        break;
                                }
                        }
                        if (ans != 0) break;
                }
                if (ans != 0) break;
        }

        printf("Part 2: %lu\n\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int32 size = ll->length;
        input.size = size;
        input.numbers = calloc(size, sizeof(int32));
        int32 *numbers = input.numbers;

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                int32 num = strtol(str, NULL, 10);
                numbers[i] = num;
                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day01.txt";
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

