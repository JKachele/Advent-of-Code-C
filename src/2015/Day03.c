/*************************************************
 *File----------src/2015/Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Nov 05, 2026 11:12:36 EST
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
#include "../util/hashtable.h"

#define INPUT_BUFFER_SIZE 16384

enum direction {
        RIGHT,
        DOWN,
        LEFT,
        UP
};

struct input {
        int numMoves;
        enum direction *moves;
};

u64 posToHash(const void *key, u64 mod) {
        ivec2 pos = *(ivec2*)key;
        u64 hash = *(u32*)&pos.x;
        hash <<= 32;
        hash |= *(u32*)&pos.y;
        return hash % mod;
}

int cmpPos(const void* a, const void* b) {
        ivec2 v1 = *(ivec2*)a;
        ivec2 v2 = *(ivec2*)b;
        if (ivec2Eq(v1, v2)) {
                return 0;
        } else {
                return 1;
        }
}

void part1(struct input *input) {
        const ivec2 Dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        int numMoves = input->numMoves;
        enum direction *moves = input->moves;

        hashtable ht = ht_create(0, sizeof(ivec2), sizeof(int), posToHash, cmpPos);

        ivec2 pos = {0};
        ht_add(&ht, &pos, (int[]){1});

        for (int i = 0; i < numMoves; i++) {
                pos = ivec2Add(pos, Dirs[moves[i]]);
                ht_add(&ht, &pos, (int[]){1});
        }

        printf("Part 1: Houses Visited = %lu\n\n", ht.size);
}

void part2(struct input *input) {
        const ivec2 Dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        int numMoves = input->numMoves;
        enum direction *moves = input->moves;

        hashtable ht = ht_create(0, sizeof(ivec2), sizeof(int), posToHash, cmpPos);

        ivec2 santa = {0};
        ivec2 elf = {0};
        ht_add(&ht, &santa, (int[]){1});

        for (int i = 0; i < numMoves; i++) {
                if (i % 2 == 0) {
                        santa = ivec2Add(santa, Dirs[moves[i]]);
                        ht_add(&ht, &santa, (int[]){1});
                } else {
                        elf = ivec2Add(elf, Dirs[moves[i]]);
                        ht_add(&ht, &elf, (int[]){1});
                }
        }

        printf("Part 2: Houses Visited = %lu\n", ht.size);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.numMoves = strlen(str);
        input.moves = malloc(input.numMoves * sizeof(enum direction));
        for (int i = 0; i < input.numMoves; i++) {
                switch (str[i]) {
                case '>':
                        input.moves[i] = RIGHT;
                        break;
                case 'v':
                        input.moves[i] = DOWN;
                        break;
                case '<':
                        input.moves[i] = LEFT;
                        break;
                case '^':
                        input.moves[i] = UP;
                        break;
                }
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2015/Day03.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2015/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2015/Day03.txt";
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

