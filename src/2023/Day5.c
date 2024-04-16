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

typedef struct range {
    long start;
    long end;
} range;

typedef struct transformation {
    long dest;
    long src;
    long len;
} transformation;

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

void printRange(void *data) {
    if (data == NULL) { return; }
    range *curRange = (range*)data;
    printf("%ld, %ld", curRange->start, curRange->end);
}

void printTrans(void *data) {
    if (data == NULL) { return; }
    transformation *trans = (transformation*)data;
    printf("%ld, %ld, %ld", trans->dest, trans->src, trans->len);
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
    // printLongArray(out, numSeeds);

    return current;
}

void addRangeNode(llist *ll, long start, long end) {
    range *newRange = malloc(sizeof(range));
    newRange->start = start;
    newRange->end = end; 
    llist_add(ll, newRange);
}

void addTransformNode(llist *ll, long dest, long src, long len) {
    transformation *newTransform = malloc(sizeof(transformation));
    newTransform->dest = dest;
    newTransform->src = src;
    newTransform->len = len;
    llist_add(ll, newTransform);
}

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
    // printLongArray(seeds, numSeeds);

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
}

void part2(llist *ll) {
    llNode *current = ll->head;

    // Get Seeds
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
    current = current->next;

    // Convert seeds list to ranges list
    llist *ranges = llist_create();
    llNode *curSeed = seedLL->head;
    while (curSeed != NULL) {
        long start = *(long*)curSeed->data;
        curSeed = curSeed->next;
        long length = *(long*)curSeed->data;
        long end = start + length - 1;
        addRangeNode(ranges, start, end);
        curSeed = curSeed->next;
    }
    // llist_print(ranges, printRange);

    // Get array of Transformation lists
    llist *transformations[7];
    for (int i = 0; i < 7; i++) {
        llist *curTrans = llist_create();
        current = current->next->next;
        while (current != NULL && ((char*)current->data)[0] != '\0') {
            char str[BUFFER_SIZE];
            strncpy(str, (char*)current->data, BUFFER_SIZE);
            long dest, src, range;
            dest = strtol(strtok(str, " "), NULL, 10);
            src = strtol(strtok(NULL, " "), NULL, 10);
            range = strtol(strtok(NULL, " "), NULL, 10);
            addTransformNode(curTrans, dest, src, range);
            current = current->next;
        }
        // llist_print(curTrans, printTrans);
        transformations[i] = curTrans;
    }

    // printf("Part 2: Lowest Location = %ld\n", minLocation);
}

int main(int argc, char *argv[]) {
    // llist *ll = getInputFile("assets/2023/Day5.txt");
    llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}

