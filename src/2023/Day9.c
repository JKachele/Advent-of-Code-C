/*************************************************
 *File----------Day9.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday May 13, 2024 08:10:21 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

int numNumbers(char *str) {
    int num = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            num++;
        }
    }
    return num + 1;
}

bool allSame(int nums[], int size) {
    for (int i = 1; i < size; i++) {
        if (nums[i] != nums[i-1]) {
            return false;
        }
    }
    return true;
}

int predictOASIS(int data[], int size) {
    if (allSame(data, size)) {
        return data[0];
    }
    int diffs[size-1];
    for (int i = 0; i < (size - 1); i++) {
        diffs[i] = data[i+1] - data[i];
    }
    return data[size-1] + predictOASIS(diffs, size-1);
}

int revertOASIS(int data[], int size) {
    if (allSame(data, size)) {
        return data[0];
    }
    int diffs[size-1];
    for (int i = 0; i < (size - 1); i++) {
        diffs[i] = data[i+1] - data[i];
    }
    return data[0] - revertOASIS(diffs, size-1);
}

void part1(llist *ll) {
    llNode *current = ll->head;
    int predictionSum = 0;
    while(current != NULL) {
        char curStr[BUFFER_SIZE];
        strncpy(curStr, (char*)(current->data), BUFFER_SIZE);
        int numReadings = numNumbers(curStr);
        int readings[numReadings];
        readings[0] = strtol(strtok(curStr, " "), NULL, 10);
        for (int i = 1; i < numReadings; i++) {
            readings[i] = strtol(strtok(NULL, " "), NULL, 10);
        }
        int prediction = predictOASIS(readings, numReadings);
        // printf("Prediction: %d\n", prediction);
        predictionSum += prediction;
        current = current->next;
    }
    printf("Part 1: Prediction Sum = %d\n", predictionSum);
}

void part2(llist *ll) {
    llNode *current = ll->head;
    int predictionSum = 0;
    while(current != NULL) {
        char curStr[BUFFER_SIZE];
        strncpy(curStr, (char*)(current->data), BUFFER_SIZE);
        int numReadings = numNumbers(curStr);
        int readings[numReadings];
        readings[0] = strtol(strtok(curStr, " "), NULL, 10);
        for (int i = 1; i < numReadings; i++) {
            readings[i] = strtol(strtok(NULL, " "), NULL, 10);
        }
        int prediction = revertOASIS(readings, numReadings);
        // printf("Prediction: %d\n", prediction);
        predictionSum += prediction;
        current = current->next;
    }
    printf("Part 2: Backwards Prediction Sum = %d\n", predictionSum);
}

int main(int argc, char *argv[]) {
    llist *ll = getInputFile("assets/2023/Day9.txt");
    // llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}


