/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 19, 2025 17:06:08 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

uint64 mixPrune(uint64 a, uint64 b) {
        return (a ^ b) % 16777216;
}

uint64 stepNumber(uint64 secret) {
        uint64 temp = secret * 64;
        secret = mixPrune(temp, secret);

        temp = secret / 32;
        secret = mixPrune(temp, secret);

        temp = secret * 2048;
        secret = mixPrune(temp, secret);

        return secret;
}

int addDiff(int seq, int diff) {
        // add 9 to diff to remove negatives
        int adjDiff = diff + 9;

        // Left shift the seg by 5 and mask to get only 20 bits
        seq <<= 5;
        seq = seq & (ipow(2, 20) - 1);

        // Add the adjusted diff to the sequence
        seq += adjDiff;

        return seq;
}

void clearSeqSeen(bool arr[], int num) {
        for(int i = 0; i < num; i++) {
                arr[i] = false;
        }
}

void part1(llist *ll) {
        const int NUM_STEPS = 2000;
        int numEntries = ll->length;
        uint64 entries[numEntries];

        llNode *current = ll->head;
        for(int i = 0; i < numEntries; i++) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                entries[i] = strtol(str, (char**)NULL, 10);
                current = current->next;
        }

        MAKE_LOOP(i, NUM_STEPS, j, numEntries) {
                entries[j] = stepNumber(entries[j]);
        }

        uint64 total = 0;
        for(int i = 0; i < numEntries; i++) {
                total += entries[i];
                // printf("%lu\n", entries[i]);
        }

        printf("Part 1: Sum of 2000th secret: %lu\n\n", total);
}

void part2(llist *ll) {
        const int NUM_STEPS = 2000;
        int numEntries = ll->length;
        uint64 entries[numEntries];

        llNode *current = ll->head;
        for(int i = 0; i < numEntries; i++) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                entries[i] = strtol(str, (char**)NULL, 10);
                current = current->next;
        }

        // Sequences encoded into a 20 bit number, 5 bits per diff
        int16 totalBananas[ipow(2, 20)];
        for (int i = 0; i < numEntries; i++) {
                bool seqSeen[ipow(2, 20)];
                clearSeqSeen(seqSeen, ipow(2, 20));
                int seq = 0;
                for (int j = 0; j < NUM_STEPS; j++) {
                        uint64 new = stepNumber(entries[i]);
                        int diff = new - entries[i];
                        seq = addDiff(seq, diff);
                }
        }

        uint64 total = 0;
        for(int i = 0; i < numEntries; i++) {
                total += entries[i];
                // printf("%lu\n", entries[i]);
        }

        printf("Part 1: Sum of 2000th secret: %lu\n\n", total);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day22.txt");
        } else {
                ll = getInputFile("assets/2024/Day22.txt");
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
