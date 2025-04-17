/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 17, 2025 16:42:52 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

typedef struct {
        int height;
        int dist;
        bool explored;
} cell;

typedef tll(ivec2) tllvec;

static const ivec2 dirs[4] = {{0,-1},{1,0},{0,1},{-1,0}};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(const ivec2 SIZE, cell grid[SIZE.y][SIZE.x]) {
        if (!Debug) return;
        for (int y = 0; y < SIZE.y; y++) {
                for (int x = 0; x < SIZE.x; x++) {
                        if (grid[y][x].height < 10) {
                                printf("%d  ", grid[y][x].height);
                        } else {
                                printf("%d ", grid[y][x].height);
                        }
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(const ivec2 SIZE, ivec2 pos) {
        if (pos.x < 0 || pos.y < 0)
                return false;
        if (pos.x >= SIZE.x || pos.y >= SIZE.y)
                return false;
        return true;
}

void initCell(cell *c) {
        c->height = 0;
        c->dist = INT_MAX;
        c->explored = false;
}

void part1(llist *ll) {
        const ivec2 SIZE = {.x = getLongestLine(ll), .y = ll->length};
        cell grid[SIZE.y][SIZE.x];
        MAKE_LOOP(y, SIZE.y, x, SIZE.x)
                initCell(&grid[y][x]);

        ivec2 start = {.x = 0, .y = 0};
        ivec2 end = {.x = 0, .y = 0};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                if ((int)strlen(str) != SIZE.x)
                        printf("INVALID INPUT SIZE ON LINE %d\n", line);

                for (int x = 0; x < SIZE.x; x++) {
                        if (str[x] == 'S') {
                                start = (ivec2){.x = x, .y = line};
                                grid[line][x].height = 0;
                                grid[line][x].dist = 0;
                        } else if (str[x] == 'E') {
                                end = (ivec2){.x = x, .y = line};
                                grid[line][x].height = 25;
                        } else {
                                grid[line][x].height = str[x] - 'a';
                        }

                }

                current = current->next;
                line++;
        }
        printGrid(SIZE, grid);

        int pathLen = 0;

        printf("Part 1: Path Length: %d\n\n", pathLen);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day12.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day12.txt");
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

