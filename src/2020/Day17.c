/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 17, 2026 18:01:41 UTC
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
#include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 4096
#define NUM_STEPS 6

typedef tll(ivec4) tllivec4;

typedef struct cube {
        u64 id; // 64-bit id: 16 bits for each X, Y, Z, and W
        bool active;
        int adjActive;
        int adjActiveNew;
        // Pointers for binary tree structure
        struct cube *left;
        struct cube *right;
} cube;

struct input {
        tllivec4 activeCubes;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

u64 postoid(ivec4 pos) {
        const u64 Mask = 0x000000000000FFFF;
        u64 id = 0;
        for (int i = 0; i < 4; i++) {
                id <<= 16;
                id += (pos.raw[i] + 0x7FFF) & Mask;
        }
        return id;
}

ivec4 idtopos(u64 id) {
        const u64 Mask = 0x000000000000FFFF;
        ivec4 pos = {0};
        for (int i = 3; i >= 0; i--) {
                pos.raw[i] = id & Mask;
                pos.raw[i] -= 0x7FFF;
                id >>= 16;
        }
        return pos;
}

void insertOrModify(cube **cur, u64 id, int cubeOn, int adjActive) {
        // If cur is NULL, create new cube
        if ((*cur) == NULL) {
                (*cur) = malloc(sizeof(cube));
                (*cur)->id = id;
                (*cur)->active = (cubeOn == 1);
                (*cur)->adjActive = 0;
                (*cur)->adjActiveNew = (adjActive > 0) ? adjActive : 0;
                (*cur)->left = NULL;
                (*cur)->right = NULL;
                return;
        }

        // If cur id matches id, modify its value
        if ((*cur)->id == id) {
                if (cubeOn == 1)
                        (*cur)->active = true;
                else if (cubeOn == -1)
                        (*cur)->active = false;
                (*cur)->adjActiveNew += adjActive;
                return;
        }

        // If cur id is less than id, go right
        if ((*cur)->id < id) {
                insertOrModify(&((*cur)->right), id, cubeOn, adjActive);
                return;
        }

        // If cur id is greater than id, go left
        if ((*cur)->id > id) {
                insertOrModify(&((*cur)->left), id, cubeOn, adjActive);
        }
}

void updateCube(cube **root, ivec4 pos, bool active, bool part1) {
        // Modify cube active status
        u64 id = postoid(pos);
        insertOrModify(root, id, active ? 1 : -1, 0);

        // Update counters of adjacent cubes
        for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                        for (int dz = -1; dz <= 1; dz++) {
                                for (int dw = -1; dw <= 1; dw++) {
                                        // Only modify W for part 2
                                        if (part1 && dw != 0) continue;

                                        ivec4 newPos = ivec4Add(pos, (ivec4){{dx, dy, dz, dw}});
                                        if (ivec4Eq(pos, newPos)) continue;
                                        int counterDiff = active ? 1 : -1;
                                        u64 id = postoid(newPos);
                                        insertOrModify(root, id, 0, counterDiff);
                                }
                        }
                }
        }
}

void updateCounters(cube **cur) {
        if ((*cur) == NULL) return;

        (*cur)->adjActive = (*cur)->adjActiveNew;
        updateCounters(&((*cur)->left));
        updateCounters(&((*cur)->right));
}

void stepAllCubes(cube **root, cube **cur, bool part1) {
        if ((*cur) == NULL) return;
        cube curCube = **cur;

        // Active cubes become inactive if they don't have exactly 2 or 3 active neighbors
        if (curCube.active && curCube.adjActive != 2 && curCube.adjActive != 3) {
                updateCube(root, idtopos(curCube.id), false, part1);
        }

        // Inactive cubes with exactly 3 active neighbors become active
        if (!curCube.active && curCube.adjActive == 3) {
                updateCube(root, idtopos(curCube.id), true, part1);
        }

        stepAllCubes(root, &((*cur)->left), part1);
        stepAllCubes(root, &((*cur)->right), part1);
}

u64 getActiveCubes(cube *cur) {
        if (cur == NULL) return 0;

        u64 numActive = (cur->active) ? 1 : 0;
        numActive += getActiveCubes(cur->left);
        numActive += getActiveCubes(cur->right);
        return numActive;
}

void freeTree(cube **cur) {
        if ((*cur) == NULL) return;

        freeTree(&(*cur)->left);
        freeTree(&(*cur)->right);
        free(*cur);
}

void part1(struct input input) {
        // Root of binary tree
        cube *root = NULL;

        // Add initial active cubes to binary tree
        tll_foreach(input.activeCubes, it) {
                updateCube(&root, it->item, true, true);
        }
        updateCounters(&root);

        for (int i = 0; i < NUM_STEPS; i++) {
                stepAllCubes(&root, &root, true);
                updateCounters(&root);
        }

        u64 numActive = getActiveCubes(root);
        freeTree(&root);

        printf("Part 1: %lu\n\n", numActive);
}

void part2(struct input input) {
        // Root of binary tree
        cube *root = NULL;

        // Add initial active cubes to binary tree
        tll_foreach(input.activeCubes, it) {
                updateCube(&root, it->item, true, false);
        }
        updateCounters(&root);

        for (int i = 0; i < NUM_STEPS; i++) {
                stepAllCubes(&root, &root, false);
                updateCounters(&root);
        }

        u64 numActive = getActiveCubes(root);
        freeTree(&root);

        printf("Part 2: %lu\n", numActive);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < (int)strlen(str); x++) {
                        if (str[x] == '#') {
                                ivec4 coords = {{x, y, 0, 0}};
                                tll_push_back(input.activeCubes, coords);
                        }
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day17.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day17.txt";
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

