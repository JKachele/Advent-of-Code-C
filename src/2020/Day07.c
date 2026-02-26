/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 19:48:26 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/talist.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096
#define SEED 0x12345678
#define MASK 0x000FFFFF

typedef struct baghash {
        char *name;
        u32 hash;
} baghash;
typedef tal(baghash) talbaghash;
baghash *BagHashes;
int NumHashes;

typedef struct rule {
        int num;
        u32 id;
} rule;
typedef tll(rule) tllrule;

typedef struct bag {
        u32 id;
        tllrule rules;
        u8 contsinsTarget; // 0: Unknown, 1: Yes, 2: No
} bag;
typedef tll(bag) tllbag;

struct input {
        tllbag bags;
        talbaghash hashes;
        u32 targetHash;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int getHashIndex(u32 hash, talbaghash hashes) {
        for (int i = 0; i < (int)hashes.length; i++) {
                if (hashes.array[i].hash == hash)
                        return i;
        }
        printf("Warning: Hash %u not found\n", hash);
        return 0;
}

void printRules(tllrule rules, talbaghash hashes) {
        tll_foreach(rules, it) {
                rule curRule = it->item;
                int ruleBagIndex = getHashIndex(curRule.id, hashes);
                printf("%d %s bags ", curRule.num, hashes.array[ruleBagIndex].name);
        }
}

void printBags(tllbag bags, talbaghash hashes) {
        tll_foreach(bags, it) {
                bag curBag = it->item;
                int bagIdIndex = getHashIndex(curBag.id, hashes);
                printf("%s Contains: ", hashes.array[bagIdIndex].name);
                printRules(curBag.rules, hashes);
                printf("\n");
        }
}

// Added top tag to test if target can contain itself
bool containsTarget(bag *hashTable[], u32 bagHash, u32 target, bool top) {
        // Make sure hash table entry exists
        if (hashTable[bagHash] == NULL) {
                printf("ERROR: Hash table entry for %u not found\n", bagHash);
                return false;
        }

        bag *curBag = hashTable[bagHash];

        // Exit Conditions
        if (curBag->contsinsTarget != 0)        // Already know for current bag
                return curBag->contsinsTarget == 1;
        if (!top && bagHash == target)          // Bag is target
                return true;
        if (curBag->rules.length == 0)          // Bag contains no other bags
                return false;

        bool contains = false;
        tll_foreach(curBag->rules, it) {
                rule curRule = it->item;
                if (containsTarget(hashTable, curRule.id, target, false)) {
                        contains = true;
                        break;
                }
        }

        if (contains)
                curBag->contsinsTarget = 1;
        else
                curBag->contsinsTarget = 2;
        return contains;
}

void mapBags(tllbag bags, bag *hashTable[], u32 target) {
        tll_foreach(bags, it) {
                containsTarget(hashTable, it->item.id, target, true);
        }
}

u64 numContains(bag *hashTable[], u32 bagHash) {
        // Make sure hash table entry exists
        if (hashTable[bagHash] == NULL) {
                printf("ERROR: Hash table entry for %u not found\n", bagHash);
                return false;
        }

        bag *curBag = hashTable[bagHash];

        // Exit Conditions
        if (curBag->rules.length == 0)          // Bag contains no other bags
                return 0;

        u64 numBags = 0;
        tll_foreach(curBag->rules, it) {
                rule curRule = it->item;
                numBags += curRule.num;
                numBags += curRule.num * numContains(hashTable, curRule.id);
        }

        return numBags;
}

void part1(struct input input) {
        tllbag bags = input.bags;
        talbaghash hashes = input.hashes;
        // printBags(bags, hashes);

        // Create hash table for bags
        bag **bagHashTable = calloc(MASK, sizeof(bag*));
        tll_foreach(bags, it) {
                bag *curBag = &it->item;
                bagHashTable[curBag->id] = curBag;
        }

        mapBags(bags, bagHashTable, input.targetHash);

        // Count bags that can contain target
        int bagCount = 0;
        tll_foreach(bags, it) {
                if (it->item.contsinsTarget == 1)
                        bagCount++;
        }

        free(bagHashTable);
        printf("Part 1: %d\n\n", bagCount);
}

void part2(struct input input) {
        tllbag bags = input.bags;
        talbaghash hashes = input.hashes;
        // printBags(bags, hashes);

        // Create hash table for bags
        bag **bagHashTable = calloc(MASK, sizeof(bag*));
        tll_foreach(bags, it) {
                bag *curBag = &it->item;
                bagHashTable[curBag->id] = curBag;
        }

        u64 bagCount = numContains(bagHashTable, input.targetHash);

        free(bagHashTable);
        printf("Part 2: %lu\n\n", bagCount);
}

struct input parseInput(llist *ll) {
        const size_t NAME_LENGTH = 20;
        const char *TargetBagName = "shiny gold";
        struct input input = {0};
        tllbag bags = tll_init();
        talbaghash hashes = tal_init();

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                bag curBag = {0};
                baghash curHash = {0};

                curHash.name = malloc(NAME_LENGTH * sizeof(char));
                char *bagName1 = strtok(str, " ");
                char *bagName2 = strtok(NULL, " ");
                int name1Length = strlen(bagName1) + 1;
                strncpy(curHash.name, bagName1, NAME_LENGTH);
                strncpy(curHash.name + name1Length, bagName2, NAME_LENGTH - name1Length);
                curHash.name[name1Length - 1] = ' ';  // Add Space between names
                curHash.name[NAME_LENGTH - 1] = '\0'; // Protect from overflows

                curBag.id = FNV(bagName1, SEED);
                curBag.id *= FNV(bagName2, SEED);
                curBag.id = curBag.id & MASK; // Mask to 20 bits to reduce hash table size
                curHash.hash = curBag.id;

                // Check for target bag
                if (strcmp(curHash.name, TargetBagName) == 0)
                        input.targetHash = curBag.id;

                // skip "bags contain"
                strtok(NULL, " ");
                strtok(NULL, " ");

                // Get rules for current bag
                char *tok = strtok(NULL, " ");
                while (tok != NULL) {
                        // Check for empty bags
                        if (strcmp(tok, "no") == 0)
                                break;

                        // Add bag rule
                        rule curRule = {0};
                        curRule.num = strtol(tok, NULL, 10);
                        curRule.id  = FNV(strtok(NULL, " "), SEED);
                        curRule.id *= FNV(strtok(NULL, " "), SEED);
                        curRule.id = curRule.id & MASK;
                        tll_push_back(curBag.rules, curRule);

                        // Check for any more rules
                        strtok(NULL, " ");      // "bags"
                        tok = strtok(NULL, " ");      // If NULL, no more rules
                }

                tll_push_back(bags, curBag);
                tal_add(hashes, curHash);
                i++;
        }

        input.bags = bags;
        input.hashes = hashes;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();
        part2(input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

