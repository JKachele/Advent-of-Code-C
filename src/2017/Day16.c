/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 16, 2026 13:48:56 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stddef.h>
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

#define INPUT_BUFFER_SIZE 65536
#define NUM_PROGS 16

typedef tal(u64) talu64;

enum move_t {
        SPIN,
        EXCHANGE,
        PARTNER
};

struct move {
        enum move_t type;
        u8 param1;
        u8 param2;
};
typedef tal(struct move) talmove;

struct input {
        talmove moves;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMoves(talmove moves) {
        tal_for(moves, i) {
                struct move m = moves.array[i];
                if (m.type == SPIN) {
                        printf("Spin %d\n", m.param1);
                } else if (m.type == EXCHANGE) {
                        printf("Exchange %d and %d\n", m.param1, m.param2);
                } else {
                        printf("Partner %c and %c\n", m.param1 + 'a', m.param2 + 'a');
                }
        }
}

struct move parseMove(char *input) {
        struct move m = {0};
        if (input[0] == 's') {
                m.type = SPIN;
                m.param1 = strtol(input + 1, NULL, 10);
        } else if (input[0] == 'x') {
                m.type = EXCHANGE;
                char *end;
                m.param1 = strtol(input+1, &end, 10);
                m.param2 = strtol(end+1, NULL, 10);
        } else {
                m.type = PARTNER;
                m.param1 = input[1] - 'a';
                m.param2 = input[3] - 'a';
        }
        return m;
}

void spin(u8 progs[], int size) {
        u8 temp[NUM_PROGS];
        for (int i = 0; i < NUM_PROGS; i++) { temp[i] = progs[i]; }

        // Shift front of line back
        for (int i = 0; i < NUM_PROGS - size; i++) {
                progs[i + size] = temp[i];
        }

        // Move progs at back to front
        for (int i = 0; i < size; i++) {
                int tempI = (NUM_PROGS - size) + i;
                progs[i] = temp[tempI];
        }
}

void exchange(u8 progs[], int p1, int p2) {
        u8 temp = progs[p1];
        progs[p1] = progs[p2];
        progs[p2] = temp;
}

void partner(u8 progs[], int p1, int p2) {
        int index1 = 0;
        int index2 = 0;
        for (int i = 0; i < NUM_PROGS; i++) {
                if (progs[i] == p1)
                        index1 = i;
                if (progs[i] == p2)
                        index2 = i;
        }
        exchange(progs, index1, index2);
}

void performMove(u8 progs[], struct move move) {
        switch (move.type) {
        case SPIN:
                spin(progs, move.param1);
                break;
        case EXCHANGE:
                exchange(progs, move.param1, move.param2);
                break;
        case PARTNER:
                partner(progs, move.param1, move.param2);
                break;
        }
}


void part1(struct input *input) {
        talmove moves = input->moves;
        // printMoves(moves);

        u8 progs[NUM_PROGS];
        for (int i = 0; i < NUM_PROGS; i++) { progs[i] = i; }

        tal_for(moves, i) {
                struct move move = moves.array[i];
                performMove(progs, move);
        }

        printf("Part 1: ");
        for (int i = 0; i < NUM_PROGS; i++) {
                printf("%c", progs[i] + 'a');
        }
        printf("\n\n");
}

void part2(struct input *input) {
        const int NumSteps = 1000000000; // 1 billion

        talmove moves = input->moves;
        // printMoves(moves);

        // Using Tortoise and hare cycle detection
        u8 progsT[NUM_PROGS];
        u8 progsH[NUM_PROGS];
        for (int i = 0; i < NUM_PROGS; i++) {
                progsT[i] = i;
                progsH[i] = i;
        }

        int tortI = 0;
        int hareI = 0;
        for (;;) {
                // Move Tortoise 1 step
                performMove(progsT, moves.array[(tortI++) % moves.length]);
                // Move Hare 2 steps
                performMove(progsH, moves.array[(hareI++) % moves.length]);
                performMove(progsH, moves.array[(hareI++) % moves.length]);

                // Compare tortoise ans hare
                bool equal = true;
                for (int i = 0; i < NUM_PROGS; i++) {
                        if (progsT[i] != progsH[i]) {
                                equal = false;
                                break;
                        }
                }
                if (equal) {
                        break;
                }
        }

        // Dont need min cycle length. I'm lazy 
        int cycleLength = hareI - tortI;
        int finalCycleIndex = (NumSteps - tortI) % cycleLength;

        // Will use tortiose to get final state
        for (int i = 0; i < finalCycleIndex; i++) {
                performMove(progsT, moves.array[(tortI++) % moves.length]);
        }

        printf("Part 2: ");
        for (int i = 0; i < NUM_PROGS; i++) {
                printf("%c", progsT[i] + 'a');
        }
        printf("\n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *tok = strtok(str, ",");
        while (tok != NULL) {
                struct move m = parseMove(tok);
                tal_add(input.moves, m);
                tok = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day16.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day16.txt";
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

