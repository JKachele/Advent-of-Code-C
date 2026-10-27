/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 27, 2026 13:33:25 EDT
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
        RECT,
        ROTR,
        ROTC
};

struct instruction {
        enum opperation opp;
        int a;
        int b;
};

struct input {
        int numInstrs;
        struct instruction *instrs;
};

static ivec2 SIZE = {{50, 6}};

void printDisplay(bool display[][SIZE.x]) {
        for (int y = 0; y < SIZE.y; y++) {
                for (int x = 0; x < SIZE.x; x++) {
                        printf("%c", display[y][x] ? '#' : ' ');
                }
                printf("\n");
        }
        printf("\n");
}

void rect(bool display[][SIZE.x], int a, int b) {
        for (int y = 0; y < b; y++) {
                for (int x = 0; x < a; x++) {
                        display[y][x] = true;
                }
        }
}

void rotr(bool display[][SIZE.x], int a, int b) {
        bool temp[SIZE.x];
        for (int x = 0; x < SIZE.x; x++) {
                temp[x] = display[a][x];
        }
        for (int x = 0; x < SIZE.x; x++) {
                display[a][(x + b) % SIZE.x] = temp[x];
        }
}

void rotc(bool display[][SIZE.x], int a, int b) {
        bool temp[SIZE.y];
        for (int y = 0; y < SIZE.y; y++) {
                temp[y] = display[y][a];
        }
        for (int y = 0; y < SIZE.y; y++) {
                display[(y + b) % SIZE.y][a] = temp[y];
        }
}

void part1_2(struct input *input) {
        int numInstrs = input->numInstrs;
        struct instruction *instrs = input->instrs;

        bool display[SIZE.y][SIZE.x];
        for (int y = 0; y < SIZE.y; y++) {
                for (int x = 0; x < SIZE.x; x++) {
                        display[y][x] = false;
                }
        }

        for (int i = 0; i < numInstrs; i++) {
                struct instruction instr = instrs[i];
                switch (instr.opp) {
                case RECT:
                        rect(display, instr.a, instr.b);
                        break;
                case ROTR:
                        rotr(display, instr.a, instr.b);
                        break;
                case ROTC:
                        rotc(display, instr.a, instr.b);
                        break;
                }
                // printf("%d, a:%d, b:%d\n", instr.opp, instr.a, instr.b);
                // printDisplay(display);
        }

        int numLit = 0;
        for (int y = 0; y < SIZE.y; y++) {
                for (int x = 0; x < SIZE.x; x++) {
                        if(display[y][x]) {
                                numLit++;
                        }
                }
        }

        printf("Part 1: Pixels Lit = %d\n\n", numLit);

        printf("Part 2: \n");
        printDisplay(display);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstrs = ll->length;
        input.instrs = malloc(input.numInstrs * sizeof(struct instruction));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct instruction instr;

                char *opp = strtok(str, " ");
                if (opp[1] == 'e') {
                        // RECT
                        instr.opp = RECT;
                        instr.a = strtol(strtok(NULL, "x"), NULL, 10);;
                        instr.b = strtol(strtok(NULL, ""), NULL, 10);;
                } else {
                        opp = strtok(NULL, " ");
                        if (opp[0] == 'r') {
                                // Rotate Row
                                instr.opp = ROTR;
                        } else {
                                // Rotate Column
                                instr.opp = ROTC;
                        }
                        instr.a = strtol(strtok(NULL, " ") + 2, NULL, 10);
                        strtok(NULL, " ");
                        instr.b = strtol(strtok(NULL, ""), NULL, 10);
                }

                input.instrs[i++] = instr;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day08.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                SIZE = (ivec2){{7, 3}};
        } else {
                char *file = "assets/inputs/2016/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
                        parseTime, pt1Time, totalTime);

        return 0;
}

