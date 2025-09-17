/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 17, 2025 13:33:15 UTC
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

typedef struct line {
        ivec2 a;
        ivec2 b;
} line;
typedef tll(line) tlline;

struct input {
        tlline lines;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, int map[][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (map[y][x] == 0)
                                printf(". ");
                        else
                                printf("%d ", map[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

int32 drawVerticalLine(ivec2 size, int map[][size.x], line l) {
        int32 minY = l.a.y < l.b.y ? l.a.y : l.b.y;
        int32 maxY = l.a.y > l.b.y ? l.a.y : l.b.y;

        int numIntersections = 0;
        for (int y=minY; y<=maxY; y++) {
                map[y][l.a.x]++;
                if (map[y][l.a.x] == 2)
                        numIntersections++;
        }
        return numIntersections;
}

int32 drawHorizontalLine(ivec2 size, int map[][size.x], line l) {
        int32 minX = l.a.x < l.b.x ? l.a.x : l.b.x;
        int32 maxX = l.a.x > l.b.x ? l.a.x : l.b.x;

        int numIntersections = 0;
        for (int x=minX; x<=maxX; x++) {
                map[l.a.y][x]++;
                if (map[l.a.y][x] == 2)
                        numIntersections++;
        }
        return numIntersections;
}

int32 drawUpDiagionalLine(ivec2 size, int map[][size.x], line l) {
        ivec2 minPt = l.a.x < l.b.x ? l.a : l.b;
        int32 len = abs(l.b.x - l.a.x);

        int numIntersections = 0;
        for (int i=0; i<=len; i++) {
                int32 x = minPt.x + i;
                int32 y = minPt.y + i;
                map[y][x]++;
                if (map[y][x] == 2)
                        numIntersections++;
        }
        return numIntersections;
}

int32 drawDownDiagionalLine(ivec2 size, int map[][size.x], line l) {
        ivec2 minXPt = l.a.x < l.b.x ? l.a : l.b;
        int32 len = abs(l.b.x - l.a.x);

        int numIntersections = 0;
        for (int i=0; i<=len; i++) {
                int32 x = minXPt.x + i;
                int32 y = minXPt.y - i;
                map[y][x]++;
                if (map[y][x] == 2)
                        numIntersections++;
        }
        return numIntersections;
}

void part1(struct input input) {
        tlline lines = input.lines;
        tlline flatLines = tll_init();

        // Filter list to only horizontal or vertical lines
        ivec2 max = {0};
        tll_foreach(lines, it) {
                line l = it->item;
                if (l.a.x == l.b.x || l.a.y == l.b.y) {
                        tll_push_back(flatLines, l);
                        if (l.a.x > max.x)
                                max.x = l.a.x;
                        if (l.b.x > max.x)
                                max.x = l.b.x;
                        if (l.a.y > max.y)
                                max.y = l.a.y;
                        if (l.b.y > max.y)
                                max.y = l.b.y;
                }
        }
        max.x++;
        max.y++;

        int map[max.y][max.x];
        for (int i=0; i<max.x*max.y; i++)
                map[i/max.x][i%max.x] = 0;

        int numIntersections = 0;
        tll_foreach(flatLines, it) {
                line l = it->item;
                // Vertical
                if (l.a.x == l.b.x) {
                        numIntersections += drawVerticalLine(max, map, l);
                } else if (l.a.y == l.b.y) { // Vertical
                        numIntersections += drawHorizontalLine(max, map, l);
                }
        }
        // printMap(max, map);

        printf("Part 1: %d\n\n", numIntersections);
}

void part2(struct input input) {
        tlline lines = input.lines;

        // Get max coord of line
        ivec2 max = {0};
        tll_foreach(lines, it) {
                line l = it->item;
                if (l.a.x > max.x)
                        max.x = l.a.x;
                if (l.b.x > max.x)
                        max.x = l.b.x;
                if (l.a.y > max.y)
                        max.y = l.a.y;
                if (l.b.y > max.y)
                        max.y = l.b.y;
        }
        max.x++;
        max.y++;

        int map[max.y][max.x];
        for (int i=0; i<max.x*max.y; i++)
                map[i/max.x][i%max.x] = 0;

        int numIntersections = 0;
        tll_foreach(lines, it) {
                line l = it->item;
                // Vertical
                if (l.a.x == l.b.x) {
                        numIntersections += drawVerticalLine(max, map, l);
                        continue;
                }
                // Vertical
                if (l.a.y == l.b.y) { 
                        numIntersections += drawHorizontalLine(max, map, l);
                        continue;
                }
                // Diagonals
                double slope = (double)(l.b.y - l.a.y) / (l.b.x - l.a.x);
                if (slope > 0) {
                        numIntersections += drawUpDiagionalLine(max, map, l);
                        continue;
                }
                numIntersections += drawDownDiagionalLine(max, map, l);
        }
        // printMap(max, map);

        printf("Part 2: %d\n\n", numIntersections);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tlline lines = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;

                line l = {0};
                l.a.x = strtol(strtok(str, ","), NULL, 10);
                l.a.y = strtol(strtok(NULL, " "), NULL, 10);
                strtok(NULL, " ");
                l.b.x = strtol(strtok(NULL, ","), NULL, 10);
                l.b.y = strtol(strtok(NULL, ""), NULL, 10);

                tll_push_back(lines, l);

                current = current->next;
        }
        input.lines = lines;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day05.txt";
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

