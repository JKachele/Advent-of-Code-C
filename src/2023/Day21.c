/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 04, 2025 14:59:55 UTC
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
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096
#define NUM_STEPS 64

typedef tll(int32) tllint32;

typedef struct plot {
        bool open;
        bool steps[NUM_STEPS+1];
} plot;

typedef struct step {
        ivec2 pos;
        int32 num;
} step;

typedef tll(step) tllstep;

struct input {
        ivec2 size;
        plot **field;
        ivec2 start;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printField(ivec2 size, plot field[][size.x], int num) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (field[y][x].steps[num])
                                printf("O");
                        else if (field[y][x].open)
                                printf(".");
                        else
                                printf("#");
                }
                printf("\n");
        }
        printf("\n");
}

void resetField(ivec2 size, plot field[][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        memset(field[y][x].steps, 0, (NUM_STEPS+1)*sizeof(bool));
                }
        }
}

int64 walkField(ivec2 size, plot field[][size.x], ivec2 start) {
        const ivec2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        tllstep queue = tll_init();
        step initStep = {start, 0};
        tll_push_back(queue, initStep);

        while (tll_length(queue) > 0) {
                step curS = tll_pop_front(queue);
                if (curS.num == NUM_STEPS)
                        continue;

                for (int i=0; i<4; i++) {
                        ivec2 newPos = addIVec2(curS.pos, dirs[i]);
                        plot *newPlot = &field[newPos.y][newPos.x];
                        if (newPlot->open && !newPlot->steps[curS.num+1]) {
                                newPlot->steps[curS.num+1] = true;
                                step newStep = {newPos, curS.num+1};
                                tll_push_back(queue, newStep);
                        }
                }
        }

        int32 numReached = 0;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (field[y][x].steps[NUM_STEPS])
                                numReached++;
                }
        }

        return numReached;
}

void part1(struct input input) {
        ivec2 size = input.size;
        plot (*field)[size.x] = (plot(*)[size.x])input.field;
        resetField(size, field);
        ivec2 start = input.start;

        int64 numPlotsReached = walkField(size, field, start);
        printField(size, field, NUM_STEPS);

        printf("Part 1: %ld\n\n", numPlotsReached);
}

void part2(struct input input) {
        ivec2 size = input.size;
        plot (*field)[size.x] = (plot(*)[size.x])input.field;
        resetField(size, field);
        ivec2 start = input.start;


        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        // Adding buffer of rocks around field
        ivec2 size = {getLongestLine(ll) + 2, ll->length + 2};
        plot field[size.y][size.x];
        memset(field, 0, size.y*size.x*sizeof(plot));
        ivec2 start = {0};

        llNode *current = ll->head;
        int32 yIndex = 1;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                for (int x = 1; x < size.x - 1; x++) {
                        if (str[x-1] == '.') {
                                field[yIndex][x].open = true;
                        } else if (str[x-1] == 'S') {
                                field[yIndex][x].open = true;
                                start = (ivec2){x, yIndex};
                        }
                }

                current = current->next;
                yIndex++;
        }
        printField(size, field, 0);
        struct input input = {size, (plot**)field, start};

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day21.txt";
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

