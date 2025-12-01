/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 01, 2025 14:03:18 UTC
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
#define START_POS 50

struct input {
        int32 numMoves;
        int32 *moves;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input input) {
        int32 pos = START_POS;
        int32 numAtZero = 0;
        for (int i=0; i<input.numMoves; i++) {
                pos += input.moves[i];
                // Circular pos 0-99
                pos = (pos % 100);
                if (pos < 0)
                        pos += 100;

                // Check if at 0
                if (pos == 0)
                        numAtZero++;
        }

        printf("Part 1: %d\n\n", numAtZero);
}

void part2(struct input input) {
        int32 pos = START_POS;
        int32 numCrossZero = 0;
        for (int i=0; i<input.numMoves; i++) {
                int newPos = pos + input.moves[i];

                // Count zero crossings
                int crossings = abs(newPos / 100);
                // if pos is negative or at 0, add a crossing
                if (newPos <= 0)
                        crossings += 1;
                // if pos started at 0 and dir is negative, remove a crossing
                if (pos == 0 && newPos < 0)
                        crossings -= 1;
                numCrossZero += crossings;

                // Circular pos 0-99
                newPos = (newPos % 100);
                if (newPos < 0)
                        newPos += 100;
                pos = newPos;
        }

        printf("Part 2: %d\n", numCrossZero);
}

struct input parseInput(llist *ll) {
        struct input input = {0, NULL};
        input.numMoves = ll->length;
        input.moves = malloc(ll->length * sizeof(int32));

        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char *str = (char*)current->data;
                bool left = str[0] == 'L';
                int num = strtol(str+1, NULL, 10);
                input.moves[i] = left ? num * -1: num;

                current = current->next;
                i++;
        }

        // for (int i=0; i<input.numMoves; i++) {
        //         printf("%d ", input.moves[i]);
        // }
        // printf("\n");

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day01.txt";
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

