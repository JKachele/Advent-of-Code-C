/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Nov 02, 2026 15:13:38 EST
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
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

struct input {
        int rowLen;
        bool *row;
};

static int NumRows = 40;

bool isTrap(int rowLen, bool row[], int index) {
        bool left = (index-1 < 0) ? false : row[index-1];
        bool center = row[index];
        bool right = (index+1 >= rowLen) ? false : row[index+1];

        if (left && center && !right) {
                return true;
        } else if (!left && center && right) {
                return true;
        } else if (left && !center && !right) {
                return true;
        } else if (!left && !center && right) {
                return true;
        }
        return false;
}

void part1(struct input *input) {
        int rowLen = input->rowLen;

        int numSafe = 0;
        bool row[rowLen];
        for (int i = 0; i < rowLen; i++) {
                row[i] = input->row[i];
                if (!row[i]) {
                        numSafe++;
                }
        }

        for (int i = 1; i < NumRows; i++) {
                bool newRow[rowLen];
                for (int i = 0; i < rowLen; i++) {
                        newRow[i] = isTrap(rowLen, row, i);
                }
                for (int i = 0; i < rowLen; i++) {
                        row[i] = newRow[i];
                        if (!newRow[i]) {
                                numSafe++;
                        }
                }
        }

        printf("Part 1: Safe Tiles = %d\n\n", numSafe);
}

void part2(struct input *input) {
        NumRows = 400000;
        int rowLen = input->rowLen;
        bool *row = input->row;

        int64 numSafe = 0;
        for (int i = 0; i < rowLen; i++) {
                if (!row[i]) {
                        numSafe++;
                }
        }

        bool newRow[rowLen];
        for (int i = 1; i < NumRows; i++) {
                for (int j = 0; j < rowLen; j++) {
                        newRow[j] = isTrap(rowLen, row, j);
                }
                for (int j = 0; j < rowLen; j++) {
                        row[j] = newRow[j];
                        if (!newRow[j]) {
                                numSafe++;
                        }
                }
        }

        printf("Part 2: Safe Tiles = %ld\n", numSafe);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.rowLen = strlen(str);
        input.row = malloc(input.rowLen * sizeof(bool));
        for (int i = 0; i < input.rowLen; i++) {
                if (str[i] == '^') {
                        input.row[i] = true;
                } else {
                        input.row[i] = false;
                }
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day18.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                NumRows = 10;
        } else {
                char *file = "assets/inputs/2016/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

