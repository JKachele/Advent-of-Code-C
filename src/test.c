/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util/vector.h"

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int arr[9100] = {0};
        int loop[11] = {435, 345,423, 568, 14643, 3457, 6522, 8642, 8536, 75311, 86368};

        for (int i = 0; i < 286; i++)
                arr[i] = i;
        for (int i = 286; i < 9100; i++)
                arr[i] = loop[(i - 286) % 11];

        int t = 1;
        int h = 2;
        while (arr[t] != arr[h]) {
                t++;
                h += 2;
        }
        printf("T= %d, H=%d\n", t, h);

        int finalCycleIndex = (9000 - t) % t;
        printf("cycle index = %d\n", finalCycleIndex);

        int value = arr[t + finalCycleIndex];
        printf("Value = %d vs %d\n", value, arr[9000]);

        return 0;
}

