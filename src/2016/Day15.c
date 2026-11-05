/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Nov 02, 2026 10:14:47 EST
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
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024
#define NUM_WORDS 12

struct disk {
        int index;
        int numPos;
        int startPos;
};

struct input {
        int numDisks;
        struct disk *disks;
};

void splitLine(char *line, char *words[]) {
        char *tok = strtok(line, " ");
        for (int i = 0; i < NUM_WORDS; i++) {
                words[i] = malloc(strlen(tok) + 1);
                strcpy(words[i], tok);
                tok = strtok(NULL, " ");
        }

}

void freeWords(char *words[]) {
        for (int i = 0; i < NUM_WORDS; i++) {
                free(words[i]);
        }
}

bool passesDisk(int timeDropped, struct disk disk) {
        return (timeDropped + disk.index + disk.startPos) % disk.numPos == 0;
}

void part1(struct input *input) {
        int numDisks = input->numDisks;
        struct disk *disks = input->disks;

        int time = ((disks[0].numPos - disks[0].startPos) - 1);
        for (;;) {
                bool allPass = true;
                for (int i = 0; i < numDisks; i++) {
                        if (!passesDisk(time, disks[i])) {
                                allPass = false;
                                break;
                        }
                }
                if (allPass) {
                        break;
                }
                time += disks[0].numPos;
        }

        printf("Part 1: Time = %d\n\n", time);
}

void part2(struct input *input) {
        int numDisks = input->numDisks;
        struct disk *disks = input->disks;

        numDisks++;
        disks = realloc(disks, numDisks * sizeof(struct disk));
        disks[numDisks-1] = (struct disk){numDisks, 11, 0};

        int time = ((disks[0].numPos - disks[0].startPos) - 1);
        for (;;) {
                bool allPass = true;
                for (int i = 0; i < numDisks; i++) {
                        if (!passesDisk(time, disks[i])) {
                                allPass = false;
                                break;
                        }
                }
                if (allPass) {
                        break;
                }
                time += disks[0].numPos;
        }

        printf("Part 2: Time = %d\n", time);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numDisks = ll->length;
        input.disks = malloc(input.numDisks * sizeof(struct disk));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                char *words[NUM_WORDS];
                splitLine(str, words);

                struct disk disk;
                disk.index = i + 1;
                disk.numPos = strtol(words[3], NULL, 10);
                disk.startPos = strtol(words[11], NULL, 10);

                input.disks[i++] = disk;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day15.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

