/*************************************************
 *File----------Day7.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 23, 2024 07:56:26 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

typedef enum {
    High = 0,
    OnePair = 1,
    TwoPair = 2,
    ThreeKind = 3,
    Full = 4,
    FourKind = 5,
    FiveKind  = 6
} strength;

typedef struct {
    int cards[5];
    int bid;
    int strength;
} hand;

const char cardValue[13] = {
    '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'
};

void copyHandArray(hand *handSrc[], hand *handDest[], int length) {
    for (int i = 0; i < length; i++) {
        hand *newHand = malloc(sizeof(hand));
        for (int j = 0; j < 5; j++) {
            newHand->cards[j] = handSrc[i]->cards[j];
        }
        newHand->strength = handSrc[i]->strength;
        newHand->bid = handSrc[i]->bid;
        handDest[i] = newHand;
    }
}

void quicksortHands(hand *hands[], int length) {

}

int getStrength(int cards[5]) {
    // Count Max similar cards
    int maxPair = 0;
    int secondPair = 0;
        int maxvalue = 0;
    for (int i = 0; i < 4; i++) {
        int matches = 1;
        for (int j = i+1; j < 5; j++) {
            if (cards[i] == cards[j]) {
                matches++;
            }
        }
        if (matches > maxPair) {
            secondPair= maxPair;
            maxPair = matches;
            maxvalue = cards[i];
        } else if (matches > secondPair && cards[i] != maxvalue) {
            secondPair = matches;
        }
    }
    if (maxPair == 5) { return FiveKind; }
    if (maxPair == 4) { return FourKind; }
    if (maxPair == 3 && secondPair == 2) { return Full; }
    if (maxPair == 3) { return ThreeKind; }
    if (maxPair == 2 && secondPair == 2) { return TwoPair; }
    if (maxPair == 2) { return OnePair; }
    return High;
}

hand *initHand(char *cards, int bid) {
    hand *curHand = malloc(sizeof(hand));
    for (int i = 0; i < 5; i++) {
        // If card value is numeric
        if (cards[i] >= 50 && cards[i] <= 57) {
            curHand->cards[i] = cards[i] - 50;
        } else {
            // Loop through last 5 card values
            for (int j = 8; j < 13; j++) {
                if (cards[i] == cardValue[j]) {
                    curHand->cards[i] = j;
                }
            }
        }
    }
    curHand->bid = bid;
    curHand->strength = getStrength(curHand->cards);

    return curHand;
}

void part1(llist *ll) {
    hand *hands[ll->length];
    llNode *current = ll->head;
    int nodeNum = 0;
    while(current != NULL) {
        char handStr[BUFFER_SIZE];
        strncpy(handStr, (char*)current->data, BUFFER_SIZE);
        char *cards = strtok(handStr, " ");
        int bid = strtol(strtok(NULL, ""), NULL, 10);
        hands[nodeNum] = initHand(cards, bid);
        // printf("%s: Strength = %d\n", cards, hands[nodeNum]->strength);
        current = current->next;
        nodeNum++;
    }
    printf("Part 1: \n");
}

void part2(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
    // llist *ll = getInputFile("assets/2023/Day7.txt");
    llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}

