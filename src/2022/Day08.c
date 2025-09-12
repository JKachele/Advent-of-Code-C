/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 02, 2025 09:00:31 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"
#include "../util/vector.h"

typedef struct {
        int8 height;
        bool visible;
        int32 score;
} tree;

static const bool DEBUG = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (DEBUG)
                vprintf(format, args);
        va_end(args);
}

void printForest(const ivec2 SIZE, tree forest[SIZE.y][SIZE.x], bool heights) {
        if (!DEBUG) return;
        for (int y = 0; y < SIZE.y; y++) {
                for (int x = 0; x < SIZE.x; x++) {
                        if (heights)
                                printf("%u ", forest[y][x].height);
                        else
                                printf("%d ", forest[y][x].visible ? 1 : 0);
                }
                printf("\n");
        }
}

int getVisibleTrees(const ivec2 SIZE, tree trees[SIZE.y][SIZE.x]) {
        // Right to Left
        for (int y = 0; y < SIZE.y; y++) {
                int8 tallest = -1;
                for (int x = 0; x < SIZE.x; x++) {
                        if (trees[y][x].height > tallest) {
                                tallest = trees[y][x].height;
                                trees[y][x].visible = true;
                        }
                        // No tree can be taller than 9
                        if (tallest == 9) break;
                }
        }

        // Left to Right
        for (int y = 0; y < SIZE.y; y++) {
                int8 tallest = -1;
                for (int x = SIZE.x - 1; x >= 0; x--) {
                        if (trees[y][x].height > tallest) {
                                tallest = trees[y][x].height;
                                trees[y][x].visible = true;
                        }
                        // No tree can be taller than 9
                        if (tallest == 9) break;
                }
        }

        // Top to Bottom
        for (int x = 0; x < SIZE.x; x++) {
                int8 tallest = -1;
                for (int y = 0; y < SIZE.y; y++) {
                        if (trees[y][x].height > tallest) {
                                tallest = trees[y][x].height;
                                trees[y][x].visible = true;
                        }
                        // No tree can be taller than 9
                        if (tallest == 9) break;
                }
        }

        // Bottom to Top
        for (int x = 0; x < SIZE.x; x++) {
                int8 tallest = -1;
                for (int y = SIZE.y - 1; y >= 0; y--) {
                        if (trees[y][x].height > tallest) {
                                tallest = trees[y][x].height;
                                trees[y][x].visible = true;
                        }
                        // No tree can be taller than 9
                        if (tallest == 9) break;
                }
        }

        int numVisible = 0;
        MAKE_LOOP(y, SIZE.y, x, SIZE.x) {
                if (trees[y][x].visible)
                        numVisible++;
        }

        return numVisible;
}

int getTreeScore(const ivec2 SIZE, tree trees[SIZE.y][SIZE.x], ivec2 tree) {
        const int height = trees[tree.y][tree.x].height;

        int score = 1;

        int viewDist = 0;
        // Look Up
        for (int y = tree.y - 1; y >= 0; y--) {
                viewDist++;
                if (trees[y][tree.x].height >= height)
                        break;
        }
        score *= viewDist;

        viewDist = 0;
        // Look Down
        for (int y = tree.y + 1; y < SIZE.y; y++) {
                viewDist++;
                if (trees[y][tree.x].height >= height)
                        break;
        }
        score *= viewDist;

        viewDist = 0;
        // Look Right
        for (int x = tree.x + 1; x < SIZE.x; x++) {
                viewDist++;
                if (trees[tree.y][x].height >= height)
                        break;
        }
        score *= viewDist;

        viewDist = 0;
        // Look Left
        for (int x = tree.x - 1; x >= 0; x--) {
                viewDist++;
                if (trees[tree.y][x].height >= height)
                        break;
        }
        score *= viewDist;

        return score;
}

int getHighestScore(const ivec2 SIZE, tree trees[SIZE.y][SIZE.x]) {
        int highestScore = 0;

        MAKE_LOOP(y, SIZE.y, x, SIZE.x) {
                ivec2 pos = {.x = x, .y = y};
                int score = getTreeScore(SIZE, trees, pos);
                if (score > highestScore)
                        highestScore = score;
        }       

        return highestScore;
}

void part1(llist *ll) {
        const ivec2 SIZE = {.y = ll->length, .x = getLongestLine(ll)};
        tree forest[SIZE.y][SIZE.x];

        llNode *current = ll->head;
        int row = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                for (int i = 0; i < SIZE.x; i++) {
                        forest[row][i].height = (int)(str[i] - '0');
                        forest[row][i].visible = false;
                }

                current = current->next;
                row++;
        }
        printForest(SIZE, forest, true);
        debugP("\n");

        int numVisible = getVisibleTrees(SIZE, forest);
        printForest(SIZE, forest, false);

        printf("Part 1: Trees Visible: %d\n\n", numVisible);
}

void part2(llist *ll) {
        const ivec2 SIZE = {.y = ll->length, .x = getLongestLine(ll)};
        tree forest[SIZE.y][SIZE.x];

        llNode *current = ll->head;
        int row = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                for (int i = 0; i < SIZE.x; i++) {
                        forest[row][i].height = (int)(str[i] - '0');
                        forest[row][i].visible = false;
                }

                current = current->next;
                row++;
        }
        printForest(SIZE, forest, true);

        int32 highestScore = getHighestScore(SIZE, forest);

        printf("Part 2: Highest Score: %d\n\n", highestScore);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day08.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day08.txt");
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

