/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 02, 2026 19:30:33 UTC
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

#define INPUT_BUFFER_SIZE 4096
#define NUM_MOONS 4
#define NUM_STEPS 1000

typedef struct state {
        int64 steps;
        int64 id;
} state;
typedef tal(state) talstate;

typedef struct moon {
        ivec3 pos;
        ivec3 vel;
} moon;

struct input {
        moon moons[NUM_MOONS];
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMoons(moon moons[NUM_MOONS]) {
        for (int i = 0; i < NUM_MOONS; i++) {
                moon m = moons[i];
                printf("Pos: (%d, %d, %d) ", m.pos.x, m.pos.y, m.pos.z);
                printf("Vel: (%d, %d, %d)\n", m.vel.x, m.vel.y, m.vel.z);
        }
        printf("\n");
}

void updateVel(moon *m1, moon *m2) {
        for (int i = 0; i < 3; i++) {
                int posDiff = m1->pos.raw[i] - m2->pos.raw[i];
                if (posDiff < 0) {
                        m1->vel.raw[i]++;
                        m2->vel.raw[i]--;
                } else if (posDiff > 0) {
                        m1->vel.raw[i]--;
                        m2->vel.raw[i]++;
                }
        }
}

void updateMoons(moon moons[NUM_MOONS]) {
        // Update velocities of each pair of moons
        for (int i = 0; i < NUM_MOONS - 1; i++) {
                for (int j = i + 1; j < NUM_MOONS; j++) {
                        updateVel(&moons[i], &moons[j]);
                }
        }

        // Update positions based on velocities
        for (int i = 0; i < NUM_MOONS; i++) {
                moons[i].pos = ivec3Add(moons[i].pos, moons[i].vel);
        }
}

int64 statetoid(moon *moons, int axis) {
        // 16 bits per moon. 8 for position, 8 for velocity
        int64 id = 0;
        for (int i = 0; i < NUM_MOONS; i++) {
                id <<= 8;
                id += moons[i].pos.raw[axis];
                id <<= 8;
                id += moons[i].vel.raw[axis];
        }
        return id;
}

int checkState(talstate *states, int start, int end, int64 id, int step) {
        if (start > end) {
                state s = {step, id};
                tal_insert(*states, start, s);
                return -1;
        }

        int pivot = (start + end) / 2;
        int64 pivotId = states->array[pivot].id;
        if (pivotId == id)
                return states->array[pivot].steps;
        else if (pivotId < id)
                return checkState(states, pivot + 1, end, id, step);
        else // if (pivotId > id)
                return checkState(states, start, pivot - 1, id, step);
}

void part1(struct input *input) {
        moon moons[NUM_MOONS];
        memcpy(moons, input->moons, sizeof(moon) * NUM_MOONS);

        for (int step = 0; step < NUM_STEPS; step++) {
                updateMoons(moons);
        }
        // printMoons(moons);

        int64 totalEnergy = 0;
        for (int i = 0; i < NUM_MOONS; i++) {
                int kin = 0;
                int pot = 0;
                for (int j = 0; j < 3; j++) {
                        pot += abs(moons[i].pos.raw[j]);
                        kin += abs(moons[i].vel.raw[j]);
                }
                totalEnergy += (int64)kin * pot;
        }

        printf("Part 1: %ld\n\n", totalEnergy);
}

void part2(struct input *input) {
        moon moons[NUM_MOONS];
        memcpy(moons, input->moons, sizeof(moon) * NUM_MOONS);

        talstate states[3] = {0};
        int64 loopLen[3] = {0};
        bool allFound = false;
        int steps = 0;
        while (!allFound) {
                if (steps % 1000 == 0)
                        printf(".");
                allFound = true;
                for (int i = 0; i < 3; i++) {
                        if (loopLen[i] != 0)
                                continue;
                        int64 id = statetoid(moons, i);
                        int loop = checkState(&states[i], 0, states[i].length - 1, id, steps);
                        if (loop != -1) {
                                loopLen[i] = steps - loop;
                        } else {
                                allFound = false;
                        }
                }
                updateMoons(moons);
                steps++;
        }
        // printf("%ld, %ld, %ld\n", loopLen[0], loopLen[1], loopLen[2]);
        int64 loopLength = lcm(lcm(loopLen[0], loopLen[1]), loopLen[2]);

        printf("\nPart 2: %ld\n", loopLength);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                char *tok = strtok(str + 1, ", >");
                for (int j = 0; j < 3; j++) {
                        input.moons[i].pos.raw[j] = strtol(tok + 2, NULL, 10);
                        input.moons[i].vel.raw[j] = 0;
                        tok = strtok(NULL, ", >");
                }

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
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

