/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 27, 2026 14:37:45 EDT
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

#define INPUT_BUFFER_SIZE 16384

typedef tal(char) talchar;

struct input {
        char *file;
};

u64 getUncompressedLength(char *file, int start, int end) {
        u64 length = 0;

        for (int i = start; i <= end; i++) {
                if (file[i] != '(') {
                        length++;
                        continue;
                }

                int len = 0;
                int times = 0;
                while (file[++i] != 'x') {
                        len *= 10;
                        len += file[i] - '0';
                }
                while (file[++i] != ')') {
                        times *= 10;
                        times += file[i] - '0';
                }

                u64 subLen = getUncompressedLength(file, i + 1, i + len);
                length += subLen * times;
                i += len;
        }

        return length;
}

void part1(struct input *input) {
        char *file = malloc(strlen(input->file + 1));
        strcpy(file, input->file);

        talchar uncompressed = tal_init();
        while (*file != '\0') {
                char c = *file;
                if (c != '(') {
                        tal_add(uncompressed, c);
                        file++;
                        continue;
                }

                // Repeat Group
                file++;
                int len = strtol(strtok(file, "x"), NULL, 10);
                int times = strtol(strtok(NULL, ")"), NULL, 10);
                file = strtok(NULL, "");
                for (int i = 0; i < times; i++) {
                        for (int j = 0; j < len; j++) {
                                tal_add(uncompressed, file[j]);
                        }
                }
                file += len;
        }
        tal_add(uncompressed, '\0');
        // printf("%s\n", uncompressed.array);

        printf("Part 1: Uncompressed Length = %lu\n\n", uncompressed.length - 1);
}

void part2(struct input *input) {
        char *file = input->file;

        u64 uncompressedLen = getUncompressedLength(file, 0, strlen(file) - 1);

        printf("Part 1: Uncompressed Length = %lu\n\n", uncompressedLen);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.file = malloc(strlen(str) + 1);
        strcpy(input.file, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day09.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day09.txt";
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

