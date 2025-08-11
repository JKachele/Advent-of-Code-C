/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>

struct testStruct {
        int i;
        int arr[100];
};

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        struct testStruct ts = {0};
        memset(&ts, 0, sizeof(struct testStruct));
        for (int i = 0; i < 100; i++) {
                printf("%d: %s\n", i, (ts.arr[i] == 0 ? "False" : "True"));
        }

        return 0;
}

