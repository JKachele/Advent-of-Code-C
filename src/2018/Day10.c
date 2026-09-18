/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 18, 2026 13:15:08 EDT
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

struct point {
        lvec2 pos;
        lvec2 vel;
};
typedef tal(struct point) talpoint;

struct input {
        talpoint points;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printSky(talpoint points, lvec2 lower, lvec2 upper) {
        lvec2 size = {{upper.x - lower.x + 1, upper.y - lower.y + 1}};
        bool sky[size.y][size.x];
        for (int y = 0; y < size.y; y++)
                for (int x = 0; x < size.x; x++)
                        sky[y][x] = false;

        tal_for(points, i) {
                lvec2 pos = points.array[i].pos;
                sky[pos.y - lower.y][pos.x - lower.x] = true;
        }
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        printf("%c", sky[y][x] ? '#': '.');
                }
                printf("\n");
        }
        printf("\n");
}

long getArea(lvec2 lower, lvec2 upper) {
        return (upper.x - lower.x) * (upper.y - lower.y);
}

void part1_2(struct input *input) {
        talpoint points = input->points;

        lvec2 lower = {{INT32_MAX, INT32_MAX}};
        lvec2 upper = {{INT32_MIN, INT32_MIN}};
        tal_for(points, i) {
                struct point p = points.array[i];
                if (p.pos.x < lower.x) lower.x = p.pos.x;
                if (p.pos.y < lower.y) lower.y = p.pos.y;
                if (p.pos.x > upper.x) upper.x = p.pos.x;
                if (p.pos.y > upper.y) upper.y = p.pos.y;
        }
        long area = getArea(lower, upper); 
        long lastArea;
        // printSky(points, lower, upper);

        int time = 0;
        // Message should be when te area is the smallest
        do {
                lastArea = area;
                lower = (lvec2){{INT32_MAX, INT32_MAX}};
                upper = (lvec2){{INT32_MIN, INT32_MIN}};
                // Move points and calculate bounds
                tal_for(points, i) {
                        struct point *p = &points.array[i];

                        // Move point
                        p->pos.x += p->vel.x;
                        p->pos.y += p->vel.y;

                        // Calc bounds
                        if (p->pos.x < lower.x) lower.x = p->pos.x;
                        if (p->pos.y < lower.y) lower.y = p->pos.y;
                        if (p->pos.x > upper.x) upper.x = p->pos.x;
                        if (p->pos.y > upper.y) upper.y = p->pos.y;
                }
                area = getArea(lower, upper);
                time++;
                // printSky(points, lower, upper);
        } while (lastArea >= area);

        // Undo last move
        tal_for(points, i) {
                struct point *p = &points.array[i];
                p->pos.x -= p->vel.x;
                p->pos.y -= p->vel.y;
        }
        printSky(points, lower, upper);

        printf("Part 2: Time = %d\n", time - 1);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                struct point point;
                strtok(str, "<");
                point.pos.x = strtol(strtok(NULL, ","), NULL, 10);
                point.pos.y = strtol(strtok(NULL, ">"), NULL, 10);
                strtok(NULL, "<");
                point.vel.x = strtol(strtok(NULL, ","), NULL, 10);
                point.vel.y = strtol(strtok(NULL, ">"), NULL, 10);

                tal_add(input.points, point);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day10.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day10.txt";
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

