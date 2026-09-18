/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 18, 2026 12:14:38 EDT
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

struct marble {
        int num;
        struct marble *next;
        struct marble *prev;
};

struct input {
        int players;
        int lastMarble;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printCircle(struct marble *node, int player, int score) {
        int lastNode = node->num;
        while (node->num != 0) node = node->next;

        printf("[%02d: %04d]  %d ", player + 1, score, node->num);
        node = node->next;

        while (node->num != 0) {
                int num = node->num;
                if (num == lastNode)
                        printf("(%d) ", node->num);
                else
                        printf("%d ", node->num);
                node = node->next;
        }
        printf("\n");
}

void part1(struct input *input) {
        // Place first marble
        struct marble *cur = malloc(sizeof(struct marble));
        cur->num = 0;
        cur->next = cur;
        cur->prev = cur;

        int scores[input->players];
        for (int i = 0; i < input->players; i++) scores[i] = 0;

        int player = 0;
        for (int i = 1; i <= input->lastMarble; i++) {
                if (i % 23 != 0) {
                        struct marble *new = malloc(sizeof(struct marble));
                        new->num = i;
                        new->next = cur->next->next;
                        new->prev = cur->next;
                        cur->next->next->prev = new;
                        cur->next->next = new;
                        cur = new;
                } else {
                        scores[player] += i;
                        cur = cur->prev->prev->prev->prev->prev->prev;
                        struct marble *toRemove = cur->prev;
                        scores[player] += toRemove->num;
                        toRemove->prev->next = toRemove->next;
                        toRemove->next->prev = toRemove->prev;
                        free(toRemove);
                }
                // printCircle(cur, player, scores[player]);
                player = (player + 1) % input->players;
        }

        // Find max score
        int maxScore = 0;
        for (int i = 0; i < input->players; i++) {
                // printf("[%d] ", scores[i]);
                if (scores[i] > maxScore)
                        maxScore = scores[i];
        }
        // printf("\n");

        printf("Part 1: High Score = %d\n\n", maxScore);
}

void part2(struct input *input) {
        input->lastMarble *= 100;

        // Place first marble
        struct marble *cur = malloc(sizeof(struct marble));
        cur->num = 0;
        cur->next = cur;
        cur->prev = cur;

        long scores[input->players];
        for (int i = 0; i < input->players; i++) scores[i] = 0;

        int player = 0;
        for (int i = 1; i <= input->lastMarble; i++) {
                if (i % 23 != 0) {
                        struct marble *new = malloc(sizeof(struct marble));
                        new->num = i;
                        new->next = cur->next->next;
                        new->prev = cur->next;
                        cur->next->next->prev = new;
                        cur->next->next = new;
                        cur = new;
                } else {
                        scores[player] += i;
                        cur = cur->prev->prev->prev->prev->prev->prev;
                        struct marble *toRemove = cur->prev;
                        scores[player] += toRemove->num;
                        toRemove->prev->next = toRemove->next;
                        toRemove->next->prev = toRemove->prev;
                        free(toRemove);
                }
                // printCircle(cur, player, scores[player]);
                player = (player + 1) % input->players;
        }

        // Find max score
        long maxScore = 0;
        for (int i = 0; i < input->players; i++) {
                // printf("[%ld] ", scores[i]);
                if (scores[i] > maxScore)
                        maxScore = scores[i];
        }
        // printf("\n");

        printf("Part 2: High Score = %ld\n", maxScore);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.players = strtol(strtok(str, " "), NULL, 10);
        strtok(NULL, "w");
        strtok(NULL, " ");
        input.lastMarble = strtol(strtok(NULL, " "), NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day09.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day09.txt";
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

