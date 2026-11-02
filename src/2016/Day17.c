/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Nov 02, 2026 11:21:11 EST
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
#include "../util/md5.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(char*) talstr;

struct state {
        char *passSeq;
        ivec2 pos;
        int dist;
        int moves;
        struct state *next;
};

struct input {
        char *passwd;
};

int mDist(ivec2 a, ivec2 b) {
        return abs(b.x - a.x) + abs(b.y - a.y);
}

bool validPos(ivec2 pos) {
        return (pos.x >= 0) && (pos.y >= 0) && (pos.x < 4) && (pos.y < 4);
}

bool placeBefore(struct state *cur, struct state *next) {
        int curValue = cur->moves + cur->dist;
        int nextValue = next->moves + next->dist;
        return (nextValue < curValue);
}

void addToQueue(struct state **queue, struct state *state) {
        if (*queue == NULL || placeBefore(*queue, state)) {
                state->next = *queue;
                *queue = state;
                return;
        }

        struct state *prev = *queue;
        struct state *cur = prev->next;
        while (cur != NULL) {
                if (placeBefore(cur, state)) {
                        state->next = cur;
                        prev->next = state;
                        return;
                }
                prev = cur;
                cur = cur->next;
        }
        prev->next = state;
}

// A* Algorithm
char *findPath(char *passwd, bool longest) {
        const ivec2 Dirs[4] = {{{0, -1}}, {{0, 1}}, {{-1, 0}}, {{1, 0}}};
        const char DirChars[4] = {'U', 'D', 'L', 'R'};
        const ivec2 Initial = {{0, 0}};
        const ivec2 Target = {{3, 3}};

        struct state *queue = malloc(sizeof(struct state));
        queue->passSeq = malloc(strlen(passwd + 1));
        strcpy(queue->passSeq, passwd);
        queue->pos = Initial;
        queue->dist = mDist(Initial, Target);
        queue->moves = 0;
        queue->next = NULL;

        talstr paths = tal_init();
        int itterations = 0;
        while (queue != NULL) {
                struct state *cur = queue;
                queue = queue->next;

                if (ivec2Eq(cur->pos, Target)) {
                        char *path = malloc(cur->moves + 1);
                        strcpy(path, cur->passSeq + strlen(passwd));
                        tal_add(paths, path);
                        free(cur->passSeq);
                        free(cur);
                        if (longest) {
                                continue;
                        } else {
                                break;
                        }
                }

                u8 result[16];
                md5(cur->passSeq, result);
                char doors[5];
                sprintf(doors, "%02x%02x", result[0], result[1]);

                for (int i = 0; i < 4; i++) {
                        // Check of door is open
                        if (doors[i] <= 'a') { continue; }

                        // Check of pos is valid
                        ivec2 nextPos = ivec2Add(cur->pos, Dirs[i]);
                        if (!validPos(nextPos)) { continue; }

                        // Create new state
                        struct state *next = malloc(sizeof(struct state));
                        next->passSeq = malloc(strlen(cur->passSeq) + 2);
                        sprintf(next->passSeq, "%s%c", cur->passSeq, DirChars[i]);
                        next->pos = nextPos;
                        next->dist = mDist(nextPos, Target);
                        next->moves = cur->moves + 1;
                        next->next = NULL;
                        addToQueue(&queue, next);
                }
                free(cur->passSeq);
                free(cur);
        }

        // Free queue
        while (queue != NULL) {
                struct state *cur = queue;
                queue = queue->next;
                free(cur);
        }

        char *result;
        if (longest) {
                int maxLen = 0;
                int maxPath = 0;
                tal_for(paths, i) {
                        int len = strlen(paths.array[i]);
                        if (len > maxLen) {
                                maxLen = len;
                                maxPath = i;
                        }
                }
                result = malloc(strlen(paths.array[maxPath]) + 1);
                strcpy(result, paths.array[maxPath]);
        } else {
                result = malloc(strlen(paths.array[0]) + 1);
                strcpy(result, paths.array[0]);
        }

        tal_for(paths, i) {
                free(paths.array[i]);
        }
        tal_destroy(paths);

        return result;
}

void part1(struct input *input) {
        char *passwd = input->passwd;

        char *moves = findPath(passwd, false);
        if (moves == NULL) {
                moves = "No Path Found";
        }

        printf("Part 1: Moves = %s\n\n", moves);
}

void part2(struct input *input) {
        char *passwd = input->passwd;

        char *moves = findPath(passwd, true);

        int numMoves = 0;
        if (moves != NULL) {
                numMoves = strlen(moves);
        }

        printf("Part 2: Longest Path = %d\n", numMoves);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.passwd = malloc(strlen(str) + 1);
        strcpy(input.passwd, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day17.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day17.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day17.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

