/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 29, 2025 18:45:51 UTC
 *License-------GNU GPL-3.0
 ************************************************/

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

struct input {
        bool alg[512];
        ivec2 size;
        bool **image;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printImage(ivec2 s, bool i[s.y][s.x]) {
        for (int y=0; y<s.y; y++) {
                for (int x=0; x<s.x; x++) {
                        if (i[y][x])
                                printf("#");
                        else
                                printf(".");
                }
                printf("\n");
        }
        printf("\n");
}

void fillImage(ivec2 s, bool i[][s.x], ivec2 nS, bool new[][nS.x], int border) {
        border++;
        for (int y=0; y<nS.y; y++) {
                for (int x=0; x<nS.x; x++) {
                        new[y][x] = false;
                }
        }

        for (int y=0; y<s.y; y++) {
                for (int x=0; x<s.x; x++) {
                        new[y+border][x+border] = i[y][x];
                }
        }
}

bool validPos(ivec2 s, ivec2 p) {
        return p.x >= 0 && p.x < s.x && p.y >= 0 && p.y < s.y;
}

int pixelToNum(ivec2 s, bool i[s.y][s.x], ivec2 p, bool alg[], int itteration) {
        int num = 0;
        for (int y=-1; y<=1; y++) {
                for (int x=-1; x<=1; x++) {
                        ivec2 pos = {{p.x+x, p.y+y}};
                        num <<= 1;
                        if (validPos(s, pos)) {
                                if (i[pos.y][pos.x])
                                        num++;
                                continue;
                        }
                        // pos outside image

                        if (alg[0] == false)
                                continue; // Doesn't alternate

                        if (itteration % 2 == 1) {
                                num++;
                        }
                }
        }
        return num;
}

void enhanceImage(ivec2 s, bool i[][s.x], bool alg[], int itteration) {
        bool old[s.y][s.x];
        memcpy(old, i, sizeof(bool) * s.y * s.x);
        for (int y=0; y<s.y; y++) {
                for (int x=0; x<s.x; x++) {
                        ivec2 pos = {{x, y}};
                        int index = pixelToNum(s, old, pos, alg, itteration);
                        i[y][x] = alg[index];
                }
        }
}

void part1(struct input input) {
        const int NUM_ITERATIONS = 2;

        bool *alg = input.alg;
        ivec2 size = input.size;
        bool (*image)[size.x] = (bool(*)[size.x])input.image;
        // printImage(size, image);

        // Add border of 3 to buffer for 2 enhansments
        int border = (NUM_ITERATIONS + 1) * 2;
        ivec2 newSize = {{size.x + border, size.y + border}};
        bool newImage[newSize.y][newSize.x];
        fillImage(size, image, newSize, newImage, NUM_ITERATIONS);
        // printImage(newSize, newImage);

        enhanceImage(newSize, newImage, alg, 0);
        enhanceImage(newSize, newImage, alg, 1);
        // printImage(newSize, newImage);

        int numLit = 0;
        for (int y=0; y<newSize.y; y++) {
                for (int x=0; x<newSize.x; x++) {
                        if (newImage[y][x])
                                numLit++;
                }
        }

        printf("Part 1: %d\n\n", numLit);
}

void part2(struct input input) {
        const int NUM_ITERATIONS = 50;

        bool *alg = input.alg;
        ivec2 size = input.size;
        bool (*image)[size.x] = (bool(*)[size.x])input.image;
        // printImage(size, image);

        // Add border of 3 to buffer for 2 enhansments
        int border = (NUM_ITERATIONS + 1) * 2;
        ivec2 newSize = {{size.x + border, size.y + border}};
        bool newImage[newSize.y][newSize.x];
        fillImage(size, image, newSize, newImage, NUM_ITERATIONS);
        // printImage(newSize, newImage);

        for (int i=0; i<NUM_ITERATIONS; i++) {
                enhanceImage(newSize, newImage, alg, i);
                // printImage(newSize, newImage);
        }
        // printImage(newSize, newImage);

        int numLit = 0;
        for (int y=0; y<newSize.y; y++) {
                for (int x=0; x<newSize.x; x++) {
                        if (newImage[y][x])
                                numLit++;
                }
        }

        printf("Part 2: %d\n\n", numLit);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.size.y = ll->length - 2;
        input.size.x = strlen((char*)ll->tail->data);
        ivec2 size = input.size;
        input.image = calloc(size.x * size.y, sizeof(bool));
        bool (*image)[size.x] = (bool(*)[size.x])input.image;

        char *str = (char*)ll->head->data;
        for (int i=0; i<512; i++) {
                if (str[i] == '#')
                        input.alg[i] = true;
        }

        llNode *current = ll->head->next->next;
        int y = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        if (str[x] == '#')
                                image[y][x] = true;
                        else
                                image[y][x] = false;
                }

                current = current->next;
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day20.txt";
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

