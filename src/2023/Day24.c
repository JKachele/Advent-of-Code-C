/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 08, 2025 22:13:21 UTC
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
#define TEST_MIN 7
#define TEST_MAX 27
#define MIN 200000000000000
#define MAX 400000000000000

typedef struct line {
        lvec2 start;
        lvec2 end;
} line;

typedef struct stone {
        lvec3 pos;
        lvec3 vel;
} stone;
typedef tll(stone) tllstone;

struct input {
        tllstone stones;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printStone(stone s) {
        if (!Debug) return;
        printf("(%ld, %ld, %ld) - ", s.pos.x, s.pos.y, s.pos.z);
        printf("(%ld, %ld, %ld)\n", s.vel.x, s.vel.y, s.vel.z);
}

void printStones(tllstone stones) {
        if (!Debug) return;
        tll_foreach(stones, it) {
                printStone(it->item);
        }
        printf("\n");
}

bool intersect2d(stone a, stone b, int64 minVal, int64 maxVal) {
        double m1 = (double)a.vel.y / a.vel.x;
        double m2 = (double)b.vel.y / b.vel.x;
        if (m1 == m2) // Parallel
                return false;

        double b1 = a.pos.y - (m1 * a.pos.x);
        double b2 = b.pos.y - (m2 * b.pos.x);

        double xInt = (b2 - b1) / (m1 - m2);
        double yInt = (m1 * xInt) + b1;
        // printf("    %0.2f, %0.2f\n", xInt, yInt);

        // Intersection muse be in range
        if (xInt < minVal || xInt > maxVal)
                return false;
        if (yInt < minVal || yInt > maxVal)
                return false;

        // Intersection must be in the stone's future
        if ((a.vel.x > 0 && xInt < a.pos.x) || (a.vel.x < 0 && xInt > a.pos.x))
                return false;
        if ((a.vel.y > 0 && yInt < a.pos.y) || (a.vel.y < 0 && yInt > a.pos.y))
                return false;
        if ((b.vel.x > 0 && xInt < b.pos.x) || (b.vel.x < 0 && xInt > b.pos.x))
                return false;
        if ((b.vel.y > 0 && yInt < b.pos.y) || (b.vel.y < 0 && yInt > b.pos.y))
                return false;

        return true;
}

void part1(struct input input) {
        tllstone stones = input.stones;
        int numStones = tll_length(stones);
        printStones(stones);

        stone *stoneArr = calloc(numStones, sizeof(stone));
        int i = 0;
        tll_foreach(stones, it) {
                stoneArr[i] = it->item;
                i++;
        }

        int64 minVal = Debug ? TEST_MIN : MIN;
        int64 maxVal = Debug ? TEST_MAX : MAX;
        int numIntersections = 0;
        for (int i=0; i<numStones; i++) {
                for (int j=i+1; j<numStones; j++) {
                        bool inter = intersect2d(stoneArr[i], stoneArr[j],
                                        minVal, maxVal);
                        if (inter)
                                numIntersections++;
                }
        }

        printf("Part 1: %d\n\n", numIntersections);
}

void part2(struct input input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;

                stone s = {0};
                
                char *tok = strtok(str, ", ");
                for (int i=0; i<3; i++) {
                        s.pos.raw[i] = strtol(tok, NULL, 10);
                        tok = strtok(NULL, ", ");
                }

                tok = strtok(NULL, ", ");
                for (int i=0; i<3; i++) {
                        s.vel.raw[i] = strtol(tok, NULL, 10);
                        tok = strtok(NULL, ", ");
                }

                tll_push_back(input.stones, s);
                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day24.txt";
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

