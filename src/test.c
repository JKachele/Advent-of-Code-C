/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "util/util.h"

uint32 hash(uint32 x) {
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = (x >> 16) ^ x;
        return x;
}

uint16 getHash(uint32 a, uint32 b) {
        int32 rockHash = hash(a) % UINT16_MAX + 1;
        printf("RockHash: %u\n", rockHash);
        int32 jetHash = hash(b) % UINT16_MAX + 1;
        printf("JetHash: %u\n", jetHash);
        rockHash = rockHash << 16;
        uint16 hashIndex = hash(rockHash * jetHash) % UINT16_MAX + 1;
        return hashIndex;
}

/*
COLISION!
Seen: (4, 951)
State: (0, 2372)
COLISION!
Seen: (0, 1280)
State: (1, 2664)
COLISION!
Seen: (3, 3467)
State: (0, 3683)
COLISION!
Seen: (0, 3744)
State: (2, 4442)
COLISION!
Seen: (3, 4642)
State: (2, 6472)
COLISION!
Seen: (1, 3409)
State: (3, 7158)
COLISION!
Seen: (0, 2461)
State: (4, 7577)
COLISION!
Seen: (4, 7519)
State: (1, 7640)
COLISION!
Seen: (0, 181)
State: (4, 7926)
COLISION!
Seen: (0, 4840)
State: (1, 8300)
COLISION!
Seen: (2, 4295)
State: (1, 8631)
COLISION!
Seen: (1, 3956)
State: (3, 8735)
COLISION!
Seen: (2, 727)
State: (0, 8818)
COLISION!
Seen: (0, 8818)
State: (4, 8836)
COLISION!
Seen: (1, 6737)
State: (0, 9130)
COLISION!
Seen: (3, 7947)
State: (4, 9357)
COLISION!
Seen: (1, 4212)
State: (4, 9873)
COLISION!
Seen: (2, 7440)
State: (4, 5)
COLISION!
Seen: (2, 9573)
State: (3, 199)
COLISION!
Seen: (4, 358)
State: (3, 226)
COLISION!
Seen: (4, 6170)
State: (3, 546)
COLISION!
Seen: (4, 8836)
State: (2, 727)
COLISION!
Seen: (0, 2372)
State: (4, 951)
COLISION!
Seen: (1, 2664)
State: (0, 1280)
*/

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int32 a1 = 1;
        int32 a2 = 2664;
        printf("(%d, %d) - %u\n", a1, a2, getHash(a1, a2));
        a1 = 0;
        a2 = 1280;
        printf("(%d, %d) - %u\n", a1, a2, getHash(a1, a2));

        printf("%d\n", INT16_MAX);

        return 0;
}

