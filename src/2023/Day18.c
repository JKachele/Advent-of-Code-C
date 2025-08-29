/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Aug 29, 2025 15:56:25 UTC
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
        direction dir;
        int32 length;
        uint32 color;
} planstep;

typedef tll(planstep) tllplan;
typedef tll(ivec2) tllivec2;

typedef struct {
        int32 x1;
        int32 x2;
        int32 y1;
        int32 y2;
} ivec4;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPath(tllivec2 path, ivec4 bounds) {
        if (!Debug) return;
        ivec2 offset = {-bounds.x1, -bounds.y1};
        ivec2 size = {bounds.x2 - bounds.x1 + 1, bounds.y2 - bounds.y1 + 1};
        bool ground[size.y][size.x];
        memset(ground, 0, size.y * size.x * sizeof(bool));

        tll_foreach(path, it) {
                ivec2 pos = addIVec2(offset, it->item);
                ground[pos.y][pos.x] = true;
        }

        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (ground[y][x])
                                printf("#");
                        else
                                printf(".");
                }
                printf("\n");
        }
        printf("\n");
}

tllivec2 digPlan(tllplan plan) {
        const ivec2 dirs[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        tllivec2 path = tll_init();

        ivec2 cur = {0, 0};
        tll_push_back(path, cur);
        tll_foreach(plan, it) {
                planstep step = it->item;
                for (int i=0; i<step.length; i++) {
                        cur = addIVec2(cur, dirs[step.dir]);
                        tll_push_back(path, cur);
                }
        }
        return path;
}

ivec4 getDigBounds(tllivec2 path) {
        ivec4 bounds = {INT32_MAX, INT32_MIN, INT32_MAX, INT32_MIN};

        tll_foreach(path, it) {
                ivec2 pos = it->item;
                if (pos.x < bounds.x1) bounds.x1 = pos.x;
                if (pos.x > bounds.x2) bounds.x2 = pos.x;
                if (pos.y < bounds.y1) bounds.y1 = pos.y;
                if (pos.y > bounds.y2) bounds.y2 = pos.y;
        }

        return bounds;
}

int32 fillPath(tllivec2 path, ivec4 bounds) {
        const ivec2 dirs[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

        // Create grid
        ivec2 offset = {-bounds.x1, -bounds.y1};
        ivec2 size = {bounds.x2 - bounds.x1 + 1, bounds.y2 - bounds.y1 + 1};
        bool ground[size.y][size.x];
        memset(ground, 0, size.y * size.x * sizeof(bool));
        tll_foreach(path, it) {
                ivec2 pos = addIVec2(offset, it->item);
                ground[pos.y][pos.x] = true;
        }

        // Find spot inside path
        ivec2 insideStart = {0};
        for (int y=0; y<size.y; y++) {
                if (ground[y][0] && !ground[y][1]) {
                        insideStart = (ivec2){1, y};
                        break;
                }
        }
        if (insideStart.x == 0 && insideStart.y == 0) {
                printf("No spot found inside path\n");
                return 0;
        }

        // Queue of diggers
        tllivec2 queue = tll_init();
        tll_push_back(queue, insideStart);
        while (tll_length(queue) > 0) {
                ivec2 cur = tll_pop_front(queue);
                // printf("Dug at (%d, %d)\n", cur.x, cur.y);

                for (int i=0; i<4; i++) {
                        ivec2 next = addIVec2(cur, dirs[i]);
                        if (!ground[next.y][next.x]) {
                                ground[next.y][next.x] = true;
                                tll_push_back(queue, next);
                        }
                }
        }

        int32 numDug = 0;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (ground[y][x])
                                numDug++;
                }
        }

        return numDug;
}

void part1(llist *ll) {
        tllplan plan = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                planstep step = {0};
                char *token = strtok(str, " ");
                switch (token[0]) {
                case 'U':
                        step.dir = UP;
                        break;
                case 'R':
                        step.dir = RIGHT;
                        break;
                case 'D':
                        step.dir = DOWN;
                        break;
                case 'L':
                        step.dir = LEFT;
                        break;
                default:
                        printf("Unknown direction: %s\n", token);
                }
                
                token = strtok(NULL, " ");
                step.length = strtol(token, (char**)NULL, 10);

                token = strtok(NULL, "#");
                token = strtok(NULL, ")");
                step.color = strtol(token, (char**)NULL, 16);
                
                tll_push_back(plan, step);
                current = current->next;
        }
        tllivec2 path = digPlan(plan);
        ivec4 bounds = getDigBounds(path);
        ivec2 size = {bounds.x2 - bounds.x1 + 1, bounds.y2 - bounds.y1 + 1};
        // printf("Size: %dx%d\n", size.x, size.y);
        printPath(path, bounds);
        int32 area = fillPath(path, bounds);

        printf("Part 1: %d\n\n", area);
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
                char *file = "assets/tests/2023/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day18.txt";
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

