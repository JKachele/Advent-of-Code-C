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

linkedList_s createLinkedList() {
    linkedList_s ll;
    ll.head = NULL;
    ll.tail = NULL;
    return ll;
}

node_s *createSNode(char str[]) {
    node_s *result = malloc(sizeof(node_s));
    strcpy(result->str, str);
    result->prev = NULL;
    result->next = NULL;
    return result;
}

node_s *insertSNodeAtHead(node_s **head, node_s **tail, node_s *nodeToInsert) {
    nodeToInsert->next = *head;
    if (*head == NULL) {
        *tail = nodeToInsert;
    } else {
        (*head)->prev = nodeToInsert;
    }
    *head = nodeToInsert;
    nodeToInsert->prev = NULL;
    return nodeToInsert;
}

node_s *insertSNodeAtTail(node_s **head, node_s **tail, node_s *nodeToInsert) {
    nodeToInsert->prev = *tail;
    if (*tail == NULL) {
        *head = nodeToInsert;
    } else {
        (*tail)->next = nodeToInsert;
    }
    *tail = nodeToInsert;
    nodeToInsert->next = NULL;
    return nodeToInsert;
}

void insertSNodeAfterNode(node_s **tail, node_s *nodeToInsertAfter, node_s *nodeToInsert) {
    nodeToInsert->next = nodeToInsertAfter->next;
    if (nodeToInsert->next == NULL) {
        *tail = nodeToInsert;
    } else {
        nodeToInsert->next->prev = nodeToInsert;
    }
    nodeToInsert->prev = nodeToInsertAfter;
    nodeToInsertAfter->next = nodeToInsert;
}

void insertSNodeBeforeNode(node_s **head, node_s *nodeToInsertBefore, node_s *nodeToInsert) {
    nodeToInsert->prev = nodeToInsertBefore->prev;
    if (nodeToInsert->prev == NULL) {
        *head = nodeToInsert;
    } else {
        nodeToInsert->prev->next = nodeToInsert;
    }
    nodeToInsert->next = nodeToInsertBefore;
    nodeToInsertBefore->prev = nodeToInsert;
}

void removeSNode(node_s **head, node_s **tail, node_s *nodeToRemove) {
    if (*head == nodeToRemove) {
        *head = nodeToRemove->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
        free(nodeToRemove);
        return;
    } else if (*tail == nodeToRemove) {
        *tail = nodeToRemove->prev;
        if (*tail != NULL) {
            (*tail)->next = NULL;
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
    return;
}

node_s *getSNodeAtIndex(node_s *head, int index) {
    node_s *temp = head;
    int i = 0;
    while (temp != NULL) {
        if (i == index) return temp;
        temp = temp->next;
        i++;
    }
    return NULL;
}

node_s *getSNodeWithValue(node_s *head, char str[]) {
    node_s *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->str, str) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

int getSNodeIndex(node_s *node, linkedList_s ll) {
    node_s *temp = ll.head;
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

int getLinkedListLength(linkedList_s ll) {
    node_s *temp = ll.head;
    int i = 0;
    while (temp != NULL) {
        i++;
        temp = temp->next;
    }
    return i;
}

int getLongestNodeValue(linkedList_s ll) {
    node_s *temp = ll.head;
    int i = 0;
    while (temp != NULL) {
        if (strlen(temp->str) > i) {
            i = strlen(temp->str);
        }
        temp = temp->next;
    }
    return i;
}

void printSNodeList(node_s *head) {
    node_s *temp = head;
    while (temp != NULL) {
        printf("[%s]\n", temp->str);
        temp = temp->next;
    }
    // printf("\n");
}
