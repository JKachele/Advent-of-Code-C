/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 08, 2025 16:21:21 UTC
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

typedef enum tileType {
        FOREST,
        PATH,
        SLOPE
} tiletype;

typedef enum direction {
        RIGHT,
        DOWN,
        LEFT,
        UP
} direction;

typedef struct tile {
        tiletype type;
        direction dir;
        bool seen;
} tile;

typedef struct state {
        ivec2 pos;
        ivec2 prev;
        int32 cost;
} state;

typedef tll(state) tllstate;
typedef tll(ivec2) tllivec2;

struct input {
        ivec2 size;
        tile **map;
        ivec2 start;
        ivec2 end;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, tile map[size.y][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (map[y][x].type == FOREST) {
                                printf("#");
                                continue;
                        } else if (map[y][x].type == PATH) {
                                printf(".");
                                continue;
                        }
                        switch (map[y][x].dir) {
                        case RIGHT:
                                printf(">");
                                break;
                        case DOWN:
                                printf("v");
                                break;
                        case LEFT:
                                printf("<");
                                break;
                        case UP:
                                printf("^");
                                break;
                        }
                }
                printf("\n");
        }
        printf("\n");
}

// Add a state to the queue, sorting from highest cost to lowest cost
void addState(tllstate *queue, ivec2 pos, ivec2 prev, int32 cost) {
        bool added = false;
        tll_foreach(*queue, it) {
                state *s = &it->item;
                if (ivec2Eq(s->pos, pos)) {
                        if (cost > s->cost) {
                                s->cost = cost;
                                s->prev = prev;
                        }
                        added = true;
                        break;
                }
        }
        if (!added) {
                state new = {pos, prev, cost};
                tll_push_back(*queue, new);
        }
}

tllivec2 getNextPos(ivec2 size, tile map[][size.x], ivec2 pos, ivec2 prev) {
        ivec2 dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        tllivec2 nextPoss = tll_init();

        tile curT = map[pos.y][pos.x];

        if (curT.type == SLOPE) {
                ivec2 next = addIVec2(pos, dirs[curT.dir]);
                if (ivec2Eq(next, prev))
                        return nextPoss;
                tll_push_back(nextPoss, next);
                return nextPoss;
        }

        for (int i=0; i<4; i++) {
                ivec2 nextP = addIVec2(pos, dirs[i]);
                if (ivec2Eq(nextP, prev))
                        continue;
                tile nextT = map[nextP.y][nextP.x];
                if (nextT.type != FOREST && !nextT.seen)
                        tll_push_back(nextPoss, nextP);
        }

        return nextPoss;
}

int32 bfs(ivec2 size, tile map[size.y][size.x], ivec2 start, ivec2 end) {
        ivec2 dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        tllstate queue = tll_init();
        state init = {start, start, 0};
        tll_push_back(queue, init);

        int32 longest = 0;
        while (tll_length(queue) > 0) {
                state curS = tll_pop_front(queue);
                ivec2 curP = curS.pos;
                // printf("Pos: %d,%d Cost: %d\n", curP.x, curP.y, curS.cost);

                if (ivec2Eq(curP, end)) {
                        if (curS.cost > longest)
                                longest = curS.cost;
                        // printf("Path found with cost %d\n", curS.cost);
                        continue;
                }

                // map[curP.y][curP.x].seen = true;
                tllivec2 nextPoss = getNextPos(size, map, curP, curS.prev);
                tll_foreach(nextPoss, it) {
                        addState(&queue, it->item, curP, curS.cost + 1);
                }
        }

        return longest;
}

void part1(struct input input) {
        ivec2 size = input.size;
        tile (*map)[size.x] = (tile(*)[size.x])input.map;
        // printMap(size, map);

        int32 longestPath = bfs(size, map, input.start, input.end);

        printf("Part 1: %d\n\n", longestPath);
}

void part2(struct input input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        // Adding extra line of forest at top and bottom
        ivec2 size = {{getLongestLine(ll), ll->length+2}};
        input.size = size;
        input.start.y = 1;
        tile map[size.y][size.x];
        memset(map, 0, size.y * size.x * sizeof(tile));

        llNode *current = ll->head;
        int y = 1;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                for (int x=0; x<size.x; x++) {
                        switch (str[x]) {
                        case '#':
                                map[y][x].type = FOREST;
                                break;
                        case '.':
                                map[y][x].type = PATH;
                                if (y == 1) input.start.x = x;
                                input.end.x = x;
                                input.end.y = y;
                                break;
                        case '>':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = RIGHT;
                                break;
                        case 'v':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = DOWN;
                                break;
                        case '<':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = LEFT;
                                break;
                        case '^':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = UP;
                                break;
                        default:
                                printf("UNKNOWN CHAR: %c\n", str[x]);
                        }
                }

                current = current->next;
                y++;
        }
        // printMap(size, map);
        input.map = (tile**)map;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day23.txt";
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

