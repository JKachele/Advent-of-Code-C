/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 19:07:39 UTC
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
#include "../util/quicksort.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(int32) tllint32;

struct input {
        int numSeats;
        int *seats;
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
        int numSeats = input.numSeats;
        int *seats = input.seats;

        int highestID = seats[numSeats - 1];

        printf("Part 1: %d\n\n", highestID);
}

void part2(struct input input) {
        int numSeats = input.numSeats;
        int *seats = input.seats;

        int mySeat = 0;
        for (int i = 0; i < numSeats - 1; i++) {
                if (seats[i] + 1 != seats[i + 1]) {
                        mySeat = seats[i] + 1;
                        break;
                }
        }

        printf("Part 2: %d\n", mySeat);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int numSeats = ll->length;
        int *seats = malloc(sizeof(int) * numSeats);
        input.numSeats = numSeats;
        input.seats = seats;

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                int row = 0;
                int rowAdd = 64;
                for (int j = 0; j < 7; j++) {
                        if (str[j] == 'B')
                                row += rowAdd;
                        rowAdd /= 2;
                }

                int col = 0;
                int colAdd = 4;
                for (int j = 0; j < 7; j++) {
                        if (str[j + 7] == 'R')
                                col += colAdd;
                        colAdd /= 2;
                }

                int id = (row * 8) + col;
                seats[i] = id;
                i++;
        }

        quicksort(seats, 0, numSeats - 1);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day05.txt";
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

