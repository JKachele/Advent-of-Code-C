/*************************************************
 *File----------linkedlist.h
 *Project-------Learn-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 11, 2024 10:45:46 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdlib.h>

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
llNode *llist_create_node(void* data, size_t size);
llNode *llist_add_node(llist *ll, llNode *nodeToInsert);
llNode *llist_add(llist *ll, void *data, size_t size);
void llist_insert_after(llist *ll, llNode *nodeToInsertAfter, llNode *nodeToInsert);
void llist_insert_before(llist *ll, llNode *nodeToInsertBefore, llNode *nodeToInsert);
void llist_remove_node(llist *ll, llNode *nodeToRemove);
llNode *llist_get_node(llist *ll, int index);
int llist_get_index(llNode *node, llist *ll);
void llist_print(llist *ll, void (*print)(void*));

#endif
