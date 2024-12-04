/*************************************************
 *File----------Day1.c
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

// Swap 2 integers in an array
void swap(int* a, int* b) {
        int temp = *a;
        *a = *b;
        *b = temp;
}

// Uses first element in range (first-last) as pivot.
// Moves elements smaller than pivot to front and larger to the back
// Moves pivot to middle and returns pivot index
int partition(int arr[], int first, int last) {
        // Initalize pivot and index pointers
        int p = arr[first];
        int i = first;
        int j = last;

        while (i < j) {
                // Find first element larger than pivot starting from the front
                while (arr[i] <= p && i < last) {
                        i++;
                }

                // Find first element smaller than pivot starting from the back
                while (arr[j] > p && j > first) {
                        j--;
                }

                // If larger is before smaller, Swap
                if (i < j) {
                        swap(&arr[i], &arr[j]);
                }
        }
        // Once all elements are moved, move the pivot to middle and return
        swap(&arr[first], &arr[j]);
        return j;
}

void quicksort(int arr[], int first, int last) {
        if (first < last) {
                int p = partition(arr, first, last);

                // Recursivly call quicksort for left and right half
                // split by the partition index
                quicksort(arr, first, p - 1);   // First half
                quicksort(arr, p + 1, last);    // Last half
        }
}

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
        llist *ll = getInputFile("assets/2024/Day1.txt");
        // llist *ll = getInputFile("assets/test.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


