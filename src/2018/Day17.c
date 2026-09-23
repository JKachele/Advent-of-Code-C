/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 23, 2026 09:24:52 EDT
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
#include "../util/util.h"
#include "../util/talist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(ivec2) talivec2;

typedef enum {
        UP,
        RIGHT,
        DOWN,
        LEFT
} direction;

struct ground {
        bool clay;
        bool full;
        bool hadWater;
};

struct input {
        talivec2 clayPos;
        ivec2 min;
        ivec2 max;
        ivec2 size;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGround(ivec2 size, struct ground ground[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (ground[y][x].clay)
                                printf("#");
                        else if (ground[y][x].full)
                                printf("~");
                        else if (ground[y][x].hadWater)
                                printf("|");
                        else
                                printf(" ");
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(ivec2 size, ivec2 pos) {
        return (pos.x >= 0) && (pos.y >= 0) && (pos.x < size.x) && (pos.y < size.y);
}

void fillWater(ivec2 size, struct ground ground[][size.x], ivec2 min) {
        const ivec2 Moves[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        const int Source = 500 - min.x;

        // Queue for water fromts
        ivec2 queue[size.x];
        ivec2 queuePtr = {0};
        queue[queuePtr.y++] = (ivec2){{Source, 0}};
        queuePtr.y = queuePtr.y % size.x;

        while (queuePtr.x != queuePtr.y) {
                ivec2 cur = queue[queuePtr.x++];
                queuePtr.x = queuePtr.x % size.x;

                // Move down
                bool foundWater = false;
                ivec2 next = ivec2Add(cur, Moves[DOWN]);
                while (validPos(size, next)) {
                        struct ground g = ground[next.y][next.x];
                        // No need to continue if reached an already reached cell
                        if (g.hadWater && !g.full) {
                                foundWater = true;
                                break;
                        }
                        if (g.clay || g.full)
                                break;
                        ground[cur.y][cur.x].hadWater = true;
                        cur = next;
                        next = ivec2Add(cur, Moves[DOWN]);
                }

                // if hit another water stream or bottom of grid,
                // mark as hadWater and move to next drop
                if (foundWater || cur.y == size.y-1) {
                        ground[cur.y][cur.x].hadWater = true;
                        continue;
                }

                // Search left and right
                ivec2 left = ivec2Add(cur, Moves[LEFT]);
                bool leftCanFall = false;
                while (validPos(size, left)) {
                        if (!ground[left.y+1][left.x].clay && !ground[left.y+1][left.x].full) {
                                leftCanFall = true;
                                break;
                        }
                        if (ground[left.y][left.x].clay) {
                                left = ivec2Add(left, Moves[RIGHT]);
                                break;
                        }
                        left = ivec2Add(left, Moves[LEFT]);
                }

                ivec2 right = ivec2Add(cur, Moves[RIGHT]);
                bool rightCanFall = false;
                while (validPos(size, right)) {
                        if (!ground[right.y+1][right.x].clay && !ground[right.y+1][right.x].full) {
                                rightCanFall = true;
                                break;
                        }
                        if (ground[right.y][right.x].clay) {
                                right = ivec2Add(right, Moves[LEFT]);
                                break;
                        }
                        right = ivec2Add(right, Moves[RIGHT]);
                }

                // If nowhere can fall, set all in row to full and add 1 above cur to queue
                if (!leftCanFall && !rightCanFall) {
                        for (int x = left.x; x <= right.x; x++)
                                ground[cur.y][x].full = true;
                        queue[queuePtr.y++] = ivec2Add(cur, Moves[UP]);
                        queuePtr.y = queuePtr.y % size.x;
                        continue;
                }

                for (int x = left.x; x <= right.x; x++)
                        ground[cur.y][x].hadWater = true;
                if (leftCanFall) {
                        queue[queuePtr.y++] = left;
                        queuePtr.y = queuePtr.y % size.x;
                }
                if (rightCanFall) {
                        queue[queuePtr.y++] = right;
                        queuePtr.y = queuePtr.y % size.x;
                }
        }
}

void part1_2(struct input *input) {
        talivec2 clayPos = input->clayPos;
        ivec2 size = input->size;
        ivec2 min = input->min;

        struct ground ground[size.y][size.x];
        for (int y = 0; y < size.y; y++)
                for (int x = 0; x < size.x; x++)
                        ground[y][x] = (struct ground){false, false, false};

        tal_for(clayPos, i) {
                ivec2 clay = clayPos.array[i];
                clay = ivec2Sub(clay, min);
                ground[clay.y][clay.x].clay = true;
        }
        // printGround(size, ground);

        fillWater(size, ground, min);
        printGround(size, ground);

        int numReachable = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        struct ground g = ground[y][x];
                        if (g.full || g.hadWater)
                                numReachable++;
                }
        }

        printf("Part 1: Cells reachable by water = %d\n\n", numReachable);

        int numFull = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        struct ground g = ground[y][x];
                        if (g.full)
                                numFull++;
                }
        }

        printf("Part 2: Cells full of water = %d\n", numFull);
}

void part2(struct input *input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.min = (ivec2){{INT32_MAX, INT32_MAX}};
        input.max = (ivec2){{0, 0}};

        // char *str = (char*)ll->head->data;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                int rangeIndex = (str[0] == 'x') ? 1 : 0;
                int fixedIndex = (str[0] == 'x') ? 0 : 1;

                int fixedPos = strtol(strtok(str+2, ","), NULL, 10);;
                strtok(NULL, "=");
                int rangeStart = strtol(strtok(NULL, "."), NULL, 10);
                int rangeEnd = strtol(strtok(NULL, "") + 1, NULL, 10);

                for (int i = rangeStart; i <= rangeEnd; i++) {
                        ivec2 clay;
                        clay.raw[fixedIndex] = fixedPos;
                        clay.raw[rangeIndex] = i;
                        tal_add(input.clayPos, clay);

                        if (clay.x < input.min.x) input.min.x = clay.x;
                        if (clay.y < input.min.y) input.min.y = clay.y;
                        if (clay.x > input.max.x) input.max.x = clay.x;
                        if (clay.y > input.max.y) input.max.y = clay.y;
                }
        }

        // Expand x size by 1 on both sides to account for water overflow
        input.min.x--;
        input.max.x++;

        input.size.x = input.max.x - input.min.x + 1;
        input.size.y = input.max.y - input.min.y + 1;

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day17.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day17.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day17.txt";
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

