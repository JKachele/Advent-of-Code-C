/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 14, 2026 17:23:11 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 16384

typedef enum stype {
        STACK,
        CUT,
        DEAL
} stype;

typedef struct shuffle {
        stype type;
        int value;
} shuffle;
typedef tll(shuffle) tllshuffle;

struct input {
        tllshuffle shuffles;
};

static int DeckSize = 10007;

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printShuffle(shuffle s) {
        switch (s.type) {
        case STACK:
                printf("Stack\n");
                break;
        case CUT:
                printf("Cut %d\n", s.value);
                break;
        case DEAL:
                printf("Deal %d\n", s.value);
        }
}

void printShuffles(tllshuffle s) {
        if (!Debug) return;
        tll_foreach(s, it)
                printShuffle(it->item);
        printf("\n");
}

void printDeck(u32 *deck) {
        if (!Debug) return;
        for (int i = 0; i < DeckSize; i++)
                printf("%d ", deck[i]);
        printf("\n");
}

// Modulo opperation that handles negative numbers;
int64 mod(int64 a, int64 m) {
        int64 ans = a % m;
        ans = (ans + m) % m;
        return ans;
}

int64 multMod(int64 a, int64 b, int64 m) {
        return ((__int128_t)a * b) % m;
}

int64 powMod(int64 x, int64 n, int64 m) {
        int64 res = 1;
        while (n > 0) {
                if ((n & 1) == 1) {
                        res = multMod(res, x, m);
                        n--;
                } else {
                        x = multMod(x, x, m);
                        n /= 2;
                }
        }
        return res;
}

void stack(u32 *deck) {
        u32 *stack = malloc(DeckSize * sizeof(u32));
        memcpy(stack, deck, DeckSize * sizeof(u32));
        for (int i = 0; i < DeckSize; i++)
                deck[i] = stack[DeckSize - 1 - i];
}

void cut(u32 *deck, int value) {
        u32 *tempDeck = malloc(DeckSize * sizeof(u32));
        memcpy(tempDeck, deck, DeckSize * sizeof(u32));

        u32 *cut = malloc(abs(value) * sizeof(u32));
        if (value > 0) {
                int uncutSize = DeckSize - value;
                memcpy(cut, deck, value * sizeof(u32));
                memcpy(deck, tempDeck + value, uncutSize * sizeof(u32));
                memcpy(deck + uncutSize, cut, value * sizeof(u32));
        } else {
                value *= -1;
                int uncutSize = DeckSize - value;
                memcpy(cut, deck + uncutSize, value * sizeof(u32));
                memcpy(deck + value, tempDeck, uncutSize * sizeof(u32));
                memcpy(deck, cut, value * sizeof(u32));
        }
}

void deal(u32 *deck, int value) {
        u32 *tempDeck = malloc(DeckSize * sizeof(u32));
        memcpy(tempDeck, deck, DeckSize * sizeof(u32));

        int pos = 0;
        for (int i = 0; i < DeckSize; i++) {
                deck[pos] = tempDeck[i];
                pos = (pos + value) % DeckSize;
        }
}

void part1(struct input *input) {
        printShuffles(input->shuffles);

        u32 *deck = malloc(DeckSize * sizeof(u32));
        for(int i = 0; i < DeckSize; i++)
                deck[i] = i;
        printDeck(deck);
        tll_foreach(input->shuffles, it) {
                shuffle s = it->item;
                switch (s.type) {
                case STACK:
                        stack(deck);
                        break;
                case CUT:
                        cut(deck, s.value);
                        break;
                case DEAL:
                        deal(deck, s.value);
                        break;
                }
                // u32 inc = ((deck[1] + DeckSize) - deck[0]) % DeckSize;
                // printf("%u, %u: ", deck[0], inc);
                // printShuffle(s);
        }
        printDeck(deck);

        int pos = -1;
        for (int i = 0; i < DeckSize; i++) {
                if (deck[i] == 2019) {
                        pos = i;
                        break;
                }
        }

        printf("Part 1: %d\n\n", pos);
}

void part2(struct input *input) {
        const int64 DeckSize = 119315717514047;
        const int64 Shuffles = 101741582076661;

        // Represent deck as an offset(Top Card) and an increment(2nd card - top card)
        int64 offset = 0;
        int64 increment = 1;

        // Complete shuffle process once
        tll_foreach(input->shuffles, it) {
                shuffle s = it->item;
                switch (s.type) {
                case STACK: // Reverses the list
                        increment *= -1;
                        increment = (increment + DeckSize) % DeckSize; // Correct for negative
                        offset = (offset + increment) % DeckSize;
                        break;
                case CUT: // Shifts the list by x
                        offset = mod(offset + multMod(increment, s.value, DeckSize), DeckSize);
                        break;
                case DEAL:
                        // New increment uses the modular inverse of the value
                        // Use Fermat's little theorem to find the modular inverse
                        increment = multMod(increment, powMod(s.value, DeckSize - 2, DeckSize),
                                        DeckSize);
                        break;
                }
                // printf("%ld, %ld: ", offset, increment);
                // printShuffle(s);
        }

        // Values of offset and increment after 1 rouns is used to get the values after n rounds
        int64 offsetDiff = offset;
        int64 incrementMul = increment;
        // printf("Offset: %ld, Increment: %ld\n", offset, increment);

        // Now we have the offset and increment after a single round of shuffles,
        // To get the next round, Increment is multipled by a constant value and
        // the offset is adjusted based off the old increment and the offset after a single shuffle

        // The increment of the nth round is just repeated multiplications aka Exponentiation!
        increment = powMod(incrementMul, Shuffles, DeckSize);

        // The offset is the geometric series of the increment multiplier
        // offset = offsetDiff * ((1 - (incrementMul ^ Shuffles)) / (1 - incrementMul))
        int64 numerator = mod(1 - powMod(incrementMul, Shuffles, DeckSize), DeckSize);
        int64 denominator = powMod(mod(1 - incrementMul, DeckSize), DeckSize - 2, DeckSize);
        int64 quotient = multMod(numerator, denominator, DeckSize);
        offset = multMod(offsetDiff, quotient, DeckSize);
        // printf("Offset: %ld, Increment: %ld\n", offset, increment);

        // Get the 2020th number in the deck
        int64 ans = (offset + ((increment * 2020) % DeckSize)) % DeckSize;

        printf("Part 2: %ld\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                shuffle s = {0};

                if (str[0] == 'c') { // cut x
                        s.type = CUT;
                        s.value = strtol(str + 4, NULL, 10);
                } else if (str[5] == 'i') { // deal into new stack
                        s.type = STACK;
                } else { // deal with increment x
                        s.type = DEAL;
                        s.value = strtol(str + 20, NULL, 10);
                }
                tll_push_back(input.shuffles, s);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
                DeckSize = 11;
        } else {
                char *file = "assets/inputs/2019/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

