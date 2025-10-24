/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/talist.h"

typedef tal(int) talint;

void binarySearch(talint *list, int n, int s) {
        int *arr = list->array;

        int low = 0;
        int high = n - 1;
        bool found = false;
        while (low <= high) {
                int mid = low + (high - low) / 2;

                if (arr[mid] == s) {
                        found = true;
                        low = mid;
                        break;
                }

                if (arr[mid] < s)
                        low = mid + 1;
                else
                        high = mid - 1;
        }

        if (found) {
                printf("\"%d\" Found at %d!\n", s, low);
                tal_remove(*list, low);
        } else {
                printf("\"%d\" Not found. (%d, %d)\n", s, low, high);
                tal_insert(*list, low, s);
        }
}

void printList(talint l) {
        for (int i = 0; i < (int)l.length; i++) {
                printf("%2d ", i);
        }
        printf("\n");
        for (int i = 0; i < (int)l.length; i++) {
                printf("%2d ", l.array[i]);
        }
        printf("\n");
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        talint l = tal_init();
        for (int i = 0; i < 99; i+=7) {
                tal_add(l, i);
        }
        printList(l);
        binarySearch(&l, l.length, 56);
        printList(l);
        binarySearch(&l, l.length, 47);
        printList(l);

}

