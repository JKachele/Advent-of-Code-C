/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Aug 18, 2025 21:10:14 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define KEY 811589153

typedef struct {
        int32 index;
        int64 value;
} fileNum;

typedef tll(fileNum) file;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printFile(file file) {
        printf("[");
        tll_foreach(file, it) {
                printf("%ld, ", it->item.value);
        }
        printf("\b\b]\n");
}

void printFileIndex(file file, int32 indexIn, int32 indexNum, int32 pad) {
        int32 index = 0;
        printf("%d: ", indexNum);
        tll_foreach(file, it) {
                if (index - indexIn >= (pad * -1) && index - indexIn <= pad) {
                        if (index == indexIn) {
                                printf("*%ld* ", it->item.value);
                        } else {
                                printf("%ld ", it->item.value);
                        }
                }
                index++;
        }
        printf("\n");
}

int32 getMoveIndex(int32 lm, int32 index, int64 move) {
        int64 moveIndex = index + move;
        if (moveIndex < 0) {
                moveIndex = ((moveIndex % lm) + lm) % lm;
        } else {
                moveIndex = moveIndex % lm;
        }
        return (int32)moveIndex;
}

void insertBefore(file *file, fileNum value, int32 index) {
        int32 i = 0;
        tll_foreach(*file, it) {
                if (i == index) {
                        tll_insert_before(*file, it, value);
                        return;
                }
                i++;
        }
}

void mix(file *file) {
        int len = tll_length(*file);
        int lm = len - 1; // Length with number removed
        for (int i=0; i<len; i++) {
                int32 index = 0;
                fileNum num;
                tll_foreach(*file, it) {
                        if (it->item.index != i) {
                                index++;
                                continue;
                        }
                        num = it->item;
                        tll_remove(*file, it);
                        break;
                }
                int32 moveIndex = getMoveIndex(lm, index, num.value);
                insertBefore(file, num, moveIndex);
                // printf("%ld: %d\n    ", num.value, moveIndex);
                // printFile(*file);
        }
}

void part1(llist *ll) {
        file file = tll_init();
        int32 len = ll->length;

        llNode *current = ll->head;
        int32 index = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                fileNum num;
                num.index = index;
                num.value = strtol(str, (char**)NULL, 10);
                tll_push_back(file, num);

                current = current->next;
                index++;
        }

        // printFile(file);
        mix(&file);
        // printFile(file);
        index = 0;
        tll_foreach(file, it) {
                if (it->item.value == 0)
                        break;
                index++;
        }

        int32 index0 = index;
        int32 index1k = (index0 + 1000) % len;
        int32 index2k = (index0 + 2000) % len;
        int32 index3k = (index0 + 3000) % len;
        // printf("%lu\n", tll_length(file));
        // printf("%d, %d, %d, %d\n\n", index0, index1k, index2k, index3k);
        printFileIndex(file, index1k, 1000, 0);
        printFileIndex(file, index2k, 2000, 0);
        printFileIndex(file, index3k, 3000, 0);

        int32 sum = 0;
        index = 0;
        tll_foreach(file, it) {
                if (index == index1k || index == index2k || index == index3k)
                        sum += it->item.value;
                index++;
        }

        printf("Part 1: Grove Coordinates = %d\n\n", sum);
}

void part2(llist *ll) {
        file file = tll_init();
        int32 len = ll->length;

        llNode *current = ll->head;
        int32 index = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                fileNum num;
                num.index = index;
                num.value = strtol(str, (char**)NULL, 10) * KEY;
                tll_push_back(file, num);

                current = current->next;
                index++;
        }

        // printFile(file);
        for (int i=0; i<10; i++) {
                mix(&file);
                // printf("%d: \n    ", i + 1);
                // printFile(file);
        }
        index = 0;
        tll_foreach(file, it) {
                if (it->item.value == 0)
                        break;
                index++;
        }

        int32 index0 = index;
        int32 index1k = (index0 + 1000) % len;
        int32 index2k = (index0 + 2000) % len;
        int32 index3k = (index0 + 3000) % len;
        // printf("%lu\n", tll_length(file));
        // printf("%d, %d, %d, %d\n\n", index0, index1k, index2k, index3k);
        printFileIndex(file, index1k, 1000, 0);
        printFileIndex(file, index2k, 2000, 0);
        printFileIndex(file, index3k, 3000, 0);

        int64 sum = 0;
        index = 0;
        tll_foreach(file, it) {
                if (index == index1k || index == index2k || index == index3k)
                        sum += it->item.value;
                index++;
        }

        printf("Part 2: Grove Coordinates = %ld\n\n", sum);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day20.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day20.txt");
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

