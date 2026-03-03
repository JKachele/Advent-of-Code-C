/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Feb 26, 2026 18:05:50 UTC
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

typedef tll(u64) tllu64;

struct input {
        int len;
        u64 *message;
        u64 invalid;
};

static int WindowSize = 25;
static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool isValidNumber(u64 message[], int len, int index) {
        if (index < WindowSize || index >= len)
                return false;

        u64 num = message[index];

        for (int i = index - WindowSize; i < index - 1; i++) {
                for (int j = i + 1; j < index; j++) {
                        if (message[i] + message[j] == num)
                                return true;
                }
        }

        return false;
}

u64 part1(struct input input) {
        int len = input.len;
        u64 *message = input.message;

        u64 curNum;
        for (int i = WindowSize; i < len; i++) {
                curNum = message[i];
                if (!isValidNumber(message, len, i))
                        break;
        }

        printf("Part 1: %lu\n\n", curNum);
        return curNum;
}

void part2(struct input input, u64 invalidNum) {
        int len = input.len;
        u64 *message = input.message;

        tllu64 window = tll_init();
        tll_push_back(window, message[0]);
        u64 windowSum = message[0];

        int i = 1;
        while (windowSum != invalidNum) {
                if (windowSum < invalidNum) {
                        tll_push_back(window, message[i]);
                        windowSum += message[i];
                        i++;
                } else {
                        u64 removeNum = tll_pop_front(window);
                        windowSum -= removeNum;
                }
        }

        // Get max and min numbers in window
        u64 minNum = tll_front(window);
        u64 maxNum = tll_front(window);
        tll_foreach(window, it) {
                u64 num = it->item;
                if (num < minNum)
                        minNum = num;
                if (num > maxNum)
                        maxNum = num;
        }

        u64 weekness = minNum + maxNum;

        printf("Part 2: %lu\n", weekness);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int len = ll->length;
        u64 *message = malloc(sizeof(u64) * len);

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                message[i] = strtol(str, NULL, 10);
                i++;
        }

        input.len = len;
        input.message = message;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
                WindowSize = 5;
        } else {
                char *file = "assets/inputs/2020/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        u64 invalidNum = part1(input);
        clock_t pt1 = clock();
        part2(input, invalidNum);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

