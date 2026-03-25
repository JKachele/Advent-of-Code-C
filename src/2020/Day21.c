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

struct input {
        tllfood foodList;
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

void part1(struct input input) {
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

        // Count number of ingredients that are not allergens
        int numIngredients = 0;
        tll_foreach(input.foodList, it) {
                tll_foreach(it->item.ingredients, itt) {
                        if (ingredients[itt->item] == 0)
                                numIngredients++;
                }
        }

        printf("Part 1: %d\n\n", numIngredients);
}

void part2(struct input input) {
        printf("Part 2: \n");
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
                        ingredient = strtok(NULL, " ");
                }

                // Get allergens
                ingredient = strtok(NULL, ", )");
                while (ingredient != NULL) {
                        u32 hash = FNV(ingredient, SEED) & MASK;
                        tll_push_back(cur.allergens, hash);
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

