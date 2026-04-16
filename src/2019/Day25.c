/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 16, 2026 14:02:48 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/talist.h"
#include "../util/Intcode2019.h"
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 32768

struct input {
        talint64 intcode;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);
        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        haltmode state = {0};
        char userInput[256];

        for (;;) {
                state = runIntcode(&intcode, state, &inputs, &outputs);

                for (int i = 0; i < (int)outputs.length; i++) {
                        printf("%c", (char)outputs.array[i]);
                }
                tal_destroy(outputs);
                if (state.halt)
                        break;

                fgets(userInput, sizeof(userInput), stdin);
                if (strcmp(userInput, "exit\n") == 0)
                        break;
                for (int i = 0; userInput[i] != 0; i++) {
                        tal_add(inputs, userInput[i]);
                }
        }
        printf("Exited!\n");
        tal_destroy(intcode);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                int64 num = strtoll(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        part1(&input);

        return 0;
}

