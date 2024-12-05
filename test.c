/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <regex.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        char* str = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
        // char* str = "A8B";
        char* regexStr = "mul\\([[:digit:]]+,[[:digit:]]+\\)";
        // char* regexStr = "A[0-9]+[[:alpha:]]+";


        regex_t testRegex;
        if (regcomp(&testRegex, regexStr, REG_EXTENDED) != 0) {
                fprintf(stderr, "Regex Compilation Error\n");
                return EXIT_FAILURE;
        }

        regmatch_t rm[1];
        if (regexec(&testRegex, str, 1, rm, 0) == 0) {
                printf("Regex Found!\n");
                printf("<<%s>>\n", str);
                printf("String: <<%.*s>>\n", (int)(rm[0].rm_eo - rm[0].rm_so), str + rm[0].rm_so);
        } else {
                printf("No Regex Found\n");
                return 0;
        }

        return 0;
}

