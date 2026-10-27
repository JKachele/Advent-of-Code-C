/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Oct 26, 2026 10:44:14 EDT
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
        int numTriangles;
        ivec3 *triangles;
};

bool triangleValid(int a, int b, int c) {
        if ((a + b) <= c) {
                return false;
        }
        if ((b + c) <= a) {
                return false;
        }
        if ((c + a) <= b) {
                return false;
        }
        return true;
}

void part1(struct input *input) {
        int numTriangles = input->numTriangles;
        ivec3 *triangles = input->triangles;

        int numValid = 0;
        for (int i = 0; i < numTriangles; i++) {
                ivec3 tri = triangles[i];
                if (triangleValid(tri.a, tri.b, tri.c)) {
                        numValid++;
                }
        }

        printf("Part 1: Valid Triangles = %d\n\n", numValid);
}

void part2(struct input *input) {
        int numTriangles = input->numTriangles;
        ivec3 *triangles = input->triangles;

        int numValid = 0;
        for (int i = 0; i < numTriangles; i += 3) {
                ivec3 tri1 = triangles[i];
                ivec3 tri2 = triangles[i+1];
                ivec3 tri3 = triangles[i+2];
                for (int j = 0; j < 3; j++) {
                        if (triangleValid(tri1.raw[j], tri2.raw[j], tri3.raw[j])) {
                                numValid++;
                        }
                }
        }

        printf("Part 2: Valid Triangles = %d\n", numValid);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numTriangles = ll->length;
        input.triangles = malloc(input.numTriangles * sizeof(ivec3));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                ivec3 triangle;
                triangle.a = strtol(strtok(str, " "), NULL, 10);
                triangle.b = strtol(strtok(NULL, " "), NULL, 10);
                triangle.c = strtol(strtok(NULL, " "), NULL, 10);

                input.triangles[i++] = triangle;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day03.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day03.txt";
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
        double totalTime = ((double)(pt2 - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

