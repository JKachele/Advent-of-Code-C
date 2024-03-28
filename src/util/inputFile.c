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

    while ((read = getline(&line, &len, inputFile)) != -1) {    // set "line" to the line in the file and test for end of file
        line[strlen(line) - 1] = '\0';                          // Remove newline character from string
        llist_create_add_node(ll, line);
    }

    fclose(inputFile);
    if (line) {
        free(line);
    }
    return ll;
}

