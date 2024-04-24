/*************************************************
 *File----------arraylist.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 23, 2024 21:31:18 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#define DEFAULT_LIST_SIZE 10

typedef struct {
    long capacity;
    long size;
    void **data;
} arraylist;

arraylist *arraylist_create();

#endif

