/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 18, 2026 11:46:34 EDT
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
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 65536

typedef tal(int) talint;

struct input {
        talint tree;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int metadataSum(talint tree, int *index) {
        int sum = 0;

        // Read node header
        int numChildren = tree.array[(*index)++];
        int numMetadata = tree.array[(*index)++];

        // Evaluate child nodes
        for (int i = 0; i < numChildren; i++) {
                sum += metadataSum(tree, index);
        }

        // Sum metadata
        for (int i = 0; i < numMetadata; i++) {
                sum += tree.array[(*index)++];
        }

        return sum;
}

int nodeValue(talint tree, int *index) {
        // Read node header
        int numChildren = tree.array[(*index)++];
        int numMetadata = tree.array[(*index)++];

        // Store child node values
        int childValues[numChildren + 1];
        childValues[0] = 0;

        // Evaluate child nodes
        for (int i = 1; i <= numChildren; i++) {
                childValues[i] = nodeValue(tree, index);
        }

        // Get node value
        int value = 0;
        for (int i = 0; i < numMetadata; i++) {
                int metadata = tree.array[(*index)++];
                if (numChildren == 0)
                        value += metadata;
                else if (metadata <= numChildren)
                        value += childValues[metadata];
        }

        return value;
}

void part1(struct input *input) {
        talint tree = input->tree;

        int index = 0;
        int metadata = metadataSum(tree, &index);

        printf("Part 1: Metadata Sum = %d\n\n", metadata);
}

void part2(struct input *input) {
        talint tree = input->tree;

        int index = 0;
        int value = nodeValue(tree, &index);

        printf("Part 2: Root Node Value = %d\n\n", value);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *tok = strtok(str, " ");
        while (tok != NULL) {
                tal_add(input.tree, strtol(tok, NULL, 10));
                tok = strtok(NULL, " ");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day08.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day08.txt";
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

