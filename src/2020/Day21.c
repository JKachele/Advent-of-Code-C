/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 24, 2026 19:32:17 UTC
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
#define SEED 0x12345678
#define MASK 0xfffff

typedef tll(u32) tllu32;

typedef struct food {
        tllu32 ingredients;
        tllu32 allergens;
} food;
typedef tll(food) tllfood;

typedef struct hashname {
        u32 hash;
        char *name;
} hashname;
typedef tll(hashname) tllhashname;

typedef struct allergen {
        u32 ingredientHash;
        char *allergen;
} allergen;

struct input {
        tllfood foodList;
        tllu32 ingredients;
        tllu32 allergens;
        tllhashname hashNames;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printList(tllu32 list) {
        tll_foreach(list, it) {
                printf("%u ", it->item);
        }
        printf("\n");
}

void printHashNames(tllhashname hashNames) {
        tll_foreach(hashNames, it) {
                printf("%s: %u\n", it->item.name, it->item.hash);
        }
}

void addHashName(tllhashname *hashNames, u32 hash, char *name) {
        bool added = false;
        tll_foreach(*hashNames, it) {
                if (it->item.hash == hash) {
                        if (strncmp(it->item.name, name, strlen(name)) != 0) {
                                printf("Hash collision detected for '%s' and '%s' (hash %u)\n",
                                                it->item.name, name, hash);
                        }
                        added = true;
                        break;
                }
                if (it->item.hash > hash) {
                        hashname hn = {hash, NULL};
                        hn.name = malloc(strlen(name) + 1);
                        strncpy(hn.name, name, strlen(name) + 1);
                        tll_insert_before(*hashNames, it, hn);
                        added = true;
                        break;
                }
        }
        if (!added) {
                hashname hn = {hash, NULL};
                hn.name = malloc(strlen(name) + 1);
                strncpy(hn.name, name, strlen(name) + 1);
                tll_push_back(*hashNames, hn);
        }
}

void searchHashnames(tllhashname hashNames, u32 hash, char **name) {
        tll_foreach(hashNames, it) {
                if (it->item.hash == hash) {
                        *name = it->item.name;
                        return;
                }
        }
}

void addU32(tllu32 *list, u32 value) {
        bool added = false;
        tll_foreach(*list, it) {
                if (it->item == value) {
                        added = true;
                        break;
                }
                if (it->item > value) {
                        added = true;
                        tll_insert_before(*list, it, value);
                        break;
                }
        }
        if (!added) {
                tll_push_back(*list, value);
        }
}

void concatList(tllu32 *dest, tllu32 src) {
        tll_foreach(src, it) {
                u32 hash = it->item;
                bool exists = false;
                tll_foreach(*dest, itt) {
                        if (hash == itt->item) {
                                exists = true;
                                break;
                        }
                }
                if (!exists)
                        tll_push_back(*dest, hash);
        }
}

void removeIngredients(tllu32 *allergens, tllu32 *allergenIng, u32 allergen, u32 ingredient) {
        tll_foreach(*allergens, it) {
                u32 a = it->item;
                if (a == allergen) {
                        tll_remove(*allergens, it);
                        continue;
                }
                tll_foreach(allergenIng[a], itt) {
                        if (itt->item == ingredient) {
                                tll_remove(allergenIng[a], itt);
                                break;
                        }
                }
        }
}

void intersectLists(tllu32 *dest, tllu32 list1) {
        // Anything in dest that isn't in list 1 is removed from dest
        tll_foreach(*dest, it) {
                u32 hash = it->item;
                bool found = false;
                tll_foreach(list1, itt) {
                        if (hash == itt->item) {
                                found = true;
                                break;
                        }
                }
                if (!found)
                        tll_remove(*dest, it);
        }
}

// Too lazy to implement a better algorithm
void bubbleSort(allergen *allergens, int n) {
        bool sorted = false;
        while (!sorted) {
                sorted = true;
                for (int i = 0; i < n - 1; i++) {
                        allergen *a = &allergens[i];
                        allergen *b = &allergens[i+1];
                        int minLen = (strlen(a->allergen) < strlen(b->allergen)) ?
                                strlen(a->allergen) : strlen(b->allergen);
                        if (strncmp(a->allergen, b->allergen, minLen) > 0) {
                                allergen tmp = *a;
                                *a = *b;
                                *b = tmp;
                                sorted = false;
                        }
                }
        }
}

void part1_2(struct input input) {
        // Create a hash map of allergens and the list of ingredients that could contain each
        // allergen. As the list is traversed, remove ingredients until there is only one left
        tllu32 *allergenIng = calloc(MASK, sizeof(tllu32));
        tllu32 allergens = tll_init();

        // Once an allergen has exactly one ingredient, it's added to the list of ingredients
        u32 *ingredients = calloc(MASK, sizeof(tllu32));

        tll_foreach(input.foodList, it) {
                food cur = it->item;
                tll_foreach(cur.allergens, itt) {
                        if (tll_length(allergenIng[itt->item]) == 0) {
                                tll_push_back(allergens, itt->item);
                                concatList(&allergenIng[itt->item], cur.ingredients);
                                continue;
                        }

                        // Any ingredient that appears in the allergen's list that isnt in the
                        // current food's ingredients list is removed from the allergen's list
                        intersectLists(&allergenIng[itt->item], cur.ingredients);
                }
        }

        while (tll_length(allergens) > 0) {
                tll_foreach(allergens, it) {
                        // If an allergen has only one possible ingredient, add it to the list of
                        // ingredients and remove ingredient from other allergens
                        if (tll_length(allergenIng[it->item]) == 1) {
                                u32 ingredient = tll_front(allergenIng[it->item]);
                                ingredients[ingredient] = it->item;
                                removeIngredients(&allergens, allergenIng, it->item, ingredient);
                                continue;
                        }
                }
        }

        int numIngredients = 0;
        tll_foreach(input.foodList, it) {
                tll_foreach(it->item.ingredients, itt) {
                        if (ingredients[itt->item] == 0)
                                numIngredients++;
                }
        }

        printf("Part 1: %d\n\n", numIngredients);

        /******************************** PART2 ********************************/
        // printHashNames(input.hashNames);

        // Create array of ingredient with allergens
        int numA = tll_length(input.allergens);
        allergen *allergensArray = malloc(numA * sizeof(allergen));
        int i = 0;
        tll_foreach(input.ingredients, it) {
                if (ingredients[it->item] != 0) {
                        allergen *a = &allergensArray[i];
                        a->ingredientHash = it->item;
                        searchHashnames(input.hashNames, ingredients[it->item], &a->allergen);
                        // printf("Ingredient: %d, Allergen: %s\n", it->item, a->allergen);
                        i++;
                }
        }

        // Sort array by allergen name
        bubbleSort(allergensArray, numA);

        printf("Part 2:\n");
        for (int j = 0; j < numA; j++) {
                if (j != 0) printf(",");
                char *ingredient;
                searchHashnames(input.hashNames, allergensArray[j].ingredientHash, &ingredient);
                printf("%s", ingredient);
        }
        printf("\n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                food cur = {0};

                // Get ingredients
                char *ingredient = strtok(str, " ");
                while (ingredient != NULL) {
                        if (ingredient[0] == '(')
                                break;
                        u32 hash = FNV(ingredient, SEED) & MASK;
                        tll_push_back(cur.ingredients, hash);
                        addHashName(&input.hashNames, hash, ingredient);
                        addU32(&input.ingredients, hash);
                        ingredient = strtok(NULL, " ");
                }

                // Get allergens
                ingredient = strtok(NULL, ", )");
                while (ingredient != NULL) {
                        u32 hash = FNV(ingredient, SEED) & MASK;
                        tll_push_back(cur.allergens, hash);
                        addHashName(&input.hashNames, hash, ingredient);
                        addU32(&input.allergens, hash);
                        ingredient = strtok(NULL, ", )");
                }

                tll_push_back(input.foodList, cur);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1_2(input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

