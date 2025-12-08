/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 08, 2025 13:40:10 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/talist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(int) tllint;

typedef struct box {
        ivec3 pos;
        int net;
} box;

typedef struct pair {
        ivec2 boxes;
        double dist;
} pair;

typedef struct net {
        int netId;
        tllint boxes;
} net;
typedef tal(net) talnet;

struct input {
        int numBoxes;
        box *boxes;
};

static bool Testing = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Testing)
                vprintf(format, args);
        va_end(args);
}

void printNet(net net) {
        if (!Testing) return;
        printf("Net %d: Length: %lu, Boxes: ",
                        net.netId, tll_length(net.boxes));
        tll_foreach(net.boxes, it) {
                printf("[%d] ", it->item);
        }
        printf("\n");
}

void printNets(talnet nets) {
        if (!Testing) return;
        for (int i = 0; i < (int)nets.length; i++) {
                net net = nets.array[i];
                printf("Net %d: Length: %lu, Boxes: ",
                                net.netId, tll_length(net.boxes));
                tll_foreach(net.boxes, it) {
                        printf("[%d] ", it->item);
                }
                printf("\n");
        }
}

/********QuickSort********/
void swapPair(pair *a, pair *b) {
        pair temp = *a;
        *a = *b;
        *b = temp;
}

int partitionPair(pair *arr, int first, int last) {
        // Initalize pivot and index pointers
        double p = arr[first].dist;
        int i = first;
        int j = last;

        while (i < j) {
                // Find first element larger than pivot starting from the front
                while (arr[i].dist <= p && i < last) {
                        i++;
                }

                // Find first element smaller than pivot starting from the back
                while (arr[j].dist > p && j > first) {
                        j--;
                }

                // If larger is before smaller, Swap
                if (i < j) {
                        swapPair(&arr[i], &arr[j]);
                }
        }
        // Once all elements are moved, move the pivot to middle and return
        swapPair(&arr[first], &arr[j]);
        return j;
}

void quicksortPairs(pair *arr, int first, int last) {
        if (first < last) {
                int p = partitionPair(arr, first, last);

                // Recursivly call quicksort for left and right half
                // split by the partition index
                quicksortPairs(arr, first, p - 1);   // First half
                quicksortPairs(arr, p + 1, last);    // Last half
        }
}
/******QuickSort End******/

double distance(const ivec3 a, const ivec3 b) {
        double xDiffSq = pow(b.x - a.x, 2);
        double yDiffSq = pow(b.y - a.y, 2);
        double zDiffSq = pow(b.z - a.z, 2);
        return sqrt(xDiffSq + yDiffSq + zDiffSq);
}

void getpairs(int numBoxes, box *boxes, int numPairs, pair *pairs) {
        int p = 0;
        for (int i = 0; i < numBoxes; i++) {
                for (int j = i + 1; j < numBoxes; j++) {
                        pairs[p].boxes.x = i;
                        pairs[p].boxes.y = j;
                        pairs[p].dist = distance(boxes[i].pos, boxes[j].pos);
                        p++;
                }
        }
}

void combineNets(int numBoxes, box *boxes, ivec2 pair, net *netA, net *netB) {
        tllint *netB_boxes = &netB->boxes;

        tll_foreach(*netB_boxes, it) {
                tll_push_back(netA->boxes, it->item);
                boxes[it->item].net = netA->netId;
                tll_remove(*netB_boxes, it);
        }
}

void makeConnection(ivec2 pair, int numBoxes, box *boxes,
                talnet *nets, tllint *netIds) {
        // 4 scenarios for each pair: 
        //      1: Neither box is in a  net
        //      2: One box is in a net
        //      3: Both boxes are in different nets
        //      4: Both boxes are in the same net
        box *boxA = &boxes[pair.x];
        int netA_id = boxA->net;
        box *boxB = &boxes[pair.y];
        int netB_id = boxB->net;

        // Scenario 1: Neither box is in a net
        //      Create new net and add both boxes
        if (netA_id == -1 && netB_id == -1) {
                int netId = nets->length;
                net new = {netId, (tllint)tll_init()};
                tll_push_back(new.boxes, pair.x);
                tll_push_back(new.boxes, pair.y);
                tal_add(*nets, new);
                tll_push_back(*netIds, netId);
                boxA->net = netId;
                boxB->net = netId;
        }

        // Scenario 2: One box is in a net
        //      Add other box to net
        if (netA_id != -1 && netB_id == -1) {
                net *netA = &nets->array[boxA->net];
                tll_push_back(netA->boxes, pair.y);
                boxB->net = boxA->net;
        } else if (netA_id == -1 && netB_id != -1) {
                net *netB = &nets->array[boxB->net];
                tll_push_back(netB->boxes, pair.x);
                boxA->net = boxB->net;
        }

        // Scenario 3: Both boxes are in different nets
        //      Remove all boxes from boxB's net and add to boxA's net
        if (netA_id != -1 && netB_id != -1 && netA_id != netB_id) {
                net *netB = &nets->array[boxB->net];
                net *netA = &nets->array[boxA->net];
                combineNets(numBoxes, boxes, pair, netA, netB);
                tll_foreach(*netIds, it) {
                        if (it->item == netB_id)
                                tll_remove(*netIds, it);
                }
        }

        // Scenario 4: Both boxes are in the same net
        //      Do nothing
}

void part1(struct input input) {
        const int numConnections = Testing ? 10 : 1000;

        int numBoxes = input.numBoxes;
        box *boxes = input.boxes;

        // Get distance of all pairs of boxes and sort from shortest to longest
        int numPairs = numBoxes * (numBoxes - 1) / 2;
        pair *pairs = malloc(numPairs * sizeof(pair));
        getpairs(numBoxes, boxes, numPairs, pairs);
        quicksortPairs(pairs, 0, numPairs-1);

        // Make 1000(10) shortest connections
        talnet nets = tal_init();
        tllint netIds = tll_init();
        for (int i = 0; i < numConnections; i++) {
                makeConnection(pairs[i].boxes, numBoxes, boxes, &nets, &netIds);
        }
        printNets(nets);

        // Get 3 longest connections
        u32 long1 = 0;
        u32 long2 = 0;
        u32 long3 = 0;
        for (int i = 0; i < (int)nets.length; i++) {
                u32 length = tll_length(nets.array[i].boxes);

                if (length > long1) {
                        long3 = long2;
                        long2 = long1;
                        long1 = length;
                } else if (length > long2) {
                        long3 = long2;
                        long2 = length;
                } else if (length > long3) {
                        long3 = length;
                }
        }
        // printf("Longest connections: %u, %u, %u\n", long1, long2, long3);

        u64 longestMult = long1 * long2 * long3;

        printf("Part 1: %ld\n\n", longestMult);
}

void part2(struct input input) {
        int numBoxes = input.numBoxes;
        box *boxes = input.boxes;
        // Reset box net ids
        for (int i = 0; i < numBoxes; i++) {
                boxes[i].net = -1;
        }

        // Get distance of all pairs of boxes and sort from shortest to longest
        int numPairs = numBoxes * (numBoxes - 1) / 2;
        pair *pairs = malloc(numPairs * sizeof(pair));
        getpairs(numBoxes, boxes, numPairs, pairs);
        quicksortPairs(pairs, 0, numPairs-1);

        // Connect boxes until all are in 1 net
        talnet nets = tal_init();
        tllint netIds = tll_init();
        ivec2 lastConnection;
        for (int i = 0; i < numPairs; i++) {
                makeConnection(pairs[i].boxes, numBoxes, boxes, &nets, &netIds);

                // If length of first net is every box, break
                int netLength = tll_length(nets.array[tll_front(netIds)].boxes);
                if (netLength == numBoxes) {
                        lastConnection = pairs[i].boxes;
                        break;
                }
        }
        box b1 = boxes[lastConnection.x];
        box b2 = boxes[lastConnection.y];

        u64 lastConnX = (u64)b1.pos.x * (u64)b2.pos.x;

        printf("Part 2: %lu\n", lastConnX);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int numBoxes = ll->length;
        box *boxes = malloc(numBoxes * sizeof(box));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                boxes[i].pos.x = strtol(strtok(str, ","), NULL, 10);
                boxes[i].pos.y = strtol(strtok(NULL, ","), NULL, 10);
                boxes[i].pos.z = strtol(strtok(NULL, ""), NULL, 10);
                boxes[i].net = -1;

                i++;
        }

        input.numBoxes = numBoxes;
        input.boxes = boxes;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Testing = true;
        } else {
                char *file = "assets/inputs/2025/Day08.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();
        part2(input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

