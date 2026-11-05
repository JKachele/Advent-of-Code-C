/*************************************************
 *File----------src/2015/Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Nov 05, 2026 12:14:57 EST
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

struct input {
        int numStrs;
        char **strs;
};

bool isVowel(char c) {
        return (c == 'a') || (c == 'e') || (c == 'i') || (c == 'o') || (c == 'u');
}

void part1(struct input *input) {
        int numStrs = input->numStrs;
        char **strs = input->strs;

        const char *bad[] = {"ab", "cd", "pq", "xy"};

        int numNice = 0;
        for (int i = 0; i < numStrs; i++) {
                char *str = strs[i];
                int numVowels = 0;
                bool doubleChar = false;
                bool badStr = false;
                char last = 0;
                for (int i = 0; i < (int)strlen(str); i++) {
                        if (isVowel(str[i])) {
                                numVowels++;
                        }
                        if (str[i] == last) {
                                doubleChar = true;
                        }
                        for (int j = 0; j < 4; j++) {
                                if (last == bad[j][0] && str[i] == bad[j][1]) {
                                        badStr = true;
                                        break;
                                }
                        }
                        last = str[i];
                }
                if (numVowels >= 3 && doubleChar && !badStr) {
                        numNice++;
                }
        }

        printf("Part 1: Nice Strings = %d\n\n", numNice);
}

void part2(struct input *input) {
        int numStrs = input->numStrs;
        char **strs = input->strs;


        int numNice = 0;
        int8 pairs[676/*26x26*/];
        for (int i = 0; i < numStrs; i++) {
                char *str = strs[i];
                for (int i = 0; i < 676; i++) { pairs[i] = -1; }

                bool hasPair = false;
                bool hasXYX = false;
                char last = str[0];
                char last2 = 0;
                for (int i = 1; i < (int)strlen(str); i++) {
                        int index = ((last - 'a') * 26) + (str[i] - 'a');
                        if (pairs[index] == -1) {
                                pairs[index] = i;
                        } else if (pairs[index] != (i - 1)) {
                                hasPair = true;
                        }

                        if (str[i] == last2) {
                                hasXYX = true;
                        }
                        last2 = last;
                        last = str[i];
                }
                if (hasPair && hasXYX) {
                        numNice++;
                }
        }

        printf("Part 2: Nice Strings = %d\n", numNice);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numStrs = ll->length;
        input.strs = malloc(input.numStrs * sizeof(char*));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                char *cpy = malloc(strlen(str) + 1);
                strcpy(cpy, str);
                input.strs[i++] = cpy;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2015/Day05.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2015/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2015/Day05.txt";
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

