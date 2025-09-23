/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 23, 2025 14:24:23 UTC
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

struct input {
        ivec2 targetMin;
        ivec2 targetMax;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool reachesTarget(ivec2 targetMin, ivec2 targetMax, ivec2 startVel) {
        ivec2 vel = startVel;
        ivec2 pos = {0};
        while (pos.x <= targetMax.x && pos.y >= targetMin.y) {
                pos = ivec2Add(pos, vel);
                if (pos.x >= targetMin.x && pos.x <= targetMax.x &&
                                pos.y >= targetMin.y && pos.y <= targetMax.y) {
                        return true;
                }

                if (vel.x > 0)
                        vel.x--;
                else if (vel.x < 0)
                        vel.x++;
                vel.y--;
        }
        return false;
}

void part1(struct input input) {
        ivec2 targetMin = input.targetMin;

        // Fastest Y vel hits bottom on zone directly after passing 0
        int32 startVelY = abs(targetMin.y) - 1;

        int32 maxY = 0;
        for (int i=startVelY; i>0; i--) {
                maxY += i;
        }

        printf("Part 1: %d\n\n", maxY);
}

void part2(struct input input) {
        ivec2 targetMin = input.targetMin;
        ivec2 targetMax = input.targetMax;

        // Fastest Y vel hits bottom on zone directly after passing 0
        int32 maxVelY = (targetMin.y * -1) - 1;
        int32 minVelY = targetMin.y;

        int32 minVelX = 1;
        int32 maxVelX = targetMax.x;

        // Brute Force Baby!!!!
        int32 numValidVels = 0;
        for (int y=minVelY; y<=maxVelY; y++) {
                for (int x=minVelX; x<=maxVelX; x++) {
                        ivec2 startVel = {{x, y}};
                        if (reachesTarget(targetMin, targetMax, startVel))
                                numValidVels++;
                }
        }

        printf("Part 2: %d\n", numValidVels);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        strtok(str, "=");
        input.targetMin.x = strtol(strtok(NULL, "."), NULL, 10);
        input.targetMax.x = strtol(strtok(NULL, ",")+1, NULL, 10);
        strtok(NULL, "=");
        input.targetMin.y = strtol(strtok(NULL, "."), NULL, 10);
        input.targetMax.y = strtol(strtok(NULL, "")+1, NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day17.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day17.txt";
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

