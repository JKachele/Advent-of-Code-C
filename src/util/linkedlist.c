/*************************************************
 *File----------linkedlist.c
 *Project-------Learn-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 11, 2024 10:46:02 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

llist *llist_create() {
    llist *ll;
    ll->head = NULL;
    ll->tail = NULL;
    ll->length = 0;
    return ll;
}

llNode *llist_create_node(char str[]) {
    llNode *result = malloc(sizeof(llNode));
    strcpy(result->str, str);
    result->prev = NULL;
    result->next = NULL;
    return result;
}

// llNode *insertSNodeAtHead(llNode **head, llNode **tail, llNode *nodeToInsert) {
//     nodeToInsert->next = *head;
//     if (*head == NULL) {
//         *tail = nodeToInsert;
//     } else {
//         (*head)->prev = nodeToInsert;
//     }
//     *head = nodeToInsert;
//     nodeToInsert->prev = NULL;
//     return nodeToInsert;
// }

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

llNode *llist_create_add_node(llist *ll, char str[]) {
    llNode *node = llist_create_node(str);
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
    if (ll->head == nodeToRemove) {
        ll->head = nodeToRemove->next;
        if (ll->head != NULL) {
            (ll->head)->prev = NULL;
        }
        free(nodeToRemove);
        return;
    } else if (ll->tail == nodeToRemove) {
        ll->tail = nodeToRemove->prev;
        if (ll->tail != NULL) {
            (ll->tail)->next = NULL;
        }
        free(nodeToRemove);
        return;
    }else {
        nodeToRemove->prev->next = nodeToRemove->next;
        if (nodeToRemove->next != NULL) {
            nodeToRemove->next->prev = nodeToRemove->prev;
        }
        nodeToRemove->next = NULL;
        nodeToRemove->prev = NULL;
    }
    free(nodeToRemove);
    ll->length--;
    return;
}

llNode *llist_get_node(llist *ll, int index) {
    llNode *temp = ll->head;
    int i = 0;
    while (temp != NULL) {
        if (i == index) return temp;
        temp = temp->next;
        i++;
    }
    return NULL;
}

// llNode *getSNodeWithValue(llNode *head, char str[]) {
//     llNode *temp = head;
//     while (temp != NULL) {
//         if (strcmp(temp->str, str) == 0) return temp;
//         temp = temp->next;
//     }
//     return NULL;
// }

int llist_get_index(llNode *node, llist *ll) {
    llNode *temp = ll->head;
    int i = 0;
    while (temp != NULL) {
        if (temp == node) {
            return i;
        }
        i++;
        temp = temp->next;
    }
    return -1;
}

int llist_get_longest_node(llist *ll) {
    llNode *temp = ll->head;
    int i = 0;
    while (temp != NULL) {
        if (strlen(temp->str) > i) {
            i = strlen(temp->str);
        }
        temp = temp->next;
    }
    return i;
}

void llist_print(llist *ll) {
    llNode *temp = ll->head;
    while (temp != NULL) {
        printf("[%s]\n", temp->str);
        temp = temp->next;
    }
    // printf("\n");
}
