/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 23, 2026 11:58:31 EDT
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

#define INPUT_BUFFER_SIZE 1024

enum direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
};

struct instruction {
        int length;
        enum direction *dirs;
};

struct input {
        int numDigits;
        struct instruction *instrs;
};

static const ivec2 Dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
static const char DirChars[] = {'U', 'R', 'D', 'L'};

void printInstrs(int numInstrs, struct instruction instrs[]) {
        for (int i = 0; i < numInstrs; i++) {
                struct instruction instr = instrs[i];
                printf("%d: [", i);
                for (int j = 0; j < instr.length; j++) {
                        printf("%c, ", DirChars[instr.dirs[j]]);
                }
                printf("\b]\n");
        }
}

bool validPos(ivec2 pos, int size) {
        return (pos.x >=0) && (pos.y >= 0) && (pos.x < size) && (pos.y < size);
}

void part1(struct input *input) {
        int numDigits = input->numDigits;
        struct instruction *instrs = input->instrs;
        // printInstrs(numInstrs, instrs);

        const u8 pad[3][3] = {{1, 2, 3},
                              {4, 5, 6},
                              {7, 8, 9}};
        int code = 0;
        ivec2 pos = {{1, 1}};
        for (int i = 0; i < numDigits; i++) {
                struct instruction instr = instrs[i];
                for (int j = 0; j < instr.length; j++) {
                        ivec2 next = ivec2Add(pos, Dirs[instr.dirs[j]]);
                        if (validPos(next, 3)) {
                                pos = next;
                        }
                }
                code *= 10;
                code += pad[pos.y][pos.x];
        }

        printf("Part 1: Code = %d\n\n", code);
}

void part2(struct input *input) {
        int numDigits = input->numDigits;
        struct instruction *instrs = input->instrs;
        // printInstrs(numInstrs, instrs);

        const char pad[5][5] = {{' ', ' ', '1', ' ', ' '},
                                {' ', '2', '3', '4', ' '},
                                {'5', '6', '7', '8', '9'},
                                {' ', 'A', 'B', 'C', ' '},
                                {' ', ' ', 'D', ' ', ' '}};
        char code[numDigits + 1];
        ivec2 pos = {{0, 2}};
        for (int i = 0; i < numDigits; i++) {
                struct instruction instr = instrs[i];
                for (int j = 0; j < instr.length; j++) {
                        ivec2 next = ivec2Add(pos, Dirs[instr.dirs[j]]);
                        if (validPos(next, 5) && pad[next.y][next.x] != ' ') {
                                pos = next;
                        }
                }
                code[i] = pad[pos.y][pos.x];
        }
        code[numDigits] = '\0';

        printf("Part 2: Code = %s\n", code);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numDigits = ll->length;
        input.instrs = malloc(input.numDigits * sizeof(struct instruction));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                int length = strlen(str);
                input.instrs[i].length = length;
                input.instrs[i].dirs = malloc(length * sizeof(enum direction));
                for (int j = 0; j < length; j++) {
                        switch (str[j]) {
                        case 'U':
                                input.instrs[i].dirs[j] = UP;
                                break;
                        case 'R':
                                input.instrs[i].dirs[j] = RIGHT;
                                break;
                        case 'D':
                                input.instrs[i].dirs[j] = DOWN;
                                break;
                        case 'L':
                                input.instrs[i].dirs[j] = LEFT;
                                break;
                        }
                }

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day02.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day02.txt";
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
        double totalTime = ((double)(pt2 - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

