/*************************************************
 *File----------Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 09, 2026 16:30:27 EDT
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

static int NUM_BANKS = 16;

struct bankHash {
        u64 hash1;
        u64 hash2;
};
typedef tal(struct bankHash) talhash;

struct input {
        u8 *banks;
};


static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

struct bankHash banksToHash(u8 banks[]) {
        struct bankHash hash = {0};
        for (int i = 0; i < NUM_BANKS && i < 8; i++) {
                hash.hash1 <<= 8;
                hash.hash1 += banks[i];
        }

        for (int i = 8; i < NUM_BANKS; i++) {
                hash.hash2 <<= 8;
                hash.hash2 += banks[i];
        }

        return hash;
}

void redistribute(u8 banks[]) {
        // Find bank with most blocks
        u8 maxBlocks = 0;
        int maxBank = 0;
        for (int i = 0; i < NUM_BANKS; i++) {
                if (banks[i] > maxBlocks) {
                        maxBlocks = banks[i];
                        maxBank = i;
                }
        }

        u8 blocks = banks[maxBank];
        banks[maxBank] = 0;
        int bank = maxBank;
        while (blocks > 0) {
                bank = (bank + 1) % NUM_BANKS;
                banks[bank]++;
                blocks--;
        }
}

int addhash(talhash *hashes, struct bankHash hash) {
        int foundIndex = -1;
        tal_for(*hashes, i) {
                if (hashes->array[i].hash1 == hash.hash1 && hashes->array[i].hash2 == hash.hash2) {
                        foundIndex = i;
                        break;
                }
        }

        if (foundIndex == -1) {
                tal_add(*hashes, hash);
        }

        return foundIndex;
}

void part1_2(struct input *input) {
        u8 *banks = input->banks;

        talhash hashes = tal_init();
        tal_add(hashes, banksToHash(banks));

        int numCycles = 0;
        int repeatIndex = 0;
        for (;;) {
                numCycles++;
                redistribute(banks);
                struct bankHash hash = banksToHash(banks);
                repeatIndex = addhash(&hashes, hash);
                if (repeatIndex != -1) {
                        break;
                }
        }

        int loopSize = numCycles - repeatIndex;

        printf("Part 1: First Repeat at %d\n\n", numCycles);
        printf("Part 2: Loop Size = %d\n\n", loopSize);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        input.banks = malloc(NUM_BANKS * sizeof(int));

        char *str = (char*)ll->head->data;

        char *tok = strtok(str, "\t");
        for (int i = 0; i < NUM_BANKS; i++) {
                input.banks[i] = strtol(tok, NULL, 10);
                tok = strtok(NULL, "\t");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day06.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
                NUM_BANKS = 4;
        } else {
                char *file = "assets/inputs/2017/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

