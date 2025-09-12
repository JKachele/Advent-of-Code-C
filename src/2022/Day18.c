/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Aug 15, 2025 19:19:49 UTC
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
#include "../util/util.h"
#include "../util/vector.h"

typedef tll(ivec3) tllivec3;

const ivec3 Offsets[6] = {
        {{
                0, 0, 1
        }},{{
                0, 1, 0
        }},{{
                1, 0, 0
        }},{{
                0, 0, -1
        }},{{
                0, -1, 0
        }},{{
                -1, 0, 0
        }}
};

struct cube {
        bool lava;
        bool seen;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool isValidPosition(ivec3 size, ivec3 pos) {
        if (pos.x < 0 || pos.y < 0 || pos.z < 0 ||
                        pos.x >= size.x || pos.y >= size.y || pos.z >= size.z)
                return false;
        return true;
}

int32 getSurfaceArea(ivec3 size, bool drops[size.z][size.y][size.x]) {
        int32 surfaceArea = 0;
        MAKE_LOOP(z, size.z, y, size.y, x, size.x) {
                if (!drops[z][y][x]) continue;
                // printf("(%d,%d,%d)\n", x, y, z);
                for (int i=0; i<6; i++) {
                        ivec3 newPos = ivec3Add((ivec3){{x, y, z}}, Offsets[i]);
                        if (isValidPosition(size, newPos)) {
                                if (!drops[newPos.z][newPos.y][newPos.x])
                                        surfaceArea++;
                        } else {
                                surfaceArea++;
                        }
                }
        }
        return surfaceArea;
}

void fillSpace(ivec3 s, struct cube drops[s.z][s.y][s.x], ivec3 pos) {
        drops[pos.z][pos.y][pos.x].seen = true;
        for (int i=0; i<6; i++) {
                ivec3 new = ivec3Add(pos, Offsets[i]);
                if (isValidPosition(s, new) &&
                                !drops[new.z][new.y][new.x].seen &&
                                !drops[new.z][new.y][new.x].lava) {
                        fillSpace(s, drops, new);
                }
        }
}

int32 getSeenSurfaceArea(ivec3 s, struct cube drops[s.z][s.y][s.x]) {
        int32 surfaceArea = 0;
        MAKE_LOOP(z, s.z, y, s.y, x, s.x) {
                if (!drops[z][y][x].seen) continue;
                // printf("(%d,%d,%d)\n", x, y, z);
                for (int i=0; i<6; i++) {
                        ivec3 newPos = ivec3Add((ivec3){{x, y, z}}, Offsets[i]);
                        if (isValidPosition(s, newPos)) {
                                if (drops[newPos.z][newPos.y][newPos.x].lava)
                                        surfaceArea++;
                        }
                }
        }
        return surfaceArea;
}

void part1(llist *ll) {
        tllivec3 cubes = tll_init();
        ivec3 size = {0};
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                ivec3 cube;
                cube.x = strtol(strtok(str, ","), (char**)NULL, 10) + 1;
                if (cube.x > size.x) size.x = cube.x;
                cube.y = strtol(strtok(NULL, ","), (char**)NULL, 10) + 1;
                if (cube.y > size.y) size.y = cube.y;
                cube.z = strtol(strtok(NULL, ","), (char**)NULL, 10) + 1;
                if (cube.z > size.z) size.z = cube.z;
                tll_push_back(cubes, cube);

                current = current->next;
        }
        size = ivec3Add(size, (ivec3){{2, 2, 2}});
        // printf("Size: (%d,%d,%d)\n", size.x, size.y, size.z);
        bool droplets[size.z][size.y][size.x];
        memset(droplets, 0, sizeof(droplets));
        tll_foreach(cubes, it) {
                ivec3 pos = it->item;
                droplets[pos.z][pos.y][pos.x] = true;
        }

        int32 surfaceArea = getSurfaceArea(size, droplets);

        printf("Part 1: Surface Area = %d\n\n", surfaceArea);
}

void part2(llist *ll) {
        tllivec3 cubes = tll_init();
        ivec3 size = {0};
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                ivec3 cube;
                cube.x = strtol(strtok(str, ","), (char**)NULL, 10) + 1;
                if (cube.x > size.x) size.x = cube.x;
                cube.y = strtol(strtok(NULL, ","), (char**)NULL, 10) + 1;
                if (cube.y > size.y) size.y = cube.y;
                cube.z = strtol(strtok(NULL, ","), (char**)NULL, 10) + 1;
                if (cube.z > size.z) size.z = cube.z;
                tll_push_back(cubes, cube);

                current = current->next;
        }
        size = ivec3Add(size, (ivec3){{2, 2, 2}});
        // printf("Size: (%d,%d,%d)\n", size.x, size.y, size.z);
        struct cube droplets[size.z][size.y][size.x];
        memset(droplets, 0, sizeof(droplets));
        tll_foreach(cubes, it) {
                ivec3 pos = it->item;
                droplets[pos.z][pos.y][pos.x].lava = true;
        }

        fillSpace(size, droplets, (ivec3){{0, 0, 0}});
        // Count seen spaces
        int32 surfaceArea = getSeenSurfaceArea(size, droplets);

        printf("Part 2: Surface Area = %d\n\n", surfaceArea);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day18.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day18.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

