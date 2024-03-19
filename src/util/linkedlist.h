/*************************************************
 *File----------linkedlist
 *Project-------Learn-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 11, 2024 10:45:46 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#define BUFFER_SIZE 256

struct node_s {
    char str[BUFFER_SIZE];
    struct node_s *prev;
    struct node_s *next;
};
typedef struct node_s node_s;

struct linkedList_s {
    node_s *head;
    node_s *tail;
};
typedef struct linkedList_s linkedList_s;

node_s *createSNode(char str[]);
node_s *insertSNodeAtHead(node_s **head, node_s **tail, node_s *nodeToInsert);
node_s *insertSNodeAtTail(node_s **head, node_s **tail, node_s *nodeToInsert);
void insertSNodeAfterNode(node_s **tail, node_s *nodeToInsertAfter, node_s *nodeToInsert);
void insertSNodeBeforeNode(node_s **head, node_s *nodeToInsertBefore, node_s *nodeToInsert);
void removeSNode(node_s **head, node_s **tail, node_s *nodeToRemove);
node_s *getSNodeAtIndex(node_s *head, int index);
node_s *getSNodeWithValue(node_s *head, char str[]);
void printSNodeList(node_s *head);

#endif
