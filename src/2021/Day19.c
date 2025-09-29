/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 24, 2025 12:01:31 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <math.h>
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

typedef tll(ivec2) tllivec2;
typedef tll(ivec3) tllivec3;

typedef struct beaconVec {
        int b1;
        int b2;
        ivec3 vec;
        ivec3 adjVec;
} beaconVec;

typedef struct scanner {
        ivec3 pos;
        int32 numBeacons;
        ivec3 *beacons;
        ivec3 *adjBeacons;
        int32 numVecs;
        beaconVec *vecs;
        bool checked;
} scanner;

struct input {
        int numScanners;
        scanner *scanners;

};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printScanner(scanner s) {
        if (!Debug) return;
        for (int i=0; i<s.numBeacons; i++) {
                ivec3 pos = s.beacons[i];
                printf("(%d, %d, %d)\n", pos.x, pos.y, pos.z);
        }
        printf("\n");
}

void printScanners(int numS, scanner *scanners) {
        if (!Debug) return;
        for (int i=0; i<numS; i++) {
                printf("Scanner %d:\n", i);
                printScanner(scanners[i]);
        }
}

double ivec3Mag(ivec3 v) {
        return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

int32 nChoose2(int32 n) {
        if (n <= 0)
                return 0;
        return (n * (n-1)) / 2;
}

bool ivec3Unique(ivec3 v) {
        int x = abs(v.x);
        int y = abs(v.y);
        int z = abs(v.z);
        return x != y && x != z && y != z;
}

void tlltoscanner(tllivec3 tllist, scanner *s) {
        s->numBeacons = tll_length(tllist);
        s->beacons = calloc(s->numBeacons, sizeof(ivec3));
        int i = 0;
        tll_foreach(tllist, it) {
                ivec3 vec = it->item;
                for (int j=0; j<3; j++)
                        s->beacons[i].raw[j] = vec.raw[j];
                i++;
        }
}

bool allAdjusted(int numS, scanner *scanners, int count) {
        bool adj = true;
        debugP("%d: Adjusted: ", count);
        for (int i=0; i<numS; i++) {
                if (scanners[i].adjBeacons == NULL)
                        adj =  false;
                else
                        debugP("%d ", i);
        }
        debugP("\n");
        return adj;
}

void setBeaconVecs(scanner *s) {
        s->numVecs = nChoose2(s->numBeacons);
        s->vecs = calloc(s->numVecs, sizeof(beaconVec));

        int k = 0;
        for (int i=0; i<s->numBeacons; i++) {
                for (int j=i+1; j<s->numBeacons; j++) {
                        s->vecs[k].b1 = i;
                        s->vecs[k].b2 = j;
                        s->vecs[k].vec = ivec3Sub(s->beacons[j], s->beacons[i]);
                        s->vecs[k].adjVec = s->vecs[k].vec; 
                        k++;
                }
        }
}

bool ivec3Match(ivec3 a, ivec3 b) {
        // Check if vectors are the same length
        double distV1 = ivec3Mag(a);
        double distV2 = ivec3Mag(b);
        if (fabs(distV1 - distV2) > 1e-9)
                return false;

        // Check if each value is present in the other vector
        for (int i=0; i<3; i++) {
                bool found = false;
                for (int j=0; j<3; j++) {
                        if (a.raw[i] == b.raw[j])
                                found = true;
                        else if ((a.raw[i] * -1) == b.raw[j])
                                found = true;
                }
                if (!found)
                        return false;
        }
        return true;
}

void testVectorMatch(beaconVec v1, beaconVec v2, int32 *com, ivec2 *opts) {
        if (ivec3Match(v1.vec, v2.vec) == false)
                return;

        // On first match, dont know which beacon goes with which
        // Store the 2 possible and check for commonality with next match
        if (opts[v1.b1].x == -1) {
                opts[v1.b1] = (ivec2){{v2.b1, v2.b2}};
        } else {
                ivec2 op = opts[v1.b1];
                if (v2.b1 == op.x || v2.b1 == op.y)
                        com[v1.b1] = v2.b1;
                else if (v2.b2 == op.x || v2.b2 == op.y)
                        com[v1.b1] = v2.b2;
                else
                        printf("Matching Error!\n");
        }

        if (opts[v1.b2].x == -1) {
                opts[v1.b2] = (ivec2){{v2.b1, v2.b2}};
        } else {
                ivec2 op = opts[v1.b2];
                if (v2.b1 == op.x || v2.b1 == op.y)
                        com[v1.b2] = v2.b1;
                else if (v2.b2 == op.x || v2.b2 == op.y)
                        com[v1.b2] = v2.b2;
                else
                        printf("Matching Error!\n");
        }
}

tllivec2 getCommonBeacons(scanner s1, scanner s2) {
        int32 common[s1.numBeacons];
        for (int i=0; i<s1.numBeacons; i++) common[i] = -1;

        ivec2 options[s1.numBeacons];
        for (int i=0; i<s1.numBeacons; i++) options[i] = (ivec2){{-1, -1}};

        // Test every combination of vectors
        for (int i=0; i<s1.numVecs; i++) {
                beaconVec v1 = s1.vecs[i];
                for (int j=0; j<s2.numVecs; j++) {
                        beaconVec v2 = s2.vecs[j];
                        testVectorMatch(v1, v2, common, options);
                }
        }

        tllivec2 commonBeacons = tll_init();
        for (int i=0; i<s1.numBeacons; i++) {
                if (common[i] == -1)
                        continue;
                ivec2 match = {{i, common[i]}};
                tll_push_back(commonBeacons, match);
                // ivec3 b1 = s1.beacons[i];
                // ivec3 b2 = s2.beacons[common[i]];
                // printf("(%d, %d, %d) - ", b1.x, b1.y, b1.z);
                // printf("(%d, %d, %d)\n", b2.x, b2.y, b2.z);
        }

        return commonBeacons;
}

ivec3 getBeaconVec(scanner s, int b1, int b2, bool adj) {
        bool reverseVec = false;
        if (b1 > b2) {
                reverseVec = true;
                int tmp = b1;
                b1 = b2;
                b2 = tmp;
        }

        int vecIndex = (s.numVecs - nChoose2(s.numBeacons - b1));
        vecIndex += ((b2 - b1) - 1);
        ivec3 vec = s.vecs[vecIndex].vec;
        if (adj)
                vec = s.vecs[vecIndex].adjVec;
        if (reverseVec) {
                for (int i=0; i<3; i++)
                        vec.raw[i] = vec.raw[i] * -1;
        }
        return vec;
}

// Return vector of transformations to vec to get to vecRef (1 indexed)
// (-2, 3, -1): vec.x = -vecRef.y, vec.y = vecRef.z etc...
ivec3 getRotation(ivec3 vecRef, ivec3 vec) {
        ivec3 rot = {0};

        // Compare x,y,z with ref to get rotations
        for (int i=0; i<3; i++) {
                for (int j=0; j<3; j++) {
                        if (vec.raw[i] == vecRef.raw[j])
                                rot.raw[i] = j + 1;
                        else if (vec.raw[i] == (vecRef.raw[j] * -1))
                                rot.raw[i] = (j + 1) * -1;
                }
        }
        
        return rot;
}

ivec3 ivec3Rotate(ivec3 vec, ivec3 rot) {
        ivec3 rotated = {0};
        for (int i=0; i<3; i++) {
                int r = rot.raw[i];
                if (r > 0) {
                        r -= 1;  // Subtract 1 to switch from 1 index
                        rotated.raw[r] = vec.raw[i];
                } else {
                        r = (r * -1) - 1;
                        rotated.raw[r] = vec.raw[i] * -1;
                }
        }
        return rotated;
}

void adjustScanner(scanner ref, scanner *s, tllivec2 commonBeacons) {
        // Get a vector from each scaner that has the same beacons
        ivec2 com1 = {{-1, -1}};
        ivec2 com2 = {{-1, -1}};
        ivec3 refVec = getBeaconVec(ref, com1.x, com2.x, true);
        ivec3 vec = getBeaconVec(*s, com1.y, com2.y, false);
        tll_foreach(commonBeacons, it) {
                if (com1.x == -1) {
                        com1 = it->item;
                        continue;
                }
                com2 = it->item;

                refVec = getBeaconVec(ref, com1.x, com2.x, true);
                vec = getBeaconVec(*s, com1.y, com2.y, false);

                // Make sure all 3 values are different to avoid rotation errors
                if (ivec3Unique(refVec) && ivec3Unique(vec))
                        break;

                // else, reset com and try again
                com1 = (ivec2){{-1, -1}};
                com2 = (ivec2){{-1, -1}};
        }
        // Make sure vectors are set
        if (com2.x == -1) {
                printf("Unique Vectors not found!\n");
                return;
        }

        // Get rotation vector
        ivec3 rot = getRotation(refVec, vec);
        
        // Rotate all beacons and vectors
        s->adjBeacons = calloc(s->numBeacons, sizeof(ivec3));
        for (int i=0; i<s->numBeacons; i++)
                s->adjBeacons[i] = ivec3Rotate(s->beacons[i], rot);
        for (int i=0; i<s->numVecs; i++)
                s->vecs[i].adjVec = ivec3Rotate(s->vecs[i].vec, rot);

        // Get translate vector
        ivec3 trans = ivec3Sub(ref.adjBeacons[com1.x], s->adjBeacons[com1.y]);
        s->pos = trans;

        // Translate all beacons
        for (int i=0; i<s->numBeacons; i++)
                s->adjBeacons[i] = ivec3Add(s->adjBeacons[i], trans);
}

void addBeacon(tllivec3 *beacons, ivec3 b) {
        bool found = false;
        // Add in ascending order in x then y then z
        tll_foreach(*beacons, it) {
                ivec3 cur = it->item;
                if (cur.x == b.x && cur.y == b.y && cur.z == b.z) {
                        return;
                }
                if (cur.x == b.x && cur.y == b.y && cur.z > b.z) {
                        tll_insert_before(*beacons, it, b);
                        found = true;
                        break;
                } else if (cur.x == b.x && cur.y > b.y) {
                        tll_insert_before(*beacons, it, b);
                        found = true;
                        break;
                } else if (cur.x > b.x) {
                        tll_insert_before(*beacons, it, b);
                        found = true;
                        break;
                }
        }
        if (!found) {
                tll_push_back(*beacons, b);
        }
}

int32 mDist(ivec3 a, ivec3 b) {
        ivec3 diff = ivec3Sub(a, b);
        return abs(diff.x) + abs(diff.y) + abs(diff.z);
}

void part1(struct input input) {
        int numS = input.numScanners;
        scanner *scanners = input.scanners;

        // Scanner 0 is reference, Copy beacons to adjusted
        scanner *s0 = &scanners[0];
        s0->pos = (ivec3){0}; // No Translation
        s0->adjBeacons = calloc(s0->numBeacons, sizeof(ivec3));
        for (int i=0; i<s0->numBeacons; i++)
                s0->adjBeacons[i] = s0->beacons[i];

        // Get the beacon Vectors
        for (int i=0; i<numS; i++) {
                setBeaconVecs(&scanners[i]);
        }

        // Compare scanners until all are adjusted
        // tllivec2 common = getCommonBeacons(scanners[0], scanners[1]);
        int count = 0;
        int i = 0;
        while (allAdjusted(numS, scanners, count) == false){
                scanner *s1 = &scanners[i];

                // Only compare to adjusted scanners that havent been checked
                if (s1->adjBeacons == NULL || s1->checked)
                        goto INC;
                s1->checked = true;

                for (int j=0; j<numS; j++) {
                        if (i == j) continue;
                        scanner *s2 = &scanners[j];

                        // No need to check already adjusted scanners
                        if (s2->adjBeacons != NULL)
                                continue;

                        tllivec2 common = getCommonBeacons(*s1, *s2);

                        if (tll_length(common) >= 12) {
                                adjustScanner(*s1, s2, common);
                        }
                }
INC:
                i++;
                if (i >= numS)
                        i = 0;
                count++;
        }

        // Count unique adjusted beacons
        tllivec3 uniqueBeacons = tll_init();
        for (int i=0; i<numS; i++) {
                ivec3 *adjBeacons = scanners[i].adjBeacons;
                ivec3 *beacons = scanners[i].beacons;
                ivec3 pos = scanners[i].pos;
                debugP("Scanner %d: (%d, %d, %d)\n", i, pos.x, pos.y, pos.z);
                for (int j=0; j<scanners[i].numBeacons; j++) {
                        ivec3 b = beacons[j];
                        ivec3 adj = adjBeacons[j];
                        debugP("(%d, %d, %d) -> ", b.x, b.y, b.z);
                        debugP("(%d, %d, %d)\n", adj.x, adj.y, adj.z);
                        addBeacon(&uniqueBeacons, adjBeacons[j]);
                }
                debugP("\n");
        }
        int32 numUniqueBeacons = tll_length(uniqueBeacons);

        printf("Part 1: %d\n\n", numUniqueBeacons);
}

void part2(struct input input) {
        int numS = input.numScanners;
        scanner *scanners = input.scanners;

        // Get largest manhattan distance between scanners
        int32 maxDist = 0;
        for (int i=0; i<numS; i++) {
                for (int j=i+1; j<numS; j++) {
                        int32 dist = mDist(scanners[i].pos, scanners[j].pos);
                        if (dist > maxDist)
                                maxDist = dist;
                }
        }

        printf("Part 2: %d\n\n", maxDist);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        // Number of scanners is num empty lines + 1
        input.numScanners = 1;
        llNode *current = ll->head;
        while(current != NULL) {
                if (strlen((char*)current->data) == 0)
                        input.numScanners++;
                current = current->next;
        }

        input.scanners = calloc(input.numScanners, sizeof(scanner));

        tllivec3 beacons = tll_init();
        int i = 0;
        current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;

                // If line is a coordinate
                if (strlen(str) > 0 && strchr(str, ',') != NULL) {
                        ivec3 pos = {0};
                        pos.x = strtol(strtok(str, ","), NULL, 10);
                        pos.y = strtol(strtok(NULL, ","), NULL, 10);
                        pos.z = strtol(strtok(NULL, ""), NULL, 10);
                        tll_push_back(beacons, pos);
                }

                // Is line is empty or last line, scanner is done
                if (strlen(str) == 0 || current->next == NULL) {
                        tlltoscanner(beacons, &input.scanners[i]);
                        i++;

                        beacons = (tllivec3)tll_init();
                }

                current = current->next;
        }
        // printScanners(input.numScanners, input.scanners);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day19.txt";
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

