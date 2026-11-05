/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Nov 03, 2026 08:11:41 EST
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
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(int) talint;

struct input {
        int numElves;
};

// Naive solution for part 2 to look for patterns
void pt2Test(int n) {
        talint elves = tal_init();

        for (int i = 1; i <= n; i++) {
                tal_add(elves, i);
        }

        int index = 0;
        while (elves.length > 1) {
                int remove = ((elves.length / 2) + index) % elves.length;
                tal_remove(elves, remove);
                if (remove > index) {
                        index++;
                }
                index %= elves.length;
        }
        printf("%d: %d\n", n, elves.array[0]);
        tal_destroy(elves);
}

// Using Josephus problem solution where k=2 found here:
// https://en.wikipedia.org/wiki/Josephus_problem
void part1(struct input *input) {
        int numElves = input->numElves;

        // Find largest power of 2 smaller than numElves
        int m = 1;
        while ((m << 1) < numElves) {
                m <<= 1;
        }

        // Find difference between that and numElves
        int l = numElves - m;

        int survivingElf = (2 * l) + 1;

        printf("Part 1: Last elf = %d\n\n", survivingElf);
}

void part2(struct input *input) {
        int numElves = input->numElves;

        // Naive test for 1 - 100 elves, Find a pattern
        // for (int i = 1; i <= 100; i++) {
        //         pt2Test(i);
        // }
        // find largest power of 3 below elves.
        // if elves - number < number, last elf is (elves - number)
        // else number + ((elves - (number * 2)) * 2)
        
        int num = 1;
        while ((num * 3) < numElves) {
                num *= 3;
        }

        int diff = numElves - num;

        int lastElf = diff;
        if (diff > num) {
                lastElf = num + ((diff - num) * 2);
        }

        printf("Part 2: Last elf = %d\n", lastElf);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numElves = strtol((char*)ll->head->data, NULL, 10);
        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day19.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day19.txt";
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

