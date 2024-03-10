// Brandon Kmiec
// CSC 139-06
// 01-30-2024
// wunzip: file decompression

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }// end if
    else if(argc == 2) {
        int i;
        for(i = 1; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");

            if(fp == NULL) {
                printf("wunzip: file1 [file2 ...]\n");
                exit(1);
            }//end if
            else {
                char readBlock[5] = "";
                while(fread(readBlock, 1, 5, fp)) {
                    int numCycles = readBlock[0] + (readBlock[1] << 8) 
                    + (readBlock[2] << 16) + (readBlock[3] << 24);
                    
                    char charToPrint = readBlock[4];
                    
                    for(int i = 0; i < numCycles; i++) {
                        printf("%c", charToPrint);
                    }// end for
                }// end while
            }// end else
        }//end for
    }// end else if
    else {
        int i;
        for(i = 1; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if(fp == NULL) {
                printf("wunzip: cannot open file\n");
                exit(1);
            }// end if
            else {
                char *readLine = NULL;
                size_t length = 0;

                int j = 0;
                while(getline(&readLine, &length, fp) != -1) {
                    int numCycles = 0;
                    numCycles += *(readLine + j);
                    numCycles += *(readLine + j + 1);
                    numCycles += *(readLine + j + 2);
                    numCycles += *(readLine + j + 3);

                    char charToPrint = *(readLine + j + 4);

                    j += 5;

                    int k;
                    for(k = 0; k < numCycles; k++) {
                        printf("%c", charToPrint);
                    }// end for
                }// end while
            }// end else
        }// end for
    }// end else
    return 0;
}// end main
