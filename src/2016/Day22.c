/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 04, 2026 11:47:24 EST
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
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

struct node {
        ivec2 pos;
        int total;
        int used;
};

struct input {
        int numNodes;
        struct node *nodes;
};

ivec2 getNodePos(char *str) {
        ivec2 pos;
        char *end;
        pos.x = strtol(str+16, &end, 10);
        pos.y = strtol(end+2, NULL, 10);
        return pos;
}

void part1(struct input *input) {
        int numNodes = input->numNodes;
        struct node *nodes = input->nodes;

        int viable = 0;
        for (int i = 0; i < numNodes; i++) {
                struct node n1 = nodes[i];
                int avail = n1.total - n1.used;
                for (int j = 0; j < numNodes; j++) {
                        if (i == j) { continue; }
                        struct node n2 = nodes[j];
                        if (n2.used != 0 && n2.used <= avail) {
                                viable++;
                        }
                }
        }

        printf("Part 1: Viable Pairs = %d\n\n", viable);
}

void part2(struct input *input) {
        int numNodes = input->numNodes;
        struct node *nodes = input->nodes;

        // Get grid size
        ivec2 size = nodes[numNodes-1].pos;
        size.x++;
        size.y++;

        // a=used, b=total
        ivec2 (*grid)[size.x] = (ivec2(*)[size.x])malloc(size.x * size.y * sizeof(ivec2));
        for (int i = 0; i < numNodes; i++) {
                ivec2 pos = nodes[i].pos;
                grid[pos.y][pos.x].a = nodes[i].used;
                grid[pos.y][pos.x].b = nodes[i].total;
        }

        // for (int y = 0; y < size.y; y++) {
        //         for (int x = 0; x < size.x; x++) {
        //                 printf("%3d ", grid[y][x].a);
        //         }
        //         printf("\n");
        // }

        printf("Part 2: Solved by hand: 205\n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numNodes = ll->length - 2;
        input.nodes = malloc(input.numNodes * sizeof(struct node));

        int i = 0;
        for (llNode *cur = ll->head->next->next; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                struct node node;
                char *posStr = strtok(str, " ");
                node.pos = getNodePos(posStr);

                char *total = strtok(NULL, " ");
                node.total = strtol(total, NULL, 10);

                char *used = strtok(NULL, " ");
                node.used = strtol(used, NULL, 10);

                input.nodes[i++] = node;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day22.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

