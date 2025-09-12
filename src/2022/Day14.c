/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 29, 2025 10:31:57 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"
#include "../util/vector.h"

typedef tll(ivec2) ivectll;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printRocks(ivectll rocks[], int num) {
        if (!Debug) return;
        for (int i = 0; i < num; i++) {
                tll_foreach(rocks[i], it) {
                        ivec2 v = it->item;
                        printf("[%d, %d] ",v.x, v.y);
                }
                printf("\n");
        }
}

void printCave(ivec2 size, int cave[size.y][size.x]) {
        if (!Debug) return;
        int minX = 499 - size.y;
        for (int y = 0; y < size.y; y++) {
                for (int x = minX; x < size.x; x++) {
                        char c = '.';
                        if (cave[y][x] == 1)
                                c = '#';
                        if (cave[y][x] == 2)
                                c = 'o';
                        printf("%c", c);
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(ivec2 size, ivec2 pos) {
        return (pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y);
}

void drawLine(ivec2 size, int cave[size.y][size.x], ivec2 start, ivec2 end) {
        ivec2 movement;
        int diffX = end.x - start.x;
        int diffY = end.y - start.y;
        movement.x = (int)(diffX == 0 ? 0 : diffX / abs(diffX));
        movement.y = (int)(diffY == 0 ? 0 : diffY / abs(diffY));

        ivec2 cur = start;
        while (!ivec2Eq(cur, end) && validPos(size, cur)) {
                cave[cur.y][cur.x] = 1;
                cur.x += movement.x;
                cur.y += movement.y;
        }
        cave[end.y][end.x] = 1;
}

void buildCave(ivec2 size, int cave[size.y][size.x],
                ivectll rocks[], int numRocks) {
        for (int i = 0; i < numRocks; i++) {
                ivec2 start = tll_pop_front(rocks[i]);
                ivec2 end = {.x = 0, .y = 0};
                tll_foreach(rocks[i], it) {
                        end = it->item;
                        drawLine(size, cave, start, end);
                        start = end;
                }
        }
}

// 0: Grain came to rest, 1: Grain falls into void 2: Grain doesnt move
int dropGrain(ivec2 size, int cave[size.y][size.x]) {
        ivec2 grainPos = {.x = 500, .y = 0};
        while (validPos(size, grainPos)) {
                // Check below current position
                ivec2 below = {.x = grainPos.x, .y = grainPos.y + 1};
                if (!validPos(size, below)) break;
                if (cave[below.y][below.x] == 0) {
                        grainPos = below;
                        continue;
                }

                // Check down and to the left
                below.x = grainPos.x - 1;
                if (!validPos(size, below)) break;
                if (cave[below.y][below.x] == 0) {
                        grainPos = below;
                        continue;
                }

                // Check down and to the right
                below.x = grainPos.x + 1;
                if (!validPos(size, below)) break;
                if (cave[below.y][below.x] == 0) {
                        grainPos = below;
                        continue;
                }

                // If all fails, Grain rests
                cave[grainPos.y][grainPos.x] = 2;
                // If grain rests in original pos
                if (grainPos.x == 500 && grainPos.y == 0)
                        return 2;
                return 0;
        }
        // If grain falls into void
        return 1;
}

int pourSand(ivec2 size, int cave[size.y][size.x]) {
        int sandGrains = 0;
        int grainRests = 0;
        while (grainRests == 0) {
                grainRests = dropGrain(size, cave);
                if (grainRests != 1)
                        sandGrains++;
        }
        return sandGrains;
}

void part1(llist *ll) {
        const int NUM_ROCKS = ll->length;
        ivectll rocks[NUM_ROCKS];
        for (int i = 0; i < NUM_ROCKS; i++)
                rocks[i] = (ivectll)tll_init();

        ivec2 size = {.x = 0, .y = 0};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                if (strlen(str) == 0)
                        continue;

                char *nums = strtok(str, ",");
                while (nums != NULL) {
                        ivec2 vec;
                        vec.x = strtol(nums, (char**)NULL, 10);
                        if (vec.x > size.x) size.x = vec.x;

                        nums = strtok(NULL, " ");
                        if (nums == NULL) break;
                        vec.y = strtol(nums, (char**)NULL, 10);
                        if (vec.y > size.y) size.y = vec.y;

                        tll_push_back(rocks[line], vec);

                        nums = strtok(NULL, " ");
                        if (nums == NULL) break;
                        nums = strtok(NULL, ",");
                }

                current = current->next;
                line++;
        }
        printRocks(rocks, NUM_ROCKS);
        debugP("MaxX: %d, MaxY: %d\n", size.x, size.y);
        
        // Increase size by one - size = maxIndex + 1
        size.x++;
        size.y++;

        int cave[size.y][size.x];
        MAKE_LOOP(y, size.y, x, size.x) cave[y][x] = 0;

        buildCave(size, cave, rocks, NUM_ROCKS);
        printCave(size, cave);

        int resting = pourSand(size, cave);
        printCave(size, cave);

        printf("Part 1: Num grains at rest: %d\n\n", resting);
}

void part2(llist *ll) {
        const int NUM_ROCKS = ll->length;
        ivectll rocks[NUM_ROCKS];
        for (int i = 0; i < NUM_ROCKS; i++)
                rocks[i] = (ivectll)tll_init();

        ivec2 size = {.x = 0, .y = 0};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                if (strlen(str) == 0)
                        continue;

                char *nums = strtok(str, ",");
                while (nums != NULL) {
                        ivec2 vec;
                        vec.x = strtol(nums, (char**)NULL, 10);
                        if (vec.x > size.x) size.x = vec.x;

                        nums = strtok(NULL, " ");
                        if (nums == NULL) break;
                        vec.y = strtol(nums, (char**)NULL, 10);
                        if (vec.y > size.y) size.y = vec.y;

                        tll_push_back(rocks[line], vec);

                        nums = strtok(NULL, " ");
                        if (nums == NULL) break;
                        nums = strtok(NULL, ",");
                }

                current = current->next;
                line++;
        }
        printRocks(rocks, NUM_ROCKS);
        debugP("MaxX: %d, MaxY: %d\n", size.x, size.y);
        
        size.y += 3; // Add 3 for floor
        size.x = 501 + size.y; // Max distance a grain can go + a bit of buffer


        int cave[size.y][size.x];
        MAKE_LOOP(y, size.y, x, size.x) cave[y][x] = 0;
        for (int x = 0; x < size.x; x++)
                cave[size.y - 1][x] = 1;

        buildCave(size, cave, rocks, NUM_ROCKS);
        printCave(size, cave);

        int resting = pourSand(size, cave);
        printCave(size, cave);

        printf("Part 2: Num grains at rest: %d\n\n", resting);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day14.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day14.txt");
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

