/*************************************************
 *File----------Day5.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 02, 2024 15:26:20 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

// Support Functions {{{
void printLong(void* data) {
    if (data == NULL) {return;}
    long *num = (long*)data;
    printf("%ld", *num);
}

void llToArray(llist *ll, long arr[]) {
    llNode *current = ll->head;
    int i = 0;
    while (current != NULL) {
        arr[i] = *(long*)current->data;
        i++;
        current = current->next;
    }
}

void initLongArray(long arr[], long len, long initValue) {
    for (int i = 0; i < len; i++) {
        arr[i] = initValue;
    }
}

void printLongArray(long arr[], long len) {
    for (int i = 0; i < len; i++) {
        printf("[%ld] ", arr[i]);
    }
    printf("\n");
}

llNode *processSection(llNode *start, long in[], long out[], int numSeeds) {
    llNode *current = start;
    // while the line is not empty
    while (current != NULL && ((char*)current->data)[0] != '\0') {
        char str[BUFFER_SIZE];
        strncpy(str, (char*)current->data, BUFFER_SIZE);
        long dest, src, range;
        dest = strtol(strtok(str, " "), NULL, 10);
        src = strtol(strtok(NULL, " "), NULL, 10);
        range = strtol(strtok(NULL, " "), NULL, 10);
        for (int i = 0; i < numSeeds; i++) {
            if (in[i] >= src && in[i] < src + range) {
                out[i] = dest + (in[i] - src);
            }
        }

        current = current->next;
    }
    for (int i = 0; i < numSeeds; i++) {
        if (out[i] == -1) {
            out[i] = in[i];
        }
    }
    printLongArray(out, numSeeds);

    return current;
}   // }}}

// Part 1 {{{
void part1(llist *ll) {
    llNode *current = ll->head;

    char seedStr[BUFFER_SIZE];
    strncpy(seedStr, (char*)current->data, BUFFER_SIZE);
    llist *seedLL = llist_create();
    char *token = strtok(seedStr, " ");
    token = strtok(NULL, " ");
    while (token != NULL) {
        long *seedPtr = malloc(sizeof(long));
        if (!seedPtr) {exit(-1);}
        *seedPtr = strtol(token, NULL, 10);
        llist_add(seedLL, seedPtr);
        token = strtok(NULL, " ");
    }
    long seeds[seedLL->length];
    llToArray(seedLL, seeds);
    int numSeeds = seedLL->length;
    printLongArray(seeds, numSeeds);

    // Skip next 2 lines to get to the seed to soil nums
    current = current->next->next->next;

    long soils[numSeeds];
    initLongArray(soils, numSeeds, -1);
    current = processSection(current, seeds, soils, numSeeds);
    current = current->next->next;

    long fert[numSeeds];
    initLongArray(fert, numSeeds, -1);
    current = processSection(current, soils, fert, numSeeds);
    current = current->next->next;

    long water[numSeeds];
    initLongArray(water, numSeeds, -1);
    current = processSection(current, fert, water, numSeeds);
    current = current->next->next;

    long light[numSeeds];
    initLongArray(light, numSeeds, -1);
    current = processSection(current, water, light, numSeeds);
    current = current->next->next;

    long temp[numSeeds];
    initLongArray(temp, numSeeds, -1);
    current = processSection(current, light, temp, numSeeds);
    current = current->next->next;

    long humidity[numSeeds];
    initLongArray(humidity, numSeeds, -1);
    current = processSection(current, temp, humidity, numSeeds);
    current = current->next->next;

    long location[numSeeds];
    initLongArray(location, numSeeds, -1);
    current = processSection(current, humidity, location, numSeeds);

    long minLocation = location[0];
    for (int i = 1; i < numSeeds; i++) {
        if (location[i] < minLocation) {
            minLocation = location[i];
        }
    }

    printf("Part 1: Lowest Location = %ld\n", minLocation);
}   // }}}

// Part 2 {{{
void part2(llist *ll) {
    llNode *current = ll->head;

    char seedStr[BUFFER_SIZE];
    strncpy(seedStr, (char*)current->data, BUFFER_SIZE);
    llist *seedLL = llist_create();
    char *token = strtok(seedStr, " ");
    token = strtok(NULL, " ");
    while (token != NULL) {
        long *seedPtr = malloc(sizeof(long));
        if (!seedPtr) {exit(-1);}
        *seedPtr = strtol(token, NULL, 10);
        llist_add(seedLL, seedPtr);
        token = strtok(NULL, " ");
    }
    long seeds[seedLL->length];
    llToArray(seedLL, seeds);
    int numSeeds = seedLL->length;
    printLongArray(seeds, numSeeds);

    // Skip next 2 lines to get to the seed to soil nums
    current = current->next->next->next;

    long soils[numSeeds];
    initLongArray(soils, numSeeds, -1);
    current = processSection(current, seeds, soils, numSeeds);
    current = current->next->next;

    long fert[numSeeds];
    initLongArray(fert, numSeeds, -1);
    current = processSection(current, soils, fert, numSeeds);
    current = current->next->next;

    long water[numSeeds];
    initLongArray(water, numSeeds, -1);
    current = processSection(current, fert, water, numSeeds);
    current = current->next->next;

    long light[numSeeds];
    initLongArray(light, numSeeds, -1);
    current = processSection(current, water, light, numSeeds);
    current = current->next->next;

    long temp[numSeeds];
    initLongArray(temp, numSeeds, -1);
    current = processSection(current, light, temp, numSeeds);
    current = current->next->next;

    long humidity[numSeeds];
    initLongArray(humidity, numSeeds, -1);
    current = processSection(current, temp, humidity, numSeeds);
    current = current->next->next;

    long location[numSeeds];
    initLongArray(location, numSeeds, -1);
    current = processSection(current, humidity, location, numSeeds);

    long minLocation = location[0];
    for (int i = 1; i < numSeeds; i++) {
        if (location[i] < minLocation) {
            minLocation = location[i];
        }
    }

    printf("Part 2: Lowest Location = %ld\n", minLocation);
}   // }}}

int main(int argc, char *argv[]) {
    // llist *ll = getInputFile("assets/2023/Day5.txt");
    llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}

