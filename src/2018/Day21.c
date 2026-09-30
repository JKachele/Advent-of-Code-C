/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 25, 2026 14:54:42 EDT
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
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024
#define NUM_REGS 6
#define NUM_OPPS 16

typedef tal(long) tallong;
typedef tal(lvec4) tallvec4;
typedef void (*opperation)(long regs[NUM_REGS], lvec4 instr);

struct input {
        int pcReg;
        tallvec4 program;
        opperation opps[NUM_OPPS];
};

//************************ Opperations ************************
const char *OppNames[NUM_OPPS] = {"addr", "addi", "mulr", "muli", "banr", "bani",
        "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"};

void addr(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
        regs[c] = a + b;
}

void addi(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = instr.raw[2];
        long c = instr.raw[3];
        regs[c] = a + b;
}

void mulr(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
        regs[c] = a * b;
}

void muli(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = instr.raw[2];
        long c = instr.raw[3];
        regs[c] = a * b;
}

void banr(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
        regs[c] = a & b;
}

void bani(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = instr.raw[2];
        long c = instr.raw[3];
        regs[c] = a & b;
}

void borr(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
        regs[c] = a | b;
}

void bori(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = instr.raw[2];
        long c = instr.raw[3];
        regs[c] = a | b;
}

void setr(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long c = instr.raw[3];
        regs[c] = a;
}

void seti(long regs[NUM_REGS], lvec4 instr) {
        long a = instr.raw[1];
        long c = instr.raw[3];
        regs[c] = a;
}

void gtir(long regs[NUM_REGS], lvec4 instr) {
        long a = instr.raw[1];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
        regs[c] = (a > b) ? 1 : 0;
}

void gtri(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = instr.raw[2];
        long c = instr.raw[3];
        regs[c] = (a > b) ? 1 : 0;
}

void gtrr(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
        regs[c] = (a > b) ? 1 : 0;
}

void eqir(long regs[NUM_REGS], lvec4 instr) {
        long a = instr.raw[1];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
        regs[c] = (a == b) ? 1 : 0;
}

void eqri(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = instr.raw[2];
        long c = instr.raw[3];
        regs[c] = (a == b) ? 1 : 0;
}

void eqrr(long regs[NUM_REGS], lvec4 instr) {
        long a = regs[instr.raw[1]];
        long b = regs[instr.raw[2]];
        long c = instr.raw[3];
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

void printProgram(tallvec4 program) {
        tal_for(program, i) {
                lvec4 p = program.array[i];
                printf("%s %ld %ld %ld\n", OppNames[p.x], p.y, p.z, p.w);
        }
}

void printRegs(long regs[NUM_REGS]) {
        printf("[");
        for (int i = 0; i < NUM_REGS; i++) {
                printf("%ld ", regs[i]);
        }
        printf("] ");
}

void printState(tallvec4 program, long regs[NUM_REGS], int pc) {
        lvec4 instr = program.array[pc];
        printf("%d: %s %ld %ld %ld\n", pc, OppNames[instr.a], instr.b, instr.c, instr.d);
        printf("[%ld", regs[0]);
        for (int i = 1; i < 6; i++)
                printf(", %ld", regs[i]);
        printf("]\n\n");
}

bool addIfNew(tallong *reg3s, long reg3) {
        tal_for(*reg3s, i) {
                if (reg3s->array[i] == reg3)
                        return true;
        }
        tal_add(*reg3s, reg3);
        return false;
}

void runProgram() {
        tallong reg3s = tal_init();

        long reg[6] = {0};
        reg[0] = 16311888;

        reg[3] = 123;
        do {
                reg[3] = reg[3] & 456;
        } while (reg[3] != 72);

        reg[3] = 0;
INSTR_6:
        reg[2] = reg[3] | 65536;
        reg[3] = 10736359;
INSTR_8:
        reg[1] = reg[2] & 255;
        reg[3] += reg[1];
        reg[3] = reg[3] & 16777215;
        reg[3] *= 65899;
        reg[3] = reg[3] & 16777215;
        if (reg[2] < 256)
                goto INSTR_28;

        reg[1] = 0;
        for (;;) {
                reg[5] = reg[1] + 1;
                reg[5] *= 256;
                if (reg[5] > reg[2])
                        break;

                reg[1] += 1;
        }
        reg[2] = reg[1];
        goto INSTR_8;

INSTR_28:
        if (addIfNew(&reg3s, reg[3])) {
                printf("First repeat: %ld\n", reg[3]);
                printf("Last reg[3]: %ld\n", reg3s.array[reg3s.length - 1]);
                return;
        }
        // if (reg[3] == reg[0]) {
        //         printf("Halted!\n");
        //         return;
        // }
        goto INSTR_6;
}

void part1(struct input *input) {
        int pcReg = input->pcReg;
        tallvec4 program = input->program;
        opperation *opps = input->opps;

        runProgram();

        // long regs[NUM_REGS] = {0};
        // regs[0] = 0;
        // int pc = 0;
        // long instret = 0;
        // for (int i = 0; i < 2000; i++) {
        // // while(pc != 29) {
        // // while(pc < (int)program.length) {
        //         regs[pcReg] = pc;
        //         lvec4 instr = program.array[pc];
        //
        //         opps[instr.x](regs, instr);
        //         printState(program, regs, pc);
        //
        //         pc = regs[pcReg] + 1;
        //         instret++;
        // }
        // printf("Instret: %ld\n", instret);

        // printf("Part 1: Reg 0 = %ld\n\n", regs[0]);
}

void part2(struct input *input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        input.pcReg = ((char*)ll->head->data)[4] - '0';
        for (llNode *cur = ll->head->next; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (str[0] == '\0') continue;
                lvec4 instr;

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
        printf("Running Advent of Code 2018/Day21.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day21.txt";
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

