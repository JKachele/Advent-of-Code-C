/*************************************************
 *File----------arraylist.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 23, 2024 21:31:13 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "arraylist.h"
#include <stdlib.h>


arraylist *arraylist_create() {
    arraylist *list = (arraylist*)malloc(sizeof(arraylist));

    list->size = 0;
    list->capacity = DEFAULT_LIST_SIZE;
    list->data = calloc(DEFAULT_LIST_SIZE, sizeof(void*));
    list->data[0] = NULL;

    return list;
}

