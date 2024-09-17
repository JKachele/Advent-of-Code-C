/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        char *cfg = "Hello!";

        char cfgUnfold[(strlen(cfg) * 5) + 5];
        strncpy(cfgUnfold, cfg, strlen(cfg));
        int offset = strlen(cfg) + 1;
        for (int i = 1; i < 5; i++) {
                cfgUnfold[(offset * i) - 1] = '?';
                strncpy(cfgUnfold + (offset * i), cfg, strlen(cfg));
        }

        printf("%s\n", cfgUnfold);

        return 0;
}

