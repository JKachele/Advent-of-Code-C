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
#include <stdbool.h>
#include "../util/linkedlist.h"
// #include "../util/util.h"

#define BUFFER_SIZE 32768

typedef struct {
        int size;
        int id;
        bool seen;
} block;

void printBlock(void *data) {
        if (data == NULL)
                return;
        block *b = (block*)data;
        printf("%d:%d", b->size, b->id);
}

void printMem(llist *map) {
        llNode *cur = map->head;
        while (cur != NULL) {
                block *b = (block*)cur->data;
                for (int j = 0; j < b->size; j++) {
                        if (b->id >= 0)
                                printf("%d", b->id);
                        else
                                printf(".");
                }
                cur = cur->next;
        }
        printf("\n");
}

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
        int len = strlen(str);
        block diskmap[len];
        llist *map = llist_create();

        int memorySize = 0;
        for (int i = 0; i < len; i++) {
                block *curBlock = malloc(sizeof(block));
                int size = str[i] - '0';
                curBlock->size = size;
                curBlock->seen = false;
                if (i % 2 == 0)
                        curBlock->id = i / 2;
                else
                        curBlock->id = -1;
                llist_add(map, curBlock);
        }
        // llist_print(map, printBlock);
        // printMem(map);

        // Compress Memory
        llNode *end = map->tail;
        while (end != NULL) {
                llNode *start = map->head;
                
                // Get last used block
                while (end != NULL && (((block*)end->data)->seen
                                || ((block*)end->data)->id == -1)) {
                        end = end->prev;
                }
                if (end == NULL)
                        continue;

                // Get first empty block with enough space
                int endSize = ((block*)end->data)->size;
                while (start != NULL && (((block*)start->data)->id != -1
                                || ((block*)start->data)->size < endSize)) {
                        start = start->next;
                }
                // printMem(map);

                // If no space available, continue with next block in
                if (start == NULL || llist_get_index(start, map) >= 
                                        llist_get_index(end, map)) {
                        ((block*)end->data)->seen = true;
                        end = end->prev;
                        continue;
                }

                // Move block to free space
                block *free = (block*)start->data;
                block *file = (block*)end->data;
                if (free->size == file->size) {
                        // Simply switch block IDs
                        free->id = file->id;
                        file->id = -1;
                        free->seen = true;
                        continue;
                }

                // If free block is larger, need to create new block
                block *new = malloc(sizeof(block));
                new->id = file->id;
                new->size = file->size;
                new->seen = true;
                file->id = -1;
                free->size -= new->size;
                llNode* newNode = llist_create_node(new);
                llist_insert_before(map, start, newNode);
        }
        // llist_print(map, printBlock);
        // printMem(map);

        // File Checksum
        long checksum = 0;
        llNode *cur = map->head;
        int index = 0;
        while (cur != NULL) {
                block *b = (block*)cur->data;
                if (b->id != -1) {
                        for (int i = index; i < index + b->size; i++) {
                                checksum += b->id * i;
                        }
                }
                index += b->size;
                cur = cur->next;
        }

        printf("Part 2: File Checksum: %ld\n\n", checksum);
}

int main(int argc, char *argv[]) {
        char *input;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                input = getInput("assets/test.txt");
                // char *input = "1313165";
        else
                input = getInput("assets/2024/Day9.txt");
        // printf("%s\n", input);

        part1(input);
        part2(input);

        return 0;
}


