/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 14:54:58 UTC
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
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef struct llnode {
        u32 num;
        struct llnode *next;
} llnode;

struct input {
        llnode *head;
        llnode *head2;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printCups(u32 *cups, u32 head) {
        u32 cur = head;
        do {
                printf("%u ", cur);
                cur = cups[cur];
        } while (cur != head);
        printf("\n");
}

void llistPrint(llnode *head) {
        llnode *cur = head;
        do {
                printf("%u ", cur->num);
                cur = cur->next;
        } while (cur != head);
        printf("\n");
}

void llistAdd(llnode *head, u32 num) {
        llnode *cur = head;
        do {
                if (cur->next == head) {
                        llnode *new = malloc(sizeof(llnode));
                        new->num = num;
                        new->next = head;
                        cur->next = new;
                        break;
                }
                cur = cur->next;
        } while (cur != head);
}

void llistInsertAfter(llnode *head, u32 dest, u32 num) {
        llnode *cur = head;
        do {
                if (cur->num == dest) {
                        llnode *new = malloc(sizeof(llnode));
                        new->num = num;
                        new->next = cur->next;
                        cur->next = new;
                        break;
                }
                cur = cur->next;
        } while (cur != head);
}

void llistRemove(llnode *head, u32 num) {
        llnode *cur = head;
        do {
                if (cur->next->num == num) {
                        llnode *next = cur->next;
                        cur->next = cur->next->next;
                        free(next);
                        break;
                }
                cur = cur->next;
        } while (cur != head);
}

u64 allCupsAfter1(llnode *head) {
        u64 num = 0;
        bool found1 = false;
        llnode *cur = head;
        do {
                if (!found1) {
                        if (cur->num == 1)
                                found1 = true;
                } else {
                        num *= 10;
                        num += cur->num;
                }
                cur = cur->next;
        } while (!(found1 && cur->num == 1));
        return num;
}

u32 llistToCupArr(llnode *head, u32 *cups) {
        llnode *curNode = head;
        u32 curNum = head->num;
        do {
                cups[curNode->num] = curNode->next->num;
                curNum = curNode->num;
                curNode = curNode->next;
        } while (curNode != head);
        return curNum;
}

void crabMove(llnode **head) {
        // Remove the 3 cups after the current cup
        u32 removed[3] = {0};
        u32 curNum = (*head)->num;
        for (int i = 0; i < 3; i++) {
                removed[i] = (*head)->next->num;
                llistRemove(*head, removed[i]);
        }

        // Find destination cup
        u32 dest = curNum - 1;
        for (;;) {
                if (dest <= 0)
                        dest = 9;
                bool inRemoved = false;
                for (int i = 0; i < 3; i++) {
                        if (removed[i] == dest) {
                                inRemoved = true;
                                break;
                        }
                }
                if (!inRemoved)
                        break;
                dest--;
        }

        // Add the removed cups after the destination cup
        for (int i = 2; i >= 0; i--) {
                llistInsertAfter(*head, dest, removed[i]);
        }

        // Set head to next in line
        *head = (*head)->next;
}

void crabMove2(u32 *cups, u32 head, int maxNum) {
        // Remove 3 cups after head
        u32 removed[3] = {0};
        for (int i = 0; i < 3; i++) {
                removed[i] = cups[head];
                cups[head] = cups[removed[i]];
                cups[removed[i]] = 0;
        }

        // Find destination cup
        u32 dest = head - 1;
        for (;;) {
                if (dest <= 0)
                        dest = maxNum;
                if (cups[dest] != 0)
                        break;
                dest--;
        }

        // Add the removed cups after the destination cup
        u32 next = cups[dest];
        u32 cur = dest;
        for (int i = 0; i < 3; i++) {
                cups[cur] = removed[i];
                cur = removed[i];
        }
        cups[removed[2]] = next;
}

void part1(struct input input) {
        const int NumMoves = 100;
        llnode *head = input.head;
        // llistPrint(head);

        for (int i = 0; i < NumMoves; i++) {
                crabMove(&head);
                // llistPrint(head);
        }
        // llistPrint(head);

        u64 cups = allCupsAfter1(head);

        printf("Part 1: %lu\n\n", cups);
}

void part2(struct input input) {
        const int NumMoves = 10000000;
        const int MaxCup = 1000000;

        // Convert llist to array
        u32 *cups = calloc(MaxCup + 1, sizeof(u32));
        u32 curNum = llistToCupArr(input.head2, cups);
        u32 head = input.head2->num;
        // printCups(cups, head);

        // Add the remaining cups after the last cup
        for (int i = 10; i <= MaxCup; i++) {
                cups[curNum] = i;
                cups[i] = head;
                curNum = i;
        }
        // printCups(cups, head);

        u32 cur = head;
        for (int i = 0; i < NumMoves; i++) {
                crabMove2(cups, cur, MaxCup);
                cur = cups[cur];
        }
        // printCups(cups, head);

        u64 cupAns = (u64)cups[1] * cups[cups[1]];

        printf("Part 2: %ld\n", cupAns);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        input.head = malloc(sizeof(llnode));
        input.head->num = str[0] - '0';
        input.head->next = input.head;

        input.head2 = malloc(sizeof(llnode));
        input.head2->num = str[0] - '0';
        input.head2->next = input.head2;

        int i = 1;
        while (str[i] != '\0') {
                llistAdd(input.head, str[i] - '0');
                llistAdd(input.head2, str[i] - '0');
                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day23.txt";
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

