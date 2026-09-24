/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 24, 2026 13:26:47 EDT
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

#define INPUT_BUFFER_SIZE 16384

typedef tal(ivec2) talivec2;

typedef enum {
        NORTH,
        EAST,
        SOUTH,
        WEST
} direction;

enum type {
        OPEN,
        DOOR,
        WALL
};

struct cell {
        enum type type;
        int dist;
        bool visited;
};

struct startend {
        talivec2 starts;
        talivec2 ends;
};
typedef tal(struct startend) talstartend;

struct input {
        char *regex;
};

const char DirChars[] = {'N', 'E', 'S', 'W', '\0'};
const ivec2 Dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printlist(talivec2 list) {
        tal_for(list, i)
                printf("(%d, %d)\n", list.array[i].x, list.array[i].y);
        printf("\n");
}

void printArea(ivec2 size, struct cell area[][size.x], ivec2 min) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (area[y][x].type == OPEN) {
                                if ((x + min.x == 0) && (y + min.y == 0)) {
                                        printf("X");
                                } else {
                                        printf(" ");
                                }
                        } else if (area[y][x].type == WALL) {
                                // printf("#");
                                printf("\xE2\x96\x92");
                        } else {
                                printf(" ");
                        }
                }
                printf("\n");
        }
        printf("\n");
}

void copyTalivec2(talivec2 *master, talivec2 *copy) {
        tal_for(*master, i)
                tal_add(*copy, master->array[i]);
}

void tal_add_unique(talivec2 *list, ivec2 vec) {
        tal_for(*list, i) {
                if (ivec2Eq(list->array[i], vec))
                        return;
        }
        tal_add(*list, vec);
}

void tal_append_unique(talivec2 *list1, talivec2 *list2) {
        tal_for(*list2, i) {
                tal_add_unique(list1, list2->array[i]);
        }
}

void updatePos(talivec2 *pos, ivec2 move, talivec2 *doors) {
        tal_for(*pos, i) {
                // Add new door
                tal_add_unique(doors, ivec2Add(pos->array[i], move));
                // Move twice to go through door to next room
                pos->array[i].x += move.x * 2;
                pos->array[i].y += move.y * 2;
        }
}

talivec2 findDoors(char *regex) {
        talivec2 doors = tal_init();

        // Rooms will be at {even, even} positions
        // Doors will be at {even, odd} or {odd, even} positions
        // {odd, odd} positions are always walls

        // Stack of current positions
        talivec2 pos = tal_init();
        tal_add(pos, (ivec2){0});

        // Stack of starts and ends for branches
        talstartend moveStack = tal_init();
        struct startend startends = {0};
        tal_add(startends.starts, (ivec2){0});

        for (int i = 0; i < (int)strlen(regex); i++) {
        // while (*regex != '\0') {
                char c = regex[i];

                // Regular Move
                if (strchr(DirChars, c) != NULL) {
                        for (int i = 0; i < 4; i++) {
                                if (c == DirChars[i]) {
                                        updatePos(&pos, Dirs[i], &doors);
                                        break;
                                }
                        }
                }
                // Start of a branch
                else if (c == '(') {
                        tal_add(moveStack, startends);
                        startends = (struct startend){0};
                        copyTalivec2(&pos, &startends.starts);
                }
                // Alternate branch
                else if (c == '|') {
                        tal_append_unique(&startends.ends, &pos);
                        pos = (talivec2){0};
                        copyTalivec2(&startends.starts, &pos);
                }
                // End of branch
                else if (c == ')') {
                        tal_append_unique(&pos, &startends.ends);
                        startends = tal_pop_back(moveStack);
                }
        }

        return doors;
}

int findLongestPath(ivec2 size, struct cell area[][size.x], ivec2 start) {
        int longest = 0;

        talivec2 queue = tal_init();
        tal_add(queue, start);
        area[start.y][start.x].dist = 0;
        area[start.y][start.x].visited = true;

        while (queue.length > 0) {
                ivec2 cur = tal_pop_front(queue);
                struct cell curCell = area[cur.y][cur.x];
                if (curCell.dist > longest) {
                        longest = curCell.dist;
                }

                for (int i = 0; i < 4; i++) {
                        ivec2 door = ivec2Add(cur, Dirs[i]);
                        ivec2 next = ivec2Add(door, Dirs[i]);
                        if (area[door.y][door.x].type == DOOR && !area[next.y][next.x].visited) {
                                area[next.y][next.x].dist = curCell.dist + 1;
                                area[next.y][next.x].visited = true;
                                tal_add(queue, next);
                        }
                }
        }

        return longest;
}

void part1_2(struct input *input) {
        char *regex = input->regex;

        talivec2 doors = findDoors(regex);
        // printlist(doors);

        ivec2 min = {{INT32_MAX, INT32_MAX}};
        ivec2 max = {{INT32_MIN, INT32_MIN}};
        tal_for(doors, i) {
                ivec2 door = doors.array[i];
                if (door.x < min.x) min.x = door.x;
                if (door.y < min.y) min.y = door.y;
                if (door.x > max.x) max.x = door.x;
                if (door.y > max.y) max.y = door.y;
        }
        min.x -= (min.x % 2 == 0) ? 1 : 2;
        min.y -= (min.y % 2 == 0) ? 1 : 2;
        max.x += (max.x % 2 == 0) ? 2 : 3;
        max.y += (max.y % 2 == 0) ? 2 : 3;
        ivec2 size = ivec2Sub(max, min);

        struct cell area[size.y][size.x];
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        area[y][x].type = WALL;
                        area[y][x].visited = false;
                }
        }
        tal_for(doors, i) {
                ivec2 door = ivec2Sub(doors.array[i], min);
                area[door.y][door.x].type = DOOR;
                if ((door.x + min.x) % 2 == 0) {
                        area[door.y-1][door.x].type = OPEN;
                        area[door.y+1][door.x].type = OPEN;
                } else {
                        area[door.y][door.x-1].type = OPEN;
                        area[door.y][door.x+1].type = OPEN;
                }
        }
        // printArea(size, area, min);

        ivec2 start = {{0 - min.x, 0 - min.y}};
        int longestPath = findLongestPath(size, area, start);

        printf("Part 1: Longest Path = %d\n\n", longestPath);

        int numLong = 0;
        for (int y = 1; y < size.y; y += 2) {
                for (int x = 1; x < size.x; x += 2) {
                        if (area[y][x].dist >= 1000)
                                numLong++;
                }
        }

        printf("Part 2: Rooms over 1000 away = %d\n", numLong);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.regex = malloc(strlen(str) + 1);
        strcpy(input.regex, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day20.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day20.txt";
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

