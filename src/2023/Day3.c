/*************************************************
 *File----------Day3.c
 *Project-------{{PROJECT}}
 *Author--------Justin Kachele
 *Created-------Sunday Mar 24, 2024 18:55:44 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

int MAX_X;
int MAX_Y;

typedef struct vector2 {
    int x;
    int y;
} vector2;

bool isNum(char i) {
    return (i >= '0' && i <= '9');
}

vector2 initVector2(int x, int y) {
    vector2 vect;
    vect.x = x;
    vect.y = y;
    return vect;
}

int llist_get_longest_node(llist *ll) {
    llNode *current = ll->head;
    int longest = -1;
    while (current != NULL) {
        int length = strlen((char*)current->data);
        if ( length > longest) {
            longest = length;
        }
        current = current->next;
    }
    return longest;
}

int findNumber(char schem[MAX_X], int x) {
    int firstIndex = 0;
    int num = 0;
    for (int i = x; i >= 0; i--) {
        if (isNum(schem[i])) {
            firstIndex = i;
        } else {
            break;
        }
    }

    for (int i = firstIndex; i < MAX_X; i++) {
        if (!isNum(schem[i])) break;
        int digit = schem[i] - '0';
        num = (num * 10) + digit;
    }

    return num;
}

// Part 1 {{{
void part1(llist *ll) {
    llNode *current = ll->head;
    char schematic[MAX_Y][MAX_X];
    for(int y = 0; current != NULL; y++) {
        char str[BUFFER_SIZE];
        strncpy(str, (char*)current->data, BUFFER_SIZE);
        for (int x = 0; x < strlen(str); x++) {
            schematic[y][x] = str[x];
        }
        current = current->next;
    }

    int partNumSum = 0;
    bool numberFound = false;
    bool isPartNum = false;
    int currentNum = 0;
    for (int y = 0; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++) {
            char current = schematic[y][x];
            if (current >= '0' && current <= '9') {
                if (numberFound) {
                    currentNum = (currentNum * 10) + (current - '0');
                } else {
                    currentNum = current - '0';
                    numberFound = true;
                }

                // check for adjacent symbols
                if (isPartNum) {
                    continue;
                }
                for (int yMod = y - 1; yMod <= y + 1; yMod++) {
                    for (int xMod = x - 1; xMod <= x + 1; xMod++) {
                        if (yMod < 0 || yMod >= MAX_Y ||
                            xMod < 0 || xMod >= MAX_X) {
                            continue;
                        }
                        char c = schematic[yMod][xMod];
                        if ((c < '0' || c > '9') && c != '.') {
                            isPartNum = true;
                            break;
                        }
                    }
                }
            } else {
                if (numberFound) {
                    if (isPartNum) {
                        partNumSum += currentNum;
                        isPartNum = false;
                    }
                    currentNum = 0;
                    numberFound = false;
                }
            }
        }
        if (numberFound) {
            if (isPartNum) {
                partNumSum += currentNum;
                isPartNum = false;
            }
            currentNum = 0;
            numberFound = false;
        }
    }
    printf("Part 1: Part Number Sum = %d\n", partNumSum);
}   // }}}

// Part 2 {{{
void part2(llist *ll) {
    llNode *current = ll->head;
    char schematic[MAX_Y][MAX_X];
    int gearRatioSum = 0;
    for(int y = 0; current != NULL; y++) {
        char str[BUFFER_SIZE];
        strncpy(str, (char*)current->data, BUFFER_SIZE);
        for (int x = 0; x < strlen(str); x++) {
            schematic[y][x] = str[x];
        }
        current = current->next;
    }

    for (int y = 0; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++) {
            char current = schematic[y][x];
            if (current == '*') {
                int numCount = 0;
                int gearRatio = 1;
                vector2 offsets[] = {
                    initVector2(0, -1),
                    initVector2(-1, 0),
                    initVector2(1, 0),
                    initVector2(0, 1)
                };
                vector2 cornerOffsets[] = {
                    initVector2(-1, -1),
                    initVector2(1, -1),
                    initVector2(-1, 1),
                    initVector2(1, 1)
                };
                bool numPlace[4] = {false};
                bool numPlaceCorner[4] = {false};
                //  *0*     0*1
                //  1*2     ***
                //  *3*     2*3
                for (int i = 0; i < 4; i++) {
                    int xOff = x + offsets[i].x;
                    int yOff = y + offsets[i].y;
                    if (isNum(schematic[yOff][xOff])) {
                        numCount++;
                        numPlace[i] = true;
                    }
                }

                if (numCount != 2) {
                    if (!numPlace[0]) {
                        if (isNum(schematic[y-1][x-1])) {
                            numCount++;
                            numPlaceCorner[0] = true;
                        }
                        if (isNum(schematic[y-1][x+1])) {
                            numCount++;
                            numPlaceCorner[1] = true;
                        }
                    }
                    if(numCount != 2 && !numPlace[3]) {
                        if (isNum(schematic[y+1][x-1])) {
                            numCount++;
                            numPlaceCorner[2] = true;
                        }
                        if (isNum(schematic[y+1][x+1])) {
                            numCount++;
                            numPlaceCorner[3] = true;
                        }
                    }
                }
                if (numCount == 2) {
                    for (int i = 0; i < 4; i++) {
                        int xOff = x + offsets[i].x;
                        int yOff = y + offsets[i].y;
                        if (numPlace[i]) {
                            int gear = findNumber(schematic[yOff], xOff);
                            // printf("Gear: %d\n", gear);
                            gearRatio = gearRatio * gear;
                        }
                    }
                    for (int i = 0; i < 4; i++) {
                        int xOff = x + cornerOffsets[i].x;
                        int yOff = y + cornerOffsets[i].y;
                        if (numPlaceCorner[i]) {
                            int gear = findNumber(schematic[yOff], xOff);
                            // printf("Gear: %d\n", gear);
                            gearRatio = gearRatio * gear;
                        }
                    }
                    // printf("Gear Ratio: %d\n", gearRatio);
                    gearRatioSum += gearRatio; 
                }
            }
        }
    }
    printf("Part 2: Gear Ratio Sum: %d\n", gearRatioSum);
}   // }}}

int main(int argc, char *argv[]) {
    llist *ll = getInputFile("assets/2023/Day3.txt");
    // llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);
    MAX_X = llist_get_longest_node(ll);
    MAX_Y = ll->length;

    part1(ll);
    part2(ll);

    return 0;
}

