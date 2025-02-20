/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Feb 20, 2025 14:59:43 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define MAX_VERT_ID 2525

// Computers are defined as a 2 char string.
// Convert into int A:0 - Z:25 and concat
typedef struct {
        int16 a;
        int16 b;
} link;

typedef tll(int16) tllint16;
typedef tll(tllint16) tllclique;

char *idToStr(int id) {
        char *idStr = malloc(3);
        idStr[0] = (id / 100) + 'a';
        idStr[1] = (id % 100) + 'a';
        idStr[2] = '\0';
        return idStr;
}


/*
 * algorithm BronKerbosch2(R, P, X) is
 *     if P and X are both empty then
 *         report R as a maximal clique
 *     choose a pivot vertex u in P ⋃ X
 *     for each vertex v in P \ N(u) do
 *         BronKerbosch2(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
 *         P := P \ {v}
 *         X := X ⋃ {v}
 */
void BK(tllclique *cliques, tllint16 nodes[],
                tllint16 clique, tllint16 avail, tllint16 exclude) {
        if (tll_length(avail) == 0 && tll_length(exclude) == 0)
                tll_push_back(*cliques, clique);

        int16 pivot;
        if (tll_length(avail) > 0)
                pivot = tll_pop_front(avail);
        else
                pivot = tll_pop_front(exclude);

}

void addLink(tllint16 *verts, tllint16 vertAdj[MAX_VERT_ID], link link) {
        bool foundA = false;
        bool foundB = false;
        tll_foreach(*verts, it) {
                int16 node = it->item;
                if (node == link.a) {
                        tll_push_back(vertAdj[node], link.b);
                        foundA = true;
                } else if (node == link.b) {
                        tll_push_back(vertAdj[node], link.a);
                        foundB = true;
                }
        }
        if (!foundA) {
                tll_push_back(*verts, link.a);
                tll_push_back(vertAdj[link.a], link.b);
        }
        if (!foundB) {
                tll_push_back(*verts, link.b);
                tll_push_back(vertAdj[link.b], link.a);
        }
}

int32 findCliques(link links[], int16 numLinks) {
        tllint16 verts = tll_init();
        tllint16 vertAdj[MAX_VERT_ID];
        for (int i = 0; i < MAX_VERT_ID; i++) {
                vertAdj[i] = (tllint16)tll_init();
        }

        for (int i = 0; i < numLinks; i++) {
                addLink(&verts, vertAdj, links[i]);
        }

        tllclique cliques = tll_init();

        return 0;
}

void part1(llist *ll) {
        int16 numLinks = ll->length;
        link links[numLinks];

        llNode *current = ll->head;
        for (int i = 0; i < numLinks; i++) {
                char *str = (char*)current->data;

                // AB-CD
                links[i].a = ((str[0] - 'a') * 100) + (str[1] - 'a');
                links[i].b = ((str[3] - 'a') * 100) + (str[4] - 'a');

                current = current->next;
        }

        findCliques(links, numLinks);

        printf("Part 1: \n\n");
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day23.txt");
        } else {
                ll = getInputFile("assets/2024/Day23.txt");
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

