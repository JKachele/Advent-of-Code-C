/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 01, 2026 09:50:51 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
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
#include "../util/quicksort.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(int64) talint64;

struct bot {
        lvec3 pos;
        long range;
};
typedef tal(struct bot) talbot;

struct qbot {
        lvec4 pos;
        int64 range;
};

struct region {
        lvec2 range[4];
};

struct input {
        talbot bots;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBot(struct bot bot) {
        printf("(%ld, %ld, %ld) ", bot.pos.x, bot.pos.y, bot.pos.z);
        printf("%ld\n", bot.range);
}

void printBots(talbot bots) {
        tal_for(bots, i) {
                printBot(bots.array[i]);
        }
        printf("\n");
}

void printSplits(talint64 splits[4]) {
        for (int i = 0; i < 4; i++) {
                talint64 split = splits[i];
                printf("(%lu): ", split.length);
                tal_for(split, i) {
                        printf("[%ld] ", split.array[i]);
                }
                printf("\n");
        }
        printf("\n");
}

long getDist(lvec3 a, lvec3 b) {
        return labs(a.x - b.x) + labs(a.y - b.y) + labs(a.z - b.z);
}

int cmp(void *a, void *b) {
        if (*(int64*)a > *(int64*)b) {
                return 1;
        }
        if (*(int64*)a == *(int64*)b) {
                return 0;
        }
        return -1;
}

void sortSplits(talint64 splits[4]) {
        for (int i = 0; i < 4; i++) {
                quicksort(splits[i].array, sizeof(int64), 0, splits[i].length - 1, cmp);
        }
}

void dedupSplits(talint64 splits[4]) {
        for (int i = 0; i < 4; i++) {
                talint64 *split = &splits[i];
                tal_for(*split, j) {
                        if (j == 0) continue;
                        if (split->array[j] == split->array[j-1]) {
                                tal_remove(*split, j);
                                j--;
                        }
                }
        }
}

struct region getRegion(talint64 splits[4], lvec2 indexRanges[4]) {
        struct region r;
        for (int i = 0; i < 4; i++) {
                r.range[i] = (lvec2){{
                        splits[i].array[indexRanges[i].a],
                        splits[i].array[indexRanges[i].b] - 1
                }};
        }
        return r;
}

/* Find a solution for (N0 + N1 + N2 == N3) that minimizes the
 * distance to origin, i.e. highest absolute value of N0..N3
 *
 * Boundary values for all ranges must be uniformly even or odd.
 */
int64 solveDistance(struct region r) {
        // Put range[0..2] in absolute value order
        for (int i = 0; i < 3; i++) {
                if (labs(r.range[i].b) < labs(r.range[i].a)) {
                        int64 temp = r.range[i].a;
                        r.range[i].a = r.range[i].b;
                        r.range[i].b = temp;
                }
        }

        // Get initial value for the sum (N0 + N1 + N2)
        int64 sum = r.range[0].a + r.range[1].a + r.range[2].a;
        int64 delta = 0;
        if (sum < r.range[3].a) {
                delta = r.range[3].a - sum;  // Too low
        } else if (sum > r.range[3].b) {
                delta = r.range[3].b - sum; // Too high
                r.range[3].a = r.range[3].b;
        } else {
                r.range[3].a = sum;          // Just right
        }

        // Get initial distance to origin
        int64 dist = 0;
        for (int i = 0; i < 4; i++) {
                if (labs(r.range[i].a) > dist) {
                        dist = labs(r.range[i].a);
                }
        }

        // Adjust range[0..2] until delta is zero
        while (delta != 0) {
                int64_t candidates = 0;
                for (int i = 0; i < 3; i++) {
                        lvec2 ra = r.range[i];
                        // Adjust range[0..2] as far as possible without affecting
                        // distance to origin
                        int64 step = 0;
                        if (delta > 0) {
                                // Wrong direction. Negative values would go *more* negative
                                if (ra.b <= 0) continue;

                                // How much we can increase before
                                // increasing dist or going out of range
                                int64 slack = ((dist < ra.b) ? dist : ra.b) - ra.a;

                                // Take a step using wither all the slack or just enough for delta
                                step = (slack < delta) ? slack : delta;
                        } else {
                                // Same as above except using negative ranges
                                if (ra.b >= 0) continue;
                                int64 slack = ((-dist > ra.b) ? -dist : ra.b) - ra.a;
                                step = (slack > delta) ? slack : delta;
                        }
                        r.range[i].a += step;   // Move value by step
                        delta -= step;          // Shrink delta by step

                        // If value still has room to move, increase candidates
                        if (r.range[i].a != r.range[i].b) {
                                candidates++;
                        }
                }

                if (delta != 0) {
                        // If delta isn't zero and the values cannot move,
                        // there are no valid points in the region
                        if (candidates == 0) {
                                return INT64_MAX;
                        }

                        // Increase the distance goal by the minimum amount to get delta to 0
                        // Must be raised in even increments
                        // This splits the remaining delta by the number of candidates left
                        // The delta is divided by 2 in the begining then multiplied by 2
                        // to ensure change is even
                        // (the "candidates + 1" is to make a ceiling division)
                        dist += (((labs(delta) / 2) + (candidates + 1)) / candidates) * 2;
                }
        }

        return dist;
}

int64 distToOrigin(struct region region) {
        /* The coordinates of the closest point are
         * either all odd or all even.  Solve each case
         * separately, and choose the better outcome.
         */
        struct region ra0 = region;
        struct region ra1 = region;

        /* Adjust the ranges so that Ra0 is all even, and Ra1
         * is all odd.  Mark a case bad if this is not possible.
         */
        bool bad0 = false;
        bool bad1 = false;
        for (size_t i = 0; i < 4; i++) {
                lvec2 r = region.range[i];

                if (r.a & 1) {
                        bad0 |= (r.a == r.b);
                        ra0.range[i].a++;
                } else {
                        bad1 |= (r.a == r.b);
                        ra1.range[i].a++;
                }

                if (r.b & 1) {
                        ra0.range[i].b--;
                } else {
                        ra1.range[i].b--;
                }
        }

        int64 bestDist = INT64_MAX;
        if (!bad0) {
                int64 dist = solveDistance(ra0);
                if (dist < bestDist) bestDist = dist;
        }
        if (!bad1) {
                int64 dist = solveDistance(ra1);
                if (dist < bestDist) bestDist = dist;
        }

        return bestDist;
}

int64 distToMaxIntersection(int numBots, struct qbot qbots[numBots], talint64 splits[4],
                int axis, int64 validBots, u64 bit, u64 nextBit,
                lvec2 indexRanges[4], int64 mask[numBots]) {
        // Static Vars
        static int64 bestIntersections = 0;
        static int64 bestDist = INT64_MAX;

        // Recursive function
        bool branch = false;
        for (int i = 0; i < 4; i++) {
                branch = (indexRanges[axis].a + 1 < indexRanges[axis].b);
                if (branch) {
                        break;
                }
                axis = (axis + 1) % 4;
        }

        // Leaf Node
        if (!branch) {
                // Find distance to origin (INT64_MAX means
                // there are no valid points in the region)
                struct region r = getRegion(splits, indexRanges);
                int64 dist = distToOrigin(r);
                if (dist != INT64_MAX) {
                        if (bestIntersections < validBots) {
                                bestIntersections = validBots;
                                bestDist = dist;
                        } else if (dist < bestDist){
                                bestDist = dist;
                        }
                }
                return bestDist;
        }

        // Split region
        lvec2 range = indexRanges[axis];
        int64 mid = range.a + (range.b - range.a) / 2;
        int64 threshold = splits[axis].array[mid];

        // Find qbots that intersect with each side of the axis
        int validBotsHigh = 0;
        int validBotsLow = 0;
        for (int i = 0; i < numBots; i++) {
                struct qbot q = qbots[i];
                int64 *m = &mask[i];

                bool isValid = *m & bit;
                *m &= ~(bit | nextBit);
                if (isValid) {
                        // Does it intersect the low side?
                        if (q.pos.raw[axis] - q.range < threshold) {
                                *m |= bit;
                                validBotsLow++;
                        }
                        // Does it intersect the high side?
                        if (q.pos.raw[axis] + q.range >= threshold) {
                                *m |= nextBit;
                                validBotsHigh++;
                        }
                }
        }

        // Search the side with more bots first
        if (validBotsLow > validBotsHigh) {
                // Low first
                // If num intersections is lower than the best so far, can ignore branch
                if (validBotsLow >= bestIntersections) {
                        indexRanges[axis].b = mid;
                        distToMaxIntersection(numBots, qbots, splits, (axis+1)%4,
                                        validBotsLow, bit, nextBit<<1, indexRanges, mask);
                        indexRanges[axis].b = range.b;
                }
                if (validBotsHigh >= bestIntersections) {
                        indexRanges[axis].a = mid;
                        distToMaxIntersection(numBots, qbots, splits, (axis+1)%4,
                                        validBotsHigh, nextBit, nextBit<<1, indexRanges, mask);
                        indexRanges[axis].a = range.a;
                }
        } else {
                if (validBotsHigh >= bestIntersections) {
                        indexRanges[axis].a = mid;
                        distToMaxIntersection(numBots, qbots, splits, (axis+1)%4,
                                        validBotsHigh, nextBit, nextBit<<1, indexRanges, mask);
                        indexRanges[axis].a = range.a;
                }
                if (validBotsLow >= bestIntersections) {
                        indexRanges[axis].b = mid;
                        distToMaxIntersection(numBots, qbots, splits, (axis+1)%4,
                                        validBotsLow, bit, nextBit<<1, indexRanges, mask);
                        indexRanges[axis].b = range.b;
                }
        }

        return bestDist;
}

void part1(struct input *input) {
        talbot bots = input->bots;
        // printBots(bots);

        struct bot strongest = {0};
        tal_for(bots, i) {
                struct bot bot = bots.array[i];
                if (bot.range > strongest.range) {
                        strongest = bot;
                }
        }

        int numInRange = 0;
        tal_for(bots, i) {
                struct bot bot = bots.array[i];
                if (getDist(bot.pos, strongest.pos) <= strongest.range) {
                        numInRange++;
                }
        }

        printf("Part 1: Bots in range: %d\n\n", numInRange);
}

// Used answer by u/askalski
// https://www.reddit.com/r/adventofcode/comments/a9co1u/comment/ecmpxad/
void part2(struct input *input) {
        talbot bots = input->bots;
        // printBots(bots);

        // Transform the 3d coordinates into four coordinates (-x+y+z, x-y+z, x+y-z, x+y+z)
        // These describes the point as the intersection of 4 planes which can be used as a 4D AABB
        // the planes "range" away from the 4 planes are the octahedron which the bot can see
        // This turns this problem into an AABB intersection problem
        int numBots = bots.length;
        struct qbot qbots[numBots];
        tal_for(bots, i) {
                struct bot b = bots.array[i];
                qbots[i].pos.x = (-b.pos.x + b.pos.y + b.pos.z);
                qbots[i].pos.y = ( b.pos.x - b.pos.y + b.pos.z);
                qbots[i].pos.z = ( b.pos.x + b.pos.y - b.pos.z);
                qbots[i].pos.w = ( b.pos.x + b.pos.y + b.pos.z);
                qbots[i].range = b.range;
        }

        talint64 splits[4] = {0};
        for (int i = 0; i < 4; i++) { tal_add(splits[i], 0); }
        for (int i = 0; i < numBots; i++) {
                for (int j = 0; j < 4; j++) {
                        tal_add(splits[j], qbots[i].pos.raw[j] - qbots[i].range);
                        tal_add(splits[j], qbots[i].pos.raw[j] + qbots[i].range + 1);
                }
        }

        sortSplits(splits);
        dedupSplits(splits);

        lvec2 indexRanges[4];
        for (int i = 0; i < 4; i++) { indexRanges[i] = (lvec2){{0, splits[i].length - 1}}; }

        int64 mask[numBots];
        for (int i = 0; i < numBots; i++) { mask[i] = 1; }

        int64 bestDist = distToMaxIntersection(numBots, qbots, splits,
                        0, numBots, 1, 2, indexRanges, mask);

        printf("Part 2: Distance to best spot = %ld\n", bestDist);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct bot bot;

                strtok(str, "<");
                for (int i = 0; i < 3; i++) {
                        bot.pos.raw[i] = strtol(strtok(NULL, ",>"), NULL, 10);
                }
                strtok(NULL, "=");
                bot.range = strtol(strtok(NULL, ""), NULL, 10);

                tal_add(input.bots, bot);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day23.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day23.txt";
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

