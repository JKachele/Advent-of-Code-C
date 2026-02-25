/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 17:46:05 UTC
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

struct input {
        ivec2 size;
        bool **map; // false: Empty, true: Tree
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, bool map[][size.x]) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (map[y][x] == false)
                                printf(".");
                        else
                                printf("#");
                }
                printf("\n");
        }
        printf("\n");
}

int testSlope(ivec2 size, bool map[][size.x], ivec2 slope) {
        int numTrees = 0;
        ivec2 pos = {{0, 0}};
        while (pos.y < size.y) {
                if (map[pos.y][pos.x])
                        numTrees++;
                pos.x = (pos.x + slope.x) % size.x;
                pos.y += slope.y;
        }

        return numTrees;
}

void part1(struct input input) {
        ivec2 size = input.size;
        bool map[size.y][size.x];
        memcpy(map, input.map, size.x * size.y * sizeof(bool));
        // printMap(size, map);

        int numTrees = testSlope(size, map, (ivec2){{3, 1}});

        printf("Part 1: %d\n\n", numTrees);
}

void part2(struct input input) {
        const ivec2 slopes[] = {
                (ivec2){{1, 1}},
                (ivec2){{3, 1}},
                (ivec2){{5, 1}},
                (ivec2){{7, 1}},
                (ivec2){{1, 2}}
        };
        const int numSlopes = sizeof(slopes) / sizeof(slopes[0]);

        ivec2 size = input.size;
        bool map[size.y][size.x];
        memcpy(map, input.map, size.x * size.y * sizeof(bool));

        int numTrees = 1;
        for (int i = 0; i < numSlopes; i++) {
                numTrees *= testSlope(size, map, slopes[i]);
        }

        printf("Part 2: %d\n\n", numTrees);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.map = calloc(size.x * size.y, sizeof(bool));
        bool (*map)[size.x] = (bool(*)[size.x])input.map;

        llNode *current = ll->head;
        int y = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x = 0; x < size.x; x++) {
                        if (str[x] == '.')
                                map[y][x] = false;
                        else
                                map[y][x] = true;
                }

                current = current->next;
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day03.txt";
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

