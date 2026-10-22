/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 22, 2026 11:07:39 EDT
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

enum opperation {
        SET,
        SUB,
        MUL,
        JNZ
};
const char *OPPS[] = {"SET", "SUB", "MUL", "JNZ"};

typedef struct instruction {
        enum opperation opp;
        int x;
        bool xReg;
        int y;
        bool yReg;
} instruction;

struct input {
        int numInstr;
        instruction *instrs;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printInstrs(int numInstr, instruction *instrs) {
        for (int i = 0; i < numInstr; i++) {
                printf("%s ", OPPS[instrs[i].opp]);
                if (instrs[i].xReg) {
                        printf("%c ", instrs[i].x + 'a');
                } else {
                        printf("%d ", instrs[i].x);
                }
                if (instrs[i].yReg) {
                        printf("%c ", instrs[i].y + 'a');
                } else {
                        printf("%d ", instrs[i].y);
                }
                printf("\n");
        }
}

bool isPrime(int64 n) {
        for (int i = 2; i*i <= n; i++) {
                if (n % i == 0) {
                        return false;
                }
        }
        return true;
}

/*
b = 106500
c = 123500
for(;;) {
        f = 1
        d = 2
        do {
                e = 2
                do {
                        g = (d * e) - b
                        if (g == 0)     // if (d * e == b)
                                f = 0
                        e += 1
                        g = e - b
                } while (g != 0) // while (e != b)
                d += 1
                g = d - b
        } while (g != 0) // while (d != b)
        if (f == 0)
                h += 1
        g = b - c
        if (g == 0) // if (b == c)
                return
        b += 17
}

H will only increment if for some values d and e, d * e == b.
B starts at 106500 and increments in steps of 17 to 123500
So, h will only increment if B is not prime
 */
int64 getRegH(void) {
        int64 b = 106500;
        int64 c = 123500;
        int64 h = 0;

        while (b <= c) {
                if (!isPrime(b)) {
                        h++;
                }
                b += 17;
        }

        return h;
}

void part1(struct input *input) {
        int numInstr = input->numInstr;
        instruction *instrs = input->instrs;
        // printInstrs(numInstr, instrs);

        int64 regs[8] = {0};

        int pc = 0;
        int numMul = 0;
        while (pc >= 0 && pc < numInstr) {
                instruction instr = instrs[pc];

                int64 x = instr.xReg ? regs[instr.x] : instr.x;
                int64 y = instr.yReg ? regs[instr.y] : instr.y;

                switch (instr.opp) {
                case SET:
                        regs[instr.x] = y;
                        break;
                case SUB:
                        regs[instr.x] = x - y;
                        break;
                case MUL:
                        regs[instr.x] = x * y;
                        numMul++;
                        break;
                case JNZ:
                        if (x != 0) {
                                pc += y - 1;
                        }
                        break;
                }
                pc++;
        }

        printf("Part 1: mul Instruction = %d\n\n", numMul);
}

void part2(struct input *input) {
        printf("Part 2: Reg H = %ld\n", getRegH());
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstr = ll->length;
        input.instrs = calloc(input.numInstr, sizeof(instruction));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                if (str[0] == 's' && str[1] == 'e') {           // set
                        input.instrs[i].opp = SET;
                } else if (str[0] == 's' && str[1] == 'u') {    // sub
                        input.instrs[i].opp = SUB;
                } else if (str[0] == 'm') {                     // mul
                        input.instrs[i].opp = MUL;
                } else {                                        // jnz
                        input.instrs[i].opp = JNZ;
                }

                if (str[4] >= 'a') {
                        input.instrs[i].x = str[4] - 'a';
                        input.instrs[i].xReg = true;
                } else {
                        input.instrs[i].x = str[4] - '0';
                        input.instrs[i].xReg = false;
                }

                if (strlen(str) > 5) {
                        if (str[6] >= 'a') {
                                input.instrs[i].y = str[6] - 'a';
                                input.instrs[i].yReg = true;
                        } else {
                                input.instrs[i].y = strtol(str+6, NULL, 10);
                                input.instrs[i].yReg = false;
                        }
                }

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day23.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day23.txt";
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

