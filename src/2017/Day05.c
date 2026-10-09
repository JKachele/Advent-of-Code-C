/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 09, 2026 11:17:06 EDT
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

#define INPUT_BUFFER_SIZE 1024

struct input {
        int numJumps;
        int *jumps;
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
        int numJumps = input->numJumps;
        int jumps[numJumps];
        for (int i = 0; i < numJumps; i++)
                jumps[i] = input->jumps[i];

        int pc = 0;
        int numSteps = 0;
        while (pc < numJumps) {
                jumps[pc]++;
                pc += jumps[pc] - 1;
                numSteps++;
        }

        printf("Part 1: Steps = %d\n\n", numSteps);
}

void part2(struct input *input) {
        int numJumps = input->numJumps;
        int jumps[numJumps];
        for (int i = 0; i < numJumps; i++)
                jumps[i] = input->jumps[i];

        int pc = 0;
        int numSteps = 0;
        while (pc < numJumps) {
                int jump = jumps[pc];
                if (jump < 3)
                        jumps[pc]++;
                else
                        jumps[pc]--;
                pc += jump;
                numSteps++;
        }

        printf("Part 2: Steps = %d\n", numSteps);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numJumps = ll->length;
        input.jumps = malloc(ll->length * sizeof(int));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                input.jumps[i++] = strtol(str, NULL, 10);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day05.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day05.txt";
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

