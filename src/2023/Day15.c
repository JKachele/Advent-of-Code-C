/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Aug 28, 2025 17:45:27 UTC
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

#define INPUT_BUFFER_SIZE 65536

typedef tll(char*) tllstr;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

uint8 hash(const char *str) {
        uint32 c = 0;
        uint32 hash = 0;
        while ((c = *str++) != '\0') {
                hash += c;
                hash *= 17;
                hash = hash % 256;
        }
        return (uint8)hash;
}

void part1(llist *ll) {
        tllstr initSeq = tll_init();

        char str[INPUT_BUFFER_SIZE];
        strncpy(str, (char*)ll->head->data, INPUT_BUFFER_SIZE);

        char *seqPart = strtok(str, ",");
        while (seqPart != NULL) {
                tll_push_back(initSeq, seqPart);
                seqPart = strtok(NULL, ",");
        }

        int64 hashSum = 0;
        tll_foreach(initSeq, it) {
                uint8 hashVal = hash(it->item);
                hashSum += hashVal;
                // printf("%d: %s\n", hashVal, it->item);
        }

        printf("Part 1: %ld\n\n", hashSum);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

