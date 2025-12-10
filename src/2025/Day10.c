/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Dec 10, 2025 13:02:23 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <z3.h>
#include <z3_api.h>
#include <z3_optimization.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(int) tllint;

typedef struct lightState {
        bool *lights;
        int64 numButtons;
} lightState;
typedef tll(lightState) tllLightState;

typedef struct joltState {
        int *jolts;
        int64 numButtons;
} joltState;
typedef tll(joltState) tllJoltState;

typedef struct machine {
        int numLights;
        bool *lights;
        int numButtons;
        tllint *buttons;
        int numJoltages;
        int *joltages;
} machine;

struct input {
        int numMachines;
        machine *machines;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMachines(int num, machine *ms) {
        if (!Debug) return;
        for (int i = 0; i < num; i++) {
                printf("[");
                for (int j = 0; j < ms[i].numLights; j++)
                        printf("%c", ms[i].lights[j] ? '#' : '.');
                printf("] ");

                for (int j = 0; j < ms[i].numButtons; j++) {
                        printf("(");
                        tll_foreach(ms[i].buttons[j], it)
                                printf("%d,", it->item);
                        printf("\b) ");
                }

                printf("{");
                for (int j = 0; j < ms[i].numJoltages; j++) {
                        printf("%d,", ms[i].joltages[j]);
                }
                printf("\b}\n");
        }
}

int numCharInStr(char c, char *str) {
        int count = 0;
        int i = 0;
        while (str[i] != '\0') {
                if (str[i++] == c)
                        count++;
        }
        return count;
}

void parseButton(char *str, tllint *button) {
        int num = 0;
        int i = 1; // Ignore leading '('
        while (str[i] != '\0') {
                if (str[i] == ',' || str[i] == ')') {
                        tll_push_back(*button, num);
                        num = 0;
                } else {
                        num = (num * 10) + (str[i] - '0');
                }
                i++;
        }
}

bool lightPatternMatch(int num, bool *p1, bool *p2) {
        for (int i = 0; i < num; i++) {
                if (p1[i] != p2[i])
                        return false;
        }
        return true;
}

bool lightPatternExists(int num, tllLightState states, lightState s) {
        tll_foreach(states, it) {
                if (lightPatternMatch(num, s.lights, it->item.lights))
                        return true;
        }
        return false;
}

int64 configLights(machine m) {
        // Work backwards from light config to all off.
        lightState initState = {NULL, 0};
        initState.lights = malloc(m.numLights * sizeof(bool));
        memcpy(initState.lights, m.lights, m.numLights * sizeof(bool));

        // BFS
        tllLightState states = tll_init();
        tll_push_back(states, initState);

        // Break on first state where all lights are off, Guaranteed to be fastest
        while (tll_length(states) > 0) {
                lightState s = tll_pop_front(states);

                // Check if all lights are off
                bool allOff = true;
                for (int i = 0; i < m.numLights; i++) {
                        if (s.lights[i]) {
                                allOff = false;
                                break;
                        }
                }
                if (allOff) {
                        return s.numButtons;
                }

                // Try every button
                for (int i = 0; i < m.numButtons; i++) {
                        lightState newS = {NULL, s.numButtons + 1};
                        newS.lights = malloc(m.numLights * sizeof(bool));
                        memcpy(newS.lights, s.lights, m.numLights * sizeof(bool));

                        // Toggle lights based on button
                        tll_foreach(m.buttons[i], it) {
                                newS.lights[it->item] = !newS.lights[it->item];
                        }

                        if (!lightPatternExists(m.numLights, states, newS)) // Avoid duplicates
                                tll_push_back(states, newS);
                }
                free(s.lights);
        }

        // Shouldn't be able to reach here!
        printf("Unreachable!\n");
        return -1;
}

int64 configJoltage(machine m) {
        Z3_config cfg = Z3_mk_config();
        Z3_set_param_value(cfg, "model", "true");
        Z3_context c = Z3_mk_context(cfg);
        Z3_del_config(cfg);

        Z3_optimize opt = Z3_mk_optimize(c);
        Z3_optimize_inc_ref(c, opt);

        // Declare a variable for each button
        Z3_sort int_sort = Z3_mk_int_sort(c);
        Z3_ast vars[m.numButtons];
        char name[4];
        for (int i = 0; i < m.numButtons; i++) {
                sprintf(name, "x%d", i);
                Z3_symbol s = Z3_mk_string_symbol(c, name);
                vars[i] = Z3_mk_const(c, s, int_sort);
        }

        // Make list of buttons that influence each value
        tllint *joltBtn = malloc(m.numJoltages * sizeof(tllint));
        for (int i = 0; i < m.numJoltages; i++)
                joltBtn[i] = (tllint)tll_init();
        for (int i = 0; i < m.numButtons; i++) {
                tll_foreach(m.buttons[i], it) {
                        tll_push_back(joltBtn[it->item], i);
                }
        }

        // Create an equation for each joltage value
        for (int i = 0; i < m.numJoltages; i++) {
                Z3_ast terms[m.numButtons];
                u32 numTerms = 0;
                tll_foreach(joltBtn[i], it) {
                        Z3_ast mul = Z3_mk_mul(c, 2,
                                        (Z3_ast[]){Z3_mk_int(c, 1, int_sort), vars[it->item]});
                        terms[numTerms++] = mul;
                }

                // Build LHS of eq
                Z3_ast lhs;
                if (numTerms == 1)
                        lhs = terms[0];
                else
                        lhs = Z3_mk_add(c, numTerms, terms);

                // Equality with const
                Z3_ast eq = Z3_mk_eq(c, lhs, Z3_mk_int(c, m.joltages[i], int_sort));
                Z3_optimize_assert(c, opt, eq);
        }

        // All variables are positive
        for (int i = 0; i < m.numButtons; ++i) {
                Z3_ast ge0 = Z3_mk_ge(c, vars[i], Z3_mk_int(c, 0, int_sort));
                Z3_optimize_assert(c, opt, ge0);
        }

        Z3_ast sum = Z3_mk_add(c, m.numButtons, vars);
        Z3_optimize_minimize(c, opt, sum);

        Z3_lbool result = Z3_optimize_check(c, opt, 0, NULL);
        if (result != Z3_L_TRUE) {
                if (result == Z3_L_FALSE) printf("Unsatisfiable system.\n");
                else                    printf("Result undefined.\n");
                return 0;
        }
        
        Z3_ast lower = Z3_optimize_get_lower(c, opt, 0);
        int minPresses;
        Z3_get_numeral_int(c, lower, &minPresses);

        Z3_optimize_dec_ref(c, opt);
        Z3_del_context(c);
        free(joltBtn);

        return minPresses;
}

void part1(struct input input) {
        int numMachines = input.numMachines;
        machine *machines = input.machines;
        printMachines(numMachines, machines);

        int64 totalPresses = 0;
        for (int i = 0; i < numMachines; i++) {
                int64 presses = configLights(machines[i]);
                debugp("%ld\n", presses);
                totalPresses += presses;
        }

        printf("Part 1: %ld\n\n", totalPresses);
}

// Using Z3 :(
void part2(struct input input) {
        int numMachines = input.numMachines;
        machine *machines = input.machines;

        int64 totalPresses = 0;
        for (int i = 0; i < numMachines; i++) {
                int64 presses = configJoltage(machines[i]);
                debugp("%ld\n", presses);
                totalPresses += presses;
        }

        printf("Part 2: %ld\n", totalPresses);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numMachines = ll->length;
        input.machines = malloc(input.numMachines * sizeof(machine));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                machine *m = &input.machines[i++];
                int numFields = numCharInStr(' ', str) + 1;

                // Parse light pattern
                char *lightsStr = strtok(str, " ");
                m->numLights = (strlen(lightsStr) - 2);
                m->lights = malloc(m->numLights * sizeof(bool));
                for (int j = 0; j < m->numLights; j++)
                        m->lights[j] = (lightsStr[j+1] == '#');

                // Parse buttons
                m->numButtons = numFields - 2;
                m->buttons = malloc (m->numButtons * sizeof(tllint));
                for (int j = 0; j < m->numButtons; j++) {
                        char *buttonStr = strtok(NULL, " ");
                        m->buttons[j] = (tllint)tll_init();
                        parseButton(buttonStr, &m->buttons[j]);
                }

                // Parse joltages
                char *joltagesStr = strtok(NULL, "");
                m->numJoltages = numCharInStr(',', joltagesStr) + 1;
                m->joltages = malloc(m->numJoltages * sizeof(int));
                char *jolts = strtok(joltagesStr+1, ",}");
                for (int j = 0; j < m->numJoltages; j++) {
                        m->joltages[j] = strtol(jolts, NULL, 10);
                        jolts = strtok(NULL, ",}");
                }
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day10.txt";
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

