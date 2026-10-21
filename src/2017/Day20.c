/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 20, 2026 14:35:20 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <math.h>
#include <float.h>
#include <stdint.h>
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

struct particle {
        bool valid;
        ivec3 pos;
        ivec3 vel;
        ivec3 acc;
        int *collisions;
};

struct input {
        int numP;
        struct particle *particles;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printParticles(int numP, struct particle parts[]) {
        for (int i = 0; i < numP; i++) {
                struct particle p = parts[i];
                if (!p.valid) {
                        printf("Invalid!\n");
                        continue;
                }
                printf("P=(%d, %d, %d) ", p.pos.x, p.pos.y, p.pos.z);
                printf("V=(%d, %d, %d) ", p.vel.x, p.vel.y, p.vel.z);
                printf("A=(%d, %d, %d)\n\t(", p.acc.x, p.acc.y, p.acc.z);
                for (int j = 0; j < numP; j++) {
                        if (p.collisions[j] == -1) {
                                printf("-, ");
                        } else {
                                printf("%d, ", p.collisions[j]);
                        }
                }
                printf("\b)\n");
        }
}

bool validTime(ivec3 dx, ivec3 dv, ivec3 da, int t) {
        if (t < 0) {
                return false;
        }

        //       a                b                   c
        // (da * (t^2)) + (((2 * dv) + da) * t) + (2 * dx) = 0
        for (int i = 0; i < 3; i++) {
                int a = da.raw[i] * (t * t);
                int b = ((2 * dv.raw[i]) + da.raw[i]) * t;
                int c = 2 * dx.raw[i];
                if ((a + b + c) != 0) {
                        return false;
                }
        }
        return true;
}

int getCollision(struct particle p0, struct particle p1) {
        ivec3 dx = ivec3Sub(p0.pos, p1.pos);
        ivec3 dv = ivec3Sub(p0.vel, p1.vel);
        ivec3 da = ivec3Sub(p0.acc, p1.acc);

        // Collision only if there is a time where the following is true for x, y, and z
        // (da * (t^2)) + (((2 * dv) + da) * t) + (2 * dx) = 0
        int t1 = -1;
        int t2 = -1;

        // If no acceleration delta, can use linear equasion
        if (da.x == 0 && da.y == 0 && da.z == 0) {
                t1 = (-1) * (dx.x / dv.x);
        } else {
                // Use quadratic formula to find t. Only use if delta acc is non zero
                // a = da, b = ((2 * dv) + da), c = (2 * dx)
                for (int i = 0; i < 3; i++) {
                        if (da.raw[i] == 0) continue;

                        int a = da.raw[i];
                        int b = (2 * dv.raw[i]) + da.raw[i];
                        int c = 2 * dx.raw[i];

                        int discriminant = (b * b) - (4 * a * c);
                        if (discriminant < 0) {
                                return -1; // If no real roots, no collision possible
                        }

                        int root = (int)sqrt(discriminant);

                        // If discriminant isnt a perfect square,
                        // will collide at non-integer time which is not valid
                        if ((root * root) != discriminant) {
                                return -1;
                        }

                        // Get 2 zeros
                        t1 = (-b + root) / (2 * a);
                        t2 = (-b - root) / (2 * a);
                        break;
                }
        }

        // Check if either time is valid for x, y, and z
        bool t1Valid = validTime(dx, dv, da, t1);
        bool t2Valid = validTime(dx, dv, da, t2);

        // If both valid, use smaller. If neither is valid, return -1
        if (t1Valid && t2Valid) {
                return (t1 < t2) ? t1 : t2;
        } else if (t1Valid) {
                return t1;
        } else if (t2Valid) {
                return t2;
        } else {
                return -1;
        }
}

void collideParticles(int numP, struct particle parts[], int time) {
        bool collided[numP];
        for (int i = 0; i < numP; i++) { collided[i] = false; }

        for (int i = 0; i < numP; i++) {
                struct particle *p1 = &parts[i];
                for (int j = i+1; j < numP; j++) {
                        struct particle *p2 = &parts[j];

                        if (p1->collisions[j] != time) {
                                continue;
                        }

                        p1->valid = false;
                        p2->valid = false;
                        collided[i] = true;
                        collided[j] = true;
                }
        }

        // Remove collisions with destroyed particles
        for (int i = 0; i < numP; i++) {
                if (!collided[i]) { continue; }
                for (int j = 0; j < numP; j++) {
                        parts[j].collisions[i] = -1; 
                }
        }
}

void part1(struct input *input) {
        int numP = input->numP;
        struct particle *parts = input->particles;
        // printParticles(numP, parts);

        // Use x + vt + at^2 to get dist at large time
        int64 dist[numP];
        const int64 time = 1000000l;
        const int64 time2 = time * time;
        for (int i = 0; i < numP; i++) {
                struct particle p = parts[i];
                int64 x = p.pos.x + (p.vel.x * time) + (p.acc.x * time2);
                int64 y = p.pos.y + (p.vel.y * time) + (p.acc.y * time2);
                int64 z = p.pos.z + (p.vel.z * time) + (p.acc.z * time2);
                dist[i] = labs(x) + labs(y) + labs(z);
        }

        // Find smallest distance
        int64 minDist = INT64_MAX;
        int minPart = -1;
        for (int i = 0; i < numP; i++) {
                if (dist[i] < minDist) {
                        minDist = dist[i];
                        minPart = i;
                }
        }

        printf("Part 1: Particle with smallest distance = %d\n\n", minPart);
}

/* Getting the discrete position formula:
 *      v(t) = v(t-1) + a = v(0) + (a*t)
 *      x(t) = x(t-1) + v(t) = x(t-1) + v(0) + (a*t)
 *      x(t) = x(0) + (v(0) * t) + (a * (t * (t - 1)) / 2)
 */
void part2(struct input *input) {
        int numP = input->numP;
        struct particle *particles = input->particles;
        // printParticles(numP, particles);

        // For each pair of particles, determine if and when they will colide
        // Use discrete position formula
        // x(t) = x(0) + (v(0) * t) + (a(0) * ((t * (t - 1)) / 2))
        int maxCollision = 0;
        for (int i = 0; i < numP; i++) {
                particles[i].collisions[i] = -1;
                for (int j = i + 1; j < numP; j++) {
                        int collision = getCollision(particles[i], particles[j]);
                        particles[i].collisions[j] = collision;
                        particles[j].collisions[i] = collision;
                        if (collision > maxCollision) {
                                maxCollision = collision;
                        }
                }
        }

        // For all times until last collision, check if a particle collided
        // Remove that particle from all other's collision list
        // Multiple particles can collide at the same time
        for (int t = 0; t <= maxCollision; t++) {
                collideParticles(numP, particles, t);
        }
        
        int numLeft = 0;
        for (int i = 0; i < numP; i++) {
                if (particles[i].valid) {
                        numLeft++;
                }
        }

        printf("Part 2: Particles Left = %d\n", numLeft);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numP = ll->length;
        input.particles = malloc(input.numP * sizeof(struct particle));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                ivec3 pos;
                char *tok = strtok(str+3, ",");
                pos.x = strtol(tok, NULL, 10);
                tok = strtok(NULL, ",");
                pos.y = strtol(tok, NULL, 10);
                tok = strtok(NULL, ">");
                pos.z = strtol(tok, NULL, 10);
                tok = strtok(NULL, "<");

                ivec3 vel;
                tok = strtok(NULL, ",");
                vel.x = strtol(tok, NULL, 10);
                tok = strtok(NULL, ",");
                vel.y = strtol(tok, NULL, 10);
                tok = strtok(NULL, ">");
                vel.z = strtol(tok, NULL, 10);
                tok = strtok(NULL, "<");

                ivec3 acc;
                tok = strtok(NULL, ",");
                acc.x = strtol(tok, NULL, 10);
                tok = strtok(NULL, ",");
                acc.y = strtol(tok, NULL, 10);
                tok = strtok(NULL, ">");
                acc.z = strtol(tok, NULL, 10);

                input.particles[i].valid = true;
                input.particles[i].pos = pos;
                input.particles[i].vel = vel;
                input.particles[i].acc = acc;
                input.particles[i].collisions = malloc(input.numP * sizeof(int));

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day20.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day20.txt";
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
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
        //                 parseTime, pt1Time);

        return 0;
}

