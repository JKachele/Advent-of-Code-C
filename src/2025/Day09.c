/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Dec 09, 2025 13:47:07 UTC
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

typedef struct line {
        bool vert;
        ivec2 start;
        ivec2 end;
} line;

struct input {
        int numTiles;
        ivec2 *tiles;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

// Line a1-a2 is vertical
bool intersectVert(ivec2 a1, ivec2 a2, line l, bool right) {
        // Parallel lines cannot intersect
        if (l.vert) return false;

        line rectLine = {true, a1, a2};

        // If x coord of vert line is between x coords of horizontal line and 
        // y coord of horizontal line is between the y coords of the vert line
        bool xBetween = (l.start.x < rectLine.start.x &&
                        rectLine.start.x < l.end.x) ||
                        (l.end.x < rectLine.start.x &&
                        rectLine.start.x < l.start.x);

        bool yBetween = (rectLine.start.y < l.start.y &&
                        l.start.y < rectLine.end.y) ||
                        (rectLine.end.y < l.start.y &&
                        l.start.y < rectLine.start.y);

        if (xBetween && yBetween)
                return true;

        // If line starts or ends on the other line,
        // intersects is line extends into rectangle
        if (yBetween && l.start.x == rectLine.start.x) {
                if (right && l.end.x < l.start.x)
                        return true;
                if (!right && l.start.x < l.end.x)
                        return true;
        }
        if (yBetween && l.end.x == rectLine.start.x) {
                if (right && l.start.x < l.end.x)
                        return true;
                if (!right && l.end.x < l.start.x)
                        return true;
        }
        return false;
}

// Line a1-a2 is horizontal
bool intersectHoriz(ivec2 a1, ivec2 a2, line l, bool top) {
        // Parallel lines cannot intersect
        if (!l.vert) return false;

        line rectLine = {false, a1, a2};

        // If x coord of vert line is between x coords of horizontal line and 
        // y coord of horizontal line is between the y coords of the vert line
        bool xBetween = (rectLine.start.x < l.start.x &&
                        l.start.x < rectLine.end.x) ||
                        (rectLine.end.x < l.start.x &&
                        l.start.x < rectLine.start.x);

        bool yBetween = (l.start.y < rectLine.start.y &&
                        rectLine.start.y < l.end.y) ||
                        (l.end.y < rectLine.start.y &&
                        rectLine.start.y < l.start.y);

        if (xBetween && yBetween)
                return true;

        // If line starts or ends on the other line,
        // intersects is line extends into rectangle
        if (xBetween && l.start.y == rectLine.start.y) {
                if (top && l.end.y < l.start.y)
                        return true;
                if (!top && l.start.y < l.end.y)
                        return true;
        }
        if (xBetween && l.end.y == rectLine.start.y) {
                if (top && l.start.y < l.end.y)
                        return true;
                if (!top && l.end.y < l.start.y)
                        return true;
        }
        return false;
}

bool pointOnLine(ivec2 p, line l) {
        if (l.vert) {
                return l.start.x == p.x &&
                        ((l.start.y <= p.y && l.end.y > p.y) ||
                         (l.start.y >= p.y && l.end.y < p.y));
        } else {
                return l.start.y == p.y &&
                        ((l.start.x <= p.x && l.end.x > p.x) ||
                         (l.start.x >= p.x && l.end.x < p.x));
        }
}

bool pointInside(ivec2 p, line *lines, int numLines) {
        const int MAX_Y = 10000000; // All inputs are below 1,000,000
        // Make a vertical line starting at the point and see if
        // an odd number of lines intersect
        ivec2 pointLineEnd   = {{p.x, MAX_Y}};

        int numIntersections = 0;
        for (int i = 0; i < numLines; i++) {
                line l = lines[i];
                // if point is on a line, it is inside
                if (pointOnLine(p, l))
                        return true;

                if (l.vert) {
                        continue;
                }

                // Y coord check
                if (l.start.y <= p.y)
                        continue;

                // X coord check
                if ((l.start.x <= p.x && l.end.x > p.x) ||
                    (l.end.x <= p.x && l.start.x > p.x))
                        numIntersections++;
        }

        return (numIntersections % 2 == 1);
}

bool rectValid(ivec2 corners, int numTiles, ivec2 *tiles, line *lines) {
        ivec2 c1 = tiles[corners.x];
        ivec2 c2 = tiles[corners.y];
        // Get Top-Right and Bottom-Left corners
        ivec2 tr, bl;
        tr.x = c1.x >= c2.x ? c1.x : c2.x;
        tr.y = c1.y >= c2.y ? c1.y : c2.y;
        bl.x = c1.x <  c2.x ? c1.x : c2.x;
        bl.y = c1.y <  c2.y ? c1.y : c2.y;

        // If rect is only 1 wide/tall, is Valid
        if (tr.x - bl.x == 0 || tr.y - bl.y == 0)
                return true;

        // Right Line
        ivec2 start = tr;
        ivec2 end = {{tr.x, bl.y}};
        for (int i = 0; i < numTiles; i++) {
                if (intersectVert(start, end, lines[i], true))
                        return false;
        }

        // Bottom Line
        start = end;
        end = bl;
        for (int i = 0; i < numTiles; i++) {
                if (intersectHoriz(start, end, lines[i], false))
                        return false;
        }

        // Left Line
        start = end;
        end = (ivec2){{bl.x, tr.y}};
        for (int i = 0; i < numTiles; i++) {
                if (intersectVert(start, end, lines[i], false))
                        return false;
        }

        // Top Line
        start = end;
        end = tr;
        for (int i = 0; i < numTiles; i++) {
                if (intersectHoriz(start, end, lines[i], true))
                        return false;
        }

        // Make sure rectangle in inside the loop
        // Check all 4 corners are inside the loop
        // 2 given corners are intside the loop by definition
        ivec2 c3 = {{c1.x, c2.y}};
        ivec2 c4 = {{c2.x, c1.y}};
        bool c3Inside = false;
        bool c4Inside = false;
        // if corner is one of the red tiles, it is inside
        for (int i = 0; i < numTiles; i++) {
                if (ivec2Eq(c3, tiles[i]))
                        c3Inside = true;
                if (ivec2Eq(c4, tiles[i]))
                        c4Inside = true;
        }

        // Check with raycasting if point isnt one of the red tiles
        if (!c3Inside && pointInside(c3, lines, numTiles))
                c3Inside = true;
        if (!c4Inside && pointInside(c4, lines, numTiles))
                c4Inside = true;

        return c3Inside && c4Inside;
}

void part1(struct input input) {
        int numTiles = input.numTiles;
        ivec2 *tiles = input.tiles;

        int64 largestArea = 0;
        for (int i = 0; i < numTiles; i++) {
                for (int j = i + 1; j < numTiles; j++) {
                        int64 xDist = abs(tiles[i].x - tiles[j].x) + 1;
                        int64 yDist = abs(tiles[i].y - tiles[j].y) + 1;
                        int64 area = xDist * yDist;
                        if (area > largestArea)
                                largestArea = area;
                }
        }

        printf("Part 1: %ld\n\n", largestArea);
}

void part2(struct input input) {
        int numTiles = input.numTiles;
        ivec2 *tiles = input.tiles;

        line *lines = malloc(numTiles * sizeof(line));
        for (int i = 0; i < numTiles; i++) {
                int j = i + 1;
                if (j == numTiles) j = 0;

                lines[i].start = tiles[i];
                lines[i].end = tiles[j];
                lines[i].vert = tiles[i].x == tiles[j].x;
        }

        int64 largestArea = 0;
        for (int i = 0; i < numTiles; i++) {
                for (int j = i + 1; j < numTiles; j++) {
                        ivec2 corners = {{i, j}};
                        int64 xDist = abs(tiles[i].x - tiles[j].x) + 1;
                        int64 yDist = abs(tiles[i].y - tiles[j].y) + 1;
                        int64 area = xDist * yDist;
                        if (area > largestArea &&
                            rectValid(corners, numTiles, tiles, lines)) {
                                largestArea = area;
                        }
                }
        }

        printf("Part 2: %ld\n", largestArea);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int numTiles = ll->length;
        ivec2 *tiles = malloc(numTiles * sizeof(ivec2));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                tiles[i].x = strtol(strtok(str, ","), NULL, 10);
                tiles[i].y = strtol(strtok(NULL, ""), NULL, 10);
                i++;
        }

        input.numTiles = numTiles;
        input.tiles = tiles;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day09.txt";
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

