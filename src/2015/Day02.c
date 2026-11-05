/*************************************************
 *File----------src/2015/Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Nov 05, 2026 10:58:06 EST
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

struct input {
        int numBoxes;
        ivec3 *boxes;
};

void part1(struct input *input) {
        int numBoxes = input->numBoxes;
        ivec3 *boxes = input->boxes;

        int paper = 0;
        for (int i = 0; i < numBoxes; i++) {
                ivec3 box = boxes[i];
                int minArea = 0x7FFFFFFF;
                for (int j = 0; j < 3; j++) {
                        int area = box.raw[j] * box.raw[(j+1)%3];
                        paper += area * 2;
                        if (area < minArea) {
                                minArea = area;
                        }
                }
                paper += minArea;
        }

        printf("Part 1: Paper Needed = %d\n\n", paper);
}

void part2(struct input *input) {
        int numBoxes = input->numBoxes;
        ivec3 *boxes = input->boxes;

        int ribbon = 0;
        for (int i = 0; i < numBoxes; i++) {
                ivec3 box = boxes[i];
                int minPerim = 0x7FFFFFFF;
                for (int j = 0; j < 3; j++) {
                        int perim = (2 * box.raw[j]) + (2 * box.raw[(j+1)%3]);
                        if (perim < minPerim) {
                                minPerim = perim;
                        }
                }
                ribbon += minPerim;
                ribbon += ivec3Volume(box);
        }

        printf("Part 2: Ribbon Needed = %d\n", ribbon);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numBoxes = ll->length;
        input.boxes = malloc(input.numBoxes * sizeof(ivec3));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                ivec3 box;
                box.x = strtol(strtok(str, "x"), NULL, 10);
                box.y = strtol(strtok(NULL, "x"), NULL, 10);
                box.z = strtol(strtok(NULL, ""), NULL, 10);
                input.boxes[i++] = box;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2015/Day02.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2015/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2015/Day02.txt";
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

