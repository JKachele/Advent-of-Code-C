/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 24, 2026 09:20:59 EDT
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

typedef tal(int) talint;

enum type {
        OPEN,
        TREE,
        LUMBER
};

struct acre {
        enum type type;
        int adjTrees;
        int adjLumber;
};

struct input {
        int size;
        struct acre **field;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printField(int size, struct acre field[][size]) {
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        char c = '.';
                        if (field[y][x].type == TREE) c = '|';
                        if (field[y][x].type == LUMBER) c = '#';
                        printf("%c", c);
                        // printf("%d ", field[y][x].adjTrees);
                        // printf("%d ", field[y][x].adjLumber);
                }
                printf("\n");
        }
        printf("\n");
}

void copyField(int size, struct acre field[][size], struct acre copy[][size]) {
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        copy[y][x].type = field[y][x].type;
                        copy[y][x].adjTrees = field[y][x].adjTrees;
                        copy[y][x].adjLumber = field[y][x].adjLumber;
                }
        }
}

bool validPos(int size, int x, int y) {
        return (x >= 0) && (y >= 0) && (x < size) && (y < size);
}

void setAdj(int size, struct acre field[][size], ivec2 pos) {
        enum type type = field[pos.y][pos.x].type;
        for (int y = pos.y - 1; y <= pos.y + 1; y++) {
                for (int x = pos.x - 1; x <= pos.x + 1; x++) {
                        if ((y == pos.y && x == pos.x) || !validPos(size, x, y))
                                continue;
                        if (type == TREE)
                                field[y][x].adjTrees++;
                        if (type == LUMBER)
                                field[y][x].adjLumber++;
                }
        }
}

int stepField(int size, struct acre field[][size]) {
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        struct acre *acre = &field[y][x];
                        if (acre->type == OPEN) {
                                if (acre->adjTrees >= 3)
                                        acre->type = TREE;
                        } else if (acre->type == TREE) {
                                if (acre->adjLumber >= 3)
                                        acre->type = LUMBER;
                        } else if (acre->type == LUMBER) {
                                if (acre->adjTrees < 1 || acre->adjLumber < 1)
                                        acre->type = OPEN;
                        }
                        acre->adjTrees = 0;
                        acre->adjLumber = 0;
                }
        }

        int numTrees = 0;
        int numLumber = 0;
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        if (field[y][x].type != OPEN)
                                setAdj(size, field, (ivec2){{x, y}});
                        if (field[y][x].type == TREE)
                                numTrees++;
                        if (field[y][x].type == LUMBER)
                                numLumber++;
                }
        }
        return numTrees * numLumber;
}

void part1(struct input *input) {
        const int NumMinutes = 10;
        int size = input->size;
        struct acre (*field)[size] = (struct acre(*)[size])calloc(size * size, sizeof(struct acre));
        copyField(size, (struct acre(*)[size])input->field, field);
        // printField(size, field);

        int value = 0;
        for (int i = 0; i < NumMinutes; i++) {
                // printf("%d minutes\n", i);
                // printField(size, field);
                value = stepField(size, field);
        }
        // printField(size, field);

        printf("Part 1: Resource Value = %d\n\n", value);
        free(field);
}

void part2(struct input *input) {
        const int NumMinutes = 1000000000;
        int size = input->size;
        struct acre (*field)[size] = (struct acre(*)[size])input->field;
        // printField(size, field);

        // Use tortoise and hare cycle detection
        talint values = tal_init();
        int tortoiseIndex = 1;
        int hareIndex = 2;
        tal_add(values, 0); // First value doesn't matter
        tal_add(values, stepField(size, field));
        tal_add(values, stepField(size, field));
        while (values.array[tortoiseIndex] != values.array[hareIndex]) {
                tal_add(values, stepField(size, field));
                tal_add(values, stepField(size, field));
                tortoiseIndex++;
                hareIndex += 2;
        }
        // printf("Tortoise = %d, Hare = %d\n", tortoiseIndex, hareIndex);
        int cycleLength = tortoiseIndex;

        // Verify a cycle has been found
        for (int i = 0; i < cycleLength; i++) {
                tal_add(values, stepField(size, field));
                tortoiseIndex++;
                hareIndex++;
                if (values.array[tortoiseIndex] != values.array[hareIndex]) {
                        printf("Cycle not found!\n");
                        break;
                }
        }

        int finalCycleIndex = (NumMinutes - cycleLength) % cycleLength;

        int value = values.array[cycleLength + finalCycleIndex];
        printf("Part 2: Resource Value = %d\n", value);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.size = Debug ? 10 : 50;
        int size = input.size;
        input.field = calloc(size * size, sizeof(struct acre));
        struct acre (*field)[size] = (struct acre(*)[size])input.field;

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < size && x < (int)strlen(str); x++) {
                        if (str[x] == '|') {
                                field[y][x].type = TREE;
                        } else if (str[x] == '#') {
                                field[y][x].type = LUMBER;
                        } else {
                                field[y][x].type = OPEN;
                        }
                        setAdj(size, field, (ivec2){{x, y}});
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day18.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day18.txt";
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

