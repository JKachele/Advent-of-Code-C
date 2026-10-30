/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 30, 2026 12:55:45 EDT
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
#define NUM_REGS 4

enum opperation {
        CPY,
        INC,
        DEC,
        JNZ
};

struct instruction {
        enum opperation opp;
        int x;
        bool xReg;
        int y;
        bool yReg;
};

struct input {
        int numInstrs;
        struct instruction *instrs;
};

void printInstrs(int numInstrs, struct instruction instrs[]) {
        const char *OPPS[4] = {"cpy", "inc", "dec", "jnz"};
        for (int i = 0; i < numInstrs; i++) {
                struct instruction instr = instrs[i];
                printf("%s ", OPPS[instr.opp]);
                if (instr.xReg) {
                        printf("(Reg %c) ", instr.x + 'a');
                } else {
                        printf("%d ", instr.x);
                }
                if (instr.opp == CPY || instr.opp == JNZ) {
                        if (instr.yReg) {
                                printf("(Reg %c)", instr.y + 'a');
                        } else {
                                printf("%d", instr.y);
                        }
                }
                printf("\n");
        }
}

void part1(struct input *input) {
        int numInstrs = input->numInstrs;
        struct instruction *instrs = input->instrs;
        // printInstrs(numInstrs, instrs);

        int64 regs[NUM_REGS] = {0};
        int pc = 0;
        while (pc >= 0 && pc < numInstrs) {
                struct instruction instr = instrs[pc];
                int64 x = instr.xReg ? regs[instr.x] : instr.x;
                int64 y = instr.yReg ? regs[instr.y] : instr.y;

                switch (instr.opp) {
                case CPY:
                        regs[instr.y] = x;
                        pc++;
                        break;
                case INC:
                        regs[instr.x]++;
                        pc++;
                        break;
                case DEC:
                        regs[instr.x]--;
                        pc++;
                        break;
                case JNZ:
                        if (x != 0) {
                                pc += y;
                        } else {
                                pc++;
                        }
                        break;
                }
        }

        printf("Part 1: Register A = %ld\n\n", regs[0]);
}

void part2(struct input *input) {
        int numInstrs = input->numInstrs;
        struct instruction *instrs = input->instrs;
        // printInstrs(numInstrs, instrs);

        int64 regs[NUM_REGS] = {0, 0, 1, 0};
        int pc = 0;
        while (pc >= 0 && pc < numInstrs) {
                struct instruction instr = instrs[pc];
                int64 x = instr.xReg ? regs[instr.x] : instr.x;
                int64 y = instr.yReg ? regs[instr.y] : instr.y;

                switch (instr.opp) {
                case CPY:
                        regs[instr.y] = x;
                        pc++;
                        break;
                case INC:
                        regs[instr.x]++;
                        pc++;
                        break;
                case DEC:
                        regs[instr.x]--;
                        pc++;
                        break;
                case JNZ:
                        if (x != 0) {
                                pc += y;
                        } else {
                                pc++;
                        }
                        break;
                }
        }

        printf("Part 2: Register A = %ld\n", regs[0]);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstrs = ll->length;
        input.instrs = malloc(input.numInstrs * sizeof(struct instruction));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct instruction instr = {0};

                switch (str[0]) {
                case 'c':
                        instr.opp = CPY;
                        break;
                case 'i':
                        instr.opp = INC;
                        break;
                case 'd':
                        instr.opp = DEC;
                        break;
                case 'j':
                        instr.opp = JNZ;
                        break;
                }

                char *xStr = strtok(str + 4, " ");
                if (xStr[0] >= 'a' && xStr[0] <= 'z') {
                        instr.x = xStr[0] - 'a';
                        instr.xReg = true;
                } else {
                        instr.x = strtol(xStr, NULL, 10);
                }

                if (instr.opp == CPY || instr.opp == JNZ) {
                        char *yStr = strtok(NULL, "");
                        if (yStr[0] >= 'a' && yStr[0] <= 'z') {
                                instr.y = yStr[0] - 'a';
                                instr.yReg = true;
                        } else {
                                instr.y = strtol(yStr, NULL, 10);
                        }
                }

                input.instrs[i++] = instr;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day12.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

