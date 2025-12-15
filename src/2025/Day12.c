/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Dec 11, 2025 23:29:12 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(ivec2) tllivec2;

typedef struct region {
        ivec2 size;
        int32 *presents;
} region;

struct input {
        int numPresents;
        tllivec2 *presents;
        int numRegions;
        region *regions;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPresents(tllivec2 *presents, int numPresents) {
        for (int i = 0; i < numPresents; i++) {
                printf("%d:%lu ", i, tll_length(presents[i]));
                tll_foreach(presents[i], it) {
                        printf("(%d, %d) ", it->item.x, it->item.y);
                }
                printf("\n");
        }
}

void printRegions(region *regions, int numRegions, int numPresents) {
        for (int i = 0; i < numRegions; i++) {
                printf("(%dx%d): ", regions[i].size.x, regions[i].size.y);
                for (int j = 0; j < numPresents; j++) {
                        printf("%d ", regions[i].presents[j]);
                }
                printf("\n");
        }
}

void part1(struct input input) {
        int numPresents = input.numPresents;
        tllivec2 *presents = input.presents;
        int numRegions = input.numRegions;
        region *regions = input.regions;
        // printPresents(presents, numPresents);
        // printRegions(regions, numRegions, numPresents);

        // if area of presents in region is more than the area of region, region is invalid
        int numValidRegions = 0;
        for (int i = 0; i < numRegions; i++) {
                int64 area = regions[i].size.x * regions[i].size.y;
                int64 presentArea = 0;
                for (int j = 0; j < numPresents; j++) {
                        presentArea += tll_length(presents[j]) * regions[i].presents[j];
                }

                if (presentArea <= area)
                        numValidRegions++;
        }

        printf("Part 1: %d\n\n", numValidRegions);
}

struct input parseInput(llist *ll) {
        int numPresents = 0;
        int numRegions = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (strlen(str) == 0) {
                        numPresents++;
                        numRegions = 0;
                } else {
                        numRegions++;
                }
        }

        llNode *cur;
        int i = 0;
        ivec2 pos = {{0,0}};
        tllivec2 *presents = calloc(numPresents, sizeof(tllivec2));
        for (cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (strchr(str, 'x') != NULL)
                        break;
                if (strlen(str) == 0) {
                        i++;
                        pos = (ivec2){{0,0}};
                        continue;
                }
                if (strchr(str, ':') != NULL)
                        continue;

                for (int j = 0; j < (int)strlen(str); j++) {
                        if (str[j] == '#')
                                tll_push_back(presents[i], pos);
                        pos.x++;
                }
                pos.x = 0;
                pos.y++;
        }
        // printPresents(presents, numPresents);

        region *regions = calloc(numRegions, sizeof(region));
        i = 0;
        for (; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                regions[i].size.x = strtol(strtok(str, "x"), NULL, 10);
                regions[i].size.y = strtol(strtok(NULL, ": "), NULL, 10);

                regions[i].presents = malloc(numPresents * sizeof(int32));
                for (int j = 0; j < numPresents; j++) {
                        int32 num = strtol(strtok(NULL, " "), NULL, 10);
                        regions[i].presents[j] = num;
                }

                i++;
        }
        // printRegions(regions, numRegions, numPresents);

        struct input input = {numPresents, presents, numRegions, regions};
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

