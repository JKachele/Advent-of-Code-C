/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "src/util/linkedlist.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void replaceSubString(char *str, char *find, char *replace) {
    char output[BUFFER_SIZE];
    bool substr = false;
    int start = 0;

    // Set null byte to the end of string length to speed up function
    str[strlen(str)] = '\0';
    find[strlen(find)] = '\0';
    replace[strlen(replace)] = '\0';

    // Check for substring
    int j = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == find[j]) {
            if (!substr) {
                start = i;
                substr = true;
            }
            j++;
            if (find[j] == '\0') {
                break;
            }
        } else {
            substr = false;
            start = 0;
            j = 0;
        }
    }

    // If substring was found, replace, leaving the first and last chars; 
    if (find[j] == '\0' && substr) {
        int i;
        // add the beginning of the string
        for (i = 0; i <= start; i++) {
            output[i] = str[i];
        }
        // add the replace string
        for (j = 0; j < strlen(replace); j++) {
            output[i] = replace[j];
            i++;
        }
        //add the remaining string
        for (j = start + strlen(find) - 1; j < strlen(str); j++) {
            output[i] = str[j];
            i++;
        }
        output[i] = '\0';
        strncpy(str, output, 256);
    }
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    char numString[10][256] = {"zero", "one", "two", "three", "four", "five",
            "six", "seven", "eight", "nine"};
    char nums[10][256] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    char str[256] = "lkjahsdfoneightkjshdf6ljassevensleightdf";

    for (int i = 0; i < 10; i++) {
        replaceSubString(str, numString[i], nums[i]);
    }
    printf("%s\n", str);
    return 0;
}

