/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Oct 06, 2025 17:04:16 UTC
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

typedef struct cuberange {
        ivec3 lower;
        ivec3 upper;
} cuberange;
typedef tll(cuberange) tllcuberange;

typedef struct cube {
        cuberange range;
        tllcuberange negative;
} cube;
typedef tll(cube) tllcube;

typedef struct step {
        bool on;
        ivec3 lower;
        ivec3 upper;
} step;
typedef tll(step) tllstep;

struct input {
        tllstep steps;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printStep(step s) {
        printf("%s ", s.on ? "On" : "Off");
        printf("(%d, %d, %d) -> ", s.lower.x, s.lower.y, s.lower.z);
        printf("(%d, %d, %d)\n", s.upper.x, s.upper.y, s.upper.z);
}

void printSteps(tllstep steps) {
        tll_foreach(steps, it) {
                printStep(it->item);
        }
        printf("\n");
}

void printCube(cube c) {
        cuberange r = c.range;
        printf("(%d, %d, %d) -> ", r.lower.x, r.lower.y, r.lower.z);
        printf("(%d, %d, %d)\n", r.upper.x, r.upper.y, r.upper.z);

        tll_foreach(c.negative, it) {
                cuberange rn = it->item;
                printf("  --");
                printf("(%d, %d, %d) -> ", rn.lower.x, rn.lower.y, rn.lower.z);
                printf("(%d, %d, %d)\n", rn.upper.x, rn.upper.y, rn.upper.z);
        }
}

void printCubes(tllcube cubes) {
        tll_foreach(cubes, it) {
                printCube(it->item);
        }
}

int32 min(int a, int b) {
        return a < b ? a : b;
}

int32 max(int a, int b) {
        return a > b ? a : b;
}

bool validPos(int size, int x, int y, int z) {
        return x >= 0 && x < size && y >= 0 && y < size && z >= 0 && z < size;
}

void fillCube(int size, bool region[][size][size], step s) {
        ivec3 lower = ivec3Add(s.lower, (ivec3){{50, 50, 50}});
        ivec3 upper = ivec3Add(s.upper, (ivec3){{50, 50, 50}});
        for (int z=lower.z; z<=upper.z; z++) {
                for (int y=lower.y; y<=upper.y; y++) {
                        for (int x=lower.x; x<=upper.x; x++) {
                                if (!validPos(size, x, y, z))
                                        continue;
                                region[z][y][x] = s.on;
                        }
                }
        }
}

void fillRegion(int size, bool region[][size][size], tllstep steps) {
        // Zero out region
        for (int z=0; z<size; z++)
                for (int y=0; y<size; y++)
                        for (int x=0; x<size; x++)
                                region[z][y][x] = false;

        tll_foreach(steps, it) {
                // Ignore steps entirely outside region
                ivec3 lower = it->item.lower;
                ivec3 upper = it->item.upper;
                int bound = (size - 1) / 2;
                if (lower.x >= bound || lower.y >= bound || lower.z >= bound)
                        continue;
                if (upper.x <= bound*-1 || upper.y <= bound*-1 ||
                                upper.z <= bound*-1)
                        continue;

                fillCube(size, region, it->item);
        }
}

int64 countRegion(int size, bool region[][size][size]) {
        int64 count = 0;
        for (int z=0; z<size; z++) {
                for (int y=0; y<size; y++) {
                        for (int x=0; x<size; x++) {
                                if (region[z][y][x])
                                        count++;
                        }
                }
        }
        return count;
}

bool cubeIntersects(cuberange a, cuberange b) {
        return  a.lower.x <= b.upper.x && a.upper.x >= b.lower.x &&
                a.lower.y <= b.upper.y && a.upper.y >= b.lower.y &&
                a.lower.z <= b.upper.z && a.upper.z >= b.lower.z;
}

void addCube(tllcube *cubes, step s) {
        // Subtract new cube from existing cubes
        cuberange newCubeR = {s.lower, s.upper};

        tll_foreach(*cubes, it) {
                if (cubeIntersects(it->item.range, newCubeR))
                        tll_push_back(it->item.negative, newCubeR);
        }

        // Add new cube if step is on
        if (s.on) {
                cube newCube = {0};
                newCube.range = newCubeR;
                tll_push_back(*cubes, newCube);
        }
}

void splitCube(tllcuberange *ranges, cuberange cubeR, cuberange neg) {
        if (neg.lower.x > cubeR.lower.x) {
                cuberange new = cubeR;
                new.upper.x = neg.lower.x - 1;
                tll_push_back(*ranges, new);
        }
        if (neg.upper.x < cubeR.upper.x) {
                cuberange new = cubeR;
                new.lower.x = neg.upper.x + 1;
                tll_push_back(*ranges, new);
        }
        if (neg.lower.y > cubeR.lower.y) {
                cuberange new = cubeR;
                new.lower.x = max(cubeR.lower.x, neg.lower.x);
                new.upper.x = min(cubeR.upper.x, neg.upper.x);
                new.upper.y = neg.lower.y - 1;
                tll_push_back(*ranges, new);
        }
        if (neg.upper.y < cubeR.upper.y) {
                cuberange new = cubeR;
                new.lower.x = max(cubeR.lower.x, neg.lower.x);
                new.upper.x = min(cubeR.upper.x, neg.upper.x);
                new.lower.y = neg.upper.y + 1;
                tll_push_back(*ranges, new);
        }
        if (neg.lower.z > cubeR.lower.z) {
                cuberange new = cubeR;
                new.lower.x = max(cubeR.lower.x, neg.lower.x);
                new.upper.x = min(cubeR.upper.x, neg.upper.x);
                new.lower.y = max(cubeR.lower.y, neg.lower.y);
                new.upper.y = min(cubeR.upper.y, neg.upper.y);
                new.upper.z = neg.lower.z - 1;
                tll_push_back(*ranges, new);
        }
        if (neg.upper.z < cubeR.upper.z) {
                cuberange new = cubeR;
                new.lower.x = max(cubeR.lower.x, neg.lower.x);
                new.upper.x = min(cubeR.upper.x, neg.upper.x);
                new.lower.y = max(cubeR.lower.y, neg.lower.y);
                new.upper.y = min(cubeR.upper.y, neg.upper.y);
                new.lower.z = neg.upper.z + 1;
                tll_push_back(*ranges, new);
        }
}

void processCube(tllcuberange *ranges, cube c) {
        // Create range for this cube and add cube to it
        tllcuberange newRanges = tll_init();
        tll_push_back(newRanges, c.range);

        // Use the negatives to split cube into smaller cubes
        tll_foreach(c.negative, it) {
                cuberange neg = it->item;
                tll_foreach(newRanges, it2) {
                        cuberange cubeR = it2->item;
                        if (!cubeIntersects(cubeR, neg))
                                continue;
                        tll_remove(newRanges, it2);
                        splitCube(&newRanges, cubeR, neg);
                }
        }

        // Add new cubes to the main list
        tll_foreach(newRanges, it) {
                tll_push_back(*ranges, it->item);
        }
}

int64 cubeVolume(cuberange r) {
        lvec3 size;
        size.x = (r.upper.x - r.lower.x) + 1;
        size.y = (r.upper.y - r.lower.y) + 1;
        size.z = (r.upper.z - r.lower.z) + 1;

        return size.x * size.y * size.z;
}

void part1(struct input input) {
        const int REGION_SIZE = 50;
        const int SIZE = REGION_SIZE * 2 + 1;
        tllstep steps = input.steps;

        // Brute force for now
        bool region[SIZE][SIZE][SIZE];
        fillRegion(SIZE, region, steps);

        int64 numOn = countRegion(SIZE, region);

        printf("Part 1: %ld\n\n", numOn);
}

void part2(struct input input) {
        tllstep steps = input.steps;

        tllcube cubes = tll_init();

        tll_foreach(steps, it) {
                addCube(&cubes, it->item);
        }
        // printCubes(cubes);

        // Split cubes by removing the negatives
        tllcuberange finalCubes = tll_init();
        tll_foreach(cubes, it) {
                processCube(&finalCubes, it->item);
        }

        // Get volume of split cubes
        int64 totalVolume = 0;
        tll_foreach(finalCubes, it) {
                totalVolume += cubeVolume(it->item);
        }

        printf("Part 2: %ld\n", totalVolume);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;

                step s = {0};
                char *token = strtok(str, " ");
                // "on" or "off"
                if (token[1] == 'n')
                        s.on = true;

                for (int i=0; i<3; i++) {
                        token = strtok(NULL, "=.,");
                        token = strtok(NULL, "=.,");
                        s.lower.raw[i] = strtol(token, NULL, 10);
                        token = strtok(NULL, "=.,");
                        s.upper.raw[i] = strtol(token, NULL, 10);
                }

                tll_push_back(input.steps, s);

                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day22.txt";
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

