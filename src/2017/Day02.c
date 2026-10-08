/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 08, 2026 14:18:17 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
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

#define INPUT_BUFFER_SIZE 1024

typedef tal(int) talint;
typedef tal(talint) talint2d;

struct input {
        talint2d spreadsheet;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printSpreadsheet(talint2d spreadsheet) {
        tal_for(spreadsheet, i) {
                talint row = spreadsheet.array[i];
                tal_for(row, j) {
                        printf("%d ", row.array[j]);
                }
                printf("\n");
        }
}

int findDivisable(talint row) {
        tal_for(row, i) {
                int num1 = row.array[i];
                for (int j = 0; j < i; j++) {
                        int num2 = row.array[j];
                        if (num1 % num2 == 0)
                                return num1 / num2;
                        if (num2 % num1 == 0)
                                return num2 / num1;
                }
        }
        return 0;
}

void part1(struct input *input) {
        talint2d spreadsheet = input->spreadsheet;
        // printSpreadsheet(spreadsheet);

        int checksum = 0;
        tal_for(spreadsheet, i) {
                talint row = spreadsheet.array[i];
                int max = 0;
                int min = INT32_MAX;
                tal_for(row, j) {
                        if (row.array[j] > max)
                                max = row.array[j];
                        if (row.array[j] < min)
                                min = row.array[j];
                }
                // printf("%d, %d\n", max, min);
                int diff = max - min;
                checksum += diff;
        }

        printf("Part 1: Checksum = %d\n\n", checksum);
}

void part2(struct input *input) {
        talint2d spreadsheet = input->spreadsheet;
        // printSpreadsheet(spreadsheet);

        int checksum = 0;
        tal_for(spreadsheet, i) {
                talint row = spreadsheet.array[i];
                checksum += findDivisable(row);
        }

        printf("Part 2: Checksum = %d\n", checksum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                talint row = tal_init();

                char *tok = strtok(str, " \t");
                while (tok != NULL) {
                        tal_add(row, strtol(tok, NULL, 10));
                        tok = strtok(NULL, " \t");
                }

                tal_add(input.spreadsheet, row);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day02.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day02.txt";
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

