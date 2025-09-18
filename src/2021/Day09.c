/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 18, 2025 14:30:51 UTC
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

struct input {
        ivec2 size;
        u8 **map;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, u8 map[][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        printf("%d", map[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

bool isValidPos(ivec2 size, ivec2 pos) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

bool isLowPoint(ivec2 size, u8 map[][size.x], int y, int x) {
        const ivec2 Dirs[4] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        for (int i=0; i<4; i++) {
                ivec2 new;
                new.x = x + Dirs[i].x;
                new.y = y + Dirs[i].y;
                if (isValidPos(size, new) && map[new.y][new.x] <= map[y][x])
                        return false;
        }
        return true;
}

tllivec2 getLowPoints(ivec2 size, u8 map[][size.x]) {
        tllivec2 lowPoints = tll_init();
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (isLowPoint(size, map, y, x)) {
                                ivec2 point = {{x, y}};
                                tll_push_back(lowPoints, point);
                        }
                }
        }
        return lowPoints;
}

int32 getBasinSize(ivec2 size, u8 map[][size.x], ivec2 p) {
        const ivec2 Dirs[4] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};

        bool seen[size.y][size.x];
        for (int i=0; i<size.x*size.y; i++)
                seen[i/size.x][i%size.x] = false;
        seen[p.y][p.x] = true;
        tllivec2 queue = tll_init();
        tll_push_back(queue, p);

        int32 basinSize = 1;
        while (tll_length(queue) > 0) {
                ivec2 pos = tll_pop_front(queue);

                for (int i=0; i<4; i++) {
                        ivec2 new = ivec2Add(pos, Dirs[i]);
                        if (!isValidPos(size, new))
                                continue;
                        if (map[new.y][new.x] == 9 || seen[new.y][new.x])
                                continue;
                        seen[new.y][new.x] = true;
                        tll_push_back(queue, new);
                        basinSize++;
                }
        }
        return basinSize;
}

int32 getBasins(ivec2 size, u8 map[][size.x], tllivec2 lowPoints) {
        int32 maxBasins[3] = {0};
        tll_foreach(lowPoints, it) {
                int32 basin = getBasinSize(size, map, it->item);
                if (basin > maxBasins[0]) {
                        maxBasins[2] = maxBasins[1];
                        maxBasins[1] = maxBasins[0];
                        maxBasins[0] = basin;
                } else if (basin > maxBasins[1]) {
                        maxBasins[2] = maxBasins[1];
                        maxBasins[1] = basin;
                } else if (basin > maxBasins[2]) {
                        maxBasins[2] = basin;
                }
        }
        return maxBasins[0] * maxBasins[1] * maxBasins[2];
}

void part1(struct input input) {
        ivec2 size = input.size;
        u8 (*map)[size.x] = (u8(*)[size.x])input.map;
        // printMap(size, map);

        tllivec2 lowPoints = getLowPoints(size, map);
        int32 lowSum = 0;
        tll_foreach(lowPoints, it) {
                ivec2 p = it->item;
                lowSum += map[p.y][p.x] + 1;
        }

        printf("Part 1: %d\n\n", lowSum);
}

void part2(struct input input) {
        ivec2 size = input.size;
        u8 (*map)[size.x] = (u8(*)[size.x])input.map;
        // printMap(size, map);

        tllivec2 lowPoints = getLowPoints(size, map);
        int32 maxBasins = getBasins(size, map, lowPoints);

        printf("Part 2: %d\n", maxBasins);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.map = calloc(size.y*size.x, sizeof(u8));
        u8 (*map)[size.x] = (u8(*)[size.x])input.map;

        llNode *current = ll->head;
        int y = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        map[y][x] = str[x] - '0';
                }

                current = current->next;
                y++;
        }
        // printMap(size, map);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day09.txt";
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

