/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 15, 2026 09:53:51 EDT
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
// #include "../util/talist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 16384

struct input {
        char *stream;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int getScore(char *stream, int *index, int level) {
        int totalScore = level;
        while (stream[*index] != '\0' && stream[*index] != '}') {
                char c = stream[*index];
                if (c == '!') {
                        *index += 2;
                        continue;
                }
                if (c == '{') {
                        (*index)++;
                        totalScore += getScore(stream, index, level + 1);
                }
                if (c == ',') {
                        (*index)++;
                        continue;
                }
                if (c == '<') {
                        for (;;) {
                                (*index)++;
                                char g = stream[*index];
                                if (g == '!') {
                                        (*index)++;
                                } else if (g == '>') {
                                        (*index)++;
                                        break;
                                }
                        }
                }
        }
        (*index)++;
        return totalScore;
}

int countGarbage(char *stream) {
        int garbage = 0;
        int i = 0;
        while (stream[i] != '\0') {
                char c = stream[i];
                if (c == '!') {
                        i++;
                } else if (c == '<') {
                        for (;;) {
                                i++;
                                char g = stream[i];
                                if (g == '!') {
                                        i++;
                                        continue;
                                }
                                if (g == '>') {
                                        break;
                                }
                                garbage++;
                        }
                }
                i++;
        }
        return garbage;
}

void part1(struct input *input) {
        char *stream = input->stream;
        int index = 0;
        int score = getScore(stream, &index, 0);

        printf("Part 1: Total score = %d\n\n", score);
}

void part2(struct input *input) {
        char *stream = input->stream;

        int garbage = countGarbage(stream);

        printf("Part 2: Garbage Count = %d\n", garbage);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        int numChars = strlen(str);
        input.stream = malloc(numChars + 1);
        strcpy(input.stream, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day09.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day09.txt";
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

