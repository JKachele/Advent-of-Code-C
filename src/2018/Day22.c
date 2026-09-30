/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 30, 2026 11:50:41 EDT
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
#include "../util/util.h"
#include "../util/talist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

enum type {
        ROCKY,
        WET,
        NARROW
};

enum tool {
        TORCH,
        GEAR,
        NONE
};

// Which tool can't be used in the type of region
const enum tool CantUse[3] = {/*Rocky*/NONE, /*Wet*/TORCH, /*Narrow*/GEAR};

struct region {
        int erosion;
        enum type type;
        bool target;
        int dist[3];    // 0: Torch, 1: Climbing Gear, 2: None
        int visited[3];
};

struct entry {
        ivec2 pos;
        int dist;
        enum tool tool;
        struct entry *prev;
        struct entry *next;
};

struct queue {
        struct entry *head;
        struct entry *tail;
        int len;
};

struct input {
        int depth;
        ivec2 targetPos;
};

const ivec2 Dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printCave(ivec2 size, struct region cave[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (x == 0 && y == 0)
                                printf("M");
                        else if (cave[y][x].target)
                                printf("T");
                        else if (cave[y][x].type == ROCKY)
                                printf(".");
                        else if (cave[y][x].type == WET)
                                printf("=");
                        else
                                printf("|");
                }
                printf("\n");
        }
        printf("\n");
}

void printQueue(struct queue *queue) {
        printf("Length = %d\n", queue->len);
        struct entry *cur = queue->head;
        int i = 0;
        while (cur != NULL) {
                printf("[(%d, %d), %d, %d] ", cur->pos.x, cur->pos.y, cur->tool, cur->dist);
                cur = cur->next;
                i++;
        }
        printf("\nActual Length = %d\n", i);
}

bool invalidPos(ivec2 size, ivec2 pos) {
        return (pos.x < 0) || (pos.y < 0) || (pos.x >= size.x) || (pos.y >= size.y);
}

void freeQueue(struct queue *queue) {
        struct entry *cur = queue->head;
        while (cur != NULL) {
                struct entry *next = cur->next;
                free(cur);
                cur = next;
        }
        free(queue);
}

void addEntry(struct queue *queue, struct entry *entry) {
        struct entry *cur = queue->head;
        while (cur != NULL) {
                if (cur->dist > entry->dist) {
                        entry->next = cur;
                        entry->prev = cur->prev;
                        if (cur->prev != NULL)
                                cur->prev->next = entry;
                        else
                                queue->head = entry;
                        cur->prev = entry;
                        queue->len++;
                        return;
                }
                cur = cur->next;
        }
        if (queue->head == NULL) {
                queue->head = entry;
                queue->tail = entry;
                queue->len++;
        } else {
                queue->tail->next = entry;
                entry->prev = queue->tail;
                queue->tail = entry;
                queue->len++;
        }
}

struct entry *popEntry(struct queue *queue) {
        struct entry *entry = queue->head;
        queue->head = entry->next;
        if (queue->head == NULL)
                queue->tail = NULL;
        else
                queue->head->prev = NULL;
        queue->len--;
        return entry;
}

void dijkstra(ivec2 size, struct region cave[][size.x]) {
        struct queue *queue = calloc(1, sizeof(struct queue));
        struct entry *mouth = calloc(1, sizeof(struct entry));
        addEntry(queue, mouth);

        while (queue->len > 0) {
                struct entry *cur = popEntry(queue);
                struct region reg = cave[cur->pos.y][cur->pos.x];
                cave[cur->pos.y][cur->pos.x].visited[cur->tool] = true;

                if (reg.dist[cur->tool] < cur->dist) {
                        free(cur);
                        continue;
                }

                if (reg.target && cur->tool == TORCH)
                        break;

                for (int i = 0; i < 4; i++) {
                        ivec2 nextPos = ivec2Add(cur->pos, Dirs[i]);
                        if (invalidPos(size, nextPos))
                                continue;
                        struct region nextReg = cave[nextPos.y][nextPos.x];

                        int nextDist = cur->dist + 1;
                        enum tool nextTool = cur->tool;
                        if (CantUse[nextReg.type] == nextTool)
                                continue;
                        if (nextReg.visited[nextTool] || nextReg.dist[nextTool] <= nextDist)
                                continue;

                        cave[nextPos.y][nextPos.x].dist[nextTool] = nextDist;
                        struct entry *newEntry = malloc(sizeof(struct entry));
                        newEntry->pos = nextPos;
                        newEntry->dist = nextDist;
                        newEntry->tool = nextTool;
                        newEntry->prev = NULL;
                        newEntry->next = NULL;
                        addEntry(queue, newEntry);
                }

                // Switch tools
                int nextDist = cur->dist + 7;
                enum tool nextTool = cur->tool;
                for (enum tool i = 0; i < 3; i++)
                        if (i != cur->tool && i != CantUse[reg.type])
                                nextTool = i;

                if (!reg.visited[nextTool] && nextDist < reg.dist[nextTool]) {
                        cave[cur->pos.y][cur->pos.x].dist[nextTool] = nextDist;
                        struct entry *newEntry = malloc(sizeof(struct entry));
                        newEntry->pos = cur->pos;
                        newEntry->dist = nextDist;
                        newEntry->tool = nextTool;
                        newEntry->prev = NULL;
                        newEntry->next = NULL;
                        addEntry(queue, newEntry);
                }

                free(cur);
        }
        freeQueue(queue);
}

void evalCave(ivec2 size, struct region cave[][size.x], int depth) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        struct region *r = &cave[y][x];
                        long index = 0;
                        if (r->target || (x == 0 && y == 0))
                                index = 0;
                        else if (x == 0)
                                index = y * 48271;
                        else if (y == 0)
                                index = x * 16807;
                        else
                                index = cave[y][x-1].erosion * cave[y-1][x].erosion;

                        r->erosion = (index + depth) % 20183;
                        if (r->erosion % 3 == 0)
                                r->type = ROCKY;
                        else if (r->erosion % 3 == 1)
                                r->type = WET;
                        else
                                r->type = NARROW;

                        // Initialize dijkstra dists
                        r->visited[0] = false;
                        r->visited[0] = false;
                        r->visited[0] = false;
                        if (x == 0 && y == 0) {
                                r->dist[0] = 0;
                                r->dist[1] = 0;
                                r->dist[2] = 0;
                        } else {
                                r->dist[0] = INT32_MAX;
                                r->dist[1] = INT32_MAX;
                                r->dist[2] = INT32_MAX;
                        }
                }
        }
}

void part1(struct input *input) {
        int depth = input->depth;
        ivec2 targetPos = input->targetPos;
        ivec2 size = {{targetPos.x+1, targetPos.y+1}};

        struct region cave[size.y][size.x];
        for (int y = 0; y < size.y; y++)
                for (int x = 0; x < size.x; x++)
                        cave[y][x].target = false;
        cave[targetPos.y][targetPos.x].target = true;

        evalCave(size, cave, depth);
        // printCave(size, cave);

        int riskLevel = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (cave[y][x].type == WET)
                                riskLevel += 1;
                        else if (cave[y][x].type == NARROW)
                                riskLevel += 2;
                }
        }

        printf("Part 1: cave Risk Level = %d\n\n", riskLevel);
}

void part2(struct input *input) {
        int depth = input->depth;
        ivec2 targetPos = input->targetPos;
        ivec2 size = {{(targetPos.x+1)*2, (targetPos.y+1)*2}};

        struct region cave[size.y][size.x];
        for (int y = 0; y < size.y; y++)
                for (int x = 0; x < size.x; x++)
                        cave[y][x].target = false;
        cave[targetPos.y][targetPos.x].target = true;

        evalCave(size, cave, depth);
        // printCave(size, cave);

        dijkstra(size, cave);
        struct region target = cave[targetPos.y][targetPos.x];
        // printf("Torch: %d, Gear: %d\n", target.dist[0], target.dist[1]);

        printf("Part 2: Reached Target in %d minutes\n", target.dist[TORCH]);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        strtok((char*)ll->head->data, " ");
        input.depth = strtol(strtok(NULL, ""), NULL, 10);

        strtok((char*)ll->head->next->data, " ");
        input.targetPos.x = strtol(strtok(NULL, ","), NULL, 10);
        input.targetPos.y = strtol(strtok(NULL, ""), NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day22.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

