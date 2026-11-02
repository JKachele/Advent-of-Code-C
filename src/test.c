/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util/util.h"
#include "util/md5.h"
// #include "util/md5Test.h"

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        char *test = "ihgpwlahDRRULDDULRDLUURLRLDRLRDRUDUURDLDRULUDLRDRUDUDLDU";

        u8 result[16];
        md5(test, result);
        for (int i = 0; i < 16; i++) {
                printf("%02x", result[i]);
        }
        printf("\n");

        // md5String(test, result);
        // for (int i = 0; i < 16; i++) {
        //         printf("%02x", result[i]);
        // }
        // printf("\n");

        return 0;
}

