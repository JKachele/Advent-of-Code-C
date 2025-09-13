/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 04, 2025 14:59:55 UTC
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
#define NUM_STEPS 64
#define NUM_STEPS2 26501365
#define MAX_STEPS 1000

typedef tll(int32) tllint32;

typedef struct plot {
        bool open;
        bool steps[MAX_STEPS+1];
} plot;

typedef struct plot2 {
        bool open;
        bool seen;
} plot2;

typedef struct step {
        ivec2 pos;
        int32 num;
} step;

typedef tll(step) tllstep;
typedef tll(ivec2) tllivec2;

struct input {
        ivec2 size;
        plot **field;
        ivec2 start;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printField(ivec2 size, plot field[][size.x], int num) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (field[y][x].steps[num])
                                printf("O");
                        else if (field[y][x].open)
                                printf(".");
                        else
                                printf("#");
                }
                printf("\n");
        }
        printf("\n");
}

void resetField(ivec2 size, plot field[][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        memset(field[y][x].steps, 0, (NUM_STEPS+1)*sizeof(bool));
                }
        }
}

bool validPos(ivec2 size, ivec2 pos) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

int64 walkField(ivec2 size, plot field[][size.x], ivec2 start, int64 numSteps) {
        const ivec2 dirs[4] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        tllstep queue = tll_init();
        step initStep = {start, 0};
        tll_push_back(queue, initStep);

        while (tll_length(queue) > 0) {
                step curS = tll_pop_front(queue);
                if (curS.num == numSteps)
                        continue;

                for (int i=0; i<4; i++) {
                        ivec2 newPos = ivec2Add(curS.pos, dirs[i]);
                        if (!validPos(size, newPos)) continue;
                        plot *newPlot = &field[newPos.y][newPos.x];
                        if (newPlot->open && !newPlot->steps[curS.num+1]) {
                                newPlot->steps[curS.num+1] = true;
                                step newStep = {newPos, curS.num+1};
                                tll_push_back(queue, newStep);
                        }
                }
        }

        int64 numReached = 0;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (field[y][x].steps[NUM_STEPS])
                                numReached++;
                }
        }

        return numReached;
}

void addPos(tllivec2 *steps, ivec2 pos) {
        tll_foreach(*steps, it) {
                if (ivec2Eq(it->item, pos))
                        return;
        }
        tll_push_back(*steps, pos);
}

uint32 getSeenIndex(ivec2 pos) {
        int16 x = (int16)pos.x;
        int16 y = (int16)pos.y;
        uint16 xu = *((uint16*)&x);
        uint16 yu = *((uint16*)&y);
        uint32 index = (uint32)yu << 16 | xu;
        return index;
}

ivec2 getAdjPos(ivec2 size, ivec2 pos) {
        pos.x = ((pos.x % size.x) + size.x) % size.x;
        pos.y = ((pos.y % size.y) + size.y) % size.y;
        return pos;
}

tllivec2 stepField(ivec2 size, plot field[][size.x], ivec2 start, tllivec2 steps) {
        const ivec2 dirs[4] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};

        bool *seen = calloc(UINT32_MAX, sizeof(bool));
        tllivec2 nextSteps = tll_init();
        tll_foreach(steps, it) {
                ivec2 pos = it->item;

                for (int i=0; i<4; i++) {
                        ivec2 newPos = ivec2Add(pos, dirs[i]);
                        ivec2 adjPos = getAdjPos(size, newPos);
                        uint32 seenIndex = getSeenIndex(newPos);
                        plot p = field[adjPos.y][adjPos.x];
                        if (p.open && !seen[seenIndex]) {
                                seen[seenIndex] = true;
                                tll_push_back(nextSteps, newPos);
                        }
                }
        }
        free(seen);

        return nextSteps;
}

void walkField2(ivec2 size, plot field[][size.x], ivec2 start,
                int64 limits[3], int64 numPLots[3]) {
        tllivec2 steps = tll_init();
        tll_push_back(steps, start);

        for (int i=0; i<limits[2]; i++) {
                if (i == limits[0])
                        numPLots[0] = tll_length(steps);
                else if (i == limits[1])
                        numPLots[1] = tll_length(steps);
                steps = stepField(size, field, start, steps);
        }
        numPLots[2] = tll_length(steps);
}

int64 quadInterp(int64 points[3], int64 goal, int64 len) {
        int64 n = goal / len;

        int64 a0 = points[0];
        int64 a1 = points[1];
        int64 a2 = points[2];

        int64 b0 = a0;
        int64 b1 = a1 - a0;
        int64 b2 = a2 - a1;

        return b0 + b1 * n + (n * (n - 1) / 2) * (b2 - b1);
}

void part1(struct input input) {
        ivec2 size = input.size;
        plot (*field)[size.x] = (plot(*)[size.x])input.field;
        resetField(size, field);
        ivec2 start = input.start;

        int64 numPlotsReached = walkField(size, field, start, NUM_STEPS);
        // printField(size, field, NUM_STEPS);

        printf("Part 1: %ld\n\n", numPlotsReached);
}

/* Using the fact the input has an empty row and column at the start
 * And that the start is in the exact center of the input, we can
 * deduce that the number of spaces you can reach after a number of
 * steps is quadratic. Find 3 points and use quadratic interpolation
 *
 * f(steps) = spaces reaches after setps
 * length = length of input grid
 * toEdge = steps to reach edge of grid
 *
 * f(toEdge + 0*length), f(toEdge + 1*length), f(toEdge + 2*length)
 */
void part2(struct input input) {
        ivec2 size = input.size;
        plot (*field)[size.x] = (plot(*)[size.x])input.field;
        resetField(size, field);
        ivec2 start = input.start;

        int32 length;
        if (size.x == size.y)
                length = size.x;
        else
                printf("SIZE NOT EQUAL!\n");
        int32 toEdge = length / 2;

        // Get 3 points for quadratic interpolation
        int64 points[3] = {0};
        int64 steps[3] = {toEdge, toEdge + length, toEdge + 2 * length};
        walkField2(size, field, start, steps, points);

        int64 numPlotsReached = quadInterp(points, NUM_STEPS2, length);

        printf("Part 2: %ld\n\n", numPlotsReached);
}

struct input parseInput(llist *ll) {
        ivec2 size = {{getLongestLine(ll), ll->length}};
        plot **fieldPtr = calloc(size.y*size.x, sizeof(plot));
        plot (*field)[size.x] = (plot(*)[size.x])fieldPtr;
        ivec2 start = {0};

        llNode *current = ll->head;
        int32 yIndex = 0;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                for (int x = 0; x < size.x; x++) {
                        if (str[x] == '.') {
                                field[yIndex][x].open = true;
                        } else if (str[x] == 'S') {
                                field[yIndex][x].open = true;
                                start = (ivec2){{x, yIndex}};
                        }
                }

                current = current->next;
                yIndex++;
        }
        printField(size, field, 0);
        struct input input = {size, (plot**)field, start};

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day21.txt";
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

