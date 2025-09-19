/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 19, 2025 16:43:53 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
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

typedef struct insertion {
        u16 startPair;
        u16 endPairs[2];
} insertion;
typedef tll(insertion) tllinsertion;

typedef struct pair {
        char elements[2];
        u16 hash;
} pair;
typedef tll(pair) tllpair;

struct input {
        int64 *pairs;
        tllinsertion insertions;
        tllpair pairList;
        char end;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void hashCollision(char *str, u16 hash, char *check[]) {
        if (check[hash] == NULL) {
                check[hash] = str;
                return;
        }
        if (strcmp(str, check[hash]) != 0) {
                printf("Collision at %u: %s and %s\n", hash, str, check[hash]);
                exit(1);
        }
}

void stepPolymer(int64 *pairs, tllinsertion insertions) {
        int64 *newPairs = calloc(0x10000, sizeof(int64));
        tll_foreach(insertions, it) {
                insertion rule = it->item;
                newPairs[rule.endPairs[0]] += pairs[rule.startPair];
                newPairs[rule.endPairs[1]] += pairs[rule.startPair];
        }
        memcpy(pairs, newPairs, 0x10000 * sizeof(int64));
        free(newPairs);
}

int64 countPairs(int64 *pairs, tllpair pairList, char end) {
        int64 elementCounts[26] = {0};
        elementCounts[end - 'A']++;

        // Only add first char of pair to not double count
        tll_foreach(pairList, it) {
                pair p = it->item;
                int i = p.elements[0] - 'A';
                elementCounts[i] += pairs[p.hash];
        }

        // Most common - least common
        int64 max = 0;
        int64 min = INT64_MAX;
        for (int i=0; i<26; i++) {
                if (elementCounts[i] > max)
                        max = elementCounts[i];
                if (elementCounts[i] != 0 && elementCounts[i] < min)
                        min = elementCounts[i];
        }
        return max - min;
}

void part1(struct input input) {
        const int32 NumSteps = 10;
        tllinsertion insertions = input.insertions;
        tllpair pairList = input.pairList;
        int64 *pairs = malloc(0x10000 * sizeof(int64));
        memcpy(pairs, input.pairs, 0x10000 * sizeof(int64));

        for (int i=0; i<NumSteps; i++) {
                stepPolymer(pairs, insertions);
        }

        int64 count = countPairs(pairs, pairList, input.end);

        printf("Part 1: %ld\n\n", count);
}

void part2(struct input input) {
        const int32 NumSteps = 40;
        tllinsertion insertions = input.insertions;
        tllpair pairList = input.pairList;
        int64 *pairs = malloc(0x10000 * sizeof(int64));
        memcpy(pairs, input.pairs, 0x10000 * sizeof(int64));

        for (int i=0; i<NumSteps; i++) {
                stepPolymer(pairs, insertions);
        }

        int64 count = countPairs(pairs, pairList, input.end);

        printf("Part 1: %ld\n\n", count);
}

struct input parseInput(llist *ll) {
        const u32 SEED = 0x12345678;
        struct input input = {0};
        input.pairs = calloc(0x10000, sizeof(int64));
        int64 *pairs = input.pairs;
        tllinsertion *insertions = &input.insertions;
        tllpair *pairList = &input.pairList;

        char *t = (char*)ll->head->data;
        for (int i=1; i<(int)strlen(t); i++) {
                char pair[] = {t[i-1], t[i], '\0'};
                u16 hash = (u16)FNV(pair, SEED);
                pairs[hash]++;
        }
        input.end = t[strlen(t)-1];

        char *check[0x10000] = {0};
        llNode *current = ll->head->next->next;
        while(current != NULL) {
                char *str = (char*)current->data;

                insertion rule;
                pair p;

                char *startPair = strtok(str, " ");
                u16 startHash = (u16)FNV(startPair, SEED);
                hashCollision(str, startHash, check);
                rule.startPair = startHash;

                p.elements[0] = startPair[0];
                p.elements[1] = startPair[1];
                p.hash = startHash;
                tll_push_back(*pairList, p);

                char insertChar = str[6];
                char endPair1[] = {startPair[0], insertChar, '\0'};
                char endPair2[] = {insertChar, startPair[1], '\0'};
                rule.endPairs[0] = (u16)FNV(endPair1, SEED);
                rule.endPairs[1] = (u16)FNV(endPair2, SEED);

                tll_push_back(*insertions, rule);

                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day14.txt";
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

