/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 19, 2025 17:49:04 UTC
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

typedef struct state {
        ivec2 pos;
        int32 risk;
} state;
typedef tll(state) tllstate;

struct input {
        int32 size;
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

void printMap(int32 size, u8 map[][size]) {
        for (int y=0; y<size; y++) {
                if (y != 0 && y % (size/5) == 0)
                        printf("\n");
                for (int x=0; x<size; x++) {
                        if (x != 0 && x % (size/5) == 0)
                                printf(" ");
                        printf("%d", map[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(int32 size, ivec2 pos) {
        return pos.x >= 0 && pos.x < size && pos.y >= 0 && pos.y < size;
}

void addState(tllstate *queue, ivec2 pos, int32 risk) {
        state s = {pos, risk};

        bool inserted = false;
        tll_foreach(*queue, it) {
                if (it->item.risk >= risk) {
                        tll_insert_before(*queue, it, s);
                        inserted = true;
                        break;
                }
        }
        if (!inserted) {
                tll_push_back(*queue, s);
        }
}

int32 dijkstra(int32 size, u8 map[][size]) {
        const ivec2 Dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};

        tllstate queue = tll_init();
        state init = {0};
        tll_push_back(queue, init);

        bool seen[size][size];
        for (int i=0; i<size*size; i++)
                seen[i/size][i%size] = false;

        while (tll_length(queue) > 0) {
                state curS = tll_pop_front(queue);

                // If at end, return risk
                if (curS.pos.x == size-1 && curS.pos.y == size-1)
                        return curS.risk;

                for (int i=0; i<4; i++) {
                        ivec2 newPos = ivec2Add(curS.pos, Dirs[i]);
                        if (!validPos(size, newPos))
                                continue;
                        int32 newRisk = curS.risk + map[newPos.y][newPos.x];
                        if (!seen[newPos.y][newPos.x]) {
                                seen[newPos.y][newPos.x] = true;
                                addState(&queue, newPos, newRisk);
                        }
                }
        }

        return 0;
}

void expandMap(int32 size, u8 map[][size],
                int32 expSize, u8 expMap[][expSize]) {
        for (int y=0; y<expSize; y++) {
                int ogY = y % size;
                int offsetY = y / size;
                for (int x=0; x<expSize; x++) {
                        int ogX = x % size;
                        int offset = offsetY + (x / size);

                        int32 risk = map[ogY][ogX] + offset;
                        if (risk > 9)
                                risk -= 9;

                        expMap[y][x] = risk;
                }
        }
}

void part1(struct input input) {
        int32 size = input.size;
        u8 (*map)[size] = (u8(*)[size])input.map;

        int32 lowestRisk = dijkstra(size, map);

        printf("Part 1: %d\n\n", lowestRisk);
}

void part2(struct input input) {
        int32 size = input.size;
        u8 (*map)[size] = (u8(*)[size])input.map;

        int32 expSize = size * 5;
        u8 expMap[expSize][expSize];
        expandMap(size, map, expSize, expMap);
        // printMap(expSize, expMap);

        int32 lowestRisk = dijkstra(expSize, expMap);

        printf("Part 2: %d\n\n", lowestRisk);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.size = ll->length;
        int32 size = input.size;
        input.map = calloc(size*size, sizeof(u8));
        u8 (*map)[size] = (u8(*)[size])input.map;

        llNode *current = ll->head;
        int y=0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size; x++) {
                        map[y][x] = str[x] - '0';
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
                char *file = "assets/tests/2021/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day15.txt";
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

