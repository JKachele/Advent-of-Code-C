/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 08, 2025 22:13:21 UTC
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
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096
#define TEST_MIN 7
#define TEST_MAX 27
#define MIN 200000000000000
#define MAX 400000000000000

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

typedef union vec5 {
        double raw[5];
        struct {
                double a;
                double b;
                double c;
                double d;
                double e;
        };
} vec5;

typedef union lmat4x4 {
        int64 raw[4][4];
        lvec4 col[4];
        struct {
                int64 m00, m01, m02, m03;
                int64 m10, m11, m12, m13;
                int64 m20, m21, m22, m23;
                int64 m30, m31, m32, m33;
        };
} lmat4x4;

typedef union mat4x4 {
        double raw[4][4];
        vec4 col[4];
        struct {
                double m00, m01, m02, m03;
                double m10, m11, m12, m13;
                double m20, m21, m22, m23;
                double m30, m31, m32, m33;
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

int64 adjustStones(int32 numStones, stone *stones) {
        if (numStones < 5)
                return -1;

        // Find the average position for the first 5 stones
        int64 avg = 0;
        for (int i=0; i<5; i++) {
                for (int j=0; j<3; j++) {
                        avg += stones[i].pos.raw[j];
                }
        }
        avg /= 15;

        // Subtract that from all positions
        for (int i=0; i<numStones; i++) {
                for (int j=0; j<3; j++) {
                        stones[i].pos.raw[j] -= avg;
                }
        }

        return avg;
}

vec4 vec5to4(vec5 vec) {
        return (vec4){{vec.raw[0], vec.raw[1], vec.raw[2], vec.raw[3]}};
}

double det(mat4x4 mat) {
        // a b c d
        // e f g h
        // i j k l
        // m n o p
        double a = mat.m00, b = mat.m01, c = mat.m02, d = mat.m03;
        double e = mat.m10, f = mat.m11, g = mat.m12, h = mat.m13;
        double i = mat.m20, j = mat.m21, k = mat.m22, l = mat.m23;
        double m = mat.m30, n = mat.m31, o = mat.m32, p = mat.m33;

        double det_kl = (k * p) - (l * o);
        double det_jl = (j * p) - (l * n);
        double det_jk = (j * o) - (k * n);
        double det_il = (i * p) - (l * m);
        double det_ik = (i * o) - (k * m);
        double det_ij = (i * n) - (j * m);

        double detA = (f * det_kl) - (g * det_jl) + (h * det_jk);
        double detB = (e * det_kl) - (g * det_il) + (h * det_ik);
        double detC = (e * det_jl) - (f * det_il) + (h * det_ij);
        double detD = (e * det_jk) - (f * det_ik) + (g * det_ij);

        return (a * detA) - (b * detB) + (c * detC) - (d * detD);
}

// Get coefficients for the linear equation for the x/y pos and vel of the rock
vec5 getEqCoefs(stone a, stone b, bool xy) {
        vec5 eqCoefs;
        if (xy) {
                eqCoefs.a =  (double)b.vel.y - a.vel.y;
                eqCoefs.b =  (double)a.vel.x - b.vel.x;
                eqCoefs.c =  (double)b.pos.y - a.pos.y;
                eqCoefs.d =  (double)b.pos.x - a.pos.x;
                eqCoefs.e =  (double)(a.pos.y * a.vel.x) - (b.pos.y * b.vel.x);
                eqCoefs.e += (double)(b.pos.x * b.vel.y) - (a.pos.x * a.vel.y);
        } else {
                eqCoefs.a =  (double)b.vel.z - a.vel.z;
                eqCoefs.b =  (double)a.vel.x - b.vel.x;
                eqCoefs.c =  (double)b.pos.z - a.pos.z;
                eqCoefs.d =  (double)b.pos.x - a.pos.x;
                eqCoefs.e =  (double)(a.pos.z * a.vel.x) - (b.pos.z * b.vel.x);
                eqCoefs.e += (double)(b.pos.x * b.vel.z) - (a.pos.x * a.vel.z);
        }
        return eqCoefs;
}

vec2 cramers(mat4x4 coefs, vec4 consts) {
        double coefDet = det(coefs);

        mat4x4 aMat = coefs;
        aMat.m00 = consts.x;
        aMat.m10 = consts.y;
        aMat.m20 = consts.z;
        aMat.m30 = consts.w;
        double aDet = det(aMat);

        mat4x4 bMat = coefs;
        bMat.m01 = consts.x;
        bMat.m11 = consts.y;
        bMat.m21 = consts.z;
        bMat.m31 = consts.w;
        double bDet = det(bMat);

        // printf("CoefDet: %0.2f, ADet %0.2f, BDet %0.2f\n", coefDet, aDet, bDet);

        return (vec2){{aDet / coefDet, bDet / coefDet}};
}

vec3 getRockPos(int32 numStones, stone *stones) {
        int64 scale = adjustStones(numStones, stones);

        mat4x4 eqCoefsXY;
        mat4x4 eqCoefsXZ;
        vec4 constsXY;
        vec4 constsXZ;
        for (int i=0; i<4; i++) {
                stone stoneA = stones[i];
                stone stoneB = stones[(i+1)%numStones];
                vec5 coefs = getEqCoefs(stoneA, stoneB, true);
                eqCoefsXY.col[i] = vec5to4(coefs);
                constsXY.raw[i] = coefs.e;

                coefs = getEqCoefs(stoneA, stoneB, false);
                eqCoefsXZ.col[i] = vec5to4(coefs);
                constsXZ.raw[i] = coefs.e;
        }

        vec2 xySol = cramers(eqCoefsXY, constsXY);
        vec2 xzSol = cramers(eqCoefsXZ, constsXZ);

        vec3 rockPos;
        rockPos.x = xySol.x + scale;
        rockPos.y = xySol.y + scale;
        rockPos.z = xzSol.y + scale;
        // printf("Rock Position: (%f, %f, %f)\n", rockPos.x, rockPos.y, rockPos.z);

        return rockPos;
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

        vec3 rockPos = getRockPos(numStones, stones);
        int64 posSum = rockPos.x + rockPos.y + rockPos.z;

        printf("Part 2: %ld\n", posSum);
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

