/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 31, 2026 13:03:40 UTC
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

#define INPUT_BUFFER_SIZE 16384
#define IMAGE_HEIGHT 6
#define IMAGE_WIDTH 25

typedef tll(u8) tllu8;

struct input {
        tllu8 pixels;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1_2(struct input *input) {
        int imageSize = IMAGE_HEIGHT * IMAGE_WIDTH;
        int numLayers = tll_length(input->pixels) / imageSize;
        tllu8 *layers = calloc(numLayers, sizeof(tllu8));

        int fewestZeroLayer = 0;
        int minZeros = imageSize;
        for (int i = 0; i < numLayers; i++) {
                int numZeros = 0;
                for (int j = 0; j < imageSize; j++) {
                        u8 pixel = tll_pop_front(input->pixels);
                        if (pixel == 0) numZeros++;
                        tll_push_back(layers[i], pixel);
                }
                if (numZeros < minZeros) {
                        minZeros = numZeros;
                        fewestZeroLayer = i;
                }
        }

        int numOnes = 0;
        int numTwos = 0;
        tll_foreach(layers[fewestZeroLayer], it) {
                if (it->item == 1) numOnes++;
                if (it->item == 2) numTwos++;
        }
        int ans = numOnes * numTwos;
        printf("Part 1: %d\n\n", ans);

        /************************ Part 2 ************************/
        u8 image[IMAGE_HEIGHT][IMAGE_WIDTH];
        // Set image to transparent (2)
        for (int y = 0; y < IMAGE_HEIGHT; y++)
                for (int x = 0; x < IMAGE_WIDTH; x++)
                        image[y][x] = 2;

        for (int i = numLayers-1; i >= 0; i--) {
                int j = 0;
                tll_foreach(layers[i], it) {
                        int x = j % IMAGE_WIDTH;
                        int y = j / IMAGE_WIDTH;
                        u8 pixel = it->item;

                        // Set pixel in image to white/black if layer is that color
                        if (pixel == 0)
                                image[y][x] = 0;
                        if (pixel == 1)
                                image[y][x] = 1;
                        // If layer is transparent (2), skip to next pixel

                        j++;
                }
        }

        printf("Part 2:\n");
        for (int y = 0; y < IMAGE_HEIGHT; y++) {
                for (int x = 0; x < IMAGE_WIDTH; x++) {
                        printf("%c", image[y][x] == 1 ? '#' : ' ');
                }
                printf("\n");
        }
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        int i = 0;
        while (str[i] != '\0') {
                tll_push_back(input.pixels, str[i] - '0');
                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

