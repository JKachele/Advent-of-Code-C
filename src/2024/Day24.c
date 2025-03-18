/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Feb 25, 2025 18:13:37 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define MAX_WIRES 46656

typedef enum {  // gateType
        AND,
        OR,
        XOR
} gateType;

typedef struct {  // wire
        uint16 id;
        uint8 value;
} wire;

typedef struct {  // gate
        gateType type;
        uint16 in1;
        uint16 in2;
        uint16 out;
} gate;

typedef tll(wire) tllwire;
typedef tll(gate) tllgate;

uint8 charID(char c) {
        if ((int)c <= '9')
                return c - '0';
        else
                return (c - 'a') + 10;
}

// 3 char ID encoded to 16 bits. ID consist of 0-9 and a-z: 36 chars.
// 0-9 are 0-9 and a-z are 10-35
uint16 strToID(char *idStr) {
        if (strlen(idStr) < 3) return -1;
        uint16 id = 0;
        id += charID(idStr[0]) * 1296;
        id += charID(idStr[1]) * 36;
        id += charID(idStr[2]);
        if (id > 46656)
                printf("ERROR: ID TOO HIGH\n");
        return id;
}

char idChar(uint16 id) {
        if (id < 10)
                return (char)(id + '0');
        else
                return (char)(id - 10 + 'a');
}

void idToStr(uint16 id, char *idStr) {
        idStr[0] = idChar(id / 1296);
        idStr[1] = idChar((id / 36) % 36);
        idStr[2] = idChar(id % 36);
        idStr[3] = '\0';
}

void printWires(uint8 wiresarr[]) {
        for (int i = 0; i < MAX_WIRES; i++) {
                if (wiresarr[i] == 255) continue;
                char idStr[4];
                idToStr(i, idStr);
                printf("%s - %d\n", idStr, wiresarr[i]);
        }
}

void printGates(tllgate gates) {
        tll_foreach(gates, it) {
                gate g = it->item;
                char in1[4];
                char in2[4];
                char out[4];
                idToStr(g.in1, in1);
                idToStr(g.in2, in2);
                idToStr(g.out, out);

                char *type;
                if (g.type == AND) type = "AND\0";
                else if (g.type == OR) type = "OR\0";
                else if (g.type == XOR) type = "XOR\0";
                else type = "ERROR\0";

                printf("%s %s %s -> %s\n", in1, type, in2, out);
        }
}

gate parseGate(char *gateStr) {
        gate g = {0};
        char *in1Str = strtok(gateStr, " ");
        char *gateType = strtok(NULL, " ");
        char *in2Str = strtok(NULL, " ");
        strtok(NULL, " ");
        char *outStr = strtok(NULL, "");

        g.in1 = strToID(in1Str);
        g.in2 = strToID(in2Str);
        g.out = strToID(outStr);
        // printf("%d %s %d - %d\n", in1, gateType, in2, out);

        if (strcmp(gateType, "AND") == 0)
                g.type = AND;
        else if (strcmp(gateType, "OR") == 0)
                g.type = OR;
        else if (strcmp(gateType, "XOR") == 0)
                g.type = XOR;
        else
                printf("GATE NOT FOUND!");

        return g;
}

uint8 rungate(gateType type, uint8 a, uint8 b) {
        switch (type) {
        case AND:
                return a & b;
        case OR:
                return a | b;
        case XOR:
                return a ^ b;
        default:
                printf("ERROR: INVALID GATE\n");
                return -1;
        }
}

void runLogic(tllgate gates, uint8 wires[MAX_WIRES]) {
        while (tll_length(gates) > 0) {
                tll_foreach(gates, it) {
                        gate g = it->item;
                        if (wires[g.in1] == 255 || wires[g.in2] == 255)
                                continue;

                        wires[g.out] = rungate(g.type,
                                        wires[g.in1], wires[g.in2]);

                        tll_remove(gates, it);
                }
        }
}

uint64 getOutput(uint8 wires[MAX_WIRES]) {
        uint64 out = 0;
        const uint16 ZID = strToID("z00");
        int shift = 0;
        int offset = 0;
        while (wires[ZID + offset] <= 1) {
                uint64 wire = wires[ZID + offset];
                wire <<= shift;
                out += wire;
                shift++;
                offset++;
                // Adjust for base 36 IDs
                if ((offset % 36) > 9)
                        offset = offset + 26;
        }
        return out;
}

void part1(llist *ll) {
        uint8 wires[MAX_WIRES];
        tllgate gates = tll_init();
        for (uint16 i = 0; i < MAX_WIRES; i++) wires[i] = 255;

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;
                if (strlen(str) == 0) break;

                uint16 id = strToID(str);
                uint8 value = str[5] - '0';
                wires[id] = value;

                current = current->next;
        }
        // printWires(wires);

        current = current->next;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                gate g = parseGate(str);
                tll_push_back(gates, g);

                current = current->next;
        }
        // printGates(gates);

        runLogic(gates, wires);
        printWires(wires);
        uint64 out = getOutput(wires);

        printf("Part 1: Output: %lu\n\n", out);
}

void part2(llist *ll) {
        // llNode *current = ll->head;
        // while(current != NULL) {
        //         char str[BUFFER_SIZE];
        //         strncpy(str, (char*)current->data, BUFFER_SIZE);
        //         current = current->next;
        // }
        // TODO: Code implementation of Part 2
        printf("Part 2: Done by hand: fhc,ggt,hqk,mwh,qhj,z06,z11,z35\n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day24.txt");
        } else {
                ll = getInputFile("assets/inputs/2024/Day24.txt");
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
