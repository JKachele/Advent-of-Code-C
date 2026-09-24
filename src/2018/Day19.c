/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 24, 2026 11:23:12 EDT
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
#include "../util/talist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024
#define NUM_REGS 6
#define NUM_OPPS 16

typedef tal(ivec4) talivec4;
typedef void (*opperation)(int regs[NUM_REGS], ivec4 instr);

struct input {
        int pcReg;
        talivec4 program;
        opperation opps[NUM_OPPS];
};

//************************ Opperations ************************
const char *OppNames[NUM_OPPS] = {"addr", "addi", "mulr", "muli", "banr", "bani",
        "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"};

void addr(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = a + b;
}

void addi(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs[c] = a + b;
}

void mulr(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = a * b;
}

void muli(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs[c] = a * b;
}

void banr(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = a & b;
}

void bani(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs[c] = a & b;
}

void borr(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = a | b;
}

void bori(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs[c] = a | b;
}

void setr(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int c = instr.raw[3];
        regs[c] = a;
}

void seti(int regs[NUM_REGS], ivec4 instr) {
        int a = instr.raw[1];
        int c = instr.raw[3];
        regs[c] = a;
}

void gtir(int regs[NUM_REGS], ivec4 instr) {
        int a = instr.raw[1];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = (a > b) ? 1 : 0;
}

void gtri(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs[c] = (a > b) ? 1 : 0;
}

void gtrr(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = (a > b) ? 1 : 0;
}

void eqir(int regs[NUM_REGS], ivec4 instr) {
        int a = instr.raw[1];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = (a == b) ? 1 : 0;
}

void eqri(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs[c] = (a == b) ? 1 : 0;
}

void eqrr(int regs[NUM_REGS], ivec4 instr) {
        int a = regs[instr.raw[1]];
        int b = regs[instr.raw[2]];
        int c = instr.raw[3];
        regs[c] = (a == b) ? 1 : 0;
}
//*************************************************************

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printProgram(talivec4 program) {
        tal_for(program, i) {
                ivec4 p = program.array[i];
                printf("%s %d %d %d\n", OppNames[p.x], p.y, p.z, p.w);
        }
}

void printRegs(int regs[NUM_REGS]) {
        printf("[");
        for (int i = 0; i < NUM_REGS; i++) {
                printf("%d ", regs[i]);
        }
        printf("] ");
}

void part1(struct input *input) {
        int pcReg = input->pcReg;
        talivec4 program = input->program;
        opperation *opps = input->opps;

        int regs[NUM_REGS] = {0};
        int pc = 0;
        while(pc < (int)program.length) {
                regs[pcReg] = pc;
                ivec4 instr = program.array[pc];

                // printf("pc=%2d ", pc);
                // printRegs(regs);
                // printf(" %s %d %d %d ", OppNames[instr.x], instr.y, instr.z, instr.w);

                opps[instr.x](regs, instr);

                // printRegs(regs);
                // printf("\n");

                pc = regs[pcReg] + 1;
        }

        printf("Part 1: Reg 0 = %d\n\n", regs[0]);
}

void part2(struct input *input) {
        int pcReg = input->pcReg;
        talivec4 program = input->program;
        opperation *opps = input->opps;

        int regs[NUM_REGS] = {0};
        regs[0] = 1;
        int pc = 0;
        // for (int i = 0; i < 100; i++) {
        // while(pc < (int)program.length) {
        while(pc != 1) {
                regs[pcReg] = pc;
                ivec4 instr = program.array[pc];

                // printf("pc=%2d ", pc);
                // printRegs(regs);
                // printf(" %s %d %d %d ", OppNames[instr.x], instr.y, instr.z, instr.w);

                opps[instr.x](regs, instr);

                // printRegs(regs);
                // printf("\n");

                pc = regs[pcReg] + 1;
        }
        // printf("%d\n", regs[4]);

        // Program adds all the factors of the number in reg[4]
        int num = regs[4];
        int sqrtNum = (int)sqrt((double)num) + 1;
        int factorSum = 0;
        for (int i = 1; i < sqrtNum; i++) {
                if (num % i == 0) {
                        factorSum += i;
                        if (num / i != i)
                                factorSum += num / i;
                }
        }

        printf("Part 2: Reg 0 = %d\n", factorSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        input.pcReg = ((char*)ll->head->data)[4] - '0';
        for (llNode *cur = ll->head->next; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (str[0] == '\0') continue;
                ivec4 instr;

                char *opp = strtok(str, " ");
                for (int i = 0; i < NUM_OPPS; i++) {
                        if (strcmp(opp, OppNames[i]) == 0) {
                                instr.x = i;
                                break;
                        }
                }

                for (int i = 0; i < 3; i++) {
                        instr.raw[i+1] = strtol(strtok(NULL, " "), NULL, 10);
                }
                tal_add(input.program, instr);
        }

        input.opps[0]  = addr;
        input.opps[1]  = addi;
        input.opps[2]  = mulr;
        input.opps[3]  = muli;
        input.opps[4]  = banr;
        input.opps[5]  = bani;
        input.opps[6]  = borr;
        input.opps[7]  = bori;
        input.opps[8]  = setr;
        input.opps[9]  = seti;
        input.opps[10] = gtir;
        input.opps[11] = gtri;
        input.opps[12] = gtrr;
        input.opps[13] = eqir;
        input.opps[14] = eqri;
        input.opps[15] = eqrr;

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day19.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day19.txt";
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

