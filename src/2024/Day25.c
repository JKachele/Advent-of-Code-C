/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Feb 27, 2025 14:08:37 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define LOCK_LENGTH 5
#define LOCK_DEPTH 7

typedef struct {
        uint8 pin[5];
} keylock;

typedef tll(keylock) tllkeylock;

void printKeyLocks(tllkeylock keylocks) {
        tll_foreach(keylocks, it) {
                keylock kl = it->item;
                for (int i = 0; i < LOCK_LENGTH; i++) {
                        printf("%u ", kl.pin[i]);
                }
                printf("\n");
        }
}

keylock parseLock(char block[LOCK_DEPTH][LOCK_LENGTH]) {
        keylock lock;
        for (int i = 0; i < LOCK_LENGTH; i++) {
                uint8 pinSize = 0;
                for (int j = 1; j < LOCK_DEPTH; j++) {
                        if (block[j][i] == '.')
                                break;
                        pinSize++;
                }
                lock.pin[i] = pinSize;
        }
        return lock;
}

keylock parseKey(char block[LOCK_DEPTH][LOCK_LENGTH]) {
        keylock key;
        for (int i = 0; i < LOCK_LENGTH; i++) {
                uint8 pinSize = 0;
                for (int j = LOCK_DEPTH - 2; j >= 0; j--) {
                        if (block[j][i] == '.')
                                break;
                        pinSize++;
                }
                key.pin[i] = pinSize;
        }
        return key;
}

bool validKey(keylock lock, keylock key) {
        for(int i = 0; i < LOCK_LENGTH; i++) {
                if (lock.pin[i] + key.pin[i] > LOCK_DEPTH - 2)
                        return false;
        }
        return true;
}

uint32 checkKeyLocks(tllkeylock locks, tllkeylock keys) {
        uint32 count = 0;
        tll_foreach(locks, l) {
                keylock lock = l->item;
                tll_foreach(keys, k) {
                        keylock key = k->item;
                        if (validKey(lock, key))
                                count++;
                }
        }
        return count;
}

void part1(llist *ll) {
        tllkeylock locks = tll_init();
        tllkeylock keys = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char block[LOCK_DEPTH][LOCK_LENGTH];
                char str[BUFFER_SIZE];
                for (int i = 0; i < LOCK_DEPTH; i++) {
                        strncpy(block[i], (char*)current->data, LOCK_LENGTH);
                        current = current->next;
                        if (current == NULL) break;
                }

                if (block[0][0] == '#') {
                        keylock lock = parseLock(block);
                        tll_push_back(locks, lock);
                } else if (block[0][0] == '.') {
                        keylock key = parseKey(block);
                        tll_push_back(keys, key);
                } else {
                        printf("Error Parsing Input");
                        break;
                }
                if (current == NULL) break;
                current = current->next;
        }

        // printKeyLocks(locks);
        // printf("\n");
        // printKeyLocks(keys);
        
        uint32 count = checkKeyLocks(locks, keys);

        printf("Part 1: Valid Combinations: %u\n\n", count);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day25.txt");
        } else {
                ll = getInputFile("assets/inputs/2024/Day25.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f\n", 
                        parseTime, pt1Time);

        return 0;
}
