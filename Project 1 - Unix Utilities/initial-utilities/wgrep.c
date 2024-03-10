// Brandon Kmiec
// CSC 139-06
// 01-30-2024
// wgrep: print out lines with a user-specified search term

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    char *str = NULL;
    size_t length = 0;

    // no command line arguments
    if(argc == 1){
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }// end if

    char searchTerm[strlen(argv[1])];
    strcpy(searchTerm, argv[1]);

    // search term but no file specified
    int i;
    bool flag = false;
    for(i = 0; i < argc; i++) {
        if(strcmp(argv[i], "./wgrep") == 0 && i + 2 >= argc) {
            flag = true;
        }// end if
    }// end for
    if(flag) {
        while(getline(&str, &length, stdin) != -1) {
            if(strstr(str, searchTerm) != NULL) {
                printf("%s", str);
            }// end if
        }// end while
    }// end if

    // all other cases
    else {
        int j;
        for(j = 2; j < argc; j ++) {
            FILE *fp = fopen(argv[j], "r");

            if(fp == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }// end if
            else {
                while(getline(&str, &length, fp) != -1) {
                    if(strstr(str, searchTerm) != NULL) {
                        printf("%s", str);
                    }// end if
                }// end while
            }// end else
            fclose(fp);
        }// end for
    }// end else

    return 0;
}// end main
