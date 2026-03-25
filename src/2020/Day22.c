/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 13:24:09 UTC
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

typedef tll(u8) tllu8;

typedef struct gamestate {
        tllu8 deck1;
        tllu8 deck2;
} gamestate;
typedef tll(gamestate) tllgamestate;

struct input {
        tllu8 deck1;
        tllu8 deck2;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printDeck(tllu8 deck) {
        tll_foreach(deck, it) {
                printf("%u ", it->item);
        }
        printf("\n");
}

tllu8 copyDeck(tllu8 deck, int n) {
        tllu8 copy = tll_init();
        int i = 0;
        tll_foreach(deck, it) {
                tll_push_back(copy, it->item);
                i++;
                if (i == n) break;
        }
        return copy;
}

bool deckMatch(tllu8 deck1, tllu8 deck2) {
        if (tll_length(deck1) != tll_length(deck2))
                return false;

        tllu8 deck1c = copyDeck(deck1, 0);
        tllu8 deck2c = copyDeck(deck2, 0);
        bool match = true;
        while (tll_length(deck1c) > 0 && tll_length(deck2c) > 0) {
                u8 card1 = tll_pop_front(deck1c);
                u8 card2 = tll_pop_front(deck2c);
                if (card1 != card2) {
                        match = false;
                        break;
                }
        }
        tll_free(deck1c);
        tll_free(deck2c);
        return match;
}

bool checkGameState(tllgamestate *state, tllu8 deck1, tllu8 deck2) {
        tll_foreach(*state, it) {
                tllu8 stateD1 = it->item.deck1;
                tllu8 stateD2 = it->item.deck2;
                if (deckMatch(stateD1, deck1) && deckMatch(stateD2, deck2))
                        return false;
        }
        gamestate newstate = {0};
        newstate.deck1 = copyDeck(deck1, 0);
        newstate.deck2 = copyDeck(deck2, 0);
        tll_push_back(*state, newstate);
        return true;
}

u8 highestCard(tllu8 deck) {
        u8 max = 0;
        tll_foreach(deck, it) {
                if (it->item > max)
                        max = it->item;
        }
        return max;
}

bool recursiveCombat(tllu8 *deck1, tllu8 *deck2, int depth) {
        // Record all game states to check for infinite loops
        tllgamestate state = tll_init();

        // If isnt main game, can use an optimized approach to avoid playing excessive games
        // If player 1 has the highest value card, and that value is higher then the total cards
        // in the game, player 1 is guaranteed to win
        if (depth > 0) {
                u8 max1 = highestCard(*deck1);
                u8 max2 = highestCard(*deck2);
                if (max1 > max2 && max1 > tll_length(*deck1) + tll_length(*deck2))
                        return true;
        }

        bool gameStateWin = false;
        while (tll_length(*deck1) > 0 && tll_length(*deck2) > 0) {
                // If game state has been seen before, end the game with player 1 win
                if (!checkGameState(&state, *deck1, *deck2)) {
                        gameStateWin = true;
                        break;
                }

                u8 card1 = tll_pop_front(*deck1);
                u8 card2 = tll_pop_front(*deck2);

                bool player1Win = card1 > card2;

                if (tll_length(*deck1) >= card1 && tll_length(*deck2) >= card2) {
                        tllu8 newDeck1 = copyDeck(*deck1, card1);
                        tllu8 newDeck2 = copyDeck(*deck2, card2);
                        player1Win = recursiveCombat(&newDeck1, &newDeck2, depth+1);
                        tll_free(newDeck1);
                        tll_free(newDeck2);
                }
                
                if (player1Win) {
                        tll_push_back(*deck1, card1);
                        tll_push_back(*deck1, card2);
                } else {
                        tll_push_back(*deck2, card2);
                        tll_push_back(*deck2, card1);
                }
        }

        // Free game state list
        tll_foreach(state, it) {
                tll_free(it->item.deck1);
                tll_free(it->item.deck2);
                tll_remove(state, it);
        }

        return (gameStateWin || tll_length(*deck1) > 0);
}

void part1(struct input input) {
        tllu8 deck1 = copyDeck(input.deck1, 0);
        tllu8 deck2 = copyDeck(input.deck2, 0);

        while (tll_length(deck1) > 0 && tll_length(deck2) > 0) {
                u8 card1 = tll_pop_front(deck1);
                u8 card2 = tll_pop_front(deck2);
                
                if (card1 > card2) {
                        tll_push_back(deck1, card1);
                        tll_push_back(deck1, card2);
                } else {
                        tll_push_back(deck2, card2);
                        tll_push_back(deck2, card1);
                }
        }

        tllu8 winner = (tll_length(deck1) > 0) ? deck1 : deck2;
        int multiplier = tll_length(winner);
        u64 score = 0;
        tll_foreach(winner, it) {
                score += multiplier * it->item;
                multiplier--;
        }

        tll_free(deck1);
        tll_free(deck2);

        printf("Part 1: %lu\n\n", score);
}

void part2(struct input input) {
        tllu8 deck1 = copyDeck(input.deck1, 0);
        tllu8 deck2 = copyDeck(input.deck2, 0);

        recursiveCombat(&deck1, &deck2, 0);

        tllu8 winner = (tll_length(deck1) > 0) ? deck1 : deck2;
        int multiplier = tll_length(winner);
        u64 score = 0;
        tll_foreach(winner, it) {
                score += multiplier * it->item;
                multiplier--;
        }

        tll_free(deck1);
        tll_free(deck2);

        printf("Part 2: %lu\n", score);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        tllu8 *deck = &input.deck1;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (str[0] == 'P')
                        continue;
                if (strlen(str) == 0) {
                        deck = &input.deck2;
                        continue;
                }
                u8 num = strtol(str, NULL, 10);
                tll_push_back(*deck, num);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day22.txt";
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

