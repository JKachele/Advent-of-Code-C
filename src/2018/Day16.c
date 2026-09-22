/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 22, 2026 16:27:51 EDT
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
#define NUM_CODES 16

typedef tal(int) talint;
typedef tal(ivec4) talivec4;
typedef ivec4 (*opperation)(ivec4 regs, ivec4 instr);

struct test {
        ivec4 before;
        ivec4 opp;
        ivec4 after;
};
typedef tal(struct test) taltest;

struct input {
        taltest tests;
        talivec4 instrs;
        opperation opps[NUM_CODES];
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTests(taltest tests) {
        tal_for(tests, i) {
                struct test test = tests.array[i];
                printf("Before: ");
                for (int i = 0; i < 4; i++)
                        printf("%d ", test.before.raw[i]);
                printf("\nopp: ");
                for (int i = 0; i < 4; i++)
                        printf("%d ", test.opp.raw[i]);
                printf("\nAfter: ");
                for (int i = 0; i < 4; i++)
                        printf("%d ", test.after.raw[i]);
                printf("\n\n");
        }
}

ivec4 addr(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = a + b;
        return regs;
}

ivec4 addi(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs.raw[c] = a + b;
        return regs;
}

ivec4 mulr(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = a * b;
        return regs;
}

ivec4 muli(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs.raw[c] = a * b;
        return regs;
}

ivec4 banr(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = a & b;
        return regs;
}

ivec4 bani(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs.raw[c] = a & b;
        return regs;
}

ivec4 borr(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = a | b;
        return regs;
}

ivec4 bori(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs.raw[c] = a | b;
        return regs;
}

ivec4 setr(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int c = instr.raw[3];
        regs.raw[c] = a;
        return regs;
}

ivec4 seti(ivec4 regs, ivec4 instr) {
        int a = instr.raw[1];
        int c = instr.raw[3];
        regs.raw[c] = a;
        return regs;
}

ivec4 gtir(ivec4 regs, ivec4 instr) {
        int a = instr.raw[1];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = (a > b) ? 1 : 0;
        return regs;
}

ivec4 gtri(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs.raw[c] = (a > b) ? 1 : 0;
        return regs;
}

ivec4 gtrr(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = (a > b) ? 1 : 0;
        return regs;
}

ivec4 eqir(ivec4 regs, ivec4 instr) {
        int a = instr.raw[1];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = (a == b) ? 1 : 0;
        return regs;
}

ivec4 eqri(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = instr.raw[2];
        int c = instr.raw[3];
        regs.raw[c] = (a == b) ? 1 : 0;
        return regs;
}

ivec4 eqrr(ivec4 regs, ivec4 instr) {
        int a = regs.raw[instr.raw[1]];
        int b = regs.raw[instr.raw[2]];
        int c = instr.raw[3];
        regs.raw[c] = (a == b) ? 1 : 0;
        return regs;
}

void findOpcodes(taltest tests, opperation opps[NUM_CODES], int opcodes[NUM_CODES]) {
        bool codePossable[NUM_CODES][NUM_CODES];
        int numCodes[NUM_CODES];
        for (int i = 0; i < NUM_CODES; i++) {
                for (int j = 0; j < NUM_CODES; j++) {
                        codePossable[i][j] = true;
                }
                numCodes[i] = 16;
        }

        tal_for(tests, i) {
                struct test test = tests.array[i];
                int code = test.opp.x;

                for (int j = 0; j < NUM_CODES; j++) {
                        ivec4 out = opps[j](test.before, test.opp);
                        if (!ivec4Eq(out, test.after) && codePossable[code][j]) {
                                codePossable[code][j] = false;
                                numCodes[code]--;
                        }
                }
        }

        bool visited[NUM_CODES] = {0};
        for (;;) {
                bool done = true;
                for (int i = 0; i < NUM_CODES; i++) {
                        if (numCodes[i] > 1) {
                                done = false;
                                continue;
                        }
                        if (visited[i]) continue;

                        visited[i] = true;
                        int code = 0;
                        for (int j = 0; j < NUM_CODES; j++) {
                                if (codePossable[i][j]) {
                                        code = j;
                                        break;
                                }
                        }
                        opcodes[i] = code;

                        for (int j = 0; j < NUM_CODES; j++) {
                                if (j == i) continue;
                                if (codePossable[j][code]) {
                                        codePossable[j][code] = false;
                                        numCodes[j]--;
                                }
                        }
                }
                if (done) break;
        }
}

void part1(struct input *input) {
        taltest tests = input->tests;
        opperation *opps = input->opps;
        // printTests(tests);

        int num3codes = 0;
        tal_for(tests, i) {
                struct test test = tests.array[i];

                int numCodes = 0;
                for (int j = 0; j < NUM_CODES; j++) {
                        ivec4 out = opps[j](test.before, test.opp);
                        if (ivec4Eq(out, test.after))
                                numCodes++;
                }
                if (numCodes >= 3)
                        num3codes++;
        }

        printf("Part 1: Inputs with 3 possible opp codes = %d\n\n", num3codes);
}

void part2(struct input *input) {
        taltest tests = input->tests;
        talivec4 instrs = input->instrs;
        opperation *opps = input->opps;
        // printTests(tests);

        // Opcode [index] goes to opperation *value*
        int opcodes[NUM_CODES];
        findOpcodes(tests, opps, opcodes);

        // Run program
        ivec4 regs = {0};
        tal_for(instrs, i) {
                ivec4 instr = instrs.array[i];
                int opcode = instr.x;
                regs = opps[opcodes[opcode]](regs, instr);
        }

        printf("Part 2: Reg 0 = %d\n", regs.x);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        llNode *cur;
        for (cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (strlen(str) == 0) {
                        cur = cur->next;
                        str = (char*)cur->data;
                }

                if (strlen(str) == 0) break;

                struct test test;

                char *tok = strtok(str, "[");
                for (int i = 0; i < 4; i++) {
                        test.before.raw[i] = strtol(strtok(NULL, " ]"), NULL, 10);
                }

                cur = cur->next;
                str = (char*)cur->data;
                tok = strtok(str, " ");
                for (int i = 0; i < 4; i++) {
                        test.opp.raw[i] = strtol(tok, NULL, 10);
                        tok = strtok(NULL, " ");
                }

                cur = cur->next;
                str = (char*)cur->data;
                tok = strtok(str, "[");
                for (int i = 0; i < 4; i++) {
                        test.after.raw[i] = strtol(strtok(NULL, " ]"), NULL, 10);
                }

                tal_add(input.tests, test);
        }

        for (cur = cur->next->next; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                ivec4 instr;

                char *tok = strtok(str, " ");
                for (int i = 0; i < 4; i++) {
                        instr.raw[i] = strtol(tok, NULL, 10);
                        tok = strtok(NULL, " ");
                }

                tal_add(input.instrs, instr);
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
        printf("Running Advent of Code 2018/Day16.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day16.txt";
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

