/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 04, 2026 13:04:06 EST
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
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(ivec2) talivec2;

struct target {
        u8 id;
        ivec2 pos;
        int dist;
};
typedef tal(struct target) taltarget;

struct queue {
        ivec2 pos;
        int dist;
        struct queue *next;
};

struct node {
        bool open;
        bool target;
        u8 targetId;
        bool visited;
};

struct input {
        ivec2 size;
        struct node **map;
        ivec2 pos;
        talivec2 targets;
};

void printDucts(ivec2 size, struct node map[][size.x], ivec2 pos) {
        char output[size.y][size.x];
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (map[y][x].open) {
                                if (map[y][x].target) {
                                        output[y][x] = map[y][x].targetId + '0';
                                } else {
                                        output[y][x] = '.';
                                }
                        } else {
                                output[y][x] = '#';
                        }
                }
        }
        output[pos.y][pos.x] = '*';
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        printf("%c", output[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

taltarget distToTargets(ivec2 size, struct node map[][size.x], ivec2 pos) {
        const ivec2 Dirs[] = {{{1, 0}}, {{0, 1}}, {{-1 ,0}}, {{0, -1}}};
        taltarget targets = tal_init();

        // Reset all visited flags
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        map[y][x].visited = false;
                }
        }

        struct queue *queue = malloc(sizeof(struct queue));
        struct queue *tail = queue;
        queue->pos = pos;
        queue->dist = 0;
        queue->next = NULL;

        while (queue != NULL) {
                struct queue *curPtr = queue;
                struct queue cur = *curPtr;
                struct node *curNode = &map[cur.pos.y][cur.pos.x];
                queue = queue->next;
                free(curPtr);

                if (curNode->visited) { continue; }
                curNode->visited = true;

                if (curNode->target) {
                        struct target target = {curNode->targetId, cur.pos, cur.dist};
                        tal_add(targets, target);
                }

                for (int i = 0; i < 4; i++) {
                        ivec2 nextPos = ivec2Add(cur.pos, Dirs[i]);
                        if (!curNode->open) { continue; }

                        struct queue *next = malloc(sizeof(struct queue));
                        next->pos = nextPos;
                        next->dist = cur.dist + 1;
                        next->next = NULL;
                        if (queue == NULL) {
                                queue = next;
                                tail = next;
                        } else {
                                tail->next = next;
                                tail = next;
                        }
                }
        }

        while (queue != NULL) {
                struct queue *curPtr = queue;
                queue = queue->next;
                free(curPtr);
        }

        return targets;
}

int dfs(int num, int dists[][num], bool visited[], int last, int count, bool returns) {
        int minCost = INT32_MAX;

        // If all visited, return 0. No need to return to start
        if (count == num) {
                if (returns) {
                        return dists[last][0];
                } else {
                        return 0;
                }
        }

        for (int i = 1; i < num; i++) {
                if (!visited[i]) {
                        // Mark target as visited and check all possible paths from there
                        visited[i] = true;
                        int dist = dists[last][i] +
                                dfs(num, dists, visited, i, count + 1, returns);
                        if (dist < minCost) {
                                minCost = dist;
                        }

                        // Backtrack
                        visited[i] = false;
                }
        }

        return minCost;
}

void part1_2(struct input *input) {
        ivec2 size = input->size;
        struct node (*map)[size.x] = (struct node(*)[size.x])input->map;
        ivec2 pos = input->pos;
        talivec2 targets = input->targets;
        // printDucts(size, map, pos);

        // Travelling Salesman Problem
        int numTargets = targets.length;
        int dists[numTargets][numTargets];

        // Get distances from each target to other targets;
        tal_for(targets, i) {
                ivec2 tPos = targets.array[i];
                int tId = map[tPos.y][tPos.x].targetId;

                taltarget newTargets = distToTargets(size, map, tPos);
                tal_for(newTargets, j) {
                        struct target new = newTargets.array[j];
                        dists[tId][new.id] = new.dist;
                        dists[new.id][tId] = new.dist;
                }
        }

        bool visited[numTargets];
        for (int i = 1; i < numTargets; i++) { visited[i] = false; }
        visited[0] = true;

        // No return
        int shortestPath = dfs(numTargets, dists, visited, 0, 1, false);
        printf("Part 1: Shortest Path = %d\n\n", shortestPath);

        // Return to start
        shortestPath = dfs(numTargets, dists, visited, 0, 1, true);
        printf("Part 2: Shortest Path With Return = %d\n\n", shortestPath);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.map = calloc(size.x * size.y, sizeof(struct node));
        struct node (*map)[size.x] = (struct node(*)[size.x])input.map;

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < (int)strlen(str); x++) {
                        if (str[x] == '#') {
                                map[y][x].open = false;
                        } else if (str[x] == '.') {
                                map[y][x].open = true;
                        } else if (str[x] == '0') {
                                map[y][x].open = true;
                                map[y][x].target = true;
                                map[y][x].targetId = 0;
                                ivec2 t = {{x, y}};
                                tal_add(input.targets, t);
                                input.pos.x = x;
                                input.pos.y = y;
                        } else {
                                map[y][x].open = true;
                                map[y][x].target = true;
                                map[y][x].targetId = str[x] - '0';
                                ivec2 t = {{x, y}};
                                tal_add(input.targets, t);
                        }
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day24.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
                        parseTime, pt1Time, totalTime);

        return 0;
}

