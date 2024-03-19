/*************************************************
 *File----------inputFile
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 19, 2024 14:26:18 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "inputFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


linkedList_s getInputFile(char *fileName) {
    FILE *inputFile = fopen(fileName, "r");     // Open file in read mode
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    linkedList_s ll;

    if (inputFile == NULL) {    // If file dosent exist, exit
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, inputFile)) != -1) {    // set "line" to the line in the file and test for end of file
        line[strlen(line) - 1] = '\0';                          // Remove newline character from string
        node_s *lineNode = createSNode(line);
        insertSNodeAtTail(&(ll.head), &(ll.tail), lineNode);
    }

    fclose(inputFile);
    if (line) {
        free(line);
    }
    return ll;
}

