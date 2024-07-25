/*************************************************
 *File----------linkedlist.c
 *Project-------Learn-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 11, 2024 10:46:02 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linkedlist.h"

llist *llist_create() {
    llist *ll = (llist*)malloc(sizeof(llist));
    ll->head = NULL;
    ll->tail = NULL;
    ll->length = 0;
    return ll;
}

llNode *llist_create_node(void* data) {
    llNode *node = (llNode*)malloc(sizeof(llNode));
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void llist_free_node(llNode *node) {
    free(node->data);
    free(node);
}

void llist_free(llist *ll) {
    llNode *current = ll->head;
    llNode *next;
    while (current != NULL) {
        next = current->next;
        llist_free_node(current);
        current = next;
    }
    free(ll);
}

llNode *llist_add_node(llist *ll, llNode *nodeToInsert) {
    nodeToInsert->prev = ll->tail;
    if (ll->tail == NULL) {
        ll->head = nodeToInsert;
    } else {
        (ll->tail)->next = nodeToInsert;
    }
    ll->tail = nodeToInsert;
    nodeToInsert->next = NULL;
    ll->length++;
    return nodeToInsert;
}

llNode *llist_add(llist *ll, void *data) {
    llNode *node = llist_create_node(data);
    return llist_add_node(ll, node);
}

void llist_insert_after(llist *ll, llNode *nodeToInsertAfter, llNode *nodeToInsert) {
    nodeToInsert->next = nodeToInsertAfter->next;
    if (nodeToInsert->next == NULL) {
        ll->tail = nodeToInsert;
    } else {
        nodeToInsert->next->prev = nodeToInsert;
    }
    nodeToInsert->prev = nodeToInsertAfter;
    nodeToInsertAfter->next = nodeToInsert;
    ll->length++;
}

void llist_insert_before(llist *ll, llNode *nodeToInsertBefore, llNode *nodeToInsert) {
    nodeToInsert->prev = nodeToInsertBefore->prev;
    if (nodeToInsert->prev == NULL) {
        ll->head = nodeToInsert;
    } else {
        nodeToInsert->prev->next = nodeToInsert;
    }
    nodeToInsert->next = nodeToInsertBefore;
    nodeToInsertBefore->prev = nodeToInsert;
    ll->length++;
}

void llist_remove_node(llist *ll, llNode *nodeToRemove) {
    if (ll->length == 1) {
        ll->head = NULL;
        ll->tail = NULL;
    } else if (ll->head == nodeToRemove) {
        ll->head = nodeToRemove->next;
        if (ll->head != NULL) {
            (ll->head)->prev = NULL;
        }
    } else if (ll->tail == nodeToRemove) {
        ll->tail = nodeToRemove->prev;
        if (ll->tail != NULL) {
            (ll->tail)->next = NULL;
        }
    } else {
        nodeToRemove->prev->next = nodeToRemove->next;
        if (nodeToRemove->next != NULL) {
            nodeToRemove->next->prev = nodeToRemove->prev;
        }
        nodeToRemove->next = NULL;
        nodeToRemove->prev = NULL;
    }
    llist_free_node(nodeToRemove);
    ll->length--;
    return;
}

llNode *llist_get_node(llist *ll, int index) {
    llNode *current = ll->head;
    int i = 0;
    while (current != NULL) {
        if (i == index) return current;
        current = current->next;
        i++;
    }
    return NULL;
}

int llist_get_index(llNode *node, llist *ll) {
    llNode *current = ll->head;
    int i = 0;
    while (current != NULL) {
        if (current == node) {
            return i;
        }
        i++;
        current = current->next;
    }
    return -1;
}

bool llist_equals(llist *ll1, llist *ll2, bool (*equals)(void*, void*)) {
    if (ll1->length != ll2->length) return false;
    if (ll1->head == NULL) return ll2->head == NULL;
    if (ll2->head == NULL) return ll1->head == NULL;
    llNode *cur1 = ll1->head;
    llNode *cur2 = ll2->head;
    while (cur1 != NULL && cur2 != NULL) {
        if (!equals(cur1->data, cur2->data)) {
            return false;
        }
        cur1 = cur1->next;
        cur2 = cur2->next;
    }
    // Make sure both nodes are NULL
    return cur1 == cur2;
}

void llist_print(llist *ll, void (*print)(void*)) {
    llNode *current = ll->head;
    while (current != NULL) {
        printf("[");
        print(current->data);
        printf("] ");
        current = current->next;
    }
    printf("\n");
}

bool intEquals(void *data1, void *data2) {
    int i1 = *(int*)data1;
    int i2 = *(int*)data2;
    return i1 == i2;
}

void printInt(void *data) {
    int i = *(int*)data;
    printf("%d", i);
}

void printStr(void *data) {
    char *str = (char*)data;
    printf("%s", str);
}
