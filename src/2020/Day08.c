/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Feb 26, 2026 17:23:46 UTC
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
#include "../util/talist.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef enum opperation {
        NOP,
        ACC,
        JMP
} opperation;

typedef struct instruction {
        opperation opp;
        int value;
        bool visited;
} instruction;
typedef tal(instruction) talinstruction;

struct input {
        talinstruction program;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool validPC(int pc, int programLen) {
        return pc >= 0 && pc < programLen;
}

// Returns -1 if infinite loop
int infiniteLoop(talinstruction program) {
        // Reset visited tags
        for (int i = 0; i < (int)program.length; i++)
                program.array[i].visited = false;

        int acc = 0;
        int pc = 0;
        while (validPC(pc, program.length) && program.array[pc].visited == false) {
                instruction *inst = &program.array[pc];
                inst->visited = true;

                switch (inst->opp) {
                        case NOP:
                                pc++;
                                break;
                        case ACC:
                                pc++;
                                acc += inst->value;
                                break;
                        case JMP:
                                pc += inst->value;
                                break;
                }
        }

        // If loop broke on a valid PC, it must be an infinite loop
        if (validPC(pc, program.length))
                return -1;
        else
                return acc;
}

void part1(struct input input) {
        talinstruction program = input.program;

        int acc = 0;
        int pc = 0;
        while (validPC(pc, program.length) && program.array[pc].visited == false) {
                instruction *inst = &program.array[pc];
                inst->visited = true;

                switch (inst->opp) {
                        case NOP:
                                pc++;
                                break;
                        case ACC:
                                pc++;
                                acc += inst->value;
                                break;
                        case JMP:
                                pc += inst->value;
                                break;
                }
        }

        printf("Part 1: %d\n\n", acc);
}

void part2(struct input input) {
        talinstruction program = input.program;

        // Brute Force FTW
        // Go down program and change one nop or jmp at a time and test is it loops
        int acc = -1;
        for (int i = 0; i < (int)program.length; i++) {
                instruction *inst = &program.array[i];

                // Dont change acc instructions
                if (inst->opp == ACC)
                        continue;

                // Switch opperation
                opperation prevOp;
                if (inst->opp == NOP) {
                        prevOp = NOP;
                        inst->opp = JMP;
                } else {
                        prevOp = JMP;
                        inst->opp = NOP;
                }

                // Test program
                acc = infiniteLoop(program);
                if (acc != -1)
                        break;

                // If still loop, undo switch
                inst->opp = prevOp;
        }

        printf("Part 2: %d\n", acc);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        talinstruction program = tal_init();

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                instruction inst = {0};

                char *opp = strtok(str, " ");
                if (strcmp(opp, "nop") == 0)
                        inst.opp = NOP;
                else if (strcmp(opp, "acc") == 0)
                        inst.opp = ACC;
                else if (strcmp(opp, "jmp") == 0)
                        inst.opp = JMP;
                else
                        printf("Opcode '%s' not recognized\n", opp);

                char *value = strtok(NULL, "");
                char sign = value[0];
                int num = strtol(value + 1, NULL, 10);
                if (sign == '-')
                        num *= -1;
                inst.value = num;

                tal_add(program, inst);
        }

        input.program = program;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day08.txt";
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

