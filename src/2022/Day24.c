/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Aug 22, 2025 18:38:52 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef enum {
        NORTH,
        EAST,
        SOUTH,
        WEST
} dir;

typedef struct {
        bool dirs[4]; // N, E, S, W
        bool covered;
        bool wall;
} cell;

typedef struct {
        ivec2 pos;
        int32 time;
} state;

typedef tll(state) tllstate;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printValley(ivec2 size, cell valley[size.y][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (valley[y][x].wall) {
                                printf("#");
                                continue;
                        }
                        if (!valley[y][x].covered) {
                                printf(".");
                                continue;
                        }

                        int8 numStorms = 0;
                        char c = ' ';
                        char dirs[] = {'^', '>', 'v', '<'};
                        for (int i=0; i<4; i++) {
                                if (valley[y][x].dirs[i]) {
                                        c = dirs[i];
                                        numStorms++;
                                }
                        }
                        if (numStorms > 1) {
                                c = numStorms + '0';
                        }
                        printf("%c", c);
                }
                printf("\n");
        }
        printf("\n");
}

int32 gcd(int32 a, int32 b) {
        if (b == 0)
                return a;
        return gcd(b, a % b);
}

int32 lcm(int32 a, int32 b) {
        return (a * b) / gcd(a, b);
}

void makeEmptyValley(ivec2 size, cell valley[size.y][size.x]) {
        for (int y=1; y<size.y - 1; y++) {
                for (int x=1; x<size.x-1; x++) {
                        valley[y][x].dirs[NORTH] = false;
                        valley[y][x].dirs[EAST] = false;
                        valley[y][x].dirs[SOUTH] = false;
                        valley[y][x].dirs[WEST] = false;
                        valley[y][x].covered = false;
                }
        }
}

ivec2 getMovedPos(ivec2 size, ivec2 pos, dir d, int32 time) {
        const ivec2 dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        ivec2 insize = {{size.x - 2, size.y - 2}}; // Size not including walls
        ivec2 new = {0};

        new.x = (pos.x + (dirs[d].x * time)) - 1;
        new.x = (((new.x % insize.x) + insize.x) % insize.x) + 1;

        new.y = (pos.y + (dirs[d].y * time)) - 1;
        new.y = (((new.y % insize.y) + insize.y) % insize.y) + 1;

        return new;
}

void incrementValley(ivec2 size, cell valley[size.y][size.x], int32 time) {
        cell temp[size.y][size.x];
        memcpy(temp, valley, sizeof(cell[size.y][size.x]));
        makeEmptyValley(size, temp);

        for (int y=1; y<size.y - 1; y++) {
                for (int x=1; x<size.x-1; x++) {
                        if (!valley[y][x].covered) continue;
                        for (int i=0; i<4; i++) {
                                if (!valley[y][x].dirs[i]) continue;
                                ivec2 newPos = getMovedPos(size,
                                                (ivec2){{x, y}}, i, time);
                                temp[newPos.y][newPos.x].covered = true;
                                temp[newPos.y][newPos.x].dirs[i] = true;
                        }
                }
        }
        memcpy(valley, temp, ivec2area(size) * sizeof(cell));
}

int32 getMaxStates(ivec2 size, int32 lcm) {
        return (size.x) * (size.y) * lcm;
}

void getAllValleys(ivec2 size, int32 lcm, cell valleyConst[size.y][size.x],
                cell **valleys[]) {
        cell valley[size.y][size.x];
        memcpy(valley, valleyConst, ivec2area(size) * sizeof(cell));

        for (int i=0; i<lcm; i++) {
                cell (*tmp)[size.x] = malloc(sizeof(cell[size.y][size.x]));
                memcpy(tmp, valley, sizeof(cell[size.y][size.x]));
                valleys[i] = (cell**)tmp;
                incrementValley(size, valley, 1);
        }
}

int32 getIndex(ivec2 size, int32 lcm, state s) {
        return ((((s.pos.y) * (size.x)) + (s.pos.x)) * lcm) +
                (s.time % lcm);
}

void addToQueue(tllstate *queue, state s) {
        // Loop through queue, Find first entry with higher time
        // and add state before it
        tll_foreach(*queue, it) {
                if (it->item.time > s.time) {
                        tll_insert_before(*queue, it, s);
                        return;
                }
        }
        // If not inserted, add at end
        tll_push_back(*queue, s);
}

int findPath(ivec2 size, cell valley[size.y][size.x],
                ivec2 start, ivec2 end, int32 timeStart) {
        const ivec2 moves[5] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, 0}}};
        int32 maxTime = lcm(size.x - 2, size.y - 2);
        int32 maxStates = getMaxStates(size, maxTime);
        // printf("%d, %d\n", maxTime, maxStates);

        // List of all possible valleys
        cell **valleys[maxTime];
        getAllValleys(size, maxTime, valley, valleys);
        cell (*test)[size.x] = (cell(*)[size.x])valleys[1];

        bool seen[maxStates]; // Index is Pos.y Pos.x Time % lcm
        memset(seen, false, maxStates);
        tllstate queue = tll_init(); // Queue sorted by time
        
        state initState = {start, timeStart};
        tll_push_back(queue, initState);

        while (tll_length(queue) > 0) {
                state cur = tll_pop_front(queue);

                // If at end guaranteed to be the minimum time to reach
                if (ivec2Eq(cur.pos, end)) {
                        return cur.time - timeStart;
                }

                // Get valley state for the next minuite
                cell **valPtr = valleys[(cur.time + 1) % maxTime];
                cell (*val)[size.x] = (cell(*)[size.x])valPtr;

                // 5 options for movements (4 directions and not moving)
                for (int i=0; i<5; i++) {
                        state new;
                        new.time = cur.time + 1;
                        new.pos = ivec2Add(cur.pos, moves[i]);
                        if (new.pos.x >= size.x || new.pos.y >= size.y ||
                                        new.pos.x < 0 || new.pos.y < 0)
                                continue;

                        // Skip if covered or is wall
                        if (val[new.pos.y][new.pos.x].covered ||
                                        val[new.pos.y][new.pos.x].wall)
                                continue;

                        // Check if seen already
                        int32 index = getIndex(size, maxTime, new);
                        if (!seen[index]) {
                                seen[index] = true;
                                // tll_push_back(queue, new);
                                addToQueue(&queue, new);
                        }
                }
        }

        // Should only reach here if a solution wasn't found
        return -1;
}

void solve(llist *ll) {
        ivec2 size;
        size.y = ll->length;
        size.x = getLongestLine(ll);
        cell valley[size.y][size.x];
        memset(valley, 0, ivec2area(size) * sizeof(cell));
        ivec2 start = {0};
        ivec2 end = {0};

        llNode *current = ll->head;
        int32 yindex = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        switch (str[x]) {
                        case '.':
                                if (yindex == 0)
                                        start = (ivec2){{x, yindex}};
                                if (yindex == size.y - 1)
                                        end = (ivec2){{x, yindex}};
                                break;
                        case '#':
                                valley[yindex][x].wall = true;
                                break;
                        case '^':
                                valley[yindex][x].dirs[NORTH] = true;
                                valley[yindex][x].covered = true;
                                break;
                        case '>':
                                valley[yindex][x].dirs[EAST] = true;
                                valley[yindex][x].covered = true;
                                break;
                        case 'v':
                                valley[yindex][x].dirs[SOUTH] = true;
                                valley[yindex][x].covered = true;
                                break;
                        case '<':
                                valley[yindex][x].dirs[WEST] = true;
                                valley[yindex][x].covered = true;
                                break;
                        default:
                                printf("Unknown character: %c at (%d, %d)\n",
                                                str[x], x, yindex);
                                break;
                        }
                }

                current = current->next;
                yindex++;
        }
        // printValley(size, valley);

        int32 minTime = findPath(size, valley, start, end, 0);
        printf("Part 1: %d\n\n", minTime);

        minTime += findPath(size, valley, end, start, minTime);
        minTime += findPath(size, valley, start, end, minTime);
        printf("Part 2: %d\n\n", minTime);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2022/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2022/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        solve(ll);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Solve: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

