/*************************************************
 *File----------inputFile.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 19, 2024 14:26:18 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "inputFile.h"
#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


llist *getInputFile(char *fileName) {
    FILE *inputFile = fopen(fileName, "r");     // Open file in read mode
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    llist *ll = llist_create();

    if (inputFile == NULL) {    // If file dosent exist, exit
        exit(EXIT_FAILURE);
    }

    // set "line" to the line in the file and test for end of file
    while ((read = getline(&line, &len, inputFile)) != -1) {
        line[strlen(line) - 1] = '\0';  // Remove newline character from string
        char *nodeData = malloc(BUFFER_SIZE * sizeof(char));
        strncpy(nodeData, line, BUFFER_SIZE);
        llist_add(ll, nodeData, BUFFER_SIZE * sizeof(char));
    }

    fclose(inputFile);
    if (line) {
        free(line);
    }
    return ll;
}

void printInput(void *data) {
    char *str = (char*)data;
    if (data == NULL) {
        return;
    }
    printf("%s", str);
}
