/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"
#include "util/vector.h"

#define max(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; })

typedef struct line {
        lvec2 start;
        lvec2 end;
} line;

// Checks if point p is on line a b
bool onSegment(lvec2 a, lvec2 b, lvec2 p) {
        return (p.x <= max(a.x, b.x) &&
                p.x >= min(a.x, b.x) &&
                p.y <= max(a.y, b.y) &&
                p.y >= min(a.y, b.y));
}

// Finds orientation of 3 points
// 0: collinear, 1: clockwise, 2: counterclockwise
int orientation(lvec2 a, lvec2 b, lvec2 c) {
        int64 o1 = (b.y - a.y) * (c.x - b.x);
        int64 o2 = (b.x - a.x) * (c.y - b.y);
        int64 val = o1 - o2;

        if (val == 0)
                return 0;
        return val > 0 ? 1 : 2;
}

bool intersect2d(line a, line b) {
        int o1 = orientation(a.start, a.end, b.start);
        int o2 = orientation(a.start, a.end, b.end);
        int o3 = orientation(b.start, b.end, a.start);
        int o4 = orientation(b.start, b.end, a.end);

        if (o1 != o2 && o3 != o4)
                return true;

        if (o1 == 0 && onSegment(a.start, a.end, b.start))
                return true;
        if (o2 == 0 && onSegment(a.start, a.end, b.end))
                return true;
        if (o3 == 0 && onSegment(b.start, b.end, a.start))
                return true;
        if (o4 == 0 && onSegment(b.start, b.end, a.end))
                return true;

        return false;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        line a = {{{0, 0}}, {{10, 10}}};
        line b = {{{5, 4}}, {{10, 0}}};

        printf("%s\n", intersect2d(a, b)? "Yes" : "No");

}

