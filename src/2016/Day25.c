/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Nov 05, 2026 08:46:11 EST
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
        JNZ,
        TGL,
        OUT
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
        const char *OPPS[5] = {"cpy", "inc", "dec", "jnz", "tgl"};
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

bool runProg(int64 regs[], int numInstrs, struct instruction instrs[]) {
        int pc = 0;
        int instret = 0;
        int lastOut = 1;
        int numOut = 0;
        bool validClock = false;
        bool badOutput = false;
        while (pc >= 0 && pc < numInstrs) {
                struct instruction instr = instrs[pc];
                int64 x = instr.xReg ? regs[instr.x] : instr.x;
                int64 y = instr.yReg ? regs[instr.y] : instr.y;

                switch (instr.opp) {
                case CPY:
                        if (!instr.yReg) { break; }     // INVALID!
                        regs[instr.y] = x;
                        break;
                case INC:
                        if (!instr.xReg) { break; }     // INVALID!
                        regs[instr.x]++;
                        break;
                case DEC:
                        if (!instr.xReg) { break; }     // INVALID!
                        regs[instr.x]--;
                        break;
                case JNZ:
                        if (x != 0) {
                                pc += y;
                                pc--;
                        }
                        break;
                case TGL:
                        if (pc + x >= numInstrs) { break; }
                        if (instrs[pc+x].opp == CPY) {
                                instrs[pc+x].opp = JNZ;
                        } else if (instrs[pc+x].opp == JNZ) {
                                instrs[pc+x].opp = CPY;
                        } else if (instrs[pc+x].opp == INC) {
                                instrs[pc+x].opp = DEC;
                        } else {
                                instrs[pc+x].opp = INC;
                        }
                        break;
                case OUT:
                        if ((lastOut == 1 && x != 0) || (lastOut == 0 && x != 1)) {
                                badOutput = true;
                        } else {
                                lastOut = x;
                                numOut++;
                        }
                        break;
                }
                instret++;
                pc++;

                if (badOutput) {
                        validClock = false;
                        break;
                }
                if (instret > 1000001) {
                        validClock = true;
                        break;
                }
        }
        return validClock && numOut > 100;
}

void prog(int64 input) {
        int64 a = input + 2532;
        int64 b = 0;
        int64 c = 0;
        int64 d = input + 2532;
        for (int i = 0; i < 10; i++) {
                b = a;
                a = 0;

                c = 2;
                for (;;) {
                        if (b == 0) {
                                break;
                        }
                        b--;
                        c--;
                        if (c == 0) {
                                a++;
                                c = 2;
                        }
                }
                // a = b / 2
                // c = (if a was even at top of loop) 2, else 1

                printf("%d: %ld, %ld, %ld, %ld\n", i, a, b, c, d);

                b = 2;
                for (;;) {
                        if (c == 0) {
                                break;
                        }
                        b--;
                        c--;
                }
                // b = input % 2

                printf("%ld\n", b);
                if (a == 0) {
                        a = d;
                }
        }
}

void part1(struct input *input) {
        // int numInstrs = input->numInstrs;
        // struct instruction instrs[numInstrs];
        // memcpy(instrs, input->instrs, numInstrs * sizeof(struct instruction));

        // prog(0);

        // Need to find even number over 2532 where dividing it by 2 repeatedly produces
        // alternating even and odd numbers.
        int num = 0;
        for (;;) {
                int n = num + 2532;
                bool even = true;
                bool found = true;
                while (n > 0) {
                        n /= 2;
                        bool curEven = (n % 2 == 0);
                        if (curEven == even) {
                                found = false;
                                break;
                        }
                        even = curEven;
                }
                if (found) {
                        break;
                }
                num += 2;
        }

        printf("Part 1: Register A = %d\n\n", num);
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
                case 't':
                        instr.opp = TGL;
                        break;
                case 'o':
                        instr.opp = OUT;
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
        printf("Running Advent of Code 2016/Day25.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Total: %f\n", 
                        parseTime, pt1Time, totalTime);

        return 0;
}

