/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Apr 21, 2025 15:08:53 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

typedef struct Packet {
        struct packetContents *contents;
        uint64 size;
} Packet;

typedef union PacketValue {
        int64 num;
        struct Packet *list;
} PacketValue;

typedef struct packetContents {
        union PacketValue value;
        bool isList;
} PacketContents;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPacket(Packet *p) {
        printf("[ ");
        for (uint64 i = 0; i < p->size; i++) {
                if (p->contents[i].isList) {
                        printPacket(p->contents[i].value.list);
                } else {
                        printf("%ld ", p->contents[i].value.num);
                }
        }
        printf("] ");
}

void printPackets(Packet **packets, uint64 num) {
        if (!Debug) return;
        for (uint64 i = 0; i < num; i++) {
                printPacket(packets[i]);
                printf("\n");
        }
}

void freePacket(Packet *p) {
        for (uint64 i = 0; i < p->size; i++) {
                if (p->contents[i].isList)
                        freePacket(p->contents[i].value.list);
        }

        free(p->contents);
        free(p);
}

void freePackets(Packet **packets, uint64 num) {
        for (uint64 i = 0; i < num; i++) {
                freePacket(packets[i]);
        }
}

bool isNum(char c) {
        return (c >= '0' && c <= '9');
}

uint64 packetLength(char *line) {
        // Check for empty line
        if (strlen(line) == 0) return 0;

        uint64 count = 0;
        int depth = -1;
        uint64 index = 0;
        char curChar = '\0';

        do {
                curChar = line[index];

                switch (curChar) {
                case '[':
                        // If at desired depth, count the element
                        if (depth == 0)
                                count++;
                        // Increase depth
                        depth++;
                        break;
                case ']':
                        //Decrease depth
                        depth--;
                        break;
                case ',':
                        // Do nothing
                        break;
                default:
                        // If curChar is a number
                        if (isNum(curChar)) {
                                if (depth == 0)
                                        count++;
                                // Move to end of number
                                while (isNum(line[index + 1])) {
                                        index++;
                                }
                        } else {
                                printf("ERROR: Unexpected char %c\n", curChar);
                                exit(1);
                        }
                        break;
                }
                index++;
        } while (depth > -1 && curChar != '\0');

        if (depth == -1) {
                return count;
        } else {
                printf("ERROR: Unbalanced Brackets\n");
                exit(1);
        }
}

Packet *parsepacket(char *line) {
        uint64 length = packetLength(line);

        // Allocate packet with array of contents
        Packet *packet = calloc(1, sizeof(Packet));
        PacketContents *contents;
        if (length > 0) {
                contents = (PacketContents*)calloc(length,
                                sizeof(PacketContents));
        } else {
                contents = NULL;
        }

        // Initalize packet
        *packet = (Packet){.contents = contents, .size = length};

        // Return if empty packet
        if (length == 0)
                return packet;

        // Loop through packet contents
        int depth = -1;
        int charIndex = 0;
        int listIndex = 0;
        char curChar = line[charIndex];

        while (curChar != '\0' && curChar != '\n') {
                switch (curChar) {
                case '[':
                        // If on current depth, found nested packet
                        if (depth == 0) {
                                Packet *nested = parsepacket(line + charIndex);
                                contents[listIndex] = (PacketContents){
                                        .value.list = nested,
                                        .isList = true
                                };
                                listIndex++;
                        }
                        depth++;
                        break;
                case ']':
                        depth--;
                        // If move outside of list, parsing has finished
                        if (depth == -1)
                                return packet;
                        break;
                case ',':
                        // Do Nothing
                        break;
                default:
                        // Only parse ints if on top level
                        if (depth != 0)
                                break;
                        int64 num = strtol(line + charIndex, (char**)NULL, 10);
                        contents[listIndex] = (PacketContents){
                                .value.num = num,
                                .isList = false
                        };
                        listIndex++;
                        // Move to end of number
                        while (isNum(line[charIndex + 1])) {
                                charIndex++;
                        }
                        break;
                }
                charIndex++;
                curChar = line[charIndex];
        }

        // Shouldn't reach here on correct input
        printf("ERROR: Malformed Input\n");
        exit(1);
}

// 1: P1 > P2, 0 P1 = P2, -1: P1 < p2
int comparePackets(Packet *p1, Packet *p2) {
        uint64 minSize = p1->size < p2->size ? p1->size : p2->size;

        PacketContents tempContents;
        Packet tempPacket;

        // Loop through packet contents
        for (uint64 i = 0; i < minSize; i++) {
                PacketContents *content1 = &p1->contents[i];
                PacketContents *content2 = &p2->contents[i];

                if (!content1->isList && !content2->isList) {
                        // Both values are integers
                        int64 num1 = content1->value.num;
                        int64 num2 = content2->value.num;
                        if (num1 == num2) continue;
                        return num1 > num2 ? 1 : -1;
                } else if (content1->isList && !content2->isList) {
                        // 1st is a list, 2nd is an integer
                        tempContents = (PacketContents){
                                .value.num = content2->value.num,
                                .isList = false
                        };
                        tempPacket = (Packet){
                                .contents = &tempContents,
                                .size = 1
                        };
                        Packet *nested = content1->value.list;
                        int result = comparePackets(nested, &tempPacket);
                        if (result != 0)
                                return result;
                } else if (!content1->isList && content2->isList) {
                        // 1st is an integer, 2nd is a list
                        tempContents = (PacketContents){
                                .value.num = content1->value.num,
                                .isList = false
                        };
                        tempPacket = (Packet){
                                .contents = &tempContents,
                                .size = 1
                        };
                        Packet *nested = content2->value.list;
                        int result = comparePackets(&tempPacket, nested);
                        if (result != 0)
                                return result;
                } else {
                        // Both values are lists
                        Packet *list1 = content1->value.list;
                        Packet *list2 = content2->value.list;
                        int result = comparePackets(list1, list2);
                        if (result != 0)
                                return result;
                }
        }

        // Check size of packets is no inequality was found
        if (p1->size > p2->size)
                return 1;
        else if (p1->size < p2->size)
                return -1;
        else
                return 0;
}

static void swap(Packet **a, Packet **b) {
        Packet *temp = *a;
        *a = *b;
        *b = temp;
}

// Uses first element in range (first-last) as pivot.
// Moves elements smaller than pivot to front and larger to the back
// Moves pivot to middle and returns pivot index
static int partition(Packet **arr, int first, int last) {
        // Initalize pivot and index pointers
        Packet *p = arr[first];
        int i = first;
        int j = last;

        while (i < j) {
                // Find first element larger than pivot starting from the front
                while (comparePackets(arr[i], p) < 1 && i < last) {
                        i++;
                }

                // Find first element smaller than pivot starting from the back
                while (comparePackets(arr[j], p) > 0 && j > first) {
                        j--;
                }

                // If larger is before smaller, Swap
                if (i < j) {
                        swap(&arr[i], &arr[j]);
                }
        }
        // Once all elements are moved, move the pivot to middle and return
        swap(&arr[first], &arr[j]);
        return j;
}

void sort(Packet **packets, int first, int last) {
        if (first < last) {
                int p = partition(packets, first, last);

                // Recursivly call quicksort for left and right half
                // split by the partition index
                sort(packets, first, p - 1);   // First half
                sort(packets, p + 1, last);    // Last half
        }

}

void part1(llist *ll) {
        uint64 numpackets = ((ll->length + 1) / 3) * 2;
        Packet **packets = (Packet**)calloc(numpackets, sizeof(Packet*));

        int packetIndex = 0;
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                if (strlen(str) != 0) {
                        packets[packetIndex] = parsepacket(str);
                        packetIndex++;
                }

                current = current->next;
        }
        printPackets(packets, numpackets);

        int64 indexSum = 0;
        for (uint64 i = 0; i < numpackets; i += 2) {
                int result = comparePackets(packets[i], packets[i + 1]);
                debugP("%d\n", result);
                if (result == -1) {
                        indexSum += (i / 2) + 1;
                }
        }

        printf("Part 1: Sum of correct indexes: %ld\n\n", indexSum);
        freePackets(packets, numpackets);
}

void part2(llist *ll) {
        // +2 for divider packets
        uint64 numpackets = (((ll->length + 1) / 3) * 2) + 2;
        Packet **packets = (Packet**)calloc(numpackets, sizeof(Packet*));

        int packetIndex = 0;
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                if (strlen(str) != 0) {
                        packets[packetIndex] = parsepacket(str);
                        packetIndex++;
                }

                current = current->next;
        }

        // Add divider packets
        Packet *divPacket1 = parsepacket("[[2]]");
        Packet *divPacket2 = parsepacket("[[6]]");
        packets[numpackets - 2] = divPacket1;
        packets[numpackets - 1] = divPacket2;

        // Sort packets using qsort from stdlib
        // qsort(packets, numpackets, sizeof(Packet*), &qsortComp);
        sort(packets, 0, numpackets - 1);
        printPackets(packets, numpackets);

        int64 divIndexProd = 1;
        for (uint64 i = 0; i < numpackets; i++) {
                if (packets[i] == divPacket1 ||
                                packets[i] == divPacket2) {
                        divIndexProd *= (i + 1);
                }

        }

        printf("Part 2: Product of divider indexes: %ld\n\n", divIndexProd);
        freePackets(packets, numpackets);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day13.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day13.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

