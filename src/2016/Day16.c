/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Nov 02, 2026 10:40:15 EST
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

static int DiskLen = 272;

struct input {
        char *initial;
};

void getData(char *result, const char *initial, int diskLen) {
        int dataLen = strlen(initial);
        strcpy(result, initial);
        do {
                result[dataLen] = '0';
                for (int i = 0; i < dataLen; i++) {
                        int a = (dataLen - 1) - i;
                        int b = (dataLen + 1) + i;
                        if (b >= diskLen) { break; }
                        result[b] = (result[a] == '0') ? '1' : '0';
                }
                dataLen *= 2;
                dataLen++;
        } while(dataLen < diskLen);
}

void getChecksum(char sum[], char data[], int diskLen) {
        char *cpy = malloc(diskLen);
        strncpy(cpy, data, diskLen);
        int sumLen = diskLen;
        do {
                sumLen /= 2;
                for (int i = 0; i < sumLen; i++) {
                        char c1 = cpy[i*2];
                        char c2 = cpy[(i*2)+1];
                        sum[i] = (c1 == c2) ? '1' : '0';
                }
                strncpy(cpy, sum, sumLen);
        } while (sumLen % 2 == 0);
        sum[sumLen] = '\0';
}

void part1(struct input *input) {
        const char *initial = input->initial;

        char result[DiskLen];
        getData(result, initial, DiskLen);

        char checksum[DiskLen];
        getChecksum(checksum, result, DiskLen);

        printf("Part 1: Checksum = %s\n\n", checksum);
}

void part2(struct input *input) {
        const char *initial = input->initial;
        const int NewDiskLen = 35651584;

        char *result = malloc(NewDiskLen);
        getData(result, initial, NewDiskLen);

        char *checksum = malloc(NewDiskLen);
        getChecksum(checksum, result, NewDiskLen);

        printf("Part 2: Checksum = %s\n", checksum);
        free(result);
        free(checksum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.initial = malloc(strlen(str) + 1);
        strcpy(input.initial, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day16.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                DiskLen = 20;
        } else {
                char *file = "assets/inputs/2016/Day16.txt";
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

