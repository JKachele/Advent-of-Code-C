/*************************************************
 *File----------inputFile.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 19, 2024 14:26:28 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef INPUTFILE_H
#define INPUTFILE_H

#include "linkedlist.h"
#define BUFFER_SIZE 4096

llist *getInputFile(char *fileName);
void printInput(void *data);
int getLongestLine(llist *ll);

#endif

