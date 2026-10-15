/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 15, 2026 10:47:13 EDT
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

#define INPUT_BUFFER_SIZE 1024

typedef tal(u8) talu8;

struct input {
        talu8 lengths;
        talu8 asciiSequence;
};

static int LoopSize = 256;
static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void stepHash(u8 loop[], u8 length, u8 pos) {
        if (length == 0) return;

        // Select section to reverse
        u8 rev[length];
        for (u8 i = 0; i < length; i++) {
                u8 loopI = (pos + i) % LoopSize;
                rev[i] = loop[loopI];
        }

        // Reverse section
        u8 f = 0;
        u8 b = length - 1;
        while (f < b) {
                // swap front and back
                u8 temp = rev[f];
                rev[f] = rev[b];
                rev[b] = temp;
                f++;
                b--;
        }

        // Replace loop section with reversed
        for (u8 i = 0; i < length; i++) {
                u8 loopI = (pos + i) % LoopSize;
                loop[loopI] = rev[i];
        }
}

void part1(struct input *input) {
        if (Debug) LoopSize = 5;
        talu8 lengths = input->lengths;

        u8 loop[LoopSize];
        for (int i = 0; i < LoopSize; i++) { loop[i] = i; }

        u8 pos = 0;
        int skip = 0;
        tal_for(lengths, i) {
                stepHash(loop, lengths.array[i], pos);
                pos += lengths.array[i] + skip;
                skip++;
        }

        int result = loop[0] * loop[1];

        printf("Part 1: Hash result = %d\n\n", result);
        if (Debug) LoopSize = 256;
}

void part2(struct input *input) {
        talu8 sequence = input->asciiSequence;

        // Add suffix to sequence
        const u8 suffix[] = {17, 31, 73, 47, 23};
        for (int i = 0; i < 5; i++) { tal_add(sequence, suffix[i]); }

        u8 loop[LoopSize];
        for (int i = 0; i < LoopSize; i++) { loop[i] = i; }

        // Run hash 64 times to get sparse hash
        u8 pos = 0;
        int skip = 0;
        for (int i = 0; i < 64; i++) {
                tal_for(sequence, j) {
                        stepHash(loop, sequence.array[j], pos);
                        pos += (sequence.array[j] + skip) % LoopSize;
                        skip++;
                }
        }

        u8 denseHash[16];
        for (int i = 0; i < 16; i++) {
                int loopI = i * 16;
                denseHash[i] = loop[loopI];
                for (int j = 1; j < 16; j++) {
                        denseHash[i] ^= loop[loopI + j];
                }
        }

        printf("Part 2: Hash = ");
        for (int i = 0; i < 16; i++) {
                printf("%02x", denseHash[i]);
        }
        printf("\n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        for (u64 i = 0; i < strlen(str); i++) {
                tal_add(input.asciiSequence, str[i]);
        }

        char *tok = strtok(str, ",");
        while (tok != NULL) {
                tal_add(input.lengths, strtol(tok, NULL, 10));
                tok = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day10.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day10.txt";
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

