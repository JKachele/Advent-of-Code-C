/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 13, 2026 10:36:57 EDT
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
// #include "../util/talist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

enum comparison {
        EQ, // == Equal
        NE, // != Not equal
        GT, // >  Greater than
        GE, // >= Greater than or equal
        LT, // <  Less than
        LE  // <= Less than or equal
};

struct condition {
        u8 reg;
        enum comparison comparison;
        int value;
};

struct instruction {
        u8 reg;
        bool inc;
        int value;
        struct condition condition;
};

struct input {
        int numInstructions;
        struct instruction *instructions;
        int numRegs;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printInstructions(int numInst, struct instruction instructions[]) {
        for (int i = 0; i < numInst; i++) {
                struct instruction inst = instructions[i];
                char cond[3] = {0};
                switch (inst.condition.comparison) {
                case EQ:
                        cond[0] = '=';
                        cond[1] = '=';
                        break;
                case NE:
                        cond[0] = '!';
                        cond[1] = '=';
                        break;
                case GT:
                        cond[0] = '>';
                        break;
                case GE:
                        cond[0] = '>';
                        cond[1] = '=';
                        break;
                case LT:
                        cond[0] = '<';
                        break;
                case LE:
                        cond[0] = '<';
                        cond[1] = '=';
                        break;
                }

                printf("(%u) %s %d ", inst.reg, inst.inc ? "inc" : "dec", inst.value);
                printf("if (%u) %s %d\n", inst.condition.reg, cond, inst.condition.value);
        }
}

// Max register name is 3 characters
u16 regHash(const char *name) {
        u16 hash = 0;
        for (u64 i = 0; i < strlen(name); i++) {
                hash *= 27;
                hash += (name[i] - 'a') + 1;
        }
        return hash;
}

u8 getIndex(int8 hashToIndex[], const char *name) {
        u16 hash = regHash(name);
        static int8 nextIndex = 0;
        if (hashToIndex[hash] == -1) {
                // printf("%s: %d\n", name, nextIndex);
                hashToIndex[hash] = nextIndex++;
        }
        return (u8)hashToIndex[hash];
}

struct condition parseCondition(char *input, int8 hashToIndex[]) {
        struct condition cond = {0};

        char *tok = strtok(input, " ");
        cond.reg = getIndex(hashToIndex, tok);

        tok = strtok(NULL, " ");
        if (strlen(tok) == 1) {
                if (tok[0] == '>') {
                        cond.comparison = GT;
                } else { // <
                        cond.comparison = LT;
                }
        } else {
                if (tok[0] == '=') {
                        cond.comparison = EQ;
                } else if (tok[0] == '!') {
                        cond.comparison = NE;
                } else if (tok[0] == '>') {
                        cond.comparison = GE;
                } else { // <
                        cond.comparison = LE;
                }
        }

        tok = strtok(NULL, "");
        cond.value = strtol(tok, NULL, 10);

        return cond;
}

void execInstr(int regs[], struct instruction inst) {
        bool exec = false;
        u8 cmpReg = inst.condition.reg;
        int cmpValue = inst.condition.value;
        switch (inst.condition.comparison) {
        case EQ:
                exec = (regs[cmpReg] == cmpValue);
                break;
        case NE:
                exec = (regs[cmpReg] != cmpValue);
                break;
        case GT:
                exec = (regs[cmpReg] > cmpValue);
                break;
        case GE:
                exec = (regs[cmpReg] >= cmpValue);
                break;
        case LT:
                exec = (regs[cmpReg] < cmpValue);
                break;
        case LE:
                exec = (regs[cmpReg] <= cmpValue);
                break;
        }
        if (!exec)
                return;

        if (inst.inc)
                regs[inst.reg] += inst.value;
        else
                regs[inst.reg] -= inst.value;
}

void part1_2(struct input *input) {
        int numInstructions = input->numInstructions;
        struct instruction *instructions = input->instructions;
        int numRegs = input->numRegs;
        // printInstructions(numInstructions, instructions);

        int regs[numRegs];
        for (int i = 0; i < numRegs; i++) { regs[i] = 0; }

        int largestEver = 0;
        for (int i = 0; i < numInstructions; i++) {
                execInstr(regs, instructions[i]);
                if (regs[instructions[i].reg] > largestEver)
                        largestEver = regs[instructions[i].reg];
        }

        int largestReg = 0;
        for (int i = 0; i < numRegs; i++) {
                if (regs[i] > largestReg)
                        largestReg = regs[i];
        }

        printf("Part 1: Largest Register Value = %d\n\n", largestReg);
        printf("Part 2: Largest Register Value Ever = %d\n", largestEver);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstructions = ll->length;
        input.instructions = calloc(input.numInstructions, sizeof(struct instruction));
        struct instruction *instructions = input.instructions;

        const u16 MaxHash = 27*27*27;
        int8 hashToIndex[MaxHash+1];
        for (u16 i = 0; i <= MaxHash; i++) { hashToIndex[i] = -1; }
        u8 maxReg = 0;

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct instruction *inst = &instructions[i++];

                char *tok = strtok(str, " ");
                inst->reg = getIndex(hashToIndex, tok);
                if (inst->reg > maxReg) { maxReg = inst->reg; }

                tok = strtok(NULL, " ");
                inst->inc = (strcmp(tok, "inc") == 0);

                tok = strtok(NULL, " ");
                inst->value = strtol(tok, NULL, 10);

                tok = strtok(NULL, " ");
                tok = strtok(NULL, "");
                inst->condition = parseCondition(tok, hashToIndex);
                if (inst->condition.reg > maxReg) { maxReg = inst->condition.reg; }
        }
        input.numRegs = maxReg + 1;

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day08.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

