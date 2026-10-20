/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 20, 2026 14:01:16 EDT
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
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

struct input {
        ivec2 size;
        char **map;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, char map[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        printf("%c", map[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

bool validPath(ivec2 size, char map[][size.x], ivec3 pos) {
        char c = map[pos.y][pos.x];
        if (c == '|' || c == '-' || c == '+' || (c >= 'A' && c <= 'Z')) {
                return true;
        }
        return false;
}

ivec3 move(ivec2 size, char map[][size.x], ivec3 pos) {
        // Right, Down, Left, Up
        const ivec3 Dirs[4] = {{{1, 0, 0}}, {{0, 1, 0}}, {{-1, 0, 0}}, {{0, -1, 0}}};

        ivec3 next = ivec3Add(pos, Dirs[pos.z]);
        if (validPath(size, map, next)) {
                return next;
        }

        int right = (pos.z + 1) % 4;
        next = ivec3Add(pos, Dirs[right]);
        next.z = right;
        if (validPath(size, map, next)) {
                return next;
        }

        int left = (pos.z + 3) % 4;     // Subtract 1 is same as add 5 mod 5
        next = ivec3Add(pos, Dirs[left]);
        next.z = left;
        if (validPath(size, map, next)) {
                return next;
        }

        // If no valid paths, reached end. Return -1
        return (ivec3){{-1, -1, -1}};
}

void part1(struct input *input) {
        ivec2 size = input->size;
        char (*map)[size.x] = (char(*)[size.x])input->map;
        // printMap(size, map);

        // Get starting pos, Starting facing down
        ivec3 pos = {{0, 0, 1}};        // {x, y, dir}
        for (int x = 0; x < size.x; x++) {
                if (map[0][x] == '|') {
                        pos.x = x;
                        break;
                }
        }

        char path[27] = {0};
        int pathI = 0;
        int steps = 0;
        for (;;) {
                pos = move(size, map, pos);
                steps++;

                char c = map[pos.y][pos.x];
                if (c >= 'A' && c <= 'Z') {
                        path[pathI++] = c;
                }

                if (pos.x == -1) {
                        break;
                }
        }

        printf("Part 1: Path = %s\n\n", path);
        printf("Part 2: Steps = %d\n", steps);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.size.x = getLongestLine(ll);
        input.size.y = ll->length;
        ivec2 size = input.size;
        input.map = malloc(size.x * size.y);
        char (*map)[size.x] = (char(*)[size.x])input.map;

        // char *str = (char*)ll->head->data;
        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < (int)strlen(str); x++) {
                        map[y][x] = str[x];
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day19.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

