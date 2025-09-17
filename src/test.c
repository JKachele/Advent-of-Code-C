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
#include "lib/tllist.h"
#include "util/util.h"
#include "util/vector.h"

typedef struct line {
        ivec2 a;
        ivec2 b;
} line;

int32 cross(ivec2 a, ivec2 b) {
        return (a.x * b.y) - (a.y * b.x);
}

int32 orient(ivec2 a, ivec2 b, ivec2 c) {
        ivec2 ba = ivec2Sub(b, a);
        ivec2 ca = ivec2Sub(c, a);
        return cross(ba, ca);
}

bool properInter(line a, line b) {
        int32 oa = orient(b.a, b.b, a.a);
        int32 ob = orient(b.a, b.b, a.b);
        int32 oc = orient(a.a, a.b, b.a);
        int32 od = orient(a.a, a.b, b.b);

        // Proper intersection iff opposite signs
        return ((oa * ob) < 0 && (oc * od) < 0);
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        line a  = {{{0, 9}}, {{5, 9}}};
        line b  = {{{0, 9}}, {{2, 9}}};
        printf("%s\n", properInter(a, b)? "True" : "False");
}

