/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 27, 2026 15:15:52 EDT
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

// If input instruction, Value chip in field "low" goes to bot "id"
struct botInstr {
        int id;
        int low;
        bool lowBot;
        int high;
        bool highBot;
        bool input;
};

struct input {
        int numInstrs;
        struct botInstr *instrs;
};

void printInstr(struct botInstr instr) {
        if (instr.input) {
                printf("Input Chip %d to Bot %d\n", instr.low, instr.id);
        } else {
                printf("Bot %d\n", instr.id);
                printf("\tLow  to %s %d\n", instr.lowBot ? "Bot" : "Output", instr.low);
                printf("\tHigh to %s %d\n", instr.highBot ? "Bot" : "Output", instr.high);
        }
}

void printState(int numBots, ivec2 bots[], int numOutputs, int outputs[]) {
        printf("Bots: [");
        for (int i = 0; i < numBots; i++) {
                printf("(%d, %d) ", bots[i].a, bots[i].b);
        }
        printf("\b]\nOutputs: [");
        for (int i = 0; i < numOutputs; i++) {
                printf("%d ", outputs[i]);
        }
        printf("\b]\n\n");
}

void addBotValue(ivec2 *bot, int value) {
        if (bot->a == -1) {
                bot->a = value;
                return;
        }

        if (bot->a < value) {
                bot->b = value;
        } else {
                bot->b = bot->a;
                bot->a = value;
        }
}

int stepBots(int numBots, ivec2 bots[], int outputs[], struct botInstr instrs[], bool stop) {
        const ivec2 CMP = {{17, 61}};
        ivec2 botCopy[numBots];
        for (int i = 0; i < numBots; i++) {
                botCopy[i] = bots[i];
                bots[i] = (ivec2){{-1, -1}};
        }

        for (int i = 0; i < numBots; i++) {
                ivec2 bot = botCopy[i];
                if (bot.a == -1) {
                        continue;
                } else if (bot.b == -1) {
                        addBotValue(&bots[i], bot.a);
                        continue;
                }

                if (stop && ivec2Eq(bot, CMP)) {
                        return i;
                }

                struct botInstr instr = instrs[i];

                if (instr.lowBot) {
                        addBotValue(&bots[instr.low], bot.a);
                } else {
                        outputs[instr.low] = bot.a;
                }

                if (instr.highBot) {
                        addBotValue(&bots[instr.high], bot.b);
                } else {
                        outputs[instr.high] = bot.b;
                }
        }
        return -1;
}

void part1(struct input *input) {
        int numInstrs = input->numInstrs;
        struct botInstr *instrs = input->instrs;

        int numBots = 0;
        int numOutputs = 0;
        for (int i = 0; i < numInstrs; i++) {
                struct botInstr instr = instrs[i];
                // printInstr(instr);
                if (instr.id > numBots) { numBots = instr.id; }
                if (instr.input) { continue; }
                if (!instr.lowBot && instr.low > numOutputs) { numOutputs = instr.low; }
                if (!instr.highBot && instr.high > numOutputs) { numOutputs = instr.high; }
        }
        numBots++;
        numOutputs++;

        struct botInstr botInstrs[numBots];
        ivec2 bots[numBots];
        int outputs[numOutputs];
        for (int i = 0; i < numBots; i++) { bots[i] = (ivec2){{-1, -1}}; }
        for (int i = 0; i < numOutputs; i++) { outputs[i] = 0; }

        for (int i = 0; i < numInstrs; i++) {
                struct botInstr instr = instrs[i];
                if (instr.input) {
                        addBotValue(&bots[instr.id], instr.low);
                } else {
                        botInstrs[instr.id] = instr;
                }
        }

        int botCmp;
        for (;;) {
                botCmp = stepBots(numBots, bots, outputs, botInstrs, true);
                if (botCmp != -1) {
                        break;
                }
        }

        printf("Part 1: Bot used for 17/61 = %d\n\n", botCmp);
}

void part2(struct input *input) {
        int numInstrs = input->numInstrs;
        struct botInstr *instrs = input->instrs;

        int numBots = 0;
        int numOutputs = 0;
        for (int i = 0; i < numInstrs; i++) {
                struct botInstr instr = instrs[i];
                // printInstr(instr);
                if (instr.id > numBots) { numBots = instr.id; }
                if (instr.input) { continue; }
                if (!instr.lowBot && instr.low > numOutputs) { numOutputs = instr.low; }
                if (!instr.highBot && instr.high > numOutputs) { numOutputs = instr.high; }
        }
        numBots++;
        numOutputs++;

        struct botInstr botInstrs[numBots];
        ivec2 bots[numBots];
        int outputs[numOutputs];
        for (int i = 0; i < numBots; i++) { bots[i] = (ivec2){{-1, -1}}; }
        for (int i = 0; i < numOutputs; i++) { outputs[i] = 0; }

        for (int i = 0; i < numInstrs; i++) {
                struct botInstr instr = instrs[i];
                if (instr.input) {
                        addBotValue(&bots[instr.id], instr.low);
                } else {
                        botInstrs[instr.id] = instr;
                }
        }

        for (;;) {
                stepBots(numBots, bots, outputs, botInstrs, false);
                bool done = true;
                for (int i = 0; i < numBots; i++) {
                        if (bots[i].a != -1) {
                                done = false;
                                break;
                        }
                }
                if (done) {
                        break;
                }
        }

        int ans = outputs[0] * outputs[1] * outputs[2];

        printf("Part 2: %d\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstrs = ll->length;
        input.instrs = malloc(input.numInstrs * sizeof(struct botInstr));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct botInstr instr = {0};

                // Input instruction
                if (str[0] == 'v') {
                        instr.input = true;
                        instr.low = strtol(strtok(str + 6, " "), NULL, 10);
                        strtok(NULL, " ");
                        strtok(NULL, " ");
                        strtok(NULL, " ");
                        instr.id = strtol(strtok(NULL, ""), NULL, 10);
                        input.instrs[i++] = instr;
                        continue;
                }

                instr.id = strtol(strtok(str + 4, " "), NULL, 10);
                strtok(NULL, " ");
                strtok(NULL, " ");
                strtok(NULL, " ");
                char *tok = strtok(NULL, " ");
                if (tok[0] == 'b') {
                        instr.lowBot = true;
                }
                instr.low = strtol(strtok(NULL, " "), NULL, 10);
                strtok(NULL, " ");
                strtok(NULL, " ");
                strtok(NULL, " ");
                tok = strtok(NULL, " ");
                if (tok[0] == 'b') {
                        instr.highBot = true;
                }
                instr.high = strtol(strtok(NULL, ""), NULL, 10);
                input.instrs[i++] = instr;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day10.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day10.txt";
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

