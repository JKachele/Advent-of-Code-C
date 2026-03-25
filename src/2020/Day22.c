/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 13:24:09 UTC
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
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(int) tllint;

struct input {
        tllint deck1;
        tllint deck2;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printDeck(tllint deck) {
        tll_foreach(deck, it) {
                printf("%d ", it->item);
        }
        printf("\n");
}

void part1(struct input input) {
        tllint deck1 = input.deck1;
        tllint deck2 = input.deck2;

        while (tll_length(deck1) > 0 && tll_length(deck2) > 0) {
                int card1 = tll_pop_front(deck1);
                int card2 = tll_pop_front(deck2);
                
                if (card1 > card2) {
                        tll_push_back(deck1, card1);
                        tll_push_back(deck1, card2);
                } else {
                        tll_push_back(deck2, card2);
                        tll_push_back(deck2, card1);
                }
        }

        tllint winner = (tll_length(deck1) > 0) ? deck1 : deck2;

        int multiplier = tll_length(winner);
        u64 score = 0;
        tll_foreach(winner, it) {
                score += multiplier * it->item;
                multiplier--;
        }

        printf("Part 1: %lu\n\n", score);
}

void part2(struct input input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        tllint *deck = &input.deck1;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (str[0] == 'P')
                        continue;
                if (strlen(str) == 0) {
                        deck = &input.deck2;
                        continue;
                }
                int num = strtol(str, NULL, 10);
                tll_push_back(*deck, num);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day22.txt";
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

