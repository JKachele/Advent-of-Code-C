/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Dec 11, 2025 14:13:36 UTC
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

typedef tll(u16) tllu16;

enum pathState {
        NONE,
        DAC,
        FFT,
        BOTH
};

typedef struct device {
        u16 id;
        tllu16 outputs;
        tllu16 inputs;
        u64 paths[4]; // 0: no dac or fft, 1: dac, 2: fft, 3: dac and fft
        bool seen;
} device;
typedef tll(device*) tlldev;

struct input {
        int numDevices;
        device *devices;
};

const u16 DAC_ID = 2030;
const u16 FFT_ID = 3529;
const u16 OUT_ID = 10003;

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printDevices(int numDevices, device *devices) {
        for (int i = 0; i < numDevices; i++) {
                device d = devices[i];
                printf("%d: Outputs(", d.id);
                tll_foreach(d.outputs, it) {
                        printf("%u,", it->item);
                }
                printf("\b) Inputs(");
                tll_foreach(d.inputs, it) {
                        printf("%u,", it->item);
                }
                printf("\b)\n");
        }
}

u16 strtoid(char *str) {
        u16 id = 0;
        for (int i = 0; i < 3; i++) {
                id *= 26;
                id += str[i] - 'a';
        }
        return id;
}

bool devInQueue(tlldev queue, u16 id) {
        tll_foreach(queue, it) {
                if (it->item->id == id)
                        return true;
        }
        return false;
}

u64 findPaths(tllu16 map[], u16 start) {
        if (start == OUT_ID)
                return 1;

        u64 paths = 0;
        tll_foreach(map[start], it) {
                paths += findPaths(map, it->item);
        }

        return paths;
}

u64 findPaths2(int numdevices, device *devices, int indexMap[], u16 start) {
        // Set path for start device and add to queue
        devices[indexMap[start]].paths[0] = 1;
        tlldev devQueue = tll_init();
        tll_push_back(devQueue, &devices[indexMap[start]]);

        u64 paths = 0;
        while (tll_length(devQueue) > 0) {
                device *d;
                // Find first device where all inputs ave been seen
                tll_foreach(devQueue, it) {
                        d = it->item;
                        bool valid = true;
                        tll_foreach(d->inputs, it) {
                                device in = devices[indexMap[it->item]];
                                if (!in.seen) {
                                        valid = false;
                                        break;
                                }
                        }
                        if (valid) {
                                tll_remove(devQueue, it);
                                break;
                        }
                }

                d->seen = true;

                // if device is dac or fft, move paths from the neither field to the dac/fft
                // and move the paths in the opposite dev to the both field (if dac: move fft->both)
                if (d->id == DAC_ID) {
                        d->paths[BOTH] += d->paths[FFT];
                        d->paths[FFT] = 0;
                        d->paths[DAC] += d->paths[NONE];
                        d->paths[NONE] = 0;
                }
                if (d->id == FFT_ID) {
                        d->paths[BOTH] += d->paths[DAC];
                        d->paths[DAC] = 0;
                        d->paths[FFT] += d->paths[NONE];
                        d->paths[NONE] = 0;
                }

                // For each output, add all paths from current dev and add to queue if needed
                tll_foreach(d->outputs, it) {
                        // If output is OUT, add paths to total and skip adding to queue
                        // Only count paths that passed through dac AND fft
                        if (it->item == OUT_ID) {
                                paths += d->paths[BOTH];
                                continue;
                        }

                        device *out = &devices[indexMap[it->item]];
                        if (out->seen)
                                printf("ALREADY SEEN: %u->%u\n", d->id, out->id);
                        for (int i=0; i<4; i++) 
                                out->paths[i] += d->paths[i];

                        if (!devInQueue(devQueue, out->id))
                                tll_push_back(devQueue, out);
                }
        }

        return paths;
}

void part1(struct input input) {
        const u16 Start = strtoid("you");

        // Create hashmap
        tllu16 map[UINT16_MAX] = {0};
        for (int i = 0; i < input.numDevices; i++) {
                device d = input.devices[i];
                tll_foreach(d.outputs, it) {
                        tll_push_back(map[d.id], it->item);
                }
        }

        u64 paths = findPaths(map, Start);

        printf("Part 1: %lu\n\n", paths);
}

void part2(struct input input) {
        const u16 Start = strtoid("svr");

        int numDevices = input.numDevices;
        device *devices = input.devices;

        // Create hash map to map device ids to index for fast lookups
        int indexMap[UINT16_MAX] = {0};
        for (int i = 0; i < numDevices; i++)
                indexMap[devices[i].id] = i;

        // Populate inputs for devices
        for (int i = 0; i < numDevices; i++) {
                u16 id = devices[i].id;
                tll_foreach(devices[i].outputs, it) {
                        if (it->item == OUT_ID)continue;
                        device *out = &devices[indexMap[it->item]];
                        tll_push_back(out->inputs, id);
                }
        }
        // printDevices(numDevices, devices);

        u64 paths = findPaths2(numDevices, devices, indexMap, Start);

        printf("Part 2: %lu\n", paths);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numDevices = ll->length;
        device *devices = malloc(input.numDevices * sizeof(device));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                char *idStr = strtok(str, ": ");
                devices[i].id = strtoid(idStr);
                devices[i].inputs = (tllu16)tll_init();
                devices[i].outputs = (tllu16)tll_init();
                char *outputStr = strtok(NULL, ": ");
                while (outputStr != NULL) {
                        u16 outId = strtoid(outputStr);
                        tll_push_back(devices[i].outputs, outId);
                        outputStr = strtok(NULL, " ");
                }
                for (int j=0; j<4; j++)
                        devices[i].paths[j] = 0;
                devices[i].seen = false;

                i++;
        }

        input.devices = devices;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        fflush(stdout);
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

