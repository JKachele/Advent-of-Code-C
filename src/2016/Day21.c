/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Nov 03, 2026 10:31:27 EST
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

enum opperation {
        SWP,    // Swap Positions
        SWL,    // Swap Letters
        ROR,    // Rotate Right
        ROL,    // Rotate Left
        ROP,    // Rotate based on position
        REV,    // Reverse
        MOV     // Move
};

struct instruction {
        enum opperation opp;
        u8 x;
        u8 y;
};

struct input {
        int numInstrs;
        struct instruction *instrs;
};

static char *PASSWD = "abcdefgh";
static char *TEST_PASSWD = "abcde";
static char *SCRAMBLED = "fbgdceah";

void printInstrs(int numInstrs, struct instruction instrs[]) {
        char *opps[] = {"SWP", "SWL", "ROL", "ROR", "ROP", "REV", "MOV"};
        for (int i = 0; i < numInstrs; i++) {
                struct instruction instr = instrs[i];
                enum opperation opp = instr.opp;
                printf("%s ", opps[opp]);
                if (opp == SWP || opp == ROR || opp == ROL || opp == REV || opp == MOV) {
                        printf("%d ", instr.x);
                } else {
                        printf("%c ", instr.x + 'a');
                }
                if (opp == SWP || opp == REV || opp == MOV) {
                        printf("%d", instr.y);
                } else if (opp == SWL) {
                        printf("%c", instr.y + 'a');
                }
                printf("\n");
        }
}

int wordCount(char *str) {
        int numWords = 1;
        bool space = false;
        for (int i = 0; i < (int)strlen(str); i++) {
                if (str[i] == ' ' && !space) {
                        space = true;
                        numWords++;
                } else if (str[i] != ' ') {
                        space = false;
                }
        }
        return numWords;
}

void splitLine(char *line, char *words[], int numWords) {
        char *tok = strtok(line, " ");
        for (int i = 0; i < numWords; i++) {
                words[i] = malloc(strlen(tok) + 1);
                strcpy(words[i], tok);
                tok = strtok(NULL, " ");
        }

}

void freeWords(char *words[], int numWords) {
        for (int i = 0; i < numWords; i++) {
                free(words[i]);
        }
}

void swap(int passwd[], int index1, int index2) {
        int temp = passwd[index1];
        passwd[index1] = passwd[index2];
        passwd[index2] = temp;
}

void rotatePasswd(int passwdLen, int passwd[], int rotate, bool right) {
        int copy[passwdLen];
        for (int i = 0; i < passwdLen; i++) {
                copy[i] = passwd[i];
        }

        for (int i = 0; i < passwdLen; i++) {
                int r;
                if (right) {
                        r = (((i - rotate) % passwdLen) + passwdLen) % passwdLen;
                } else {
                        r = (i + rotate) % passwdLen;
                }
                passwd[i] = copy[r];
        }
}

void rotatePos(int passwdLen, int passwd[], int letter) {
        int rotate = 0;
        for (int j = 0; j < passwdLen; j++) {
                if (passwd[j] == letter) {
                        rotate = j;
                        break;
                }
        }
        if (rotate < 4) {
                rotate++;
        } else {
                rotate += 2;
        }
        rotatePasswd(passwdLen, passwd, rotate, true);
}

void movePos(int passwd[], int from, int to) {
        int temp = passwd[from];
        if (from < to) {
                for (int i = from; i < to; i++) {
                        passwd[i] = passwd[i+1];
                }
        } else {
                for (int i = from; i > to; i--) {
                        passwd[i] = passwd[i-1];
                }
        }
        passwd[to] = temp;
}

void rotatePosReverse(int passwdLen, int passwd[], int letter) {
        // Rotate back 1
        rotatePasswd(passwdLen, passwd, 1, false);

        int ltrIndex = 0;
        for (int j = 0; j < passwdLen; j++) {
                if (passwd[j] == letter) {
                        ltrIndex = j;
                        break;
                }
        }

        // Rotate until position of letter matches number of rotations
        int i = 0;
        for (;;) {
                int numR = ltrIndex;
                if (ltrIndex >= 4) {
                        numR++;
                }
                numR %= passwdLen;
                if (numR == i) {
                        break;
                }
                rotatePasswd(passwdLen, passwd, 1, false);
                i++;
                ltrIndex--;
                if (ltrIndex < 0) {
                        ltrIndex = passwdLen - 1;
                }
        }
}

void part1(struct input *input) {
        int numInstrs = input->numInstrs;
        struct instruction *instrs = input->instrs; 
        // printInstrs(numInstrs, instrs);

        int passwdLen = strlen(PASSWD);
        int passwd[passwdLen];
        for (int i = 0; i < passwdLen; i++) {
                passwd[i] = PASSWD[i] - 'a';
        }

        for (int i = 0; i < numInstrs; i++) {
                struct instruction instr = instrs[i];
                switch (instr.opp) {
                case SWP:
                        swap(passwd, instr.x, instr.y);
                        break;
                case SWL:
                        for (int j = 0; j < passwdLen; j++) {
                                if (passwd[j] == instr.x) {
                                        passwd[j] = instr.y;
                                } else if (passwd[j] == instr.y) {
                                        passwd[j] = instr.x;
                                }
                        }
                        break;
                case ROR:
                        rotatePasswd(passwdLen, passwd, instr.x, true);
                        break;
                case ROL:
                        rotatePasswd(passwdLen, passwd, instr.x, false);
                        break;
                case ROP:
                        rotatePos(passwdLen, passwd, instr.x);
                        break;
                case REV:
                        while (instr.x < instr.y) {
                                swap(passwd, instr.x, instr.y);
                                instr.x++;
                                instr.y--;
                        }
                        break;
                case MOV:
                        movePos(passwd, instr.x, instr.y);
                        break;
                }
        }

        printf("Part 1: Scrambled Password = ");
        for (int i = 0; i < passwdLen; i++) {
                printf("%c", passwd[i] + 'a');
        }
        printf("\n\n");
}

void part2(struct input *input) {
        int numInstrs = input->numInstrs;
        struct instruction *instrs = input->instrs; 
        // printInstrs(numInstrs, instrs);

        int passwdLen = strlen(SCRAMBLED);
        int passwd[passwdLen];
        for (int i = 0; i < passwdLen; i++) {
                passwd[i] = SCRAMBLED[i] - 'a';
        }

        // Swap and reverse opperations are same in reverse
        // rotate right/left are just the opposite direction
        // Move is same with swapped x and y
        // Rotate based on pos requires new logic
        for (int i = numInstrs - 1; i >= 0; i--) {
                struct instruction instr = instrs[i];
                switch (instr.opp) {
                case SWP:
                        swap(passwd, instr.x, instr.y);
                        break;
                case SWL:
                        for (int j = 0; j < passwdLen; j++) {
                                if (passwd[j] == instr.x) {
                                        passwd[j] = instr.y;
                                } else if (passwd[j] == instr.y) {
                                        passwd[j] = instr.x;
                                }
                        }
                        break;
                case ROR:
                        rotatePasswd(passwdLen, passwd, instr.x, false);
                        break;
                case ROL:
                        rotatePasswd(passwdLen, passwd, instr.x, true);
                        break;
                case ROP: // NEW!!
                        rotatePosReverse(passwdLen, passwd, instr.x);
                        break;
                case REV:
                        while (instr.x < instr.y) {
                                swap(passwd, instr.x, instr.y);
                                instr.x++;
                                instr.y--;
                        }
                        break;
                case MOV:
                        movePos(passwd, instr.y, instr.x);
                        break;
                }
        }

        printf("Part 2: Unscrambled Password = ");
        for (int i = 0; i < passwdLen; i++) {
                printf("%c", passwd[i] + 'a');
        }
        printf("\n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numInstrs = ll->length;
        input.instrs = malloc(input.numInstrs * sizeof(struct instruction));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                int numWords = wordCount(str);
                char *words[numWords];
                splitLine(str, words, numWords);

                struct instruction instr;

                if (words[0][0] == 's') {
                        // Swap
                        if (words[1][0] == 'p') {
                                // Swap Position
                                instr.opp = SWP;
                                instr.x = strtol(words[2], NULL, 10);
                                instr.y = strtol(words[5], NULL, 10);
                        } else {
                                // Swap Letters
                                instr.opp = SWL;
                                instr.x = words[2][0] - 'a';
                                instr.y = words[5][0] - 'a';
                        }
                } else if (words[0][0] == 'r' && words[0][1] == 'o') {
                        // Rotate
                        if (words[1][0] == 'b') {
                                // Rotate based on pos
                                instr.opp = ROP;
                                instr.x = words[6][0] - 'a';
                        } else {
                                // Rotate Left/Right
                                if (words[1][0] == 'r') {
                                        instr.opp = ROR;
                                } else {
                                        instr.opp = ROL;
                                }
                                instr.x = strtol(words[2], NULL, 10);
                        }
                } else if (words[0][0] == 'r' && words[0][1] == 'e') {
                        // Reverse
                        instr.opp = REV;
                        instr.x = strtol(words[2], NULL, 10);
                        instr.y = strtol(words[4], NULL, 10);
                } else {
                        // Move
                        instr.opp = MOV;
                        instr.x = strtol(words[2], NULL, 10);
                        instr.y = strtol(words[5], NULL, 10);
                }

                input.instrs[i++] = instr;

                freeWords(words, numWords);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day21.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                PASSWD = TEST_PASSWD;
        } else {
                char *file = "assets/inputs/2016/Day21.txt";
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

