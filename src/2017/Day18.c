/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 20, 2026 08:49:20 EDT
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
#define QUEUE_SIZE 1000

enum opperation {
        SND,
        SET,
        ADD,
        MUL,
        MOD,
        RCV,
        JGZ
};
const char *OPPS[] = {"SND", "SET", "ADD", "MUL", "MOD", "RCV", "JGZ"};

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
                if (instrs[i].opp != SND && instrs[i].opp != RCV) {
                        if (instrs[i].yReg) {
                                printf("%c ", instrs[i].y + 'a');
                        } else {
                                printf("%d ", instrs[i].y);
                        }
                }
                printf("\n");
        }
}

void printInstr(instruction instr, int64 regs[], int pc) {
        printf("%d: %s ", pc, OPPS[instr.opp]);
        if (instr.xReg) {
                printf("%c(%ld) ", instr.x + 'a', regs[instr.x]);
        } else {
                printf("%d ", instr.x);
        }
        if (instr.opp != SND && instr.opp != RCV) {
                if (instr.yReg) {
                        printf("%c(%ld) ", instr.y + 'a', regs[instr.y]);
                } else {
                        printf("%d ", instr.y);
                }
        }
        printf("\n");
}

bool queueEmpty(ivec2 rwPtr) {
        return ((rwPtr.a + 1) % QUEUE_SIZE == rwPtr.b);
}

// Returns true on wait for rcv data
bool execInstr(instruction instr, int64 regs[], int *pc,
                int64 sndQ[], ivec2 *sndRW, int64 rcvQ[], ivec2 *rcvRW) {
        int64 x = instr.xReg ? regs[instr.x] : instr.x;
        int64 y = instr.yReg ? regs[instr.y] : instr.y;

        bool rcv = false;
        switch (instr.opp) {
        case SND:
                sndQ[sndRW->b] = x;
                if (sndRW->b == sndRW->a) {
                        fprintf(stderr, "Queue Full!\n");
                }
                sndRW->b = (sndRW->b + 1) % QUEUE_SIZE;
                break;
        case SET:
                regs[instr.x] = y;
                break;
        case ADD:
                regs[instr.x] = x + y;
                break;
        case MUL:
                regs[instr.x] = x * y;
                break;
        case MOD:
                regs[instr.x] = x % y;
                break;
        case RCV:
                if (queueEmpty(*rcvRW)) {
                        // Dont increment pc
                        return true;
                }
                rcvRW->a = (rcvRW->a + 1) % QUEUE_SIZE;
                regs[instr.x] = rcvQ[rcvRW->a];
                break;
        case JGZ:
                if (x > 0) {
                        *pc = *pc + (y - 1);
                }
                break;
        }
        (*pc)++;
        return false;
}

void part1(struct input *input) {
        int numInstr = input->numInstr;
        instruction *instrs = input->instrs;
        // printInstrs(numInstr, instrs);

        int64 regs[26] = {0};

        int pc = 0;
        int64 lastSound = 0;
        int64 rcvSound = 0;
        while (pc >= 0 && pc < numInstr) {
                instruction instr = instrs[pc];
                // printInstr(instr, regs, pc);

                int64 x = instr.xReg ? regs[instr.x] : instr.x;
                int64 y = instr.yReg ? regs[instr.y] : instr.y;

                bool rcv = false;
                switch (instr.opp) {
                case SND:
                        lastSound = x;
                        break;
                case SET:
                        regs[instr.x] = y;
                        break;
                case ADD:
                        regs[instr.x] = x + y;
                        break;
                case MUL:
                        regs[instr.x] = x * y;
                        break;
                case MOD:
                        regs[instr.x] = x % y;
                        break;
                case RCV:
                        if (x != 0) {
                                rcvSound = lastSound;
                                rcv = true;
                        }
                        break;
                case JGZ:
                        if (x > 0) {
                                pc += y - 1;
                        }
                        break;
                }
                pc++;
                if (rcv)
                        break;
        }

        printf("Part 1: Recovered Sound = %ld\n\n", rcvSound);
}

void part2(struct input *input) {
        int numInstr = input->numInstr;
        instruction *instrs = input->instrs;
        // printInstrs(numInstr, instrs);
        
        int64 regs0[26] = {0};
        int64 regs1[26] = {0};
        regs1[15] = 1;  // reg p starts with program id

        int64 queue0[QUEUE_SIZE];
        int64 queue1[QUEUE_SIZE];
        ivec2 rw0 = {{QUEUE_SIZE-1, 0}};
        ivec2 rw1 = {{QUEUE_SIZE-1, 0}};

        int pc0 = 0;
        int pc1 = 0;
        int64 p1SndCount = 0;
        bool p0Lock = false;
        bool p1Lock = false;
        while (pc0 >= 0 && pc0 < numInstr && pc1 >= 0 && pc1 < numInstr) {
                while (!p0Lock) {
                        p0Lock = execInstr(instrs[pc0], regs0, &pc0, queue1, &rw1, queue0, &rw0);
                }
                while (!p1Lock) {
                        if (instrs[pc1].opp == SND) { p1SndCount++; }
                        p1Lock = execInstr(instrs[pc1], regs1, &pc1, queue0, &rw0, queue1, &rw1);
                }

                if (p0Lock && !queueEmpty(rw0)) {
                        p0Lock = false;
                }
                if (p1Lock && !queueEmpty(rw1)) {
                        p1Lock = false;
                }

                if (p0Lock && p1Lock) {
                        break;
                }
        }

        printf("Part 2: Program 1 sent %ld values\n", p1SndCount);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstr = ll->length;
        input.instrs = calloc(input.numInstr, sizeof(instruction));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                if (str[0] == 's' && str[1] == 'n') {           // snd
                        input.instrs[i].opp = SND;
                } else if (str[0] == 's' && str[1] == 'e') {    // set
                        input.instrs[i].opp = SET;
                } else if (str[0] == 'a') {                     // add
                        input.instrs[i].opp = ADD;
                } else if (str[0] == 'm' && str[1] == 'u') {    // mul
                        input.instrs[i].opp = MUL;
                } else if (str[0] == 'm' && str[1] == 'o') {    // mod
                        input.instrs[i].opp = MOD;
                } else if (str[0] == 'r') {                     // rcv
                        input.instrs[i].opp = RCV;
                } else {                                        // jgz
                        input.instrs[i].opp = JGZ;
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
        printf("Running Advent of Code 2017/Day18.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day18.txt";
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
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
        //                 parseTime, pt1Time);

        return 0;
}

