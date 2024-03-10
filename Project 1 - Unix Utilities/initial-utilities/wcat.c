// Brandon Kmiec
// CSC 139-06
// 01-30-2024
// wcat: read a file and print out its contents

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int i;
    for(i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        char str[80];
        int length = 80;

        if(fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }// end if
        else {
            while(fgets(str, length, fp) != NULL) {
                printf("%s", str);
            }// end while
        }// end else

        fclose(fp);
    }// end for

return 0;
}//end main
