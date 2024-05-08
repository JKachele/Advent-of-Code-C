/*************************************************
 *File----------Day8.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 24, 2024 08:31:01 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

typedef struct {
    int left;
    int right;
} node;

typedef struct {
    char *index;
    char *left;
    char *right;
} charNode;

const int AtlasSize = 26*26*26;

node *nodeInit() {
    node *n = malloc(sizeof(node));
    n->right = 0;
    n->left = 0;
    return n;
}

int charIndex(char *indexStr) {
    int index1 = indexStr[0] - 65;
    int index2 = indexStr[1] - 65;
    int index3 = indexStr[2] - 65;
    return (index1 * 26 * 26) + (index2 * 26) + index3;
}

charNode *charNodeInit() {
    charNode *n = malloc(sizeof(charNode));
    // n->index = malloc(4);
    // n->left = malloc(4);
    // n->right = malloc(4);
    return n;
}

int nodeIndex(charNode *atlas[], int atlasLen, char *indexStr) {
    int index = 0;
    for (int i = 0; i < atlasLen; i++) {
        if (strcmp(atlas[i]->index, indexStr) == 0) {
            index = i;
            break;
        }
    }
    return index;
}

void printNode(charNode *node) {
    printf("%s, %s, %s\n",
           node->index, node->left, node->right);
}

void part1(llist *ll) {
    llNode *current = ll->head;
    char *dirs = (char*)current->data;
    current = current->next->next;
    node *atlas[AtlasSize];
    // for (int i = 0; i < AtlasSize; i++) { atlas[i] = nodeInit(); }
    while(current != NULL) {
        char *nodeStr = (char*)current->data;
        char indexStr[4] = {'\0'};
        char leftStr[4] = {'\0'};
        char rightStr[4] = {'\0'};
        // Node Format: XXX = (XXX, XXX)
        // Indexes:     0      7    12
        strncpy(indexStr, nodeStr,    3);
        strncpy(leftStr,  nodeStr+7,  3);
        strncpy(rightStr, nodeStr+12, 3);
        printf("%s\t%s\t%s\n", indexStr, leftStr, rightStr);
        int index = charIndex(indexStr);
        atlas[index] = nodeInit();
        atlas[index]->left = charIndex(leftStr);
        atlas[index]->right = charIndex(rightStr);
        printf("%d\t%d\t%d\n", index, atlas[index]->left, atlas[index]->right);

        current = current->next;
    }

    // int steps = 0;
    // int index = 0;
    // int dirIndex = 0;
    // while (index != AtlasSize - 1) {
    //     char dir = dirs[dirIndex];
    //     if (dir == 'L') {
    //         index = atlas[index]->left;
    //     } else {
    //         index = atlas[index]->right;
    //     }
    //     dirIndex++;
    //     if (dirIndex == strlen(dirs)) {
    //         dirIndex = 0;
    //     }
    //     steps++;
    // }
    //
    // printf("Part 1: Steps = %d\n", steps);
}

void part1Test(llist *ll) {
    llNode *current = ll->head;
    char *dirs = (char*)current->data;
    current = current->next->next;
    int atlasLength = ll->length - 2;
    charNode *atlas[atlasLength];
    int i = 0;
    int startIndex = 0;
    int endIndex = 0;
    while(current != NULL) {
        char *nodeStr = (char*)current->data;
        char *indexStr = malloc(4);
        char *leftStr = malloc(4);
        char *rightStr = malloc(4);
        // Node Format: XXX = (XXX, XXX)
        // Indexes:     0      7    12
        strncpy(indexStr, nodeStr,    3);
        strncpy(leftStr,  nodeStr+7,  3);
        strncpy(rightStr, nodeStr+12, 3);
        indexStr[3] = '\0';
        leftStr[3] = '\0';
        rightStr[3] = '\0';
        atlas[i] = charNodeInit();
        atlas[i]->index = indexStr;
        atlas[i]->left = leftStr;
        atlas[i]->right = rightStr;
        if (strcmp(indexStr, "AAA") == 0) {
            startIndex = i;
        } else if (strcmp(indexStr, "ZZZ") == 0) {
            endIndex = i;
        }
        current = current->next;
        i++;
    }

    // for (i = 0; i < atlasLength; i++) {
    //     printNode(atlas[i]);
    // }
    // printf("Start at %d: ", startIndex);
    // printNode(atlas[startIndex]);
    // printf("End at %d: ", endIndex);
    // printNode(atlas[endIndex]);

    i = startIndex;
    int steps = 0;
    while (i != endIndex) {
        charNode *curNode = atlas[i];
        char dir = dirs[steps % (strlen(dirs))];
        if (steps % (strlen(dirs)) == 0) {
            printf("Repeat!\n");
        }
        // printf("Step %d Index %d: %c -> ", steps, i, dir);
        // printNode(curNode);
        char *next;
        if (dir == 'R') {
            next = curNode->right;
        } else {
            next = curNode->left;
        }
        i = nodeIndex(atlas, atlasLength, next);
        steps++;
    }
    printf("Part 1: Steps = %d\n", steps);

}

void part2(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
    llist *ll = getInputFile("assets/2023/Day8.txt");
    // llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    // part1(ll);
    part1Test(ll);
    part2(ll);

    return 0;
}


