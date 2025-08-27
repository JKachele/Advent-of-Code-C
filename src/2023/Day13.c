/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Aug 26, 2025 17:46:22 UTC
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

typedef struct {
        ivec2 size;
        bool **map;
} pattern;

typedef tll(pattern) tllpattern;
typedef tll(char*) tllstr;
typedef tll(int) tllint;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPattern(pattern p) {
        if (!Debug) return;
        bool (*map)[p.size.x] = (bool(*)[p.size.x])p.map;
        for (int y=0; y<p.size.y; y++) {
                for (int x=0; x<p.size.x; x++) {
                        if (map[y][x])
                                printf("#");
                        else
                                printf(".");
                }
                printf("\n");
        }
        printf("\n");
}

void printPatterns(tllpattern patterns) {
        if (!Debug) return;
        tll_foreach(patterns, it) {
                printPattern(it->item);
        }
}

bool testVertMirrorLine(pattern p, int32 line) {
        ivec2 size = p.size;
        bool (*map)[size.x] = (bool(*)[size.x])p.map;

        for (int i=1; i<size.x; i++) {
                int32 x1 = line - (i - 1);
                int32 x2 = line + i;
                if (x1 < 0 || x2 >= size.x) break;
                for (int y=0; y<size.y; y++) {
                        if (map[y][x1] != map[y][x2])
                                return false;
                }
        }
        return true;
}

int32 findVerticalMirror(pattern p) {
        ivec2 size = p.size;
        bool (*map)[size.x] = (bool(*)[size.x])p.map;

        // Find vertical mirror line
        // Find 2 adjacent columns with the same pattern
        tllint mirrorColumns = tll_init();
        for (int x=0; x<size.x - 1; x++) {
                bool different = false;
                for (int y=0; y<size.y; y++) {
                        if (map[y][x] != map[y][x+1]) {
                                different = true;
                                break;
                        }
                }
                if (!different) {
                        tll_push_back(mirrorColumns, x);
                }
        }

        // If no mirror lines found, return 0
        if (tll_length(mirrorColumns) == 0)
                return 0;

        // expand each potential mirror line to confirm full mirror
        tll_foreach(mirrorColumns, it) {
                if (testVertMirrorLine(p, it->item))
                        return it->item + 1;
        }

        return 0;
}

bool testHroizMirrorLine(pattern p, int32 line) {
        ivec2 size = p.size;
        bool (*map)[size.x] = (bool(*)[size.x])p.map;

        for (int i=1; i<size.y; i++) {
                int32 y1 = line - (i - 1);
                int32 y2 = line + i;
                if (y1 < 0 || y2 >= size.y) break;
                for (int x=0; x<size.x; x++) {
                        if (map[y1][x] != map[y2][x])
                                return false;
                }
        }
        return true;
}

int32 findHorizontalMirror(pattern p) {
        ivec2 size = p.size;
        bool (*map)[size.x] = (bool(*)[size.x])p.map;

        // Find horizontal mirror line
        // Find 2 adjacent rows with the same pattern
        tllint mirrorRows = tll_init();
        for (int y=0; y<size.y - 1; y++) {
                bool different = false;
                for (int x=0; x<size.x; x++) {
                        if (map[y][x] != map[y+1][x]) {
                                different = true;
                                break;
                        }
                }
                if (!different) {
                        tll_push_back(mirrorRows, y);
                }
        }

        // If no mirror lines found, return 0
        if (tll_length(mirrorRows) == 0)
                return 0;

        // expand each potential mirror line to confirm full mirror
        tll_foreach(mirrorRows, it) {
                if (testHroizMirrorLine(p, it->item))
                        return it->item + 1;
        }

        return 0;
}

int32 findMirror(pattern p) {
        int32 verticalMirror = findVerticalMirror(p);
        int32 horizontalMirror = findHorizontalMirror(p);

        return verticalMirror + (100 * horizontalMirror);
}

void part1(llist *ll) {
        tllpattern patterns = tll_init();

        // Add empty line to end of input
        char *emptyStr = malloc(1);
        emptyStr[0] = '\0';
        llist_add(ll, emptyStr);

        llNode *current = ll->head;
        tllstr mapStr = tll_init();
        while(current != NULL) {
                char *str = malloc(INPUT_BUFFER_SIZE);
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                if (strlen(str) > 0) {
                        tll_push_back(mapStr, str);
                        current = current->next;
                        continue;
                }

                // If empty line
                pattern p = {0};
                p.size.x = (int)strlen(tll_front(mapStr));
                p.size.y = tll_length(mapStr);

                // Create bool array
                bool (*map)[p.size.x];
                map = calloc(p.size.x * p.size.y, sizeof(bool));

                // Populate array
                int32 y = 0;
                tll_foreach(mapStr, it) {
                        for (int x=0; x<p.size.x; x++) {
                                if (it->item[x] == '#')
                                        map[y][x] = true;
                                else if (it->item[x] == '.')
                                        map[y][x] = false;
                                else
                                        printf("Unknown '%c' at (%d, %d)\n",
                                                        it->item[x], x, y);
                        }
                        y++;
                }
                p.map = (bool**)map;

                tll_push_back(patterns, p);

                tll_free_and_free(mapStr, free);

                current = current->next;
        }
        printPatterns(patterns);

        int32 summarySum = 0;
        tll_foreach(patterns, it) {
                int32 summary = findMirror(it->item);
                summarySum += summary;
        }

        printf("Part 1: %d\n\n", summarySum);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

