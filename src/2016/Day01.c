/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 23, 2026 10:52:42 EDT
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

struct instruction {
        bool right;
        int dist;
};
typedef tal(struct instruction) talinstr;

struct input {
        talinstr instrs;
};

void printInstrs(talinstr instrs) {
        tal_for(instrs, i) {
                struct instruction instr = instrs.array[i];
                printf("%s %d\n", instr.right ? "Right" : "Left", instr.dist);
        }
}

ivec2 lineIntersection(ivec4 l1, ivec4 l2) {
        ivec2 intersection = {{-1, -1}};

        bool l1Vert = (l1.v1.x == l1.v2.x);
        bool l2Vert = (l2.v1.x == l2.v2.x);
        if (l1Vert == l2Vert) {
                // Parallel lines don't cross
                return intersection;
        }

        if (l1Vert) {
                // Check x axis
                if (l1.v1.x <= l2.v1.x && l1.v1.x < l2.v2.x) {
                        return intersection;
                }
                if (l1.v1.x >= l2.v1.x && l1.v1.x > l2.v2.x) {
                        return intersection;
                }
                // Check y axis
                if (l2.v1.y <= l1.v1.y && l2.v1.y < l1.v2.y) {
                        return intersection;
                }
                if (l2.v1.y >= l1.v1.y && l2.v1.y > l1.v2.y) {
                        return intersection;
                }
                return (ivec2){{l1.v1.x, l2.v1.y}};
        } else {
                // Check x axis
                if (l2.v1.x <= l1.v1.x && l2.v1.x < l1.v2.x) {
                        return intersection;
                }
                if (l2.v1.x >= l1.v1.x && l2.v1.x > l1.v2.x) {
                        return intersection;
                }
                // Check y axis
                if (l1.v1.y <= l2.v1.y && l1.v1.y < l2.v2.y) {
                        return intersection;
                }
                if (l1.v1.y >= l2.v1.y && l1.v1.y > l2.v2.y) {
                        return intersection;
                }
                return (ivec2){{l2.v1.x, l1.v1.y}};
        }
}

ivec2 addLine(int *numLines, ivec4 lines[], ivec2 prev, ivec2 pos) {
        ivec4 line;
        line.v1 = prev;
        line.v2 = pos;
        for (int i = 0; i < *numLines; i++) {
                ivec2 intersect = lineIntersection(lines[i], line);
                if (intersect.x != -1) {
                        return intersect;
                }
        }

        lines[(*numLines)++] = line;
        return (ivec2){{-1, -1}};
}

void part1(struct input *input) {
        const ivec2 Dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        talinstr instrs = input->instrs;
        // printInstrs(instrs);

        ivec2 pos = {0};
        int dir = 0;
        tal_for(instrs, i) {
                struct instruction instr = instrs.array[i];
                dir += instr.right ? 1 : 3;
                dir %= 4;
                ivec2 move = ivec2Scale(Dirs[dir], instr.dist);
                pos = ivec2Add(pos, move);
        }

        int dist = abs(pos.x) + abs(pos.y);

        printf("Part 1: Distance = %d\n\n", dist);
}

void part2(struct input *input) {
        const ivec2 Dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        talinstr instrs = input->instrs;
        // printInstrs(instrs);

        int numLines = 0;
        ivec4 lines[instrs.length];

        ivec2 prev = {0};
        ivec2 pos = {0};
        int dir = 0;
        ivec2 intersect = {0};
        tal_for(instrs, i) {
                struct instruction instr = instrs.array[i];
                dir += instr.right ? 1 : 3;
                dir %= 4;
                ivec2 move = ivec2Scale(Dirs[dir], instr.dist);
                prev = pos;
                pos = ivec2Add(pos, move);
                intersect = addLine(&numLines, lines, prev, pos);
                if (intersect.x != -1) {
                        break;
                }
        }

        int dist = abs(intersect.x) + abs(intersect.y);

        printf("Part 2: Distance = %d\n", dist);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *tok = strtok(str, ", ");
        while (tok != NULL) {
                struct instruction instr;
                instr.right = (tok[0] == 'R');
                instr.dist = strtol(tok+1, NULL, 10);
                tal_add(input.instrs, instr);
                tok = strtok(NULL, ", ");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day01.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day01.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(pt2 - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

