/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdint.h>

void printBin(unsigned long n, int bits) {
        // Get number of bits needed
        unsigned long i = n;
        int numBits = 0;
        while (i > 0) {
                numBits++;
                i >>= 1;
        }

        if (bits != -1)
                numBits = bits;
        
        unsigned long mask = 1 << (numBits - 1);
        while (mask > 0) {
                if ((n & mask) != 0)
                        printf("1");
                else
                        printf("0");
                mask >>= 1;
        }
        printf("\n");
}

void bin(uint64_t num) {
        if (num > 1) {
                bin(num / 2);
        }
        printf("%lu", num % 2);
}

void printBits(size_t const size, void const * const ptr) {
        unsigned char *b = (unsigned char*) ptr;
        unsigned char byte;
        int i, j;

        for (i = size - 1; i >= 0; i--) {
                for (j = 7; j >= 0; j--) {
                        byte = (b[i] >> j) & 1;
                        printf("%u", byte);
                }
        }
        printf("\n");
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        uint64_t num = 3033191740;
        // printBin(3033191740, -1);
        bin(3033191740);
        printf("\n");
        printBits(4, &num);
        printf("%lu - %lu\n", sizeof(uint64_t), sizeof(long));

        return 0;
}

