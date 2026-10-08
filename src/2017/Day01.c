/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 08, 2026 14:04:28 EDT
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
#include "../util/talist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tal(u8) talu8;

struct input {
        talu8 digits;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input *input) {
        talu8 digits = input->digits;

        int captchaSum = 0;
        for (size_t i = 0; i < digits.length - 1; i++) {
                if (digits.array[i] == digits.array[i+1]) {
                        captchaSum += digits.array[i];
                }
        }
        if (digits.array[digits.length-1] == digits.array[0]) {
                captchaSum += digits.array[0];
        }

        printf("Part 1: Captcha Sum = %d\n\n", captchaSum);
}

void part2(struct input *input) {
        talu8 digits = input->digits;

        int numDigits = digits.length;
        int halfDist = numDigits / 2;
        int captchaSum = 0;
        for (size_t i = 0; i < digits.length; i++) {
                int halfway = (i + halfDist) % numDigits;
                if (digits.array[i] == digits.array[halfway]) {
                        captchaSum += digits.array[i];
                }
        }

        printf("Part 2: Captcha Sum = %d\n", captchaSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        while (*str != '\0') {
                tal_add(input.digits, (int)(*str - '0'));
                str++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day01.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day01.txt";
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

