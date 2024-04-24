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

void printHands(hand *h[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%d:\tStrength: %d\t\t", i, h[i]->strength);
        for (int j = 0; j < 5; j++) {
            printf("%d\t", h[i]->cards[j]);
        }
        printf("\tBid: %d", h[i]->bid);
        printf("\n");
    }
}

void swap(hand **x, hand **y) {
    hand *temp = *x;
    *x = *y;
    *y = temp;
}

// Compare 2 hands. True if hand1 > hand2
bool compareHands(hand *hand1, hand *hand2) {
    if (hand1->strength != hand2->strength) {
        return hand1->strength > hand2->strength;
    }
    // If strengths are equal, loop through cards until a diff is found
    for (int i = 0; i < 5; i++) {
        if (hand1->cards[i] != hand2->cards[i]) {
            return hand1->cards[i] > hand2->cards[i];
        }
    }
    // If hands are equal return false
    return false;
}

int partition(hand *arr[], int low, int high) {
    hand *pivotValue = arr[high];
    int i = low;
    for (int j = low; j < high; j++) {
        if (!compareHands(arr[j], pivotValue)) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

void quicksortRecursion(hand *arr[], int low, int high) {
    if (low >= high) { return; }
    int pivotIndex = partition(arr, low, high);
    quicksortRecursion(arr, low, pivotIndex-1);
    quicksortRecursion(arr, pivotIndex+1, high);
}

void quicksort(hand *arr[], int length) {
    quicksortRecursion(arr, 0, length-1);
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

int getStrength2(int cards[5]) {
    // Count Max similar cards
    int maxPair = 0;
    int secondPair = 0;
    int maxvalue = 0;
    int numJs = 0;
    for (int i = 0; i < 4; i++) {
        if (cards[i] == 0) {
            numJs++;
            continue;
        }
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
    if (cards[4] == 0) { numJs++; }

    maxPair += numJs;

    if (maxPair == 5) { return FiveKind; }
    if (maxPair == 4) { return FourKind; }
    if (maxPair == 3 && secondPair == 2) { return Full; }
    if (maxPair == 3) { return ThreeKind; }
    if (maxPair == 2 && secondPair == 2) { return TwoPair; }
    if (maxPair == 2) { return OnePair; }
    return High;
}

hand *initHand(char *cards, int bid) {
    const char cardValue[13] = {
        '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };
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

hand *initHand2(char *cards, int bid) {
    hand *curHand = malloc(sizeof(hand));
    for (int i = 0; i < 5; i++) {
        // If card value is numeric
        if (cards[i] >= 50 && cards[i] <= 57) {
            curHand->cards[i] = cards[i] - 49;
        } else {
            switch (cards[i]) {
                case 'J':
                    curHand->cards[i] = 0;
                    break;
                case 'T':
                    curHand->cards[i] = 9;
                    break;
                case 'Q':
                    curHand->cards[i] = 10;
                    break;
                case 'K':
                    curHand->cards[i] = 11;
                    break;
                case 'A':
                    curHand->cards[i] = 12;
                    break;
            }
        }
    }
    curHand->bid = bid;
    curHand->strength = getStrength2(curHand->cards);

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

    quicksort(hands, ll->length);
    // printHands(hands, ll->length);

    long totalWinnings = 0;
    for (int i = 0; i < ll->length; i++) {
        int winnings = hands[i]->bid * (i+1);
        totalWinnings += winnings;
    }

    printf("Part 1: Total Winnings = %ld\n", totalWinnings);
}

void part2(llist *ll) {
    hand *hands[ll->length];
    llNode *current = ll->head;
    int nodeNum = 0;
    while(current != NULL) {
        char handStr[BUFFER_SIZE];
        strncpy(handStr, (char*)current->data, BUFFER_SIZE);
        char *cards = strtok(handStr, " ");
        int bid = strtol(strtok(NULL, ""), NULL, 10);
        hands[nodeNum] = initHand2(cards, bid);
        // printf("%s: Strength = %d\n", cards, hands[nodeNum]->strength);
        current = current->next;
        nodeNum++;
    }

    quicksort(hands, ll->length);
    // printHands(hands, ll->length);

    long totalWinnings = 0;
    for (int i = 0; i < ll->length; i++) {
        int winnings = hands[i]->bid * (i+1);
        totalWinnings += winnings;
    }

    printf("Part 2: Total Winnings = %ld\n", totalWinnings);
}

int main(int argc, char *argv[]) {
    llist *ll = getInputFile("assets/2023/Day7.txt");
    // llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}

