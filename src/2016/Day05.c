/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 27, 2026 10:47:16 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/vector.h"
#include "../util/talist.h"
#include "../util/md5.h"

#define INPUT_BUFFER_SIZE 1024

struct input {
        char *id;
};

int numDigits(int n) {
        if (n == 0) return 1;
        return (int)log10(abs(n)) + 1;
}

void part1(struct input *input) {
        char *id = input->id;
        int idLen = strlen(id);

        u64 i = 0;
        char passwd[9] = {0};
        int passwdIndx = 0;
        while (passwdIndx < 8) {
                // Get md5 hash input by appending index i onto id
                int digits = numDigits(i);
                int inputLen = idLen + digits;
                char input[inputLen + 1];
                sprintf(input, "%s%ld", id, i);
                i++;

                // Get md5 hash
                u8 result[16];
                md5(input, result);

                // Check if first 5 hex digits are 0
                if (result[0] == 0 && result[1] == 0 && result[2] < 16) {
                        // printf("%s: %02x%02x%02x\n", input, result[0], result[1], result[2]);
                        char c;
                        if (result[2] < 10) {
                                c = result[2] + '0';
                        } else {
                                c = (result[2] - 10) + 'a';
                        }
                        passwd[passwdIndx++] = c;
                }
        }

        printf("Part 1: Password = %s\n\n", passwd);
}

void part2(struct input *input) {
        char *id = input->id;
        int idLen = strlen(id);

        u64 i = 0;
        char passwd[9] = {0};
        int numCharsSolved = 0;
        while (numCharsSolved < 8) {
                // Get md5 hash input by appending index i onto id
                int digits = numDigits(i);
                int inputLen = idLen + digits;
                char input[inputLen + 1];
                sprintf(input, "%s%ld", id, i);
                i++;

                // Get md5 hash
                u8 result[16];
                md5(input, result);

                // Check if first 5 hex digits are 0 and 6th digit is less than 8
                if (result[0] == 0 && result[1] == 0 && result[2] < 8) {
                        // Only use 1st result for each position
                        if (passwd[result[2]] == '\0') {
                                char c;
                                int digit7 = result[3] / 16;
                                if (digit7 < 10) {
                                        c = digit7 + '0';
                                } else {
                                        c = (digit7 - 10) + 'a';
                                }
                                passwd[result[2]] = c;
                                numCharsSolved++;
                        }
                }
        }

        printf("Part 2: Password = %s\n", passwd);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.id = malloc(strlen(str) + 1);
        strcpy(input.id, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day05.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day05.txt";
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
        double totalTime = ((double)(pt2 - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

