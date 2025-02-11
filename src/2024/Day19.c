/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Feb 10, 2025 09:49:20 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
// #include "../util/util.h"

#define MAX_PATTERN_LEN 64

typedef tll(char*) strtll;

void printStrtll(strtll arr) {
        tll_foreach(arr, it) {
                printf("%s\n", it->item);
        }
}

bool isPossible(char *goal, strtll patterns, int8_t memo[MAX_PATTERN_LEN]) {
        int len = strlen(goal);
        if (len == 0) return true;
        if(memo[len] == -1) return false;
        if(memo[len] == 1) return true;

        tll_foreach(patterns, it) {
                char *patt = it->item;
                if (strlen(goal) < strlen(patt))
                        continue;
                if (memcmp(goal, patt, strlen(patt)) == 0) {
                        if(isPossible(goal + strlen(patt), patterns, memo)) {
                                memo[len] = 1;
                                return true;
                        }
                }
        }
        memo[len] = -1;
        return false;
}

int64_t possibilities(char *goal, strtll patterns, int64_t memo[MAX_PATTERN_LEN]) {
        int len = strlen(goal);
        if (len == 0) return 1;
        if(memo[len] == -1) return 0;
        if(memo[len] > 0) return memo[len];

        int64_t numPossible = 0;
        tll_foreach(patterns, it) {
                char *patt = it->item;
                if (strlen(goal) < strlen(patt))
                        continue;
                if (memcmp(goal, patt, strlen(patt)) == 0) {
                        numPossible += possibilities(goal + strlen(patt), patterns, memo);
                       
                }
        }
        if (numPossible == 0)
                memo[len] = -1;
        else
                memo[len] = numPossible;
        return numPossible;
}

void part1(llist *ll) {
        strtll patterns = tll_init();
        char pattStr[BUFFER_SIZE];
        strncpy(pattStr, (char*)ll->head->data, BUFFER_SIZE);
        char *patt = strtok(pattStr, ",");
        while (patt != NULL) {
                if (patt[0] == ' ')
                        patt++;
                tll_push_back(patterns, patt);
                patt = strtok(NULL, ",");
        }
        // printStrtll(patterns);

        strtll goals = tll_init();
        llNode *current = ll->head->next->next;
        while(current != NULL) {
                char *in = (char*)current->data;
                char *str = malloc(strlen(in) + 1);
                strncpy(str, in, strlen(in) + 1);
                tll_push_back(goals, str);
                current = current->next;
        }
        // printStrtll(goals);
        
        int possible = 0;
        tll_foreach(goals, it) {
                int8_t memo[MAX_PATTERN_LEN] = {0};
                bool b = isPossible(it->item, patterns, memo);
                // printf("%s - ", it->item);
                // if (b)
                //         printf("Possible\n");
                // else
                //         printf("Not Possible\n");
                possible += b;
        }

        printf("Part 1: Num Possible: %d\n\n", possible);
        tll_free_and_free(goals, free);
}

void part2(llist *ll) {
        strtll patterns = tll_init();
        char pattStr[BUFFER_SIZE];
        strncpy(pattStr, (char*)ll->head->data, BUFFER_SIZE);
        char *patt = strtok(pattStr, ",");
        while (patt != NULL) {
                if (patt[0] == ' ')
                        patt++;
                tll_push_back(patterns, patt);
                patt = strtok(NULL, ",");
        }
        // printStrtll(patterns);

        strtll goals = tll_init();
        llNode *current = ll->head->next->next;
        while(current != NULL) {
                char *in = (char*)current->data;
                char *str = malloc(strlen(in) + 1);
                strncpy(str, in, strlen(in) + 1);
                tll_push_back(goals, str);
                current = current->next;
        }
        // printStrtll(goals);
        
        int64_t possible = 0;
        tll_foreach(goals, it) {
                int64_t memo[MAX_PATTERN_LEN] = {0};
                int64_t n = possibilities(it->item, patterns, memo);
                // printf("%s - %ld\n", it->item, n);
                possible += n;
        }

        printf("Part 2: Num Possible: %ld\n\n", possible);
        tll_free_and_free(goals, free);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day19.txt");
        } else {
                ll = getInputFile("assets/2024/Day19.txt");
        }
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
