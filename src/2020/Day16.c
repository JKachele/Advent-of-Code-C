/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 17, 2026 16:01:44 UTC
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
#include "../util/vector.h"
// #include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 4096

typedef tal(int) talint;

typedef struct ticket {
        bool valid;
        talint fields;
} ticket;
typedef tal(ticket) talticket;

typedef struct rule {
        bool hasField;
        ivec2 ranges[2]; // Each rule has 2 ranges;
} rule;
typedef tal(rule) talrule;

struct input {
        talrule rules;
        ticket myTicket;
        talticket tickets;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTicket(ticket tkt) {
        printf("(%s) ", tkt.valid ? "Valid" : "Invalid");
        for (unsigned int i = 0; i < tkt.fields.length; i++) {
                printf("%d ", tkt.fields.array[i]);
        }
        printf("\n");
}

void printTickets(talticket tickets) {
        for (unsigned int i = 0; i < tickets.length; i++) {
                printTicket(tickets.array[i]);
        }
        printf("\n");
}

void printRules(talrule rules) {
        for (unsigned int i = 0; i < rules.length; i++) {
                printf("%d-%d or ", rules.array[i].ranges[0].x, rules.array[i].ranges[0].y);
                printf("%d-%d\n", rules.array[i].ranges[1].x, rules.array[i].ranges[1].y);
        }
        printf("\n");
}

ticket getTicket(char *str) {
        ticket newTicket = {0};
        newTicket.valid = true;

        char *tok = strtok(str, ",");
        while (tok != NULL) {
                int num = strtol(tok, NULL, 10);
                tal_add(newTicket.fields, num);
                tok = strtok(NULL, ",");
        }

        return newTicket;
}

bool fieldvalid(int field, rule rule) {
        for (int i = 0; i < 2; i++) {
                if (field >= rule.ranges[i].x && field <= rule.ranges[i].y)
                        return true;
        }
        return false;
}

bool ticketValid(ticket tkt, talrule rules, talint *invalidFields) {
        bool valid = true;
        for (unsigned int i = 0; i < tkt.fields.length; i++) {
                bool fieldValid = false;
                for (unsigned int j = 0; j < rules.length; j++) {
                        if (fieldvalid(tkt.fields.array[i], rules.array[j])) {
                                fieldValid = true;
                                break;
                        }
                }
                if (!fieldValid) {
                        tal_add(*invalidFields, tkt.fields.array[i]);
                        valid = false;
                }
        }
        return valid;
}

int getFieldRule(talint fields, talrule rules, int fieldIndex) {
        int numValidRules = 0;
        bool validRules[rules.length];
        for (unsigned int i = 0; i < rules.length; i++) {
                if (!rules.array[i].hasField) {
                        validRules[i] = true;
                        numValidRules++;
                } else {
                        validRules[i] = false;
                }
        }

        // Loop through each field until only one rule is valid
        for (unsigned int i = 0; i < fields.length; i++) {
                if (numValidRules == 1) break;

                int field = fields.array[i];

                // Loop through valid rules and test each
                for (unsigned int j = 0; j < rules.length; j++) {
                        if (!validRules[j]) continue;
                        if (!fieldvalid(field, rules.array[j])) {
                                validRules[j] = false;
                                numValidRules--;
                        }
                }
        }

        // If exactly 1 rule is valid, that is the rule for the field, else return -1
        if (numValidRules != 1) return -1;

        int validRuleIndex = -1;
        for (unsigned int i = 0; i < rules.length; i++) {
                if (validRules[i]) {
                        validRuleIndex = i;
                        break;
                }
        }

        return validRuleIndex;
}

void part1(struct input input) {
        talrule rules = input.rules;
        talticket tickets = input.tickets;
        // printRules(rules);
        // printTickets(tickets);

        // Test nearby tickets
        talint invalidFields = tal_init();
        for (unsigned int i = 0; i < tickets.length; i++) {
                if (!ticketValid(tickets.array[i], rules, &invalidFields))
                        tickets.array[i].valid = false;
        }

        // Add all invalid fields
        int errorRate = 0;
        for (unsigned int i = 0; i < invalidFields.length; i++) {
                // printf("%d\n", invalidFields.array[i]);
                errorRate += invalidFields.array[i];
        }

        printf("Part 1: %d\n\n", errorRate);
}

void part2(struct input input) {
        talrule rules = input.rules;
        talticket tickets = input.tickets;
        // printRules(rules);
        // printTickets(tickets);

        // The rule for each field of the tickets
        int fieldRules[rules.length];
        int numFieldsLeft = rules.length;
        for (unsigned int i = 0; i < rules.length; i++) fieldRules[i] = -1;

        // Only set a rule to a field if that is the only valid rule for that field
        // Once a rule is set to a field, rule is invalid for other fields
        // Repeat until all fields have exactly 1 valid rule
        while (numFieldsLeft > 0) {
                // For each field, make a list of all the fields of that index from all the tickets
                for (unsigned int i = 0; i < rules.length; i++) {
                        if (fieldRules[i] != -1) continue;

                        talint fields = tal_init();
                        for (unsigned int j = 0; j < tickets.length; j++) {
                                if (tickets.array[j].valid)
                                        tal_add(fields, tickets.array[j].fields.array[i]);
                        }

                        int ruleIndex = getFieldRule(fields, rules, i);
                        if (ruleIndex != -1) {
                                fieldRules[i] = ruleIndex;
                                rules.array[ruleIndex].hasField = true;
                                numFieldsLeft--;
                        }
                        tal_destroy(fields);
                }
        }

        // Multiply fields on my ticket for first 6 rules
        int64 ans = 1;
        for (unsigned int i = 0; i < rules.length; i++) {
                // printf("%d - %d\n", i, fieldRules[i]);
                if (fieldRules[i] < 6) {
                        ans *= input.myTicket.fields.array[i];
                }
        }

        printf("Part 2: %ld\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        llNode *cur = ll->head;
        while (cur != NULL) {
                char *str = (char*)cur->data;
                if (strlen(str) == 0) break;

                rule newRule = {0};
                newRule.hasField = false;

                // Dont care about field names
                strtok(str, ":");
                newRule.ranges[0].x = strtol(strtok(NULL, "-"), NULL, 10);
                newRule.ranges[0].y = strtol(strtok(NULL, " "), NULL, 10);
                strtok(NULL, " ");
                newRule.ranges[1].x = strtol(strtok(NULL, "-"), NULL, 10);
                newRule.ranges[1].y = strtol(strtok(NULL, " "), NULL, 10);

                tal_add(input.rules, newRule);
                cur = cur->next;
        }

        cur = cur->next->next;
        input.myTicket = getTicket(cur->data);

        cur = cur->next->next->next;
        while (cur != NULL) {
                char *str = (char*)cur->data;
                tal_add(input.tickets, getTicket(str));
                cur = cur->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day16.txt";
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

