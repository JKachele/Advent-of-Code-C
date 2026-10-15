/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 15, 2026 16:51:25 EDT
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
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(ivec2) talivec2;

struct input {
        talivec2 layers;
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
        talivec2 layers = input->layers;

        // Scanners are at the top every ((depth - 1) * 2) picoseconds
        int tripSeverity = 0;
        tal_for(layers, i) {
                ivec2 l = layers.array[i];
                int period = (l.b - 1) * 2;
                if ((l.a % period) == 0) {
                        tripSeverity += l.a * l.b;
                }
        }

        printf("Part 1: Trip Severity = %d\n\n", tripSeverity);
}

void part2(struct input *input) {
        talivec2 layers = input->layers;

        // Scanners are at the top every ((depth - 1) * 2) picoseconds
        int delay = 0;
        for (;;) {
                bool caught = false;
                tal_for(layers, i) {
                        ivec2 l = layers.array[i];
                        int timeInLayer = delay + l.a;
                        int period = (l.b - 1) * 2;
                        if ((timeInLayer % period) == 0) {
                                caught = true;
                                break;
                        }
                }
                if (!caught) {
                        break;
                }
                delay++;
        }

        printf("Part 2: Minimum Delay = %d\n", delay);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                ivec2 layer;
                layer.a = strtol(strtok(str, ": "), NULL, 10);
                layer.b = strtol(strtok(NULL, ""), NULL, 10);
                tal_add(input.layers, layer);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day13.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day13.txt";
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
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
        //                 parseTime, pt1Time);

        return 0;
}

