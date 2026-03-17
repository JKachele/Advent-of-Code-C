/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 16, 2026 21:12:48 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(u64) tllu64;

typedef struct instr {
        bool isMask;
        union {
                struct {
                        u64 mask0;      // Mask to set bits to 0
                        u64 mask1;      // Mask to set bits to 1
                        u64 maskX;      // Mask to set bits to X
                };
                struct {
                        u64 address;
                        u64 value;
                };
        };
} instr;
typedef tll(instr) tllinstr;

typedef struct node {
        u64 address;
        u64 value;
        struct node *left;
        struct node *right;
} node;

struct input {
        tllinstr instrs;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBin(u64 n, int bits) {
        u64 mask = 1ULL << (bits - 1);
        for (int i = 0; i < bits; i++) {
                printf("%c", (n & mask) ? '1' : '0');
                mask >>= 1;
        }
        printf(" (%lu)\n", n);
}

void printMask(u64 mask0, u64 mask1, u64 maskX) {
        u64 bitMask = 1ULL << 35;
        for (int i = 0; i < 36; i++) {
                if (mask0 & bitMask)
                        printf("0");
                else if (mask1 & bitMask)
                        printf("1");
                else if (maskX & bitMask)
                        printf("X");
                else
                        printf("*");
                bitMask >>= 1;
        }
        printf("\n");
}

void printTest(char *str, bool b) {
        for (int i = 0; i < 36; i++) {
                if (str[i] == '1' && b)
                        printf("1");
                else if (str[i] == '0' && !b)
                        printf("1");
                else
                        printf("0");
        }
        printf("\n");
}

int insertOrModify(node **cur, u64 address, u64 value) {
        // if curNode is NULL, create new node
        if ((*cur) == NULL) {
                (*cur) = malloc(sizeof(node));
                if (*cur == NULL) return 1;
                (*cur)->address = address;
                (*cur)->value = value;
                (*cur)->left = NULL;
                (*cur)->right = NULL;
                return 0;
        }

        // If curNode's address matches address, modify its value
        if ((*cur)->address == address) {
                (*cur)->value = value;
                return 0;
        }

        // If curNode's address is less than address, go right
        if ((*cur)->address < address) {
                return insertOrModify(&((*cur)->right), address, value);
        }

        // If curNode's address is greater than address, go left
        if ((*cur)->address > address) {
                return insertOrModify(&((*cur)->left), address, value);
        }
        return 1;
}

int NumAddresses = 0;
u64 sumBinaryTree(node *cur) {
        if (cur == NULL) return 0;

        NumAddresses++;
        u64 sum = cur->value;
        sum += sumBinaryTree(cur->left);
        sum += sumBinaryTree(cur->right);
        return sum;
}

int freeTree(node *cur) {
        if (cur == NULL) return 0;

        if (freeTree(cur->left))
                free(cur->left);
        if (freeTree(cur->right))
                free(cur->right);
        return 1;
}

void getAddressList(int8 address[36], tllu64 *addresses) {
        for (int i = 35; i >= 0; i--) {
                if (address[i] == -1) {
                        // Create new addresses with X as 1 and 0
                        int8 newAddress0[36];
                        memcpy(newAddress0, address, 36);
                        newAddress0[i] = 0;
                        getAddressList(newAddress0, addresses);

                        int8 newAddress1[36];
                        memcpy(newAddress1, address, 36);
                        newAddress1[i] = 1;
                        getAddressList(newAddress1, addresses);

                        return;
                }
        }

        // Only reach here if address has no 'X' in it
        // Convert and add address to list
        u64 newAddress = 0;
        for (int i = 35; i >= 0; i--) {
                newAddress <<= 1;
                newAddress += address[i];
        }
        tll_push_back(*addresses, newAddress);
}

void part1(struct input input) {
        tllinstr instrs = input.instrs;

        // Binary search tree to hold memory
        node *root = NULL;

        u64 mask0 = 0;
        u64 mask1 = 0;
        tll_foreach(instrs, it) {
                instr curInstr = it->item;
                if (curInstr.isMask) {
                        mask0 = curInstr.mask0;
                        mask1 = curInstr.mask1;
                        continue;
                }

                u64 value = curInstr.value;
                value &= ~mask0;
                value |= mask1;

                int error = insertOrModify(&root, curInstr.address, value);
                if (error) {
                        printf("Error inserting value at address %lu\n", curInstr.address);
                        break;
                }
        }

        u64 memSum = sumBinaryTree(root);
        if (freeTree(root)) free(root);
        printf("%d\n", NumAddresses);

        printf("Part 1: %lu\n\n", memSum);
}

void part2(struct input input) {
        tllinstr instrs = input.instrs;

        // Binary search tree to hold memory
        node *root = NULL;

        u64 mask0 = 0;
        u64 mask1 = 0;
        u64 maskX = 0;
        tll_foreach(instrs, it) {
                instr curInstr = it->item;
                if (curInstr.isMask) {
                        mask0 = curInstr.mask0;
                        mask1 = curInstr.mask1;
                        maskX = curInstr.maskX;
                        continue;
                }

                u64 unmaskedAddress = curInstr.address;
                int8 address[36] = {0};
                u64 bitMask = 1;
                for (int i = 0; i < 36; i++) {
                        if (mask0 & bitMask)
                                address[i] = (unmaskedAddress & bitMask) ? 1 : 0;
                        else if (mask1 & bitMask)
                                address[i] = 1;
                        else if (maskX & bitMask)
                                address[i] = -1;
                        bitMask <<= 1;
                }

                tllu64 addresses = tll_init();
                getAddressList(address, &addresses);

                int error = 0;
                tll_foreach(addresses, it) {
                        error = insertOrModify(&root, it->item, curInstr.value);
                        if (error) {
                                printf("Error inserting value at address %lu\n", it->item);
                                break;
                        }
                }
                if (error) break;
        }

        NumAddresses = 0;
        u64 memSum = sumBinaryTree(root);
        if (freeTree(root)) free(root);
        printf("%d\n", NumAddresses);

        printf("Part 2: %lu\n", memSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                instr curInstr = {0};

                if (str[1] == 'a') { // Mask instruction
                        curInstr.isMask = true;
                        // Mask starts at str[7]
                        for (int i = 7; i < (36+7); i++) {
                                curInstr.mask0 <<= 1;
                                curInstr.mask1 <<= 1;
                                curInstr.maskX <<= 1;

                                if (str[i] == '0')
                                        curInstr.mask0 += 1;
                                else if (str[i] == '1')
                                        curInstr.mask1 += 1;
                                else if (str[i] == 'X')
                                        curInstr.maskX += 1;
                        }
                } else { // Memory Instruction
                        curInstr.isMask = false;
                        // Mem address start at str[4]
                        char *addrStr = strtok(str+4, "]");
                        curInstr.address = strtoull(addrStr, NULL, 10);

                        strtok(NULL, "=");
                        char *valueStr = strtok(NULL, "");
                        curInstr.value = strtoll(valueStr, NULL, 10);
                }
                tll_push_back(input.instrs, curInstr);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day14.txt";
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

