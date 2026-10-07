/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Oct 07, 2026 10:18:05 EDT
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
#include "../util/talist.h"
#include "../util/quicksort.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(u16) talu16;
typedef tal(char*) talstr;

struct army {
        int index;
        bool infection;
        int units;
        int hp;
        int attack;
        u16 attackId;
        int initiative;
        talu16 immune;
        talu16 weak;
        int target;
        bool targeted;
};
typedef tal(struct army) talarmy;

struct input {
        talarmy armies;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printArmy(struct army army) {
        printf("%s ID: %d\n", army.infection ? "Infection" : "Immune", army.index);
        if (army.units <= 0) {
                printf("\tDead, Initiative: %d\n", army.initiative);
                return;
        }
        printf("\tUnits: %d, HP: %d, Attack: %d, ", army.units, army.hp, army.attack);
        printf("Attack Type: %d, Initiative: %d\n", army.attackId, army.initiative);
        printf("\tPower: %d\n", army.units * army.attack);
        printf("\tImmune: ( ");
        tal_for(army.immune, i) {
                printf("%d ", army.immune.array[i]);
        }
        printf(")\n\tWeak: ( ");
        tal_for(army.weak, i) {
                printf("%d ", army.weak.array[i]);
        }
        printf(")\n\tTargeting: %d\n", army.target);
}

void printArmyLess(struct army army) {
        printf("%s ID: %d\n", army.infection ? "Infection" : "Immune", army.index);
        if (army.units <= 0) {
                printf("\tDead, Initiative: %d\n", army.initiative);
                return;
        }
        printf("\tUnits: %d, ", army.units);
        printf("Power: %d, ", army.units * army.attack);
        printf("Targeting: %d\n", army.target);
}

void printArmies(talarmy armies) {
        tal_for(armies, i) {
                printArmyLess(armies.array[i]);
        }
        printf("\n");
}

u16 strToHash(const char *data) {
        u32 hash = 2166136261U; // 32-bit FNV offset basis
        for (size_t i = 0; i < strlen(data); i++) {
                hash ^= (u8)data[i];
                hash *= 16777619U;       // 32-bit FNV prime
        }
        // Fold 32-bit hash into 16-bits
        return (u16)((hash >> 16) ^ (hash & 0xFFFF));
}

talstr lineToStrArray(char *str) {
        talstr line = tal_init();

        char *tok = strtok(str, " ");
        while (tok != NULL) {
                int len = strlen(tok);
                if (tok[0] == '(') {
                        char *word = calloc(2, 1);
                        word[0] = '(';
                        tal_add(line, word);
                        word = malloc(len);
                        strcpy(word, tok+1);
                        tal_add(line, word);
                } else if (tok[len-1] == ')' || tok[len-1] == ',' || tok[len-1] == ';') {
                        char delim = tok[len-1];
                        tok[len-1] = '\0';
                        char *word = malloc(len);
                        strcpy(word, tok);
                        tal_add(line, word);
                        word = calloc(2, 1);
                        word[0] = delim;
                        tal_add(line, word);
                } else {
                        char *word = malloc(len+1);
                        strcpy(word, tok);
                        tal_add(line, word);
                }

                tok = strtok(NULL, " ");
        }

        return line;
}

// Index should be that of '(' and will return index after ')'
int parseImmuneWeak(talstr line, int index, talu16 *immune, talu16 *weak) {
        index++;
        bool im = false;
        bool wk = false;
        while (line.array[index][0] != ')') {
                if (im || wk) {
                        if (im)
                                tal_add(*immune, strToHash(line.array[index]));
                        else
                                tal_add(*weak, strToHash(line.array[index]));

                        if (line.array[index+1][0] == ',') {
                                index += 2;
                                continue;
                        }
                        if (line.array[index+1][0] == ';') {
                                index += 2;
                                im = false;
                                wk = false;
                                continue;
                        }
                        index++;
                } else {
                        if (strcmp(line.array[index], "immune") == 0) {
                                im = true;
                                index += 2;
                        } else if (strcmp(line.array[index], "weak") == 0) {
                                wk = true;
                                index += 2;
                        }
                }
        }

        return index + 1;
}

int cmpPower(void *a, void *b) {
        struct army army1 = *(struct army*)a;
        struct army army2 = *(struct army*)b;
        int power1 = army1.units * army1.attack;
        int power2 = army2.units * army2.attack;

        // Flipping 1 and -1 because we want largest power at the front
        if (power1 > power2) {
                return -1;
        }
        if (power1 == power2) {
                if (army1.initiative > army2.initiative) {
                        return -1;
                }
                return 1;
        }
        return 1;
}

int cmpInitiative(void *a, void *b) {
        struct army army1 = *(struct army*)a;
        struct army army2 = *(struct army*)b;

        // Flipping 1 and -1 because we want largest initiative at the front
        if (army1.initiative > army2.initiative) {
                return -1;
        }
        if (army1.initiative == army2.initiative) {
                return 0;
        }
        return 1;
}

void sortArmies(talarmy *armies, bool power) {
        if (power)
                quicksort(armies->array, sizeof(struct army), 0, armies->length-1, cmpPower);
        else
                quicksort(armies->array, sizeof(struct army), 0, armies->length-1, cmpInitiative);
}

void boostArmies(talarmy *armies, int boost) {
        tal_for(*armies, i) {
                if (!armies->array[i].infection) {
                        armies->array[i].attack += boost;
                }
        }
}

void resetArmies(talarmy *armies, int boost, int startingUnits[]) {
        tal_for(*armies, i) {
                if (!armies->array[i].infection) {
                        armies->array[i].attack -= boost;
                }
                armies->array[i].units = startingUnits[i];
        }
}

int getDamage(struct army attacking, struct army defending) {
        // Check if immune to attack
        tal_for(defending.immune, i) {
                if (defending.immune.array[i] == attacking.attackId)
                        return 0;
        }

        int power = attacking.units * attacking.attack;

        // Check if weak to attack
        tal_for(defending.weak, i) {
                if (defending.weak.array[i] == attacking.attackId)
                        return power * 2;
        }

        // Normal damage
        return power;
}

// {immune units, infection units}
ivec2 setTarget(talarmy *armies) {
        // Reset targets and see if one side is all dead
        int numImmune = 0;
        int numInfect = 0;
        tal_for(*armies, i) {
                armies->array[i].target = -1;
                armies->array[i].targeted = false;
                if (armies->array[i].infection)
                        numInfect += armies->array[i].units;
                else
                        numImmune += armies->array[i].units;
        }

        tal_for(*armies, i) {
                struct army *attack = &(armies->array[i]);
                if (attack->units <= 0) continue;
                int maxDamage = 0;
                int maxPower = 0;
                int maxInitiative = 0;
                int maxIndex = -1;
                tal_for(*armies, j) {
                        struct army *defend = &(armies->array[j]);
                        if (defend->units <= 0 || defend->targeted ||
                                        attack->infection == defend->infection)
                                continue;

                        int power = defend->units * defend->attack;
                        int damage = getDamage(*attack, *defend);
                        if (damage <= 0) continue;

                        bool target = false;
                        if (damage > maxDamage) {
                                target = true;
                        } else if (damage == maxDamage) {
                                if (power > maxPower) {
                                        target = true;
                                } else if (power == maxPower) {
                                        target = defend->initiative > maxInitiative;
                                }
                        }

                        if (target) {
                                maxDamage = damage;
                                maxPower = power;
                                maxInitiative = defend->initiative;
                                maxIndex = j;
                                attack->target = defend->index;
                        }
                }
                if (maxIndex != -1) {
                        armies->array[maxIndex].targeted = true;
                }
        }

        return (ivec2){{numImmune, numInfect}};
}

void attack(talarmy *armies) {
        tal_for(*armies, i) {
                struct army *attack = &(armies->array[i]);
                if (attack->units <= 0 || attack->target < 0)
                        continue;

                struct army *defend = &(armies->array[attack->target]);

                int damage = getDamage(*attack, *defend);
                int numKilled = damage / defend->hp;
                if (numKilled >= defend->units)
                        defend->units = 0;
                else
                        defend->units -= numKilled;
        }
}

int battle(talarmy *armies) {
        ivec2 numUnits;
        int prevTotalUnits = 0;
        for (;;) {
                sortArmies(armies, true);
                numUnits = setTarget(armies);
                // If unit numbers don't change, stuck in loop
                int totalUnits = numUnits.a + numUnits.b;
                if (numUnits.a == 0 || numUnits.b == 0 || totalUnits == prevTotalUnits) 
                        break;
                prevTotalUnits = totalUnits;
                sortArmies(armies, false);
                attack(armies);
        }
        sortArmies(armies, false);
        // printArmies(armies);

        if (numUnits.a == 0 || numUnits.b == 0) 
                return numUnits.a == 0 ? -numUnits.b : numUnits.a;
        else
                return -1;
}

void part1(struct input *input) {
        talarmy armies = tal_init();
        tal_copy(input->armies, armies);
        // printArmies(armies);

        int numUnits = abs(battle(&armies));

        tal_destroy(armies);
        printf("Part 1: Units left = %d\n\n", numUnits);
}

void part2(struct input *input) {
        talarmy armies = tal_init();
        tal_copy(input->armies, armies);
        // printArmies(armies);

        int startingUnits[armies.length];
        tal_for(armies, i) { startingUnits[i] = armies.array[i].units; }

        // Increase boost by 100 until first battle won
        int upper = 100;
        int lower = 0;
        for (;;) {
                boostArmies(&armies, upper);
                int result = battle(&armies);
                resetArmies(&armies, upper, startingUnits);
                if (result > 0)
                        break;
                lower = upper;
                upper += 100;
        }

        // Binary search until lowest boost where immune wins
        while (lower + 1 != upper) {
                // Test center boost
                int boost = lower + ((upper - lower) / 2);

                // Run sim
                boostArmies(&armies, boost);
                int result = battle(&armies);
                resetArmies(&armies, boost, startingUnits);

                if (result > 0) {
                        upper = boost;
                } else {
                        lower = boost;
                }
        }
        boostArmies(&armies, upper);
        int result = battle(&armies);

        printf("Part 2: Units left with %d boost = %d\n", upper, result);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        bool infection = false;
        for (llNode *cur = ll->head->next; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (strlen(str) == 0) {
                        infection = true;
                        cur = cur->next;
                        continue;
                }

                struct army army = {0};
                army.infection = infection;
                army.target = -1;
                army.targeted = false;
                talstr line = lineToStrArray(str);
                army.units = strtol(line.array[0], NULL, 10);
                army.hp = strtol(line.array[4], NULL, 10);

                int lineIdx = 7;
                if (line.array[lineIdx][0] == '(') {
                        lineIdx = parseImmuneWeak(line, lineIdx, &army.immune, &army.weak);
                }
                lineIdx += 5;
                army.attack = strtol(line.array[lineIdx++], NULL, 10);
                army.attackId = strToHash(line.array[lineIdx]);
                lineIdx += 4;
                army.initiative = strtol(line.array[lineIdx], NULL, 10);

                tal_add(input.armies, army);

                tal_free_and_destroy(line, free);
        }

        sortArmies(&input.armies, false);
        tal_for(input.armies, i) {
                input.armies.array[i].index = i;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day24.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day24.txt";
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

