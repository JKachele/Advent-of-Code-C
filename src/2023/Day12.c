/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Aug 26, 2025 11:39:16 UTC
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

#define INPUT_BUFFER_SIZE 4096

typedef enum {
        UNKNOWN,
        DAMAGED,
        OPERATIONAL
} spring;

typedef struct {
        int64 numS;
        spring *springs;
        int64 numC;
        int32 *counts;
} record;

typedef tll(record) tllrecord;

typedef struct {
        record rec;
        int64 configs;
} seenEntry;

typedef tll(seenEntry) tllseen;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printRecord(record rec) {
        for (int i=0; i<rec.numS; i++) {
                switch (rec.springs[i]) {
                        case UNKNOWN:
                                printf("?");
                                break;
                        case DAMAGED:
                                printf("#");
                                break;
                        case OPERATIONAL:
                                printf(".");
                                break;
                }
        }
        printf(" [ ");
        for (int i=0; i<rec.numC; i++) {
                printf("%d ", rec.counts[i]);
        }
        printf("]\n");
}

void printRecords(tllrecord records) {
        tll_foreach(records, it) {
                printRecord(it->item);
        }
        printf("\n");
}

record copyRecord(record rec) {
        record new = {0};
        new.numS = rec.numS;
        new.numC = rec.numC;

        spring *springs = calloc(new.numS, sizeof(spring));
        memcpy(springs, rec.springs, new.numS * sizeof(spring));
        new.springs = springs;

        int32 *counts = calloc(new.numC, sizeof(int32));
        memcpy(counts, rec.counts, new.numC * sizeof(int32));
        new.counts = counts;

        return new;
}

void freeEntry(seenEntry entry) {
        free(entry.rec.springs);
        free(entry.rec.counts);
}

int64 seenBefore(tllseen seen, record rec) {
        tll_foreach(seen, it) {
                seenEntry entry = it->item;
                record seenRec = entry.rec;

                if (seenRec.numS != rec.numS || seenRec.numC != rec.numC)
                        continue;

                // Check springs
                bool diff = false;
                for (int i=0; i<rec.numS; i++) {
                        if (seenRec.springs[i] != rec.springs[i]) {
                                diff = true;
                                break;
                        }
                }
                if (diff) continue;

                // Check Counts
                diff = false;
                for (int i=0; i<rec.numC; i++) {
                        if (seenRec.counts[i] != rec.counts[i]) {
                                diff = true;
                                break;
                        }
                }
                if (diff) continue;

                // If all is equal, return count
                return entry.configs;
        }
        return -1;
}

bool validConfig(record rec) {
	// Enough Springs left for record
        if (rec.numS < rec.counts[0])
                return false;
	// None of the first record of springs can be functional
        for (int i=0; i<rec.counts[0]; i++) {
                if (rec.springs[i] == OPERATIONAL)
                        return false;
        }
	// Either first record is equal to cfg length (All springs are broken)
	// or the spring after the first group is functional
        return (rec.numS == rec.counts[0]) ||
                (rec.springs[rec.counts[0]] != DAMAGED);
}

int64 countValidConfigs(record rec, tllseen *seen) {
        // If no springs left, add 1 if no counts are left as well
        if (rec.numS <= 0) {
                bool valid = rec.numC == 0;
                return valid ? 1 : 0;
        }
        // If no counts left, add 1 if springs conatins no damaged
        if (rec.numC == 0) {
                bool found = false;
                for (int i=0; i<rec.numS; i++) {
                        if (rec.springs[i] == DAMAGED) {
                                return 0;
                        }
                }
                return 1;
        }

        // Check if seen this configuration before
        int64 seenCount = seenBefore(*seen, rec);
        if (seenCount != -1)
                return seenCount;

	int64 numCombinations = 0;

        // If 1st spring is OPERATIONAL or UNKNOWN
        if (rec.springs[0] == OPERATIONAL || rec.springs[0] == UNKNOWN) {
                record new = {0};
                new.numS = rec.numS - 1;
                new.springs = rec.springs + 1;
                new.numC = rec.numC;
                new.counts = rec.counts;
                int64 configs = countValidConfigs(new, seen);
                numCombinations += configs;
        }

        // If 1st spring is DAMAGED or UNKNOWN Check if the config is
        if ((rec.springs[0] == DAMAGED || rec.springs[0] == UNKNOWN) &&
                        validConfig(rec)) {
                record new = {0};
                new.numS = rec.numS - rec.counts[0] - 1;
                new.springs = rec.springs + rec.counts[0] + 1;
                new.numC = rec.numC - 1;
                new.counts = rec.counts + 1;
                int64 configs = countValidConfigs(new, seen);
                numCombinations += configs;

        }
        
        seenEntry entry = {0};
        entry.rec = copyRecord(rec);
        entry.configs = numCombinations;
        tll_push_back(*seen, entry);

	return numCombinations;
}

record unfoldRecord(record rec) {
        record unfold = {0};
        unfold.numS = (rec.numS * 5) + 4;
        unfold.numC = rec.numC * 5;

        spring *springs = calloc(unfold.numS, sizeof(spring));
        memcpy(springs, rec.springs, rec.numS * sizeof(spring));
        for (int i=1; i<5; i++) {
                springs[(rec.numS * i) + (i - 1)] = UNKNOWN;
                memcpy(springs + (rec.numS * i) + i, rec.springs,
                                rec.numS * sizeof(spring));
        }
        unfold.springs = springs;

        int32 *counts = calloc(unfold.numC, sizeof(int32));
        for (int i=0; i<5; i++) {
                memcpy(counts + (rec.numC * i), rec.counts,
                                rec.numC * sizeof(int32));
        }
        unfold.counts = counts;

        return unfold;
}

void part1(llist *ll) {
        tllrecord records = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

		char *cfg = strtok(str, " ");
		char *cfgCounts = strtok(NULL, " ");

                int32 numSprings = strlen(cfg);
                spring *springs = calloc(numSprings, sizeof(spring));
                for (int i = 0; i < numSprings; i++) {
                        switch (cfg[i]) {
                        case '?':
                                springs[i] = UNKNOWN;
                                break;
                        case '#':
                                springs[i] = DAMAGED;
                                break;
                        case '.':
                                springs[i] = OPERATIONAL;
                                break;
                        default:
                                printf("Unknown spring %c\n", cfg[i]);
                        }
                }

		int32 numCounts = 1;
		for (int i = 0; i < (int)strlen(cfgCounts); i++) {
			if (cfgCounts[i] == ',')
				numCounts++;
		}
		// printf("[%s] [%s] %d\n", cfg, cfgrecords, numRecords);

		int32 *counts = calloc(numCounts, sizeof(int32));
		char *countStr = strtok(cfgCounts, ",");
		for (int i = 0; i < numCounts; i++) {
			counts[i] = strtol(countStr, NULL, 10);
			countStr = strtok(NULL, ",");
		}

                record rec = {numSprings, springs, numCounts, counts};
                tll_push_back(records, rec);

                current = current->next;
        }
        // printRecords(records);

        int32 countSum = 0;
        tll_foreach(records, it) {
                tllseen seen = tll_init();
                int32 count = countValidConfigs(it->item, &seen);
                tll_free_and_free(seen, freeEntry);
                countSum += count;
                // printf("%d: ", count);
                // printRecord(it->item);
        }

        printf("Part 1: %d\n\n", countSum);
}

void part2(llist *ll) {
        tllrecord records = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

		char *cfg = strtok(str, " ");
		char *cfgCounts = strtok(NULL, " ");

                int32 numSprings = strlen(cfg);
                spring *springs = calloc(numSprings, sizeof(spring));
                for (int i = 0; i < numSprings; i++) {
                        switch (cfg[i]) {
                        case '?':
                                springs[i] = UNKNOWN;
                                break;
                        case '#':
                                springs[i] = DAMAGED;
                                break;
                        case '.':
                                springs[i] = OPERATIONAL;
                                break;
                        default:
                                printf("Unknown spring %c\n", cfg[i]);
                        }
                }

		int32 numCounts = 1;
		for (int i = 0; i < (int)strlen(cfgCounts); i++) {
			if (cfgCounts[i] == ',')
				numCounts++;
		}
		// printf("[%s] [%s] %d\n", cfg, cfgrecords, numRecords);

		int32 *counts = calloc(numCounts, sizeof(int32));
		char *countStr = strtok(cfgCounts, ",");
		for (int i = 0; i < numCounts; i++) {
			counts[i] = strtol(countStr, NULL, 10);
			countStr = strtok(NULL, ",");
		}

                record rec = {numSprings, springs, numCounts, counts};
                tll_push_back(records, rec);

                current = current->next;
        }
        // printRecords(records);
        
        tllrecord unfolded = tll_init();
        tll_foreach(records, it) {
                record unfold = unfoldRecord(it->item);
                tll_push_back(unfolded, unfold);
        }
        // printRecords(unfolded);

        int64 countSum = 0;
        tll_foreach(unfolded, it) {
                tllseen seen = tll_init();
                int64 count = countValidConfigs(it->item, &seen);
                tll_free_and_free(seen, freeEntry);
                countSum += count;
        }

        printf("Part 2: %ld\n\n", countSum);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
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

