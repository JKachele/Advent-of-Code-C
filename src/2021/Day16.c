/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 19, 2025 18:26:40 UTC
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

typedef tll(u64) tllu64;

struct input {
        int32 len;
        bool *packet;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPacket(bool *packet, int32 len) {
        if (!Debug) return;
        for (int i = 0; i < len; i++) {
                if (packet[i])
                        printf("1");
                else
                        printf("0");
        }
        printf("\n");
}

u64 readBits(bool *bits, int numBits) {
        u64 result = 0;
        for (int i = 0; i < numBits; i++) {
                result <<= 1;
                if (bits[i])
                        result |= 1;
        }
        return result;
}

int literalPacket(bool *packet, u64 *value) {
        u64 num = 0;
        int i = 0;
        for (;;) {
                num <<= 4;
                num += readBits(packet+i+1, 4);
                if (!packet[i]) {
                        i += 5;
                        break;
                }
                i += 5;
        }
        debugP("    Literal Packet: %lu\n", num);
        if (value != NULL)
                *value = num;
        return i;
}

u64 getPacketValue(u8 type, tllu64 values) {
        u64 value = 0;
        switch (type) {
        case 0: // Sum
                tll_foreach(values, it) {
                        value += it->item;
                }
                return value;
        case 1: // Product
                value = 1;
                tll_foreach(values, it) {
                        value *= it->item;
                }
                return value;
        case 2: // Minimum
                value = UINT64_MAX;
                tll_foreach(values, it) {
                        if (it->item < value)
                                value = it->item;
                }
                return value;
        case 3: // Maximum
                value = 0;
                tll_foreach(values, it) {
                        if (it->item > value)
                                value = it->item;
                }
                return value;
        case 5: // Greater than
                if (tll_length(values) != 2) {
                        printf("GT Packet has invalid number of items\n");
                        return 0;
                }
                if (tll_front(values) > tll_back(values))
                        return 1;
                else
                        return 0;
        case 6: // Less than
                if (tll_length(values) != 2) {
                        printf("LT Packet has invalid number of items\n");
                        return 0;
                }
                if (tll_front(values) < tll_back(values))
                        return 1;
                else
                        return 0;
        case 7: // Equal
                if (tll_length(values) != 2) {
                        printf("EQ Packet has invalid number of items\n");
                        return 0;
                }
                if (tll_front(values) == tll_back(values))
                        return 1;
                else
                        return 0;
        }

        printf("Unknown Packet Type: %d\n", type);
        return 0;
}

int getVersions(int32 len, bool *packet, int32 *versionSum) {
        int32 version = readBits(packet, 3);
        *versionSum += version;
        u8 type = readBits(packet+3, 3);
        debugP("Version: %d, Type: %d\n", version, type);

        if (type == 4) {
                return literalPacket(packet+6, NULL) + 6;
        }
        // Opperator Packet
        // Length Type ID
        int32 offset = 7;
        if (packet[6]) {
                int32 numSubs = readBits(packet+7, 11);
                offset += 11;
                for (int i=0; i<numSubs; i++) {
                        offset += getVersions(len - offset,
                                        packet+offset, versionSum);
                }
        } else {
                int32 subLength = readBits(packet+7, 15);
                offset += 15;
                while (offset-22 < subLength) {
                        offset += getVersions(len - offset,
                                        packet+offset, versionSum);
                }
        }

        return offset;
}

int readPacket(int32 len, bool *packet, u64 *value) {
        u8 version = readBits(packet, 3);
        u8 type = readBits(packet+3, 3);
        debugP("Version: %d, Type: %d\n", version, type);

        // Literal Packet
        if (type == 4) {
                return literalPacket(packet+6, value) + 6;
        }

        // Opperator Packet
        int32 offset = 7;
        tllu64 values = tll_init();

        // Length Type ID
        if (packet[6]) {
                int32 numSubs = readBits(packet+7, 11);
                offset += 11;
                for (int i=0; i<numSubs; i++) {
                        u64 value = 0;
                        offset += readPacket(len - offset,
                                        packet+offset, &value);
                        tll_push_back(values, value);
                }
        } else {
                int32 subLength = readBits(packet+7, 15);
                offset += 15;
                while (offset-22 < subLength) {
                        u64 value = 0;
                        offset += readPacket(len - offset,
                                        packet+offset, &value);
                        tll_push_back(values, value);
                }
        }

        *value = getPacketValue(type, values);

        return offset;
}

void part1(struct input input) {
        int32 len = input.len;
        bool *packet = input.packet;
        // printPacket(packet, len);

        int32 versionSum = 0;
        int32 offset = getVersions(len, packet, &versionSum);

        printf("Part 1: %d\n\n", versionSum);
}

void part2(struct input input) {
        int32 len = input.len;
        bool *packet = input.packet;
        printPacket(packet, len);

        u64 value = 0;
        readPacket(len, packet, &value);

        printf("Part 2: %lu\n", value);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        int32 len = (strlen(str) * 4);
        bool *packet = calloc(len, sizeof(bool));

        for (int i = 0; i < len; i++) {
                int hexIndex = i / 4;
                int hexOffset = 3 - (i % 4);
                int hexNum = str[hexIndex] - '0';
                if (hexNum > 9)
                        hexNum = (str[hexIndex] - 'A') + 10;
                u8 mask = 1 << hexOffset;
                packet[i] = (hexNum & mask) != 0;
        }

        input.len = len;
        input.packet = packet;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day16.txt";
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

