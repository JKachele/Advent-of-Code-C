/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 08, 2025 22:13:21 UTC
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
#define TEST_MIN 7
#define TEST_MAX 27
#define MIN 200000000000000
#define MAX 400000000000000

// Vector equation of a plane in the form of w.vec = c
typedef struct plane {
        lvec3 vec;
        int64 c;
} plane;

typedef union lvec5 {
        int64 raw[5];
        struct {
                int64 a;
                int64 b;
                int64 c;
                int64 d;
                int64 e;
        };
} lvec5;

typedef union mat4x4 {
        int64 raw[4][4];
        lvec4 col[4];
        struct {
                int64 m00, m01, m02, m03;
                int64 m10, m11, m12, m13;
                int64 m20, m21, m22, m23;
                int64 m30, m31, m32, m33;
        };
} mat4x4;

typedef struct stone {
        lvec3 pos;
        lvec3 vel;
} stone;

struct input {
        stone *stones;
        int32 numStones;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printStone(stone s) {
        if (!Debug) return;
        printf("(%ld, %ld, %ld) - ", s.pos.x, s.pos.y, s.pos.z);
        printf("(%ld, %ld, %ld)\n", s.vel.x, s.vel.y, s.vel.z);
}

void printStones(int32 numStones, stone *stones) {
        if (!Debug) return;
        for (int i=0; i<numStones; i++)
                printStone(stones[i]);
        printf("\n");
}

lvec3 cross(lvec3 a, lvec3 b) {
        lvec3 c;
        c.x = (a.y * b.z) - (a.z * b.y);
        c.y = (a.z * b.x) - (a.x * b.z);
        c.z = (a.x * b.y) - (a.y * b.x);
        return c;
}

int64 dot(lvec3 a, lvec3 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

bool intersect2d(stone a, stone b, int64 minVal, int64 maxVal) {
        double m1 = (double)a.vel.y / a.vel.x;
        double m2 = (double)b.vel.y / b.vel.x;
        if (m1 == m2) // Parallel
                return false;

        double b1 = a.pos.y - (m1 * a.pos.x);
        double b2 = b.pos.y - (m2 * b.pos.x);

        double xInt = (b2 - b1) / (m1 - m2);
        double yInt = (m1 * xInt) + b1;
        // printf("    %0.2f, %0.2f\n", xInt, yInt);

        // Intersection muse be in range
        if (xInt < minVal || xInt > maxVal)
                return false;
        if (yInt < minVal || yInt > maxVal)
                return false;

        // Intersection must be in the stone's future
        if ((a.vel.x > 0 && xInt < a.pos.x) || (a.vel.x < 0 && xInt > a.pos.x))
                return false;
        if ((a.vel.y > 0 && yInt < a.pos.y) || (a.vel.y < 0 && yInt > a.pos.y))
                return false;
        if ((b.vel.x > 0 && xInt < b.pos.x) || (b.vel.x < 0 && xInt > b.pos.x))
                return false;
        if ((b.vel.y > 0 && yInt < b.pos.y) || (b.vel.y < 0 && yInt > b.pos.y))
                return false;

        return true;
}

bool linearIndependent(stone a, stone b) {
        lvec3 c = cross(a.vel, b.vel);
        return (c.x != 0) || (c.y != 0) || (c.z != 0);
}

plane findPLane(stone a, stone b) {
        lvec3 pos = lvec3Sub(a.pos, b.pos);
        lvec3 vel = lvec3Sub(a.vel, b.vel);
        lvec3 vCross = cross(a.vel, b.vel);
        plane p;
        p.vec = cross(pos, vel);
        p.c = dot(pos, vCross);
        return p;
}

stone getRock(int32 numStones, stone *stones) {
        stone rock = {0};

        // Get 3 linearly independent stones
        stone s1 = stones[0];
        stone s2;
        stone s3;
        int i;
        for (i=1; i<numStones; i++) {
                if (linearIndependent(s1, stones[i])) {
                        s2 = stones[i];
                        break;
                }
        }
        for (i+=1; i<numStones; i++) {
                if (linearIndependent(s1, stones[i]) &&
                                linearIndependent(s2, stones[i])) {
                        s3 = stones[i];
                        break;
                }
        }

        // Find 3 planes of possible rock velocities
        plane pa = findPLane(s1, s2);
        plane pb = findPLane(s1, s3);
        plane pc = findPLane(s2, s3);

        return rock;
}

lvec4 vec5to4(lvec5 vec) {
        return (lvec4){{vec.raw[0], vec.raw[1], vec.raw[2], vec.raw[3]}};
}

int64 det(mat4x4 mat) {
        // a b c d
        // e f g h
        // i j k l
        // m n o p
        int64 a = mat.m00, b = mat.m01, c = mat.m02, d = mat.m03;
        int64 e = mat.m10, f = mat.m11, g = mat.m12, h = mat.m13;
        int64 i = mat.m20, j = mat.m21, k = mat.m22, l = mat.m23;
        int64 m = mat.m30, n = mat.m31, o = mat.m32, p = mat.m33;

        int64 det_kl = (k * p) - (l * o);
        int64 det_jl = (j * p) - (l * n);
        int64 det_jk = (j * o) - (k * n);
        int64 det_il = (i * p) - (l * m);
        int64 det_ik = (i * o) - (k * m);
        int64 det_ij = (i * n) - (j * m);

        int64 detA = (f * det_kl) - (g * det_jl) + (h * det_jk);
        int64 detB = (e * det_kl) - (g * det_il) + (h * det_ik);
        int64 detC = (e * det_jl) - (f * det_il) + (h * det_ij);
        int64 detD = (e * det_jk) - (f * det_ik) + (g * det_ij);

        return (a * detA) - (b * detB) + (c * detC) - (d * detD);
}

// Get 4 linearly independent stones
void getLinearIndepStones(int32 numStones, stone *stones, stone *stoneOut) {
        stone s1 = stones[0];
        stone s2;
        stone s3;
        stone s4;
        int i;
        for (i=1; i<numStones; i++) {
                if (linearIndependent(stoneOut[0], stones[i])) {
                        s2 = stones[i];
                        break;
                }
        }
        for (i+=1; i<numStones; i++) {
                if (linearIndependent(s1, stones[i]) &&
                                linearIndependent(s2, stones[i])) {
                        s3 = stones[i];
                        break;
                }
        }
        for (i+=1; i<numStones; i++) {
                if (linearIndependent(s1, stones[i]) &&
                                linearIndependent(s2, stones[i]) &&
                                linearIndependent(s3, stones[i])) {
                        s4 = stones[i];
                        break;
                }
        }
        stoneOut[0] = s1;
        stoneOut[1] = s2;
        stoneOut[2] = s3;
        stoneOut[3] = s4;
}

// Get coefficients for the linear equation for the x/y pos and vel of the rock
lvec5 getEqCoefs(stone a, stone b, bool xy) {
        lvec5 eqCoefs;
        if (xy) {
                eqCoefs.a = b.vel.y - a.vel.y;
                eqCoefs.b = a.vel.x - b.vel.x;
                eqCoefs.c = b.pos.y - a.pos.y;
                eqCoefs.d = b.pos.x - a.pos.x;
                eqCoefs.e =  (a.pos.y * a.vel.x) - (b.pos.y * b.vel.x);
                eqCoefs.e += (b.pos.x * b.vel.y) - (a.pos.x * a.vel.y);
        } else {
                eqCoefs.a = b.vel.z - a.vel.z;
                eqCoefs.b = a.vel.x - b.vel.x;
                eqCoefs.c = b.pos.z - a.pos.z;
                eqCoefs.d = b.pos.x - a.pos.x;
                eqCoefs.e =  (a.pos.z * a.vel.x) - (b.pos.z * b.vel.x);
                eqCoefs.e += (b.pos.x * b.vel.z) - (a.pos.x * a.vel.z);
        }
        return eqCoefs;
}

lvec2 cramers(mat4x4 coefs, lvec4 consts) {
        int64 coefDet = det(coefs);

        mat4x4 xMat = coefs;
        xMat.m00 = consts.x;
        xMat.m10 = consts.y;
        xMat.m20 = consts.z;
        xMat.m30 = consts.w;
        int64 xDet = det(xMat);

        mat4x4 yMat = coefs;
        yMat.m01 = consts.x;
        yMat.m11 = consts.y;
        yMat.m21 = consts.z;
        yMat.m31 = consts.w;
        int64 yDet = det(yMat);

        printf("CoefDet: %ld, XDet %ld, YDet %ld\n", coefDet, xDet, yDet);

        return (lvec2){0};
}

lvec3 getRockPos(int32 numStones, stone *stones) {
        stone indep[4];
        getLinearIndepStones(numStones, stones, indep);

        mat4x4 eqCoefs;
        lvec4 consts;
        for (int i=0; i<4; i++) {
                // lvec5 coefs = getEqCoefs(indep[i], indep[(i+1)%4]);
                lvec5 coefs = getEqCoefs(stones[i], stones[(i+1)%numStones], true);
                eqCoefs.col[i] = vec5to4(coefs);
                consts.raw[i] = coefs.e;
        }

        printf("\n");
        for (int i=0; i<4; i++) {
                printf("[ ");
                for (int j=0; j<4; j++) {
                        printf("%ld ", eqCoefs.raw[i][j]);
                }
                printf("] %ld\n", consts.raw[i]);
        }
        printf("\n");

        lvec2 sol = cramers(eqCoefs, consts);

        return (lvec3){0};
}

void part1(struct input input) {
        stone *stones = input.stones;
        int32 numStones = input.numStones;
        // printStones(numStones, stones);

        int64 minVal = Debug ? TEST_MIN : MIN;
        int64 maxVal = Debug ? TEST_MAX : MAX;
        int numIntersections = 0;
        for (int i=0; i<numStones; i++) {
                for (int j=i+1; j<numStones; j++) {
                        bool inter = intersect2d(stones[i], stones[j],
                                        minVal, maxVal);
                        if (inter)
                                numIntersections++;
                }
        }

        printf("Part 1: %d\n\n", numIntersections);
}

// Using linear algebra method described by u/Quantris on reddit
void part2(struct input input) {
        stone *stones = input.stones;
        int32 numStones = input.numStones;
        printStones(numStones, stones);

        lvec3 RockPos = getRockPos(numStones, stones);

        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numStones = ll->length;
        input.stones = calloc(input.numStones, sizeof(stone));

        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                stone *s2 = &input.stones[i];
                
                char *tok = strtok(str, ", ");
                for (int i=0; i<3; i++) {
                        s2->pos.raw[i] = strtol(tok, NULL, 10);
                        tok = strtok(NULL, ", ");
                }

                tok = strtok(NULL, ", ");
                for (int i=0; i<3; i++) {
                        s2->vel.raw[i] = strtol(tok, NULL, 10);
                        tok = strtok(NULL, ", ");
                }

                current = current->next;
                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day24.txt";
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

