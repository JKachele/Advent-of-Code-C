/*************************************************
 *File----------Day5.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Dec 11, 2024 16:53:29 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

typedef struct {
        int before;
        int after;
} rule;

bool isValidPage(rule rules[], int numRules, int first, int last) {
        for (int i = 0; i < numRules; i++) {
                rule r = rules[i];
                if (rules[i].before == last && rules[i].after == first)
                        return false;
        }
        return true;
}

bool isValidUpdate(int update[], int numPages, rule rules[], int numRules) {
        for (int i = 0; i < numPages; i++) {
                if (update[i] == -1)
                        break;
                for (int j = i + 1; j < numPages; j++) {
                        if (update[j] == -1)
                                break;
                        if (!isValidPage(rules, numRules, update[i], update[j]))
                                return false;
                }
        }
        return true;
}

void swapPages(int update[], int numPages, rule rules[], int numRules) {
        for (int i = 0; i < numPages; i++) {
                if (update[i] == -1)
                        break;
                for (int j = i + 1; j < numPages; j++) {
                        if (update[j] == -1)
                                break;
                        if (!isValidPage(rules, numRules, update[i], update[j])) {
                                int temp = update[i];
                                update[i] = update[j];
                                update[j] = temp;
                        }
                }
        }
}

int getMiddle(int update[], int numPages) {
        for (int i = 0; i < numPages; i++) {
                if (update[i] == -1)
                        return update[(i - 1) / 2];
        }
        return -1;
}

void part1(llist *ll) {
        llNode *current = ll->head;

        // Get rules
        rule rules[ll->length];
        int numRules = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (str[0] == '\0') {
                        current = current->next;
                        break;
                }
                int before = strtol(strtok(str, "|"), (char**)NULL, 10);
                int after = strtol(strtok(NULL, ""), (char**)NULL, 10);
                rules[numRules] = (rule){before, after};
                current = current->next;
                numRules++;
        }

        int numUpdates = ll->length - (numRules + 1);
        const int maxPages = 32;
        int updates[numUpdates][maxPages];
        int updateNum = 0;
        while (current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                int pageNum = 0;
                char *pageStr = strtok(str, ",");
                while (pageStr != NULL) {
                        int page = strtol(pageStr, (char**)NULL, 10);
                        updates[updateNum][pageNum] = page;
                        pageStr = strtok(NULL, ",");
                        pageNum++;
                }
                updates[updateNum][pageNum] = -1;

                current = current->next;
                updateNum++;
        }

        int middleNums = 0;
        for (int i = 0; i < numUpdates; i++) {
                if (isValidUpdate(updates[i], maxPages, rules, numRules))
                        middleNums += getMiddle(updates[i], maxPages);
        }

        printf("\nPart 1: Valid Middle Number Sum: %d\n", middleNums);
}

void part2(llist *ll) {
        llNode *current = ll->head;

        // Get rules
        rule rules[ll->length];
        int numRules = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (str[0] == '\0') {
                        current = current->next;
                        break;
                }
                int before = strtol(strtok(str, "|"), (char**)NULL, 10);
                int after = strtol(strtok(NULL, ""), (char**)NULL, 10);
                rules[numRules] = (rule){before, after};
                current = current->next;
                numRules++;
        }

        int numUpdates = ll->length - (numRules + 1);
        const int maxPages = 32;
        int updates[numUpdates][maxPages];
        int updateNum = 0;
        while (current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                int pageNum = 0;
                char *pageStr = strtok(str, ",");
                while (pageStr != NULL) {
                        int page = strtol(pageStr, (char**)NULL, 10);
                        updates[updateNum][pageNum] = page;
                        pageStr = strtok(NULL, ",");
                        pageNum++;
                }
                updates[updateNum][pageNum] = -1;

                current = current->next;
                updateNum++;
        }

        int middleNums = 0;
        for (int i = 0; i < numUpdates; i++) {
                if (isValidUpdate(updates[i], maxPages, rules, numRules))
                        continue;
                while (!isValidUpdate(updates[i], maxPages, rules, numRules)) {
                        swapPages(updates[i], maxPages, rules, numRules);
                }
                middleNums += getMiddle(updates[i], maxPages);
        }

        printf("\nPart 2: Sorted Middle Number Sum: %d\n", middleNums);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day5.txt");
        else
                ll = getInputFile("assets/2024/Day5.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


