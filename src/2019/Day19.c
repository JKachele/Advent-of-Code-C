/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Apr 13, 2026 17:44:36 UTC
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
#include "../util/talist.h"
#include "../util/Intcode2019.h"
#include "../util/vector.h"
// #include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 16384

struct input {
        talint64 intcode;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool checkIntcode(talint64 intcodeIn, int x, int y) {
        talint64 intcode = copyIntcode(intcodeIn);

        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        tal_add(inputs, x);
        tal_add(inputs, y);
        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);
        int out = outputs.array[0];
        tal_destroy(intcode);
        tal_destroy(inputs);
        tal_destroy(outputs);
        return out == 1;
}

void part1(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        bool map[50][50] = {false};

        int numPulled = 0;
        for (int y = 0; y < 50; y++) {
                for (int x = 0; x < 50; x++) {
                        bool inBeam = checkIntcode(intcode, x, y);
                        if (inBeam) {
                                map[y][x] = true;
                                numPulled++;
                        }
                }
        }

        for (int y = 0; y < 50; y++) {
                for (int x = 0; x < 50; x++) {
                        debugp("%c", map[y][x] ? '#' : '.');
                }
                debugp("\n");
        }

        printf("Part 1: %d\n\n", numPulled);
}

void part2(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        // For each point on the lower bound of the beam. Check the point that is diagnally opposite
        // up and right 100 units. If that point is in the beam, a 100x100 square could fit
        int startX = 0;
        ivec2 pos = {0};
        for (int y = 99;; y++) {
                for (int x = startX;; x++) {
                        bool inBeam = checkIntcode(intcode, x, y);
                        if (inBeam) {
                                startX = x;
                                // Check the point that is diagnally opposite up and right 100 units
                                if (checkIntcode(intcode, x + 99, y - 99)) {
                                        pos.x = x;
                                        pos.y = y - 99;
                                }
                                break;
                        }
                }
                if (pos.y != 0)
                        break;
        }
        int64 ans = (pos.x * 10000) + pos.y;

        printf("Part 2: %ld\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                int64 num = strtoll(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day19.txt";
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

