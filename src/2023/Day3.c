/*************************************************
 *File----------Day3.c
 *Project-------{{PROJECT}}
 *Author--------Justin Kachele
 *Created-------Sunday Mar 24, 2024 18:55:44 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void part1(linkedList_s ll) {
    node_s *current = ll.head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 1: \n");
}

void part2(linkedList_s ll) {
    node_s *current = ll.head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
    linkedList_s ll = getInputFile("assets/2023/Day3.txt");
    // linkedList_s ll = getInputFile("assets/test.txt");
    printSNodeList(ll.head);

    part1(ll);
    part2(ll);

    return 0;
}

