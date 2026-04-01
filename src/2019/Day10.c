/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 31, 2026 19:06:34 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/quicksort.h"

#define INPUT_BUFFER_SIZE 4096
#define PI 3.1415926535

typedef tll(ivec2) tllivec2;

typedef struct point {
        bool asteroid;
        tllivec2 visible;
        tllivec2 blocked;
} point;

typedef struct target {
        ivec2 pos;
        float angle;
} target;

struct input {
        ivec2 size;
        point **field;
        ivec2 best;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int compareTargets(void *a, void *b) {
        target *t1 = (target*)a;
        target *t2 = (target*)b;

        if (t1->angle > t2->angle)
                return 1;
        if (t1->angle < t2->angle)
                return -1;
        return 0;
}

bool validPos(ivec2 size, int x, int y) {
        return (x >= 0 && x < size.x && y >= 0 && y < size.y);
}

ivec2 reduceSlope(ivec2 slope) {
        int divisor = gcd(slope.x, slope.y);
        return (ivec2){{slope.x / divisor, slope.y / divisor}};
}

void addPos(tllivec2 *list, ivec2 pos) {
        tll_foreach(*list, it) {
                if (ivec2Eq(it->item, pos))
                        return;
        }
        tll_push_back(*list, pos);
}

void asteroidVisible(ivec2 size, point field[][size.x], bool checked[][size.x],
                ivec2 pos, ivec2 target) {
        // Find slope of visible line
        ivec2 slope = {{target.x - pos.x, target.y - pos.y}};
        slope = reduceSlope(slope);
        
        bool found = false;
        ivec2 lastSeen = pos;
        ivec2 cur = ivec2Add(pos, slope);
        while (validPos(size, cur.x, cur.y)) {
                checked[cur.y][cur.x] = true;
                if (field[cur.y][cur.x].asteroid) {
                        if (!found) {
                                addPos(&field[pos.y][pos.x].visible, cur);
                                addPos(&field[cur.y][cur.x].visible, pos);
                                found = true;
                        } else {
                                addPos(&field[cur.y][cur.x].visible, lastSeen);
                                addPos(&field[cur.y][cur.x].blocked, pos);
                        }
                        lastSeen = cur;
                }
                cur = ivec2Add(cur, slope);
        }
}

void getVisibleAsteroids(ivec2 size, point field[][size.x], ivec2 pos) {
        // Array to see if point was already checked
        bool checked[size.y][size.x];
        for (int y = 0; y < size.y; y++)
                for (int x = 0; x < size.x; x++)
                        checked[y][x] = false;

        // Update checked list with current visible and blocked lists
        tll_foreach(field[pos.y][pos.x].visible, it)
                checked[it->item.y][it->item.x] = true;
        tll_foreach(field[pos.y][pos.x].blocked, it)
                checked[it->item.y][it->item.x] = true;

        // Check if asteroids are visible from the current position
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (!field[y][x].asteroid || (x == pos.x && y == pos.y) || checked[y][x])
                                continue;
                        ivec2 cur = {{x, y}};
                        asteroidVisible(size, field, checked, pos, cur);
                }
        }
}

void destroyAsteroids(ivec2 size, point field[][size.x], ivec2 pos) {
        tll_foreach(field[pos.y][pos.x].visible, it) {
                ivec2 cur = it->item;
                field[cur.y][cur.x].asteroid = false;
        }
        tll_free(field[pos.y][pos.x].visible);
        tll_free(field[pos.y][pos.x].blocked);
}

ivec2 get200thDestroyed(tllivec2 asteroids, ivec2 pos, int numDestroyed) {
        int numAsteroids = tll_length(asteroids);
        target targets[numAsteroids];

        int i = 0;
        tll_foreach(asteroids, it) {
                targets[i].pos = it->item;
                ivec2 diff = ivec2Sub(targets[i].pos, pos);
                // Angle of point from vertical line = atan(diff.x / -diff.y)
                targets[i].angle = atan2(diff.x, diff.y * -1);
                if (targets[i].angle < 0)
                        targets[i].angle += 2 * PI;
                i++;
        }

        // Sort targets by angle
        quicksort(targets, sizeof(target), 0, numAsteroids - 1, compareTargets);

        // Get 200th destroyed asteroid
        return targets[199-numDestroyed].pos;
}

void part1(struct input *input) {
        ivec2 size = input->size;
        point field[size.y][size.x];
        memcpy(field, input->field, sizeof(point) * size.x * size.y);

        ivec2 bestPos = {0};
        int maxVisible = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (!field[y][x].asteroid)
                                continue;
                        getVisibleAsteroids(size, field, (ivec2){{x, y}});
                        int numVisible = tll_length(field[y][x].visible);
                        if (numVisible > maxVisible) {
                                maxVisible = numVisible;
                                bestPos = (ivec2){{x, y}};
                        }
                }
        }
        input->best = bestPos;

        printf("Part 1: (%d, %d): %d\n\n", bestPos.x, bestPos.y, maxVisible);
}

void part2(struct input *input) {
        ivec2 size = input->size;
        point field[size.y][size.x];
        memcpy(field, input->field, sizeof(point) * size.x * size.y);
        ivec2 pos = input->best;

        int numDestroyed = 0;
        for(;;) {
                getVisibleAsteroids(size, field, pos);
                int newDestroyed = numDestroyed + tll_length(field[pos.y][pos.x].visible);
                if (newDestroyed >= 200) {
                        break;
                }
                destroyAsteroids(size, field, pos);
                numDestroyed = newDestroyed;
        }

        ivec2 destroyed = get200thDestroyed(field[pos.y][pos.x].visible, pos, numDestroyed);
        int ans = destroyed.x * 100 + destroyed.y;

        printf("Part 2: (%d, %d): %d\n", destroyed.x, destroyed.y, ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.field = calloc(size.x * size.y, sizeof(point));
        point (*field)[size.x] = (point(*)[size.x])input.field;

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < size.x; x++) {
                        field[y][x].asteroid = (str[x] == '#');
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
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

