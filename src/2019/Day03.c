/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 20:07:23 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(ivec2) tllivec2;

// Start is exclusive, end is inclusive
typedef struct line{
        ivec2 start;
        ivec2 end;
        bool vertical;
        int dist;
} line;
typedef tll(line) tllline;

struct intersection {
        ivec2 pos;
        int dist;
};

struct input {
        tllline wire1;
        tllline wire2;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printWire(tllline wire) {
        tll_foreach(wire, it) {
                line l = it->item;
                printf("(%d, %d) -> (%d, %d)\n", l.start.x, l.start.y, l.end.x, l.end.y);
        }
        printf("\n");
}

int pointOnLine(line l, ivec2 p) {
        // Vertical Line
        if (l.vertical) {
                if (p.x != l.start.x) return -1;
                if ((p.y > l.start.y && p.y <= l.end.y) || (p.y < l.start.y && p.y >= l.end.y)) {
                        return abs(p.y - l.start.y);
                }
                return -1;
        } else { // Horizontal Line
                if (p.y != l.start.y) return -1;
                if ((p.x > l.start.x && p.x <= l.end.x) || (p.x < l.start.x && p.x >= l.end.x)) {
                        return abs(p.x - l.start.x);
                }
                 return -1;
        }
}

struct intersection findIntersection(line l1, line l2) {
        struct intersection intersection = {{{0, 0}}, -1};

        // If both lines are vertical or both are horizontal, they don't intersect
        if (l1.vertical == l2.vertical) return intersection;

        // Get point where 2 lines cross, ignoring endpoints
        intersection.pos.x = (l1.vertical) ? l1.start.x : l2.start.x;
        intersection.pos.y = (l1.vertical) ? l2.start.y : l1.start.y;

        // Check if point lies on both lines
        int dist1 = pointOnLine(l1, intersection.pos);
        int dist2 = pointOnLine(l2, intersection.pos);
        if (dist1 == -1 || dist2 == -1)
                return intersection;

        intersection.dist = l1.dist + dist1 + l2.dist + dist2;

        return intersection;
}

void part1_2(struct input *input) {
        tllline *wire1 = &input->wire1;
        tllline *wire2 = &input->wire2;

        // Check each pair of lines for intersections
        // If intersection found, check if is min dist from origin and min dist along wires
        int minDist = INT_MAX;
        int minWireDist = INT_MAX;
        tll_foreach(*wire1, it1) {
                line *l1 = &it1->item;
                tll_foreach(*wire2, it2) {
                        line *l2 = &it2->item;
                        struct intersection intersection = findIntersection(*l1, *l2);
                        if (intersection.dist != -1) {
                                // Get manhattan distance from origin
                                int mdist = abs(intersection.pos.x) + abs(intersection.pos.y);
                                if (mdist < minDist)
                                        minDist = mdist;
                                if (intersection.dist < minWireDist)
                                        minWireDist = intersection.dist;
                        }
                }
        }

        printf("Part 1: %d\n", minDist);
        printf("Part 2: %d\n", minWireDist);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *wire1 = (char*)ll->head->data;
        char *wire2 = (char*)ll->tail->data;

        for (int i = 0; i < 2; i++) {
                char *wire = i == 0 ? wire1 : wire2;
                tllline *wireLines = (i == 0) ? &input.wire1 : &input.wire2;

                ivec2 pos = {{0, 0}};
                ivec2 nextPos = {{0, 0}};
                int dist = 0;
                char *path = strtok(wire, ",");
                while (path != NULL) {
                        bool vertical = false;
                        int num = strtol(path + 1, NULL, 10);
                        switch (path[0]) {
                        case 'U':
                                nextPos.y -= num;
                                vertical = true;
                                break;
                        case 'R':
                                nextPos.x += num;
                                vertical = false;
                                break;
                        case 'D':
                                nextPos.y += num;
                                vertical = true;
                                break;
                        case 'L':
                                nextPos.x -= num;
                                vertical = false;
                                break;
                        default:
                                printf("Error: Invalid path '%s'\n", path);
                        }

                        line l = {pos, nextPos, vertical, dist};
                        tll_push_back(*wireLines, l);
                        path = strtok(NULL, ",");
                        pos = nextPos;
                        dist += num;
                }
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

