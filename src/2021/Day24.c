/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Feb 24, 2026 22:28:34 UTC
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

const u8 popDigits[]  = {5 , 7 , 8 , 10, 11, 12, 13};
const u8 pushDigits[] = {4 , 6 , 3 , 9 , 2 , 1 , 0 };
const u8 pushC[]      = {14, 11, 11, 5 , 5 , 11, 4 };
const u8 popB[]       = {10, 9 , 3 , 5 , 10, 4 , 5 };


static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

ivec2 getMax(int b, int c) {
        ivec2 max = {0};
        for (int i = 9; i >= 0; i--) {
                u8 push = i + b - c;
                if (push > 0 && push < 10) {
                        max.x = i;
                        max.y = push;
                        break;
                }
        }
        return max;
}

ivec2 getMin(int b, int c) {
        ivec2 min = {0};
        for (int i = 1; i < 10; i++) {
                u8 push = i + b - c;
                if (push > 0 && push < 10) {
                        min.x = i;
                        min.y = push;
                        break;
                }
        }
        return min;
}

void part1(void) {
        u8 numberArr[14];
        for (int i = 0; i < 7; i++) {
                ivec2 digit = getMax(popB[i], pushC[i]);
                numberArr[popDigits[i]] = digit.x;
                numberArr[pushDigits[i]] = digit.y;
        }

        u64 num = 0;
        for (int i = 0; i < 14; i++) {
                num *= 10;
                num += numberArr[i];
        }

        printf("Part 1: %lu\n\n", num);
}

void part2(void) {
        u8 numberArr[14];
        for (int i = 0; i < 7; i++) {
                ivec2 digit = getMin(popB[i], pushC[i]);
                numberArr[popDigits[i]] = digit.x;
                numberArr[pushDigits[i]] = digit.y;
        }

        u64 num = 0;
        for (int i = 0; i < 14; i++) {
                num *= 10;
                num += numberArr[i];
        }

        printf("Part 2: %lu\n\n", num);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        part1();
        clock_t pt1 = clock();
        part2();
        clock_t pt2 = clock();

        double pt1Time = ((double)(pt1 - begin) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Part1: %f, Part2: %f\n", 
                        pt1Time, pt2Time);

        return 0;
}

