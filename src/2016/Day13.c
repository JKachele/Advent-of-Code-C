/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 30, 2026 13:26:41 EDT
 *License-------GNU GPL-3.0
 ************************************************/

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

struct state {
        ivec2 pos;
        int moves;
        struct state *next;
};

struct input {
        int num;
};

static ivec2 Target = {{31, 39}};

int mDist(ivec2 a, ivec2 b) {
        return abs(b.x - a.x) + abs(b.y - a.y);
}

int getHammingWeight(u64 n) {
        int count = 0;
        while (n > 0) {
                n &= (n - 1);
                count++;
        }
        return count;
}

bool isOpen(ivec2 pos, int num) {
        u64 open = (pos.x * pos.x) + (3 * pos.x) + (2 * pos.x * pos.y) + pos.y + (pos.y * pos.y);
        open += num;
        int hammingWeight = getHammingWeight(open);
        return (hammingWeight % 2 == 0);
}

int posCmp(ivec2 a, ivec2 b) {
        if (a.x == b.x) {
                if (a.y == b.y) {
                        return 0;
                } else if (a.y > b.y) {
                        return 1;
                } else {
                        return -1;
                }
        } else if (a.x > b.x) {
                return 1;
        } else {
                return -1;
        }
}

bool checkVisited(talivec2 *visited, ivec2 pos) {
        int left = 0;
        int right = visited->length - 1;
        while (left <= right) {
                int mid = left + ((right - left) / 2);
                ivec2 cur = visited->array[mid];
                if (posCmp(cur, pos) == 0) {
                        return true;
                } else if (posCmp(cur, pos) == -1) {
                        left = mid + 1;
                } else {
                        right = mid - 1;
                }
        }
        tal_insert(*visited, left, pos);
        return false;
}

bool stateCmp(struct state a, struct state b) {
        if (a.moves > b.moves) {
                return false;
        } else if (a.moves == b.moves) {
                int aDist = mDist(a.pos, Target);
                int bDist = mDist(b.pos, Target);
                if (aDist > bDist) {
                        return false;
                }
        }
        return true;
}

void addPosToQueue(struct state **queue, talivec2 *visited, ivec2 pos, int moves) {
        if (checkVisited(visited, pos)) { return; }

        struct state *state = malloc(sizeof(struct state));
        state->pos = pos;
        state->moves = moves;
        state->next = NULL;

        if (*queue == NULL || !stateCmp(**queue, *state)) {
                *queue = state;
                return;
        }

        struct state *prev = *queue;
        struct state *cur = prev->next;
        while (cur != NULL) {
                if (!stateCmp(*cur, *state)) {
                        state->next = cur;
                        prev->next = state;
                        return;
                }
                prev = cur;
                cur = cur->next;
        }
        prev->next = state;
}

ivec2 bfs(ivec2 start, int num) {
        const ivec2 Dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        talivec2 visited = tal_init();
        tal_add(visited, start);

        struct state *queue = malloc(sizeof(struct state));
        queue->pos = start;
        queue->moves = 0;
        queue->next = NULL;

        int dist = -1;
        int count = 0;
        while (queue != NULL) {
                struct state *curPtr = queue;
                struct state cur = *curPtr;
                queue = curPtr->next;
                free(curPtr);


                if (dist == -1 && ivec2Eq(cur.pos, Target)) {
                        dist = cur.moves;
                }

                if (cur.moves <= 50) {
                        count++;
                } else if (dist != -1) {
                        break;
                }

                for (int i = 0; i < 4; i++) {
                        ivec2 next = ivec2Add(cur.pos, Dirs[i]);
                        if (next.x < 0 || next.y < 0 || !isOpen(next, num)) { continue; }
                        addPosToQueue(&queue, &visited, next, cur.moves + 1);
                }
        }

        tal_destroy(visited);
        while (queue != NULL) {
                struct state *cur = queue;
                queue = cur->next;
                free(cur);
        }
        return (ivec2){{dist, count}};
}

void part1_2(struct input *input) {
        int num = input->num;

        ivec2 pos = {{1, 1}};
        ivec2 distCount = bfs(pos, num);

        printf("Part 1: Distance = %d\n\n", distCount.a);
        printf("Part 2: Open within 50 = %d\n\n", distCount.b);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.num = strtol(str, NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day13.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Target = (ivec2){{7, 4}};
        } else {
                char *file = "assets/inputs/2016/Day13.txt";
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

