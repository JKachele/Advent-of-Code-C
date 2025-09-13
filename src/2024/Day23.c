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
#include "../util/quicksort.h"

#define MAX_VERT_ID 2525
#define COMP 't'

// Computers are defined as a 2 char string.
// Convert into int A:0 - Z:25 and concat
typedef struct {
        int16 a;
        int16 b;
} link;

typedef struct {
        int16 a;
        int16 b;
        int16 c;
} clique3;

typedef tll(int16) tllint16;
typedef tll(tllint16) tllclique;
typedef tll(clique3) tllclique3;

void idToStr(int id, char *str) {
        str[0] = (id / 100) + 'a';
        str[1] = (id % 100) + 'a';
        str[2] = '\0';
}

void printTllint(tllint16 arr) {
        printf("[ ");
        tll_foreach(arr, it) {
                char str[3];
                idToStr(it->item, str);
                printf ("%s ", str);
        }
        printf("]\n");
}

void printVerts(tllint16 verts, tllint16 vertAdj[]) {
        int i = 0;
        tll_foreach(verts, vert) {
                int16 vertId = vert->item;
                char str[3];
                idToStr(vertId, str);
                printf ("%s ", str);
                printf("%s: [ ", str);
                tll_foreach(vertAdj[vertId], adj) {
                        char str2[3];
                        idToStr(adj->item, str2);
                        printf ("%s ", str2);
                }
                printf("]\n");
                i++;
        }
}

void printCliques(tllclique cliques) {
        tll_foreach(cliques, it) {
                tllint16 clique = it->item;
                printf("{ ");
                tll_foreach(clique, jt) {
                        char str[3];
                        idToStr(jt->item, str);
                        printf ("%s ", str);
                }
                printf("}\n");
        }
}

void printClique3(tllclique3 ll) {
        tll_foreach(ll, it) {
                clique3 c = it->item;
                char strA[3];
                char strB[3];
                char strC[3];
                idToStr(c.a, strA);
                idToStr(c.b, strB);
                idToStr(c.c, strC);
                printf("%s %s %s\n", strA, strB, strC);
        }
}

bool tllContains(tllint16 arr, int16 i) {
        tll_foreach(arr, it) {
                if (it->item == i)
                        return true;
        }
        return false;
}

bool cliquesEq(clique3 a, clique3 b) {
        return a.a == b.a && a.b == b.b && a.c == b.c;
}

bool tllCliqueContains(tllclique3 cliques, clique3 clique) {
        tll_foreach(cliques, it) {
                if (cliquesEq(it->item, clique))
                        return true;
        }
        return false;
}

tllint16 tllIntersect(tllint16 a, tllint16 b) {
        tllint16 out = tll_init();
        tll_foreach(a, it) {
                if (tllContains(b, it->item))
                        tll_push_back(out, it->item);
        }
        return out;
}

tllint16 tllCopy(tllint16 arr) {
        tllint16 arr2 = tll_init();
        tll_foreach(arr, it) {
                tll_push_back(arr2, it->item);
        }
        return arr2;
}

void tllToArr(tllint16 ll, int32 arr[]) {
        int i = 0;
        tll_foreach(ll, it) {
                arr[i] = it->item;
                i++;
        }
}

tllint16 sortClique(tllint16 clique) {
        int32 len = tll_length(clique);
        int32 arr[len];
        tllToArr(clique, arr);

        quicksort(arr, 0, len - 1);

        tllint16 sorted = tll_init();
        for (int i = 0; i < len; i++) {
                tll_push_back(sorted, arr[i]);
        }

        return sorted;
}

void cliqueToStr(tllint16 clique, char *str, int len) {
        int i = 0;
        tll_foreach(clique, it) {
                if (i + 2 > len)
                        return;
                int16 id = it->item;
                str[i++] = (id / 100) + 'a';
                str[i++] = (id % 100) + 'a';
                str[i++] = ',';
        }
        str[i - 1] = '\0';
}

/*
 * algorithm BronKerbosch2(R, P, X) is
 *     if P and X are both empty then
 *         report R as a maximal clique
 *     choose a pivot vertex u in P ⋃ X
 *     for each vertex v in P \ N(u) do
 *         BronKerbosch2(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
 *         P = P \ {v}
 *         X = X ⋃ {v}
 */
void bkAlg(tllclique *cliques, tllint16 nodes[],
                tllint16 clique, tllint16 avail, tllint16 exclude) {
        if (tll_length(avail) == 0 && tll_length(exclude) == 0) {
                tll_push_back(*cliques, tllCopy(clique));
                return;
        }

        int16 pivot;
        if (tll_length(avail) > 0)
                pivot = tll_front(avail);
        else
                pivot = tll_front(exclude);

        tll_foreach(avail, it) {
                int16 v = it->item;
                if (tllContains(nodes[pivot], v))
                        continue;

                tll_push_back(clique, v);

                tllint16 newAvail = tllIntersect(avail, nodes[v]);
                tllint16 newExclude = tllIntersect(exclude, nodes[v]);
                bkAlg(cliques, nodes, clique, newAvail, newExclude);

                tll_pop_back(clique);
                tll_remove(avail, it);
                tll_push_back(exclude, v);
        }

}

void getClique3s(tllclique3 *clique3s, int32 clique[], int len) {
        for (int i = 0; i < len - 2; i++) {
                for (int j = i + 1; j < len - 1; j++) {
                        for (int k = j + 1; k < len; k++) {
                                clique3 c = {clique[i], clique[j], clique[k]};
                                tll_push_back(*clique3s, c);
                        }
                }
        }
}

void deduplicate(tllclique3 *cliques) {
        tllclique3 test = tll_init();
        tll_foreach(*cliques, it) {
                if (!tllCliqueContains(test, it->item)) {
                        tll_push_back(test, it->item);
                } else {
                        tll_remove(*cliques, it);
                        // printf("REMOVED!\n");
                }
        }
}

uint32 numCliquesWithChar(tllclique3 cliques, char c) {
        uint32 num = 0;
        int charid = c - 'a';
        tll_foreach(cliques, it) {
                clique3 c3 = it->item;
                bool found = false;
                if (c3.a / 100 == charid)
                        found = true;
                else if (c3.b / 100 == charid)
                        found = true;
                else if (c3.c / 100 == charid)
                        found = true;

                if (found) {
                        num++;
                        // printf("%s %s %s\n", idToStr(c3.a),
                        //                 idToStr(c3.b), idToStr(c3.c));
                }
        }

        return num;
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

uint32 findNumCliques(link links[], int16 numLinks) {
        tllint16 verts = tll_init();
        tllint16 vertAdj[MAX_VERT_ID];
        for (int i = 0; i < MAX_VERT_ID; i++) {
                vertAdj[i] = (tllint16)tll_init();
        }

        for (int i = 0; i < numLinks; i++) {
                addLink(&verts, vertAdj, links[i]);
        }
        // printVerts(verts, vertAdj);

        tllclique cliques = tll_init();
        tllint16 clique = tll_init();
        tllint16 exclude = tll_init();

        bkAlg(&cliques, vertAdj, clique, verts, exclude);
        // printCliques(cliques);

        tllclique3 clique3s = tll_init();
        tll_foreach(cliques, it) {
                int32 len = tll_length(it->item);
                if (len < 3)
                        continue;

                int32 clique[len];
                tllToArr(it->item, clique);
                getClique3s(&clique3s, clique, len);
        }
        // printClique3(clique3s);
        deduplicate(&clique3s);
        return numCliquesWithChar(clique3s, COMP);
}

tllint16 findLargestClique(link links[], int16 numLinks) {
        tllint16 verts = tll_init();
        tllint16 vertAdj[MAX_VERT_ID];
        for (int i = 0; i < MAX_VERT_ID; i++) {
                vertAdj[i] = (tllint16)tll_init();
        }

        for (int i = 0; i < numLinks; i++) {
                addLink(&verts, vertAdj, links[i]);
        }
        // printVerts(verts, vertAdj);

        tllclique cliques = tll_init();
        tllint16 clique = tll_init();
        tllint16 exclude = tll_init();

        bkAlg(&cliques, vertAdj, clique, verts, exclude);
        // printCliques(cliques);
        
        int len = 0;
        int longestIndex = 0;
        int i = 0;
        tll_foreach(cliques, it) {
                if ((int)tll_length(it->item) > len) {
                        len = tll_length(it->item);
                        longestIndex = i;
                }
                i++;
        }
        i = 0;
        tll_foreach(cliques, it) {
                if (i == longestIndex)
                        return it->item;
                i++;
        }

        printf("ERROR: Longest Not Found");
        return (tllint16)tll_init();
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

        uint32 numCliques = findNumCliques(links, numLinks);

        printf("Part 1: Cliques with 't': %u\n\n", numCliques);
}

void part2(llist *ll) {
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

        tllint16 longest = sortClique(findLargestClique(links, numLinks));
        // printTllint(longest);
        
        int len = tll_length(longest);
        int strLen = len * 3;
        char cliqueStr[strLen];
        cliqueToStr(longest, cliqueStr, strLen);

        printf("Part 2: password: %s\n\n", cliqueStr);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day23a.txt");
                ll = getInputFile("assets/tests/2024/Day23.txt");
        } else {
                ll = getInputFile("assets/inputs/2024/Day23.txt");
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

