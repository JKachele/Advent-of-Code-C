/*************************************************
 *File----------src/2015/Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Nov 05, 2026 12:01:32 EST
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
#include "../util/md5.h"

#define INPUT_BUFFER_SIZE 1024

struct input {
        char *key;
};

int getNumDigits(int num) {
        int count = 0;
        int temp = abs(num); 
        do {
                count++;
                temp /= 10;
        } while (temp != 0);
        return count;
}

void part1(struct input *input) {
        char *key = input->key;
        int keyLen = strlen(key);

        int num = 1;
        for (;;) {
                char *in = malloc(keyLen + getNumDigits(num) + 1);
                sprintf(in, "%s%d", key, num);

                u8 result[16];
                md5(in, result);
                if (result[0] == 0 && result[1] == 0 && result[2] < 16) {
                        break;
                }
                num++;
        }

        printf("Part 1: %d\n\n", num);
}

void part2(struct input *input) {
        char *key = input->key;
        int keyLen = strlen(key);

        int num = 1;
        for (;;) {
                char *in = malloc(keyLen + getNumDigits(num) + 1);
                sprintf(in, "%s%d", key, num);

                u8 result[16];
                md5(in, result);
                if (result[0] == 0 && result[1] == 0 && result[2] == 0) {
                        break;
                }
                num++;
        }

        printf("Part 2: %d\n", num);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.key = malloc(strlen(str) + 1);
        strcpy(input.key, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2015/Day04.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2015/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2015/Day04.txt";
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

