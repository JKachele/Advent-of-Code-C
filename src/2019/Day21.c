/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 14, 2026 16:43:49 UTC
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
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 16384

// Jump if any of the 3 closest tiles are empty and if the 4th tile is solid
#define Program1 "NOT A J\n"\
                 "NOT B T\n"\
                 "OR T J\n"\
                 "NOT C T\n"\
                 "OR T J\n"\
                 "AND D J\n"\
                 "WALK\n"

// Jump if Program 1 would jump and if I can walk forward 1 or jump immediately again after jumping
#define Program2 "NOT A J\n"\
                 "NOT B T\n"\
                 "OR T J\n"\
                 "NOT C T\n"\
                 "OR T J\n"\
                 "AND D J\n"\
                 "NOT H T\n"\
                 "NOT T T\n"\
                 "OR E T\n"\
                 "AND T J\n"\
                 "RUN\n"

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

void part1(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        char *script = Program1;
        talint64 inputs = tal_init();
        for (int i = 0; i < (int)strlen(script); i++)
                tal_add(inputs, script[i]);
        talint64 outputs = tal_init();
        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);

        int64 result = outputs.array[outputs.length-1];
        printf("Part 1: %ld\n\n", result);
}

void part2(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        char *script = Program2;
        talint64 inputs = tal_init();
        for (int i = 0; i < (int)strlen(script); i++)
                tal_add(inputs, script[i]);
        talint64 outputs = tal_init();
        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);

        int64 result = outputs.array[outputs.length-1];
        printf("Part 2: %ld\n", result);
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
                char *file = "assets/tests/2019/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day21.txt";
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

