/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 22, 2026 11:57:52 EDT
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

struct part {
        ivec2 connections;
        bool used;
};

struct input {
        int numParts;
        struct part *parts;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printParts(int numParts, struct part parts[]) {
        for (int i = 0; i < numParts; i++) {
                ivec2 con = parts[i].connections;
                printf("%d - %d\n", con.a, con.b);
        }
        printf("\n");
}

int strongestBridge(int numParts, struct part parts[], int connection) {
        int maxStrength = 0;
        for (int i = 0; i < numParts; i++) {
                if (parts[i].used) { continue; }
                ivec2 conns = parts[i].connections;
                int partStrength = conns.a + conns.b;

                if (conns.a == connection) {
                        parts[i].used = true;
                        int strength = strongestBridge(numParts, parts, conns.b);
                        parts[i].used = false;
                        if ((strength + partStrength) > maxStrength) {
                                maxStrength = strength + partStrength;
                        }
                } else if (conns.b == connection){
                        parts[i].used = true;
                        int strength = strongestBridge(numParts, parts, conns.a);
                        parts[i].used = false;
                        if ((strength + partStrength) > maxStrength) {
                                maxStrength = strength + partStrength;
                        }
                }
        }
        return maxStrength;
}

ivec2 longestBridge(int numParts, struct part parts[], int connection, int length) {
        int maxLength = 0;
        int maxStrength = 0;
        for (int i = 0; i < numParts; i++) {
                if (parts[i].used) { continue; }
                ivec2 conns = parts[i].connections;
                int partStrength = conns.a + conns.b;

                if (conns.a == connection) {
                        parts[i].used = true;
                        ivec2 bridge = longestBridge(numParts, parts, conns.b, length + 1);
                        parts[i].used = false;
                        if (bridge.a + 1 > maxLength) {
                                maxLength = bridge.a + 1;
                                maxStrength = bridge.b + partStrength;
                        } else if (bridge.a + 1 == maxLength) {
                                if (bridge.b + partStrength > maxStrength) {
                                        maxStrength = bridge.b + partStrength;
                                }
                        }
                } else if (conns.b == connection){
                        parts[i].used = true;
                        ivec2 bridge = longestBridge(numParts, parts, conns.a, length + 1);
                        parts[i].used = false;
                        if (bridge.a + 1 > maxLength) {
                                maxLength = bridge.a + 1;
                                maxStrength = bridge.b + partStrength;
                        } else if (bridge.a + 1 == maxLength) {
                                if (bridge.b + partStrength > maxStrength) {
                                        maxStrength = bridge.b + partStrength;
                                }
                        }
                }
        }
        return (ivec2){{maxLength, maxStrength}};
}

void part1(struct input *input) {
        int numParts = input->numParts;
        struct part *parts = input->parts;
        // printParts(numParts, parts);

        int maxStrength = strongestBridge(numParts, parts, 0);

        printf("Part 1: Max Strength = %d\n\n", maxStrength);
}

void part2(struct input *input) {
        int numParts = input->numParts;
        struct part *parts = input->parts;
        // printParts(numParts, parts);

        ivec2 longest = longestBridge(numParts, parts, 0, 0);

        printf("Part 2: Longest Strength = %d\n", longest.b);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numParts = ll->length;
        input.parts = malloc(input.numParts * sizeof(struct part));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                input.parts[i].connections.a = strtol(strtok(str, "/"), NULL, 10);
                input.parts[i].connections.b = strtol(strtok(NULL, ""), NULL, 10);
                input.parts[i].used = false;

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day24.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day24.txt";
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

