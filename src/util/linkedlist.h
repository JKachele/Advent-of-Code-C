/*************************************************
 *File----------linkedlist.h
 *Project-------Learn-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 11, 2024 10:45:46 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#define BUFFER_SIZE 256

struct llist_node {
    char str[BUFFER_SIZE];
    struct llist_node *prev;
    struct llist_node *next;
};
typedef struct llist_node llNode;

struct llist {
    llNode *head;
    llNode *tail;
    int length;
};
typedef struct llist llist;

llist *llist_create();
llNode *llist_create_node(char str[]);
llNode *llist_add_node(llist *ll, llNode *nodeToInsert);
llNode *llist_create_add_node(llist *ll, char str[]);
void llist_insert_after(llist *ll, llNode *nodeToInsertAfter, llNode *nodeToInsert);
void llist_insert_before(llist *ll, llNode *nodeToInsertBefore, llNode *nodeToInsert);
void llist_remove_node(llist *ll, llNode *nodeToRemove);
llNode *llist_get_node(llist *ll, int index);
int llist_get_index(llNode *node, llist *ll);
int llist_get_longest_node(llist *ll);
void llist_print(llist *ll);

#endif
