/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Jan 02, 2025 08:35:32 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/util.h"
#include "../lib/hashmap.h"

#define INPUT_SIZE 64
#define BLINKS 25
#define BLINKS2 75

hashmap *map1;
hashmap *map2;
long stones = 0;

int printStone(const void *key, size_t ksize, uintptr_t value, void *usr) {
        long k = *(long*)key;
        long v = (long)value;
        return printf("Entry \"%ld\": %ld\n", k, v);
}

int freeStone(const void *key, size_t ksize, uintptr_t value, void *usr) {
        free((long*)key);
        return 0;
}

int numDigits(long i) {
        int digits = 0;
        while (i > 0) {
                digits++;
                i = i / 10;
        }
        return digits;
}

int updateStone(llist *stones, llNode *stone) {
        long stoneNum = *(long*)stone->data;
        
        if (stoneNum == 0) {
                *(long*)stone->data = 1;
                return 0;
        }
        int digits = numDigits(stoneNum);
        if (digits % 2 == 0) {
                long first = stoneNum / ipow(10, digits / 2); 
                long *last = malloc(sizeof(long));
                *last = stoneNum % ipow(10, digits / 2); 

                *(long*)stone->data = first;
                llNode *next = llist_create_node(last);
                llist_insert_after(stones, stone, next);
                return 0;
        }
        *(long*)stone->data = stoneNum * 2024;
        return 0;
}

int hashmap_SetAdd(hashmap* m, long *key, long value) {
        uintptr_t curVal = 0;
        if (hashmap_get(m, key, sizeof(long), &curVal) == 1) {
                value += curVal;
                hashmap_set(m, key, sizeof(long), value);
        } else {
                hashmap_set(m, key, sizeof(long), value);
        }
        return 0;
}

int blinkStone(const void *key, size_t ksize, uintptr_t value, void *usr) {
        long stoneNum = *(long*)key;
        
        if (stoneNum == 0) {
                long *newStone = malloc(sizeof(long));
                *newStone = 1;
                // hashmap_set(map2, newStone, sizeof(long), value);
                hashmap_SetAdd(map2, newStone, value);
                return 0;
        }
        int digits = numDigits(stoneNum);
        if (digits % 2 == 0) {
                long *first = malloc(sizeof(long));
                long *last = malloc(sizeof(long));
                *first = stoneNum / ipow(10, digits / 2); 
                *last = stoneNum % ipow(10, digits / 2); 

                // hashmap_set(map2, first, sizeof(long), value);
                // hashmap_set(map2, last, sizeof(long), value);
                hashmap_SetAdd(map2, first, value);
                hashmap_SetAdd(map2, last, value);

                return 0;
        }
        long *newStone = malloc(sizeof(long));
        *newStone = stoneNum * 2024;
        // hashmap_set(map2, newStone, sizeof(long), value);
        hashmap_SetAdd(map2, newStone, value);
        return 0;
}

int countStone(const void *key, size_t ksize, uintptr_t value, void *usr) {
        stones += value;
        return 0;
}

void part1(char *input) {
        char str[INPUT_SIZE];
        strncpy(str, input, INPUT_SIZE);

        llist *stones = llist_create();
        char *numStr = strtok(str, " ");
        while (numStr != NULL) {
                long *stone = malloc(sizeof(long));
                *stone = strtol(numStr, (char**)NULL, 10);
                llist_add(stones, stone);
                numStr = strtok(NULL, " ");
        }
        llist_print(stones, printLong);

        for (int i = 0; i < BLINKS; i++) {
                llNode *cur = stones->head;
                llNode *next = cur;
                while (cur != NULL) {
                        next = cur->next;
                        updateStone(stones, cur);
                        cur = next;
                }
                // llist_print(stones, printLong);
                // printf("Stones after %d blinks: %d\n", i, stones->length);
        }

        printf("Part 1: Stone Count: %d\n\n", stones->length);
}

void part2(char *input) {
        char str[INPUT_SIZE];
        strncpy(str, input, INPUT_SIZE);
        printf("%s\n", str);

        map1 = hashmap_create();
        char *numStr = strtok(str, " ");
        while (numStr != NULL) {
                long *stone = malloc(sizeof(long));
                *stone = strtol(numStr, (char**)NULL, 10);
                hashmap_set(map1, stone, sizeof(long), 1);
                numStr = strtok(NULL, " ");
        }

        for (int i = 0; i < BLINKS2; i++) {
                // hashmap_iterate(map1, printStone, NULL);
                // printf("\n");

                map2 = hashmap_create();
                hashmap_iterate(map1, blinkStone, NULL);

                hashmap_iterate(map1, freeStone, NULL);
                hashmap_free(map1);

                hashmap *tmp = map1;
                map1 = map2;
                map2 = tmp;
        }
        // hashmap_iterate(map1, printStone, NULL);
        printf("Hashmap Size: %d\n", hashmap_size(map1));
        stones = 0;
        hashmap_iterate(map1, countStone, NULL);

        printf("Part 2: Stone Count: %ld\n\n", stones);
}

int main(int argc, char *argv[]) {
        char *input;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                input = "125 17"; // Expected: 1) 55312 2) 65601038650482
        else
                input = "17639 47 3858 0 470624 9467423 5 188";
        
        part1(input);
        part2(input);

        return 0;
}
