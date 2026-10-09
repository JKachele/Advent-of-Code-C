/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 08, 2026 14:37:59 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/vector.h"
// #include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

struct input {
        int num;
};

const ivec2 DIRS[4] = {{{1, 0}}, {{0, -1}}, {{-1, 0}}, {{0, 1}}};
const ivec2 DIAGS[4] = {{{1, -1}}, {{-1, -1}}, {{-1, 1}}, {{1, 1}}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printSpiral(int size, int spiral[][size]) {
        for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                        printf("%9d ", spiral[i][j]);
                }
                printf("\n");
        }
}

bool valid(int size, ivec2 pos) {
        return (pos.x >= 0) && (pos.y >= 0) && (pos.x < size) && (pos.y < size);
}

/* 17 16 15 14 13
 * 18  5  4  3 12
 * 19  6  1  2 11
 * 20  7  8  9 10
 * 21 22 23 24 25
 */
int findDist(int num) {
        // Bottom right corners of each square are odd perfect squares
        // Distance from perfect square is sqrt(num) - 1
        int sqrtNum = (int)(sqrt((float)num));
        int sq1 = sqrtNum * sqrtNum;

        // If num is a perfect square, dist is sqrt - 1
        if (sq1 == num)
                return sqrtNum - 1;

        // Find first odd perfect square above number
        int sqrtNum2;
        if (sqrtNum %2 == 0) {
                sqrtNum2 = sqrtNum + 1;
        } else {
                sqrtNum2 = sqrtNum + 2;
        }
        int sq2 = (sqrtNum2) * (sqrtNum2); // First odd perfect square after num

        int distToSq = sq2 - num;
        int posOnEdge = distToSq % (sqrtNum2 - 1);
        int distToMiddle = abs(posOnEdge - ((sqrtNum2 - 1) / 2));
        int dist = ((sqrtNum2 - 1) / 2) + distToMiddle;
        return dist;
}

int squareValue(int size, int spiral[][size], ivec2 pos) {
        int sum = 0;
        for (int i = 0; i < 4; i++) {
                ivec2 cur = ivec2Add(pos, DIRS[i]);
                if (valid(size, cur) && spiral[cur.y][cur.x] > 0)
                        sum += spiral[cur.y][cur.x];

                cur = ivec2Add(pos, DIAGS[i]);
                if (valid(size, cur) && spiral[cur.y][cur.x] > 0)
                        sum += spiral[cur.y][cur.x];
        }
        return sum;
}

int fillSpiral(int size, int spiral[][size], int num) {
        ivec2 pos = {{size / 2, size / 2}};
        spiral[pos.y][pos.x] = 1;

        // Go in direction until able to go in the next direction
        int curDir = 0;
        int i = 2;
        for(;;) {
                pos = ivec2Add(pos, DIRS[curDir]);
                if (!valid(size, pos))
                        break;
                int value = squareValue(size, spiral, pos);
                if (value > num)
                        return value;
                spiral[pos.y][pos.x] = value;

                int nextDir = (curDir + 1) % 4;
                ivec2 turn = ivec2Add(pos, DIRS[nextDir]);
                if (spiral[turn.y][turn.x] == 0)
                        curDir = nextDir;
        }

        return 0;
}

void part1(struct input *input) {
        int num = input->num;

        int dist = findDist(num);
        printf("Part 1: Distance = %d\n\n", dist);
}

void part2(struct input *input) {
        const int SpiralSize = 11;
        int num = input->num;

        int spiral[SpiralSize][SpiralSize];
        for (int i = 0; i < SpiralSize; i++) {
                for (int j = 0; j < SpiralSize; j++) {
                        spiral[i][j] = 0;
                }
        }

        int value = fillSpiral(SpiralSize, spiral, num);
        // printSpiral(SpiralSize, spiral);

        printf("Part 2: %d\n", value);
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
        printf("Running Advent of Code 2017/Day03.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day03.txt";
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

