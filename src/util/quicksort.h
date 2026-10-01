/*************************************************
 *File----------quicksort.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 19, 2025 16:19:16 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdlib.h>

// cmp - 1: first > second, 0: first == second, -1: first < second
void quicksort(void *arr, size_t size, int first, int last, int (*cmp)(void*, void*));
void quicksortInt(int arr[], int first, int last);
int quickselect(int arr[], int first, int last, int index);
void lquicksort(long arr[], long first, long last);

#endif

