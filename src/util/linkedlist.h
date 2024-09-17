/*************************************************
 *File----------linkedlist.h
 *Project-------Learn-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 11, 2024 10:45:46 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct llNode {
        void *data;
        struct llNode *prev;
        struct llNode *next;
} llNode;

typedef struct llist {
        llNode *head;
        llNode *tail;
        int length;
} llist;

llist *llist_create();
llNode *llist_create_node(void* data);
void llist_free_node(llNode *node);
void llist_free(llist *ll);
llNode *llist_add_node(llist *ll, llNode *nodeToInsert);
llNode *llist_add(llist *ll, void *data);
void llist_insert_after(llist *ll, llNode *nodeToInsertAfter, llNode *nodeToInsert);
void llist_insert_before(llist *ll, llNode *nodeToInsertBefore, llNode *nodeToInsert);
void llist_remove_node(llist *ll, llNode *nodeToRemove);
llNode *llist_get_node(llist *ll, int index);
int llist_get_index(llNode *node, llist *ll);
bool llist_equals(llist *ll1, llist *ll2, bool (*equals)(void*, void*));
void llist_print(llist *ll, void (*print)(void*));

bool intEquals(void *data1, void *data2);
void printInt(void *data);
void printStr(void *data);

#endif
