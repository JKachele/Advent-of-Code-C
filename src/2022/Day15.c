/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 30, 2025 07:58:19 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

typedef struct sensor {
        lvec2 pos;
        lvec2 beacon;
        int64 dist;
        lvec2 src; // Sense Row Coverage
} sensor;

typedef tll(sensor) tllsense;
typedef tll(lvec2) lvectll;

static int64 SenseRow = 2000000;
static int64 MaxPos = 4000000;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printSensors(tllsense sensors) {
        if (!Debug) return;
        tll_foreach(sensors, it) {
                sensor s = it->item;
                printf("[%ld, %ld]: ", s.pos.x, s.pos.y);
                printf("[%ld, %ld] d=%ld ", s.beacon.x, s.beacon.y, s.dist);
                printf("Coverage: [%ld, %ld]\n", s.src.x, s.src.y);
        }
        printf("\n");
}

void printRanges(lvec2 ranges[], int num) {
        if (!Debug) return;
        for (int i = 0; i < num; i++) {
                debugP("[%d, %d]\n", ranges[i].x, ranges[i].y);
        }
        printf("\n");
}

void printIvectll(lvectll list) {
        if (!Debug) return;
        tll_foreach(list, it) {
                printf("[%ld, %ld]\n", it->item.x, it->item.y);
        }
        printf("\n");
}

int64 mdist(lvec2 a, lvec2 b) {
        int64 dist = labs(b.x - a.x);
        dist += labs(b.y - a.y);
        return dist;
}

lvec2 src(sensor s) {
        lvec2 coverage = {.x = -1, .y = -1};
        int64 dist2Line = labs(SenseRow - s.pos.y);
        if (dist2Line > s.dist)
                return coverage;
        int64 distDiff = s.dist - dist2Line;

        coverage.x = s.pos.x - distDiff;
        coverage.y = s.pos.x + distDiff;

        return coverage;
}

int rangeCmp(const void *a, const void *b) {
        lvec2 v1 = *(lvec2*)a;
        lvec2 v2 = *(lvec2*)b;
        int diff = v1.x - v2.x;
        if (diff != 0)
                return diff;
        return v1.y - v2.y;
}

lvectll compressRanges(lvec2 ranges[], int64 num) {
        lvectll compressed = tll_init();
        tll_push_back(compressed, ranges[0]);
        for (int i = 1; i < num; i++) {
                lvec2 curRange = tll_pop_back(compressed);

                if (ranges[i].x <= curRange.y) {
                        if (curRange.y < ranges[i].y)
                                curRange.y = ranges[i].y;
                        tll_push_back(compressed, curRange);
                } else {
                        tll_push_back(compressed, curRange);
                        tll_push_back(compressed, ranges[i]);
                }
        }
        return compressed;
}

bool validPos (lvec2 pos) {
        return pos.x >= 0 && pos.y >= 0 &&
                pos.x <= MaxPos && pos.y <= MaxPos;
}

bool inRange(tllsense sensors, lvec2 pos) {
        tll_foreach(sensors, it) {
                sensor s = it->item;
                if (mdist(s.pos, pos) <= s.dist)
                        return true;
        }
        return false;
}

// I traverse the perimeter of each sensor's range, checking if the point
// is in the range of any other sensor. If it isnt, that is the hole
lvec2 findHole(tllsense sensors) {
        lvec2 pos;
        tll_foreach(sensors, it) {
                sensor s = it->item;
                debugP("Sensor at (%d, %d)\n", s.pos.x, s.pos.y);
                // Range looks like a diamond. Traverse each side seperatly
                // Top Right
                pos.x = s.pos.x;
                // -1 so we are traversing outside of range
                pos.y = s.pos.y - s.dist - 1;
                while (pos.y <= s.pos.y) {
                        if (validPos(pos) && !inRange(sensors, pos))
                                return pos;
                        pos.x++;
                        pos.y++;
                }

                // Top Left
                pos.x = s.pos.x;
                pos.y = s.pos.y - s.dist - 1;
                while (pos.y <= s.pos.y) {
                        if (validPos(pos) && !inRange(sensors, pos))
                                return pos;
                        pos.x--;
                        pos.y++;
                }

                // Bottom Right
                pos.x = s.pos.x;
                pos.y = s.pos.y + s.dist + 1;
                while (pos.y >= s.pos.y) {
                        if (validPos(pos) && !inRange(sensors, pos))
                                return pos;
                        pos.x++;
                        pos.y--;
                }

                // Bottom Left
                pos.x = s.pos.x;
                pos.y = s.pos.y + s.dist + 1;
                while (pos.y >= s.pos.y) {
                        if (validPos(pos) && !inRange(sensors, pos))
                                return pos;
                        pos.x--;
                        pos.y--;
                }
        }
        return pos;
}

void part1(llist *ll) {
        tllsense sensors = tll_init();
        int numSensors = 0; // Number of sensors with coverage of the row

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                sensor sense;
                strtok(str, "=");
                sense.pos.x = strtol(strtok(NULL, ","), (char**)NULL, 10);
                strtok(NULL, "=");
                sense.pos.y = strtol(strtok(NULL, ":"), (char**)NULL, 10);
                strtok(NULL, "=");
                sense.beacon.x = strtol(strtok(NULL, ","), (char**)NULL, 10);
                strtok(NULL, "=");
                sense.beacon.y = strtol(strtok(NULL, ""), (char**)NULL, 10);
                
                sense.dist = mdist(sense.pos, sense.beacon);
                sense.src = src(sense);

                if (sense.src.x != -1) {
                        numSensors++;
                        tll_push_back(sensors, sense);
                }

                current = current->next;
        }
        printSensors(sensors);

        lvec2 ranges[numSensors];
        int i = 0;
        tll_foreach(sensors, it) {
                ranges[i] = it->item.src;
                i++;
        }
        qsort(ranges, numSensors, sizeof(lvec2), &rangeCmp);
        printRanges(ranges, numSensors);

        lvectll compressed = compressRanges(ranges, numSensors);
        printIvectll(compressed);

        int64 coverage = 0;
        tll_foreach(compressed, it) {
                lvec2 range = it->item;
                coverage += range.y - range.x;
        }

        printf("Part 1: Total Coverage on Y=%ld: %ld\n\n", SenseRow, coverage);
}

void part2(llist *ll) {
        tllsense sensors = tll_init();
        int numSensors = 0; // Number of sensors with coverage of the row

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                sensor sense;
                strtok(str, "=");
                sense.pos.x = strtol(strtok(NULL, ","), (char**)NULL, 10);
                strtok(NULL, "=");
                sense.pos.y = strtol(strtok(NULL, ":"), (char**)NULL, 10);
                strtok(NULL, "=");
                sense.beacon.x = strtol(strtok(NULL, ","), (char**)NULL, 10);
                strtok(NULL, "=");
                sense.beacon.y = strtol(strtok(NULL, ""), (char**)NULL, 10);
                
                sense.dist = mdist(sense.pos, sense.beacon);

                tll_push_back(sensors, sense);

                current = current->next;
        }
        printSensors(sensors);

        lvec2 holePos = findHole(sensors);
        int64 freq = (holePos.x * 4000000) + holePos.y;

        printf("Part 2: Hole at (%ld, %ld): ", holePos.x, holePos.y);
        printf("Tuning Frequency: %ld\n\n", freq);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day15.txt");
                Debug = true;
                SenseRow = 10;
                MaxPos = 20;
        } else {
                ll = getInputFile("assets/inputs/2022/Day15.txt");
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

