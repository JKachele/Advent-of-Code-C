/*************************************************
 *File----------quicksort.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 19, 2025 16:18:54 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "quicksort.h"

// Swap 2 integers in an array
static void swap(int* a, int* b) {
        int temp = *a;
        *a = *b;
        *b = temp;
}

// Uses first element in range (first-last) as pivot.
// Moves elements smaller than pivot to front and larger to the back
// Moves pivot to middle and returns pivot index
static int partition(int arr[], int first, int last) {
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

int quickselect(int arr[], int first, int last, int index) {
        if (first < last) {
                int p = partition(arr, first, last);

                if (p == index)
                        return arr[p];

                // Recursivly call quickselect for the side containing index
                if (p < index)
                        // Last Section
                        return quickselect(arr, p + 1, last, index);
                else
                        // First Section
                        return quickselect(arr, first, p - 1, index);
        } else {
                return arr[first];
        }
}
