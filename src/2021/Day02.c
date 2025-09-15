/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 15, 2025 17:05:43 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef enum direction {
        FORWARD,
        DOWN,
        UP
} direction;

typedef struct command {
        direction dir;
        int32 dist;
} command;

struct input {
        int32 numCmds;
        command *cmds;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input input) {
        command *cmds = input.cmds;

        ivec2 pos = {0};
        for (int i=0; i<input.numCmds; i++) {
                switch (cmds[i].dir) {
                case FORWARD:
                        pos.x += cmds[i].dist;
                        break;
                case DOWN:
                        pos.y += cmds[i].dist;
                        break;
                case UP:
                        pos.y -= cmds[i].dist;
                        break;
                }
        }

        int32 ans = pos.x * pos.y;

        printf("Part 1: %d\n\n", ans);
}

void part2(struct input input) {
        command *cmds = input.cmds;

        ivec2 pos = {0};
        int32 aim = 0;
        for (int i=0; i<input.numCmds; i++) {
                switch (cmds[i].dir) {
                case FORWARD:
                        pos.x += cmds[i].dist;
                        pos.y += aim * cmds[i].dist;
                        break;
                case DOWN:
                        aim += cmds[i].dist;
                        break;
                case UP:
                        aim -= cmds[i].dist;
                        break;
                }
        }

        int32 ans = pos.x * pos.y;

        printf("Part 1: %d\n\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input;
        input.numCmds = ll->length;
        command *cmds = calloc(input.numCmds, sizeof(command));

        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                char *dir = strtok(str, " ");
                int dist = strtol(strtok(NULL, " "), NULL, 10);

                cmds[i].dist = dist;
                if (dir[0] == 'f')
                        cmds[i].dir = FORWARD;
                if (dir[0] == 'd')
                        cmds[i].dir = DOWN;
                if (dir[0] == 'u')
                        cmds[i].dir = UP;

                current = current->next;
                i++;
        }
        input.cmds = cmds;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day02.txt";
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

