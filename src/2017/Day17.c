/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Oct 19, 2026 14:58:20 EDT
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
// #include "../util/talist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

struct node {
        int value;
        struct node *next;
};

struct input {
        int steps;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBuffer(struct node *buf) {
        struct node *cur = buf;
        do {
                printf("%d ", cur->value);
                cur = cur->next;
        } while (cur != buf);
        printf("\n");
}

void part1(struct input *input) {
        int steps = input->steps;

        struct node *cur = malloc(sizeof(struct node));
        cur->value = 0;
        cur->next = cur;

        for (int i = 1; i <= 2017; i++) {
                int curSteps = steps % i;
                for (int j = 0; j < curSteps; j++) {
                        cur = cur->next;
                }
                struct node *new = malloc(sizeof(struct node));
                new->value = i;
                new->next = cur->next;
                cur->next = new;
                cur = new;
        }
        // printBuffer(cur);

        int nextvalue = cur->next->value;
        struct node *head = cur;
        do{
                struct node *tofree = cur;
                cur = cur->next;
                free(tofree);
        } while (cur != head);

        printf("Part 1: %d\n\n", nextvalue);
}

void part2(struct input *input) {
        int steps = input->steps;

        struct node *cur = malloc(sizeof(struct node));
        cur->value = 0;
        cur->next = cur;

        for (int i = 1; i <= 50000000; i++) {
                int curSteps = steps % i;
                for (int j = 0; j < curSteps; j++) {
                        cur = cur->next;
                }
                struct node *new = malloc(sizeof(struct node));
                new->value = i;
                new->next = cur->next;
                cur->next = new;
                cur = new;
        }
        // printBuffer(cur);

        while (cur->value != 0) {
                cur = cur->next;
        }
        int nextvalue = cur->next->value;

        struct node *head = cur;
        do{
                struct node *tofree = cur;
                cur = cur->next;
                free(tofree);
        } while (cur != head);

        printf("Part 2: %d\n", nextvalue);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.steps = strtol(str, NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day17.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day17.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day17.txt";
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

