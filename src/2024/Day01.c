/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 02, 2024 08:47:32 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/quicksort.h"

void part1(llist *ll) {
        llNode *current = ll->head;
        int numIDs = ll->length;

        int list1[numIDs];
        int list2[numIDs];

        // Get 2 arrays from input
        int pos = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                list1[pos] = strtol(strtok(str, " "), (char**)NULL, 10);
                list2[pos] = strtol(strtok(NULL, " "), (char**)NULL, 10);

                current = current->next;
                pos++;
        }

        // Sort arrays - Quicksort
        quicksort(list1, 0, numIDs - 1);
        quicksort(list2, 0, numIDs - 1);

        int totalDist = 0;
        for (int i = 0; i < numIDs; i++) {
                totalDist += abs(list1[i] - list2[i]);
        }

        printf("\nPart 1: Total Distance: %d\n", totalDist);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        int numIDs = ll->length;

        int list1[numIDs];
        int list2[numIDs];

        // Get 2 arrays from input
        int pos = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                list1[pos] = strtol(strtok(str, " "), (char**)NULL, 10);
                list2[pos] = strtol(strtok(NULL, " "), (char**)NULL, 10);

                current = current->next;
                pos++;
        }

        // Sort arrays - Quicksort
        quicksort(list1, 0, numIDs - 1);
        quicksort(list2, 0, numIDs - 1);

        // for (int i = 0; i < numIDs; i++) {
        //         printf("%d - [%d, %d]\n", i, list1[i], list2[i]);
        // }

        long simScore = 0;
        int pos1 = 0;
        int pos2 = 0;
        while (pos1 < numIDs && pos2 < numIDs) {
                int count1 = 1;
                int count2 = 0;
                // Find number of occuranced of entry in list1
                int cur = list1[pos1];
                pos1++;
                while (list1[pos1] == cur && pos1 < numIDs) {
                        pos1++;
                        count1++;
                }

                //Find number of occurances of entry in list2
                while (list2[pos2] <= cur && pos2 < numIDs) {
                        if (list2[pos2] == cur) {
                                count2++;
                        }
                        pos2++;
                }

                int similarity = cur * count1 * count2;
                // printf("%d: %d, %d - %d\n", cur, count1, count2, similarity);
                simScore += similarity;
        }

        printf("\nPart 2: Similarity Score: %ld\n", simScore);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day01.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day01.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


