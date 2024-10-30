/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Jul 10, 2024 13:25:23 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void print(char *str)
{
	char c = *str;
	while (c != '\0') {
		printf("%c", c);
		c = *(str++);
	}
	printf("|\n");
}

bool countsValid(llist *counts, int records[], int numRecords)
{
	if (counts->length != numRecords)
		return false;
	llNode *countNode = counts->head;
	for (int i = 0; i < numRecords; i++) {
		int count = *(int *)countNode->data;
		if (count != records[i])
			return false;
		countNode = countNode->next;
	}
	return true;
}

bool isValid(char *cfg, int records[], int numRecords)
{
	llist *counts = llist_create();
	bool inGroup = false;
	int groupSize = 0;
	for (int i = 0; i < strlen(cfg); i++) {
		if (cfg[i] == '#') {
			inGroup = true;
			groupSize++;
		} else if (inGroup) {
			int *count = malloc(sizeof(int));
			*count = groupSize;
			llist_add(counts, count);
			groupSize = 0;
			inGroup = false;
		}
	}
	if (inGroup) {
		int *count = malloc(sizeof(int));
		*count = groupSize;
		llist_add(counts, count);
	}
	bool valid = countsValid(counts, records, numRecords);
	llist_free(counts);
	return valid;
}

// Part 1: Brute Force
int countBF(char *cfg, int records[], int numRecords)
{
	int cfgLen = strlen(cfg);
	int unknowns[strlen(cfg)];
	int numUnknown = 0;
	for (int i = 0; i < cfgLen; i++) {
		if (cfg[i] == '?') {
			unknowns[numUnknown] = i;
			numUnknown++;
		}
	}

	// look through all configurations of springs
	int numValid = 0;
	int numConf = 2 << (numUnknown - 1);
	for (int i = 0; i < numConf; i++) {
		char *cfgTest = malloc(strlen(cfg) + 1);
		strncpy(cfgTest, cfg, strlen(cfg) + 1);
		int x = 0, y = i;
		while (y > 0) {
			if ((y & 1) == 1)
				cfgTest[unknowns[x]] = '#';
			x++;
			y = y >> 1;
		}
		if (isValid(cfgTest, records, numRecords))
			numValid++;
		free(cfgTest);
	}
	// printf("%s - %d\n", cfg, numValid);

	return numValid;
}

// Checks if the given cfg and record is possible
bool isValidCondition(char *cfg, int record)
{
	// Enough Springs left for record
	if (strlen(cfg) < record)
		return false;
	// None of the first record of springs can be functional
	for (int i = 0; i < record; i++) {
		if (cfg[i] == '.')
			return false;
	}
	// Either first record is equal to cfg length (All springs are broken)
	// or the spring after the first group is functional
	return (strlen(cfg) == record) || (cfg[record] != '#');
}

// Dynamic programming and memoization
int count(char *cfg, int records[], int numRecords)
{
	// if cfg is empty, return 1 if records is also empty.
	if (cfg[0] == '\0')
		return (numRecords == 0) ? 1 : 0;
	// if records is empty, return 1 if cfg has no # in it.
	if (numRecords == 0)
		return (strchr(cfg, '#') == NULL) ? 1 : 0;

	int numCombinations = 0;

	// if '.' or '?'
	if (cfg[0] == '.' || cfg[0] == '?') {
		numCombinations += count(cfg + 1, records, numRecords);
	}
	// if '#' or '?'
	if (cfg[0] == '#' || cfg[0] == '?') {
		if (isValidCondition(cfg, records[0])) {
			numCombinations += count(cfg + records[0] + 1,
						 records + 1, numRecords - 1);
		}
	}

	return numCombinations;
}

void part1(llist *ll)
{
	llNode *current = ll->head;
	int arrangementSum = 0;
	while (current != NULL) {
		char *strIn = (char *)current->data;
		char *str = malloc(strlen(strIn) + 1);
		strncpy(str, strIn, strlen(strIn) + 1);

		char *cfg = strtok(str, " ");
		char *cfgrecords = strtok(NULL, " ");

		int numRecords = 1;
		for (int i = 0; i < strlen(cfgrecords); i++) {
			if (cfgrecords[i] == ',')
				numRecords++;
		}
		// printf("[%s] [%s] %d\n", cfg, cfgrecords, numRecords);

		int records[numRecords];
		char *recordstr = strtok(cfgrecords, ",");
		for (int i = 0; i < numRecords; i++) {
			records[i] = strtol(recordstr, NULL, 10);
			recordstr = strtok(NULL, ",");
		}

		// arrangementSum += countBF(cfg, records, numRecords);
		arrangementSum += count(cfg, records, numRecords);

		current = current->next;
	}

	printf("Part 1: Arrangement Sum: %d\n", arrangementSum);
}

void part2(llist *ll)
{
	llNode *current = ll->head;
	int arrangementSum = 0;

	const int NUM_UNFOLD = 5;

	while (current != NULL) {
		char *strIn = (char *)current->data;
		char *str = malloc(strlen(strIn) + 1);
		strncpy(str, strIn, strlen(strIn) + 1);
		// printf("%s - ", str);

		char *cfgFold = strtok(str, " ");
		char *cfgrecords = strtok(NULL, " ");

		// Unfold cfg
		int unfoldSize = (strlen(cfgFold) * NUM_UNFOLD) + NUM_UNFOLD;
		char cfg[unfoldSize];
		strncpy(cfg, cfgFold, strlen(cfgFold));
		int offset = strlen(cfgFold) + 1;
		for (int i = 1; i < NUM_UNFOLD; i++) {
			cfg[(offset * i) - 1] = '?';
			strncpy(cfg + (offset * i), cfgFold, strlen(cfgFold));
		}
		cfg[unfoldSize - 1] = '\0';
		// printf("[%s] [%s]\n", cfgFold, cfg);

		int numRecordsFold = 1;
		for (int i = 0; i < strlen(cfgrecords); i++) {
			if (cfgrecords[i] == ',')
				numRecordsFold++;
		}
		int numRecords = numRecordsFold * NUM_UNFOLD;

		int records[numRecords];
		char *recordstr = strtok(cfgrecords, ",");
		for (int i = 0; i < numRecordsFold; i++) {
			int record = strtol(recordstr, NULL, 10);
			for (int j = 0; j < NUM_UNFOLD; j++)
				records[i + (j * numRecordsFold)] = record;
			recordstr = strtok(NULL, ",");
		}

		printf("%s\n\t[%s]\n\t[%d", strIn, cfg, records[0]);
		for (int i = 1; i < numRecords; i++) {
			printf(", %d", records[i]);
		}
		int arrangemens = count(cfg, records, numRecords);
		arrangementSum += arrangemens;
		printf("]\n%d\n", arrangemens);

		// arrangementSum += countBF(cfg, records, numRecords);
		// arrangementSum += count(cfg, records, numRecords);

		current = current->next;
	}

	printf("Part 2: Arrangement Sum: %d\n", arrangementSum);
}

int main(int argc, char *argv[])
{
	// llist *ll = getInputFile("assets/2023/Day12.txt");
	llist *ll = getInputFile("assets/test.txt");
	// llist_print(ll, printInput);

	part1(ll);
	part2(ll);

	return 0;
}
