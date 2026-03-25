/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 18:19:44 UTC
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
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(int64) tllint64;

struct input {
        tllint64 modules;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input input) {
        int64 totalFuel = 0;
        tll_foreach(input.modules, it) {
                int64 fuel = (it->item / 3) - 2;
                totalFuel += fuel;
                // printf("%ld: %ld\n", it->item, fuel);
        }

        printf("Part 1: %ld\n\n", totalFuel);
}

void part2(struct input input) {
        int64 totalFuel = 0;
        tll_foreach(input.modules, it) {
                int64 moduleFuel = 0;
                int64 fuel = (it->item / 3) - 2;
                while (fuel > 0) {
                        moduleFuel += fuel;
                        fuel = (fuel / 3) - 2;
                }
                totalFuel += moduleFuel;
                // printf("%ld: %ld\n", it->item, moduleFuel);
        }

        printf("Part 2: %ld\n", totalFuel);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                int64 module = strtoul(str, NULL, 10);
                tll_push_back(input.modules, module);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();
        part2(input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

