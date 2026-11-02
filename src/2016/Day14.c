/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Nov 02, 2026 08:56:27 EST
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
#include "../util/md5.h"

#define INPUT_BUFFER_SIZE 1024

struct candidate {
        char c;
        int index;
};
typedef tal(struct candidate) talcan;

struct input {
        char *salt;
};

void md5toStr(u8 result[16], char str[33]) {
        for (int i = 0; i < 16; i++) {
                sprintf(&str[i*2], "%02x", result[i]);
        }
}

void stretchedHash(char *key, char resultStr[33]) {
        strncpy(resultStr, key, 33);
        u8 result[16];
        for (int i = 0; i <= 2016; i++) {
                md5(resultStr, result);
                md5toStr(result, resultStr);
        }
}

void part1(struct input *input) {
        char *salt = input->salt;

        talcan candidates = tal_init();
        int keyIndexes[64] = {0};
        int numKeys = 0;
        char key[16];
        for (int i = 0; numKeys < 64; i++) {
                sprintf(key, "%s%d", salt, i);
                u8 result[16];
                char resultStr[33];
                md5(key, result);
                md5toStr(result, resultStr);

                char prev = 0;
                int curLen = 0;
                bool isCan = false;
                for (int j = 0; j < 32; j++) {
                        char c = resultStr[j];
                        if (c != prev) {
                                prev = c;
                                curLen = 1;
                                continue;
                        }
                        curLen++;

                        if (curLen == 3 && !isCan) {
                                isCan = true;
                                struct candidate cur = {c, i};
                                tal_add(candidates, cur);
                        }

                        if (curLen == 5) {
                                tal_for(candidates, k) {
                                        struct candidate can = candidates.array[k];
                                        if (can.index == i) { continue; }
                                        if ((i - can.index) > 1000) {
                                                tal_remove(candidates, k);
                                                k--;
                                                continue;
                                        }
                                        if (can.c == c && (i - can.index) <= 1000) {
                                                keyIndexes[numKeys++] = can.index;
                                                tal_remove(candidates, k);
                                                k--;
                                        }
                                }
                        }
                }
        }
        tal_destroy(candidates);

        int lastKeyIndex = keyIndexes[63];
        printf("Part 1: Index of key = %d\n\n", lastKeyIndex);
}

void part2(struct input *input) {
        char *salt = input->salt;

        talcan candidates = tal_init();
        int keyIndexes[64] = {0};
        int numKeys = 0;
        char key[16];
        for (int i = 0; numKeys < 64; i++) {
                sprintf(key, "%s%d", salt, i);
                char resultStr[33];
                stretchedHash(key, resultStr);

                char prev = 0;
                int curLen = 0;
                bool isCan = false;
                for (int j = 0; j < 32; j++) {
                        char c = resultStr[j];
                        if (c != prev) {
                                prev = c;
                                curLen = 1;
                                continue;
                        }
                        curLen++;

                        if (curLen == 3 && !isCan) {
                                isCan = true;
                                struct candidate cur = {c, i};
                                tal_add(candidates, cur);
                        }

                        if (curLen == 5) {
                                tal_for(candidates, k) {
                                        struct candidate can = candidates.array[k];
                                        if (can.index == i) { continue; }
                                        if ((i - can.index) > 1000) {
                                                tal_remove(candidates, k);
                                                k--;
                                                continue;
                                        }
                                        if (can.c == c && (i - can.index) <= 1000) {
                                                keyIndexes[numKeys++] = can.index;
                                                tal_remove(candidates, k);
                                                k--;
                                        }
                                }
                        }
                }
        }
        tal_destroy(candidates);

        int lastKeyIndex = keyIndexes[63];
        printf("Part 2: Index of key = %d\n", lastKeyIndex);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.salt = malloc(strlen(str) + 1);
        strcpy(input.salt, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day14.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day14.txt";
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

