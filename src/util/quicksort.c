/*************************************************
 *File----------quicksort.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 19, 2025 16:18:54 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "quicksort.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef size_t u64;

// Swap 2 integers in an array
static void swapInt(int* a, int* b) {
        int temp = *a;
        *a = *b;
        *b = temp;
}

// Uses first element in range (first-last) as pivot.
// Moves elements smaller than pivot to front and larger to the back
// Moves pivot to middle and returns pivot index
static int partitionInt(int arr[], int first, int last) {
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
                        swapInt(&arr[i], &arr[j]);
                }
        }
        // Once all elements are moved, move the pivot to middle and return
        swapInt(&arr[first], &arr[j]);
        return j;
}

void quicksortInt(int arr[], int first, int last) {
        if (first < last) {
                int p = partitionInt(arr, first, last);

                // Recursivly call quicksort for left and right half
                // split by the partition index
                quicksortInt(arr, first, p - 1);   // First half
                quicksortInt(arr, p + 1, last);    // Last half
        }
}

// Swap 2 elements in an array
static void swap(void *a, void *b, u64 size) {
        char buffer[size]; // Temporary buffer for swapping

        // Swap bytes using memcpy
        memcpy(buffer, a, size);
        memcpy(a, b, size);
        memcpy(b, buffer, size);
}

// Uses first element in range (first-last) as pivot.
// Moves elements smaller than pivot to front and larger to the back
// Moves pivot to middle and returns pivot index
// cmp - 1: first > second, 0: first == second, -1: first < second
static int partition(void *arr, u64 size, int first, int last, int (*cmp)(void*, void*)) {
        // Initalize pivot and index pointers
        void *p = malloc(size);
        memcpy(p, (char*)(arr) + (first * size), size);
        int i = first;
        int j = last;

        while (i < j) {
                void *left = (char*)(arr) + (i * size);
                void *right = (char*)(arr) + (j * size);

                // Find first element larger than pivot starting from the front
                while (cmp(left, p) != 1 && i < last) {
                        i++;
                        left = (char*)(arr) + (i * size);
                }

                // Find first element smaller than pivot starting from the back
                while (cmp(right, p) == 1 && j > first) {
                        j--;
                        right = (char*)(arr) + (j * size);
                }

                // If larger is before smaller, Swap
                if (i < j) {
                        swap(left, right, size);
                }
        }
        // Once all elements are moved, move the pivot to middle and return
        void *piv = (char*)(arr) + (first * size);
        void *right = (char*)(arr) + (j * size);
        swap(piv, right, size);
        return j;
}

void quicksort(void *arr, u64 size, int first, int last, int (*cmp)(void*, void*)) {
        if (first < last) {
                int p = partition(arr, size, first, last, cmp);

                // Recursivly call quicksort for left and right half
                // split by the partition index
                quicksort(arr, size, first, p - 1, cmp);   // First half
                quicksort(arr, size, p + 1, last, cmp);    // Last half
        }
}

int quickselect(int arr[], int first, int last, int index) {
        if (first < last) {
                int p = partitionInt(arr, first, last);

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

// Swap 2 integers in an array
static void lswap(long* a, long* b) {
        long temp = *a;
        *a = *b;
        *b = temp;
}

// Uses first element in range (first-last) as pivot.
// Moves elements smaller than pivot to front and larger to the back
// Moves pivot to middle and returns pivot index
static long lpartition(long arr[], long first, long last) {
        // Initalize pivot and index pointers
        long p = arr[first];
        long i = first;
        long j = last;

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
                        lswap(&arr[i], &arr[j]);
                }
        }
        // Once all elements are moved, move the pivot to middle and return
        lswap(&arr[first], &arr[j]);
        return j;
}

void lquicksort(long arr[], long first, long last) {
        if (first < last) {
                long p = lpartition(arr, first, last);

                // Recursivly call quicksort for left and right half
                // split by the partition index
                lquicksort(arr, first, p - 1);   // First half
                lquicksort(arr, p + 1, last);    // Last half
        }
}
