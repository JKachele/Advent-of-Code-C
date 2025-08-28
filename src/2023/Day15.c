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

typedef struct {
        char *label;
        int32 focus;
} lens;

typedef tll(lens) tlllens;
typedef tll(char*) tllstr;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBox(tlllens box, int32 index) {
        printf("Box %d: ", index);
        tll_foreach(box, it) {
                lens l = it->item;
                printf("[%s %d] ", l.label, l.focus);
        }
        printf("\n");
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

void addLens(tlllens *box, const char *label, int32 focus) {
        tll_foreach(*box, it) {
                if (strcmp(it->item.label, label) == 0) {
                        it->item.focus = focus;
                        return;
                }
        }

        // If lens not present, add it to back
        lens new = {0};
        new.label = malloc(strlen(label) + 1);
        strcpy(new.label, label);
        new.focus = focus;
        tll_push_back(*box, new);
}

void removeLens(tlllens *box, const char *label) {
        tll_foreach(*box, it) {
                if (strcmp(it->item.label, label) == 0) {
                        tll_remove(*box, it);
                        return;
                }
        }
}

void sequenceStep(tlllens boxes[256], const char *step) {
        char label[strlen(step)];
        int32 oppIndex = 0;
        for (uint32 i = 0; i < strlen(step); i++) {
                if (step[i] == '=' || step[i] == '-') {
                        label[i] = '\0';
                        oppIndex = i;
                        break;
                }
                label[i] = step[i];
        }
        uint8 box = hash(label);
        char opp = step[oppIndex];

        if (opp == '-') {
                removeLens(&boxes[box], label);
                // printf("Removed Lens %s From Box %d\n", label, box);
        } else if (opp == '=') {
                int32 focus = strtol(step+oppIndex+1, (char**)NULL, 10);
                addLens(&boxes[box], label, focus);
                // printf("Added Lens %s To Box %d With Focus %d\n",
                //                 label, box, focus);
        } else {
                printf("Unknown operation: %c\n", opp);
        }
}

int32 findFocusPower(tlllens box, uint8 boxIndex) {
        int32 focusPower = 0;
        int32 lensIndex = 1;
        tll_foreach(box, it) {
                int32 lens = boxIndex + 1;
                lens *= lensIndex;
                lens *= it->item.focus;

                focusPower += lens;
                lensIndex++;
        }
        return focusPower;
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
        tllstr initSeq = tll_init();

        char str[INPUT_BUFFER_SIZE];
        strncpy(str, (char*)ll->head->data, INPUT_BUFFER_SIZE);

        char *seqPart = strtok(str, ",");
        while (seqPart != NULL) {
                tll_push_back(initSeq, seqPart);
                seqPart = strtok(NULL, ",");
        }

        tlllens boxes[256];
        for (int i=0; i<256; i++)
                boxes[i] = (tlllens)tll_init();

        tll_foreach(initSeq, it) {
                sequenceStep(boxes, it->item);
        }
        // printBox(boxes[0], 0);
        // printBox(boxes[3], 3);

        int64 totalFocusPower = 0;
        for (int i=0; i<256; i++) {
                totalFocusPower += findFocusPower(boxes[i], i);
        }

        printf("Part 2: %ld\n\n", totalFocusPower);
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

