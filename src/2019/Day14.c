/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Apr 06, 2026 22:30:40 UTC
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

#define INPUT_BUFFER_SIZE 16384
#define SEED 0x12345678

typedef struct chem {
        u32 num;
        u16 id;
} chem;
typedef tll(chem) tllchem;

typedef struct reaction {
        tllchem reactants;
        chem product;
} reaction;
typedef tll(reaction) tllreaction;

struct input {
        tllreaction reactions;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printReactons(tllreaction reactions) {
        tll_foreach(reactions, it) {
                reaction r = it->item;
                tll_foreach(r.reactants, itt) {
                        chem c = itt->item;
                        printf("%u: %u, ", c.num, c.id);
                }
                printf("=> %u: %u\n", r.product.num, r.product.id);
        }
}

int64 max(int64 a, int64 b) {
        return a > b ? a : b;
}

int64 divRoundUp(int64 a, int64 b) {
        return (a + (b - 1)) / b;
}

int64 numOreReq(reaction **reactions, int64 numFuel) {
        const u16 ORE = (u16)FNV("ORE", SEED);
        const u16 FUEL = (u16)FNV("FUEL", SEED);

        int64 *numRequired = calloc(0x10000l, sizeof(int64));
        numRequired[FUEL] = numFuel;
        for (;;) {
                bool allProduced = true;
                for (int i = 0; i < 0x10000l; i++) {
                        if (i == ORE || numRequired[i] <= 0)
                                continue;
                        allProduced = false;

                        // Get reactions for this product
                        reaction r = *reactions[i];
                        int64 numReactions = divRoundUp(numRequired[i], r.product.num);
                        int64 numProduced = r.product.num * numReactions;
                        tll_foreach(r.reactants, it) {
                                chem c = it->item;
                                numRequired[c.id] += c.num * numReactions;
                        }

                        // Product no londer needed
                        numRequired[i] -= numProduced;
                }
                if (allProduced)
                        break;
        }
        int64 ore = numRequired[ORE];
        free(numRequired);
        return ore;
}

void part1(struct input *input) {
        // printReactons(input->reactions);

        // Organize reactions into hashmap based on the product
        reaction **reactions = calloc(0x10000l, sizeof(reaction*));
        tll_foreach(input->reactions, it)
                reactions[it->item.product.id] = &it->item;

        int64 numOre = numOreReq(reactions, 1);

        printf("Part 1: %ld\n\n", numOre);
        free(reactions);
}

void part2(struct input *input) {
        const int64 MAX_ORE = 1000000000000l;

        // Organize reactions into hashmap based on the product
        reaction **reactions = calloc(0x10000l, sizeof(reaction*));
        tll_foreach(input->reactions, it)
                reactions[it->item.product.id] = &it->item;

        // Test amount of fuel
        // If n ore makes m fuel, then 1e12 ore makes at least (m * 1e12 / n) fuel.
        int64 numFuel = 1;
        for (;;) {
                int64 oreReq = numOreReq(reactions, numFuel + 1);
                if (oreReq < MAX_ORE)
                        numFuel = max(numFuel + 1, (numFuel + 1) * MAX_ORE / oreReq);
                else
                        break;
        }

        printf("Part 2: %ld\n", numFuel);
        free(reactions);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char **hashes = calloc(0x10000l, sizeof(char*));

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                reaction r = {0};

                // Get reactants and product
                bool reactants = true;
                char *tok = strtok(str, ", ");
                while (tok != NULL) {
                        if (tok[0] == '=') {
                                tok = strtok(NULL, ", ");
                                reactants = false;
                                continue;
                        }

                        chem c = {0};
                        c.num = strtoul(tok, NULL, 10);
                        tok = strtok(NULL, ", ");
                        c.id = (u16)FNV(tok, SEED);

                        // printf("%s: %d %u\n", tok, c.num, c.id);
                        if (hashes[c.id] != NULL) {
                                if (strcmp(hashes[c.id], tok) != 0) {
                                        printf("Colision! %s, %s\n", hashes[c.id], tok);
                                }
                        } else {
                                debugp("%s: %u\n", tok, c.id);
                                hashes[c.id] = malloc(strlen(tok) + 1);
                                strncpy(hashes[c.id], tok, strlen(tok) + 1);
                        }

                        if (reactants)
                                tll_push_back(r.reactants, c);
                        else
                                r.product = c;
                        tok = strtok(NULL, ", ");
                }
                tll_push_back(input.reactions, r);
        }
        debugp("\n");

        for (int i = 0; i < 0x10000; i++)
                if (hashes[i] != NULL)
                        free(hashes[i]);
        free(hashes);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day14.txt";
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

