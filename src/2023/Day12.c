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

bool isValid(char *cfg, llist *records) {
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
    bool valid = llist_equals(counts, records, intEquals);
    llist_free(counts);
    return valid;
}

bool isValidCondition(char *cfg, llist *records) {
    int record = *(int*) records->head->data;
    // Enough Springs left for first record
    bool con1 = strlen(cfg) >= record;
    // springs in the first "record" must not be functional
    bool con2 = true;
    for (int i = 0; i < record; i++) {
        if (cfg[i] == '.') con2 = false;
    }
    bool con3 = false;
    return con1 && con2 && con3;
}

// Part 1: Brute Force
int countBF(char *cfg, llist *records) {
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
            if ((y & 1) == 1) {
                cfgTest[unknowns[x]] = '#';
            }
            x++;
            y = y >> 1;
        }
        if (isValid(cfgTest, records)) {
            numValid++;
        }
        free(cfgTest);
    }
    // printf("%s - %d\n", cfg, numValid);

    return numValid;
}

// Dynamic programming and memoization
int count(char *cfg, llist *records) {
    // if cfg is empty, return 1 if records is also empty.
    if (cfg[0] == '\0')
        return (records->head == NULL) ? 1 : 0;
    // if records is empty, return 1 if cfg has no # in it.
    if (records->head == NULL)
        return (strchr(cfg, '#') == NULL) ? 1 : 0;

    int numCombinations = 0;

    // if '.' or '?'
    if (cfg[0] == '.' || cfg[0] == '?') {
        numCombinations += count(cfg + 1, records);
    }
    // if '#' or '?'
    if (cfg[0] == '#' || cfg[0] == '?') {
        if (isValidCondition(cfg, records)) {
        }
    }

    return 0;
}

void part1(llist *ll) {
    llNode *current = ll->head;
    int arrangementSum = 0;
    while(current != NULL) {
        char *str = (char*)current->data;

        char *cfg = strtok(str, " ");
        char *cfgrecords = strtok(NULL, " ");
        // printf("[%s] [%s]\n", cfg, cfgrecords);

        llist *records = llist_create();
        char* recordstr = strtok(cfgrecords, ",");
        while (recordstr != NULL) {
            int *num = malloc(sizeof(int));
            *num = strtol(recordstr, NULL, 10);
            llist_add(records, num);
            recordstr = strtok(NULL, ",");
        }
        // llist_print(records, printInt);
        arrangementSum += countBF(cfg, records);

        current = current->next;
    }

    printf("\nPart 1: Arrangement Sum: %d\n", arrangementSum);
}

void part2(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("\nPart 2: \n");
}

int main(int argc, char *argv[]) {
    // llist *ll = getInputFile("assets/2023/Day12.txt");
    llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}

