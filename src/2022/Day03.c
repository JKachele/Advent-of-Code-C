/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 19, 2025 15:29:51 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"
#include "../util/quicksort.h"

void printBag(int *bag, int len) {
        printf("[");
        for (int i = 0; i < len - 1; i++) {
                printf("%d, ", bag[i]);
        }
        printf("%d]\n", bag[len - 1]);
}

int8 getCharVal(char c) {
        if (c >= 'a')
                return (c - 'a') + 1;
        else
                return (c - 'A') + 27;
}

void fillBag(int *bag, char *str, int32 len) {
        for (int i = 0; i < len; i++) {
                bag[i] = getCharVal(str[i]);
        }
}

int findCommonChar(int *bag1, int len1, int *bag2, int len2,
                int *bag3, int len3) {
        int p1 = 0;
        int p2 = 0;
        int p3 = 0;

        while (p1 < len1 && p2 < len2 && p3 < len3) {
                int n1 = bag1[p1];
                int n2 = bag2[p2];
                int n3 = bag3[p3];

                if (n1 == n2 && n1 == n3)
                        return n1;
                
                if (n1 <= n2 && n1 <= n3)
                        p1++;
                else if (n2 <= n3)
                        p2++;
                else
                        p3++;
        }

        return 0;
}

void part1(llist *ll) {
        int32 totalPri = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                int32 len = (int)strlen(str);
                char common = 0;
                for (int i = 0; i < len / 2; i++) {
                        bool stop = false;
                        for (int j = len / 2; j < len; j++) {
                                if (str[i] == str[j]) {
                                        common = str[i];
                                        stop = true;
                                        break;
                                }
                        }
                        if (stop) break;
                }
                int8 charVal = getCharVal(common);
                // printf("%s - %c - %d\n", str, common, charVal);
                totalPri += charVal;

                current = current->next;
        }
        printf("Part 1: Total Priority: %d\n\n", totalPri);
}

void part2(llist *ll) {
        int totalPri = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int32 len1 = strlen(str);
                int bag1[len1];
                fillBag(bag1, str, len1);
                quicksort(bag1, 0, len1 - 1);

                current = current->next;
                if (current == NULL) break;

                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int32 len2 = strlen(str);
                int bag2[len2];
                fillBag(bag2, str, len2);
                quicksort(bag2, 0, len2 - 1);

                current = current->next;
                if (current == NULL) break;

                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int32 len3 = strlen(str);
                int bag3[len3];
                fillBag(bag3, str, len3);
                quicksort(bag3, 0, len3 - 1);

                // printBag(bag1, len1);
                // printBag(bag2, len2);
                // printBag(bag3, len3);
                
                totalPri += findCommonChar(bag1, len1, bag2, len2, bag3, len3);

                current = current->next;
                if (current == NULL) break;
        }
        
        printf("Part 2: Total Badge Priority: %d\n", totalPri);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day03.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day03.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

