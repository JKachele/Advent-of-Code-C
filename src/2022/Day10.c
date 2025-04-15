/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 15, 2025 17:12:42 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define SIZE_X 40
#define SIZE_Y 6

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void checkSignal(int *signalSum, int cycle, int reg) {
        if ((cycle - 20) % 40 == 0) {
                int signal = cycle * reg;
                debugP("Cycle %d: Reg=%d, Signal=%d\n", cycle, reg, signal);
                *signalSum += signal;
        }
}

void drawPixel(bool pixels[], int cycle, int reg) {
        int dist = ((cycle - 1) % 40) - reg;
        if (abs(dist) <= 1)
                pixels[cycle - 1] = true;
}

void part1(llist *ll) {
        const char *NOOP = "noop";
        const int OFFSET = 5;
        int cycle = 1;
        int reg = 1;
        int signalSum = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                checkSignal(&signalSum, cycle, reg);

                if (strcmp(str, NOOP) == 0) goto NEXT_INS;

                cycle++;
                checkSignal(&signalSum, cycle, reg);

                int add = strtol(str + OFFSET, (char**)NULL, 10);
                reg += add;

        NEXT_INS:
                cycle++;
                current = current->next;
        }

        printf("Part 1: Signal Strength Sum: %d\n\n", signalSum);
}

void part2(llist *ll) {
        const char *NOOP = "noop";
        const int OFFSET = 5;
        const int LEN = SIZE_X * SIZE_Y;
        int cycle = 1;
        int reg = 1;
        bool pixels[LEN];
        for (int i = 0; i < LEN; i++)
                pixels[i] = false;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                drawPixel(pixels, cycle, reg);

                if (strcmp(str, NOOP) == 0) goto NEXT_INS;

                cycle++;
                drawPixel(pixels, cycle, reg);

                int add = strtol(str + OFFSET, (char**)NULL, 10);
                reg += add;

        NEXT_INS:
                cycle++;
                current = current->next;
        }

        printf("Part 2:\n");
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        int i = (SIZE_X * y) + x;
                        if (pixels[i])
                                printf("#");
                        else
                                printf(".");
                }
                printf("\n");
        }
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day10.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day10.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

