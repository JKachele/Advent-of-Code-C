/*************************************************
 *File----------Day9.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 23, 2024 16:07:17 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "../util/util.h"

#define BUFFER_SIZE 32768

// Using different File input because of input file format (One long line)
char *getInput(char *fileName) {
        FILE *inputFile = fopen(fileName, "r");     // Open file in read mode
        char *line = malloc(BUFFER_SIZE);
        ssize_t read;

        if (inputFile == NULL) {    // If file dosent exist, exit
                exit(EXIT_FAILURE);
        }

        if (fgets(line, BUFFER_SIZE, inputFile)!= NULL) {
                line[strlen(line) - 1] = '\0';  // Remove newline character from string
                return line;
        }
        return NULL;
}

void part1(char *input) {
        char str[BUFFER_SIZE];
        strncpy(str, input, BUFFER_SIZE);
        int diskmap[strlen(str)];

        int memorySize = 0;
        for (int i = 0; i < strlen(str); i++) {
                int block = str[i] - '0';
                memorySize += block;
                diskmap[i] = block;
        }

        // Array of memory, each element is id. id of -1 is free space
        int memory[memorySize];
        for (int i = 0; i < memorySize; i++) {
                memory[i] = -1;
        }

        // Populate memory
        int location = 0;
        for (int i = 0; i < strlen(str); i++) {
                if (i % 2 != 0) {
                        location += diskmap[i];
                        continue;
                }
                int id = i / 2;
                for (int j = location; j < location + diskmap[i]; j++) {
                        memory[j] = id;
                }
                location += diskmap[i];
        }
        /* for (int i = 0; i < memorySize; i++) {
                if (memory[i] == -1)
                        printf("[.]");
                else
                        printf("[%d]", memory[i]);
        }
        printf("\n"); */


        // Compress Memory
        int start = 0;
        int end = memorySize - 1;
        while (start < end) {
                // Find first free block
                while (memory[start] != -1) {
                        start++;
                }
                // Find last used block
                while (memory[end] == -1) {
                        end--;
                }
                if (start >= end)
                        break;

                // swap blocks
                memory[start] = memory[end];
                memory[end] = -1;
        }
        /* for (int i = 0; i < memorySize; i++) {
                if (memory[i] == -1)
                        printf("[.]");
                else
                        printf("[%d]", memory[i]);
        }
        printf("\n"); */

        // File Checksum
        long checksum = 0;
        int block = 0;
        while (memory[block] >= 0) {
                checksum += block * memory[block];
                block++;
        }

        printf("Part 1: File Checksum: %ld\n\n", checksum);
}

void part2(char *input) {
        char str[BUFFER_SIZE];
        strncpy(str, input, BUFFER_SIZE);

        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        char *input = getInput("assets/2024/Day9.txt");
        // char *input = getInput("assets/test.txt");
        // printf("%s\n", input);

        part1(input);
        part2(input);

        return 0;
}


