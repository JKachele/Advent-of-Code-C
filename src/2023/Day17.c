/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Aug 28, 2025 21:28:40 UTC
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
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef enum {
        UP,
        RIGHT,
        DOWN,
        LEFT
} direction;

typedef struct {
        uint32 cost;
        ivec2 pos;
        uint8 straight;
        direction dir;
} crucible;

typedef tll(crucible) tllcrucible;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(ivec2 size, uint8 grid[size.y][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        printf("%d", grid[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

void printCrucible(crucible c) {
        printf("{Cost: %u, Pos: {x: %d, y: %d}, ", c.cost, c.pos.x, c.pos.y);
        printf("Straight: %u, Dir: %d}\n", c.straight, c.dir);
}

bool validPos(ivec2 size, ivec2 pos) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

direction opposite(direction dir) {
        switch (dir) {
        case UP: return DOWN;
        case RIGHT: return LEFT;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        default: return UP;
        }
}

uint32 getHash(crucible c) {
        uint32 h = 0;
        h += (uint8)c.pos.x << 24;
        h += (uint8)c.pos.y << 16;
        h += (uint8)c.dir << 8;
        h += c.straight;
        return h;
}

void addCrucible(tllcrucible *queue, crucible c) {
        bool inserted = false;
        tll_foreach(*queue, it) {
                crucible c2 = it->item;
                // Find first crucible without a lower cost and place before
                if (!inserted && c2.cost >= c.cost) {
                        tll_insert_before(*queue, it, c);
                        inserted = true;
                        break;
                }
        }
        // If not insterted, insert at back
        if (!inserted)
                tll_push_back(*queue, c);
}

tllcrucible findNext(ivec2 size, uint8 grid[size.y][size.x], crucible cur) {
        ivec2 dirs[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        tllcrucible nextCrucibles = tll_init();

        for (int i=0; i<4; i++) {
                // Can't move more than 3 in a straight line
                if ((int)cur.dir == i && cur.straight == 3)
                        continue;

                // Can't move in opposite direction
                if (i == (int)opposite(cur.dir))
                        continue;

                // Create next crucible
                crucible next;
                next.pos = addIVec2(cur.pos, dirs[i]);
                // Cannot move out of bounds
                if (!validPos(size, next.pos))
                        continue;
                next.dir = i;
                if ((int)cur.dir == i)
                        next.straight = cur.straight + 1;
                else
                        next.straight = 1;
                next.cost = cur.cost + grid[next.pos.y][next.pos.x];

                tll_push_back(nextCrucibles, next);
        }

        return nextCrucibles ;
}

tllcrucible findNextUltra(ivec2 size, uint8 grid[][size.x], crucible cur) {
        ivec2 dirs[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        tllcrucible nextCrucibles = tll_init();

        for (int i=0; i<4; i++) {
                // Can't move more than 10 in a straight line
                if ((int)cur.dir == i && cur.straight == 10)
                        continue;

                // Has to move at least 4 in a straight line
                if ((int)cur.dir != i && cur.straight < 4)
                        continue;

                // Can't move in opposite direction
                if (i == (int)opposite(cur.dir))
                        continue;

                // Create next crucible
                crucible next;
                next.pos = addIVec2(cur.pos, dirs[i]);
                // Cannot move out of bounds
                if (!validPos(size, next.pos))
                        continue;
                next.dir = i;
                if ((int)cur.dir == i)
                        next.straight = cur.straight + 1;
                else
                        next.straight = 1;
                next.cost = cur.cost + grid[next.pos.y][next.pos.x];

                tll_push_back(nextCrucibles, next);
        }

        return nextCrucibles ;
}

uint32 dijkstra(ivec2 size, uint8 grid[size.y][size.x], bool ultra) {
        const ivec2 Goal = {size.x-1, size.y-1};

        tllcrucible queue = tll_init();
        bool *seen = calloc((int64)UINT32_MAX + 1, sizeof(bool));

        crucible initRight = {grid[0][1], {1, 0}, 1, RIGHT};
        crucible initDown = {grid[1][0], {0, 1}, 1, DOWN};
        addCrucible(&queue, initRight);
        addCrucible(&queue, initDown);

        while (tll_length(queue) > 0) {
                crucible cur = tll_pop_front(queue);
                // printCrucible(cur);

                // If at goal, Guarantee the lowest cost path
                if (ivec2Eq(cur.pos, Goal)) {
                        // Ultra needs at least 4 straight moves before end
                        if (!ultra)
                                return cur.cost;
                        else if (cur.straight >= 4)
                                return cur.cost;
                        else
                                continue;
                }

                tllcrucible nextCrucibles;
                if (ultra)
                        nextCrucibles = findNextUltra(size, grid, cur);
                else
                        nextCrucibles = findNext(size, grid, cur);
                tll_foreach(nextCrucibles, it) {
                        uint32 hash = getHash(it->item);
                        // printf("%u\n", hash);
                        if (!seen[hash]) {
                                seen[hash] = true;
                                addCrucible(&queue, it->item);
                        }
                }
        }

        return 0;
}

void part1(llist *ll) {
        ivec2 size = {getLongestLine(ll), ll->length};
        uint8 grid[size.y][size.x];

        llNode *current = ll->head;
        int32 yIndex = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        grid[yIndex][x] = str[x] - '0';
                }

                current = current->next;
                yIndex++;
        }
        // printGrid(size, grid);

        uint32 minCost = dijkstra(size, grid, false);

        printf("Part 1: %u\n\n", minCost);
}

void part2(llist *ll) {
        ivec2 size = {getLongestLine(ll), ll->length};
        uint8 grid[size.y][size.x];

        llNode *current = ll->head;
        int32 yIndex = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        grid[yIndex][x] = str[x] - '0';
                }

                current = current->next;
                yIndex++;
        }
        // printGrid(size, grid);

        uint32 minCost = dijkstra(size, grid, true);

        printf("Part 2: %u\n\n", minCost);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day17.txt";
                // char *file = "assets/tests/2023/Day17b.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day17.txt";
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

