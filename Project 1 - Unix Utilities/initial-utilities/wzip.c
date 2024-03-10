// Brandon Kmiec
// CSC 139-06
// 01-31-2024
// wzip: file compression

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }// end if
    else if(argc == 2) {
        int i;
        for(i = 1; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            FILE *fp2 = fopen(argv[i], "r");
            //FILE *fp3 = fopen(argv[i], "r");

            if(fp == NULL || fp2 == NULL /*|| fp3 == NULL*/) {
                printf("wzip: cannot open file\n");
                exit(1);
            }// end if
            else {
                char *readLine = NULL;
                size_t length = 0;

                char *nextReadLine = NULL;
                size_t nextLength = 0;
                getline(&nextReadLine, &nextLength, fp2);

                //char *nextnextReadLine = NULL;
                //size_t nextnextLength = 0;
                //getline(&nextnextReadLine, &nextnextLength, fp3);
                //getline(&nextnextReadLine, &nextnextLength, fp3);

                while(getline(&readLine, &length, fp) != -1) {
                    getline(&nextReadLine, &nextLength, fp2);
                    //getline(&nextnextReadLine, &nextnextLength, fp3);

                    int count = 1;

                    char curLast = *(readLine + strlen(readLine) - 1);
                    char nextFirst = *(nextReadLine);
                    //char nextnextFirst = *(nextnextReadLine);
                    /*if(strncmp(&nextFirst, &curLast, 1) == 0 && !feof(fp2)) {
                        count = 2;

                        char strToAdd[4] = "";
                        strToAdd[3] = count >> 24;
                        strToAdd[2] = count >> 16;
                        strToAdd[1] = count >> 8;
                        strToAdd[0] = count;

                        //printf("test");
                        fwrite(strToAdd, 1, sizeof(strToAdd), stdout);
                        fwrite(&curLast, 1, sizeof(curLast), stdout);
                    }// end if
                    else {*/
                        //int count = 1;

                        int len = strlen(readLine);
                        int j;      
                        for(j = 0; j < len; j++) {
                            char current = *(readLine + j);
                            char next = *(readLine + j + 1);

                            if(current != next) {
                                char strToAdd[4] = "";
                                
                                if(strncmp(&nextFirst, &curLast, 1) == 0 && current == curLast && !feof(fp2)) {
                                    //if(strncmp(&nextnextFirst, &curLast, 1) == 0 && !feof(fp3)) {
                                    //    count = 3;
                                    //}// end if
                                    //else {
                                        count = 2;
                                    //}// end else
                                    getline(&readLine, &length, fp);
                                    getline(&nextReadLine, &nextLength, fp2);
                                    //getline(&nextnextReadLine, &nextnextLength, fp3);
                                }// end if

                                strToAdd[3] = count >> 24;
                                strToAdd[2] = count >> 16;
                                strToAdd[1] = count >> 8;
                                strToAdd[0] = count;

                                fwrite(strToAdd, 1, sizeof(strToAdd), stdout);
                                fwrite(&current, 1, sizeof(current), stdout);

                                count = 1;
                            }// end if
                            else {
                                count++;
                            }// end else
                        }// end for
                   // }// end else
                }// end while
            }// end else
            fclose(fp);
            fclose(fp2);
            //fclose(fp3);
        }// end for
    }// end else if
    else {
        char str[80] = "";

        int i;
        for(i = 1; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if(fp == NULL) {
                printf("wzip: cannot open file\n");
                exit(1);
            }// end if
            else {
                char *readLine = NULL;
                size_t length = 0;

                while(getline(&readLine, &length, fp) != -1) {
                    strncat(str, readLine, strlen(readLine));
                }// end while
            }// end else
            fclose(fp);
        }// end for
        int count = 1;
        int len = strlen(str);
        int j;
        for(j = 0; j < len; j++) {
            char current = str[j];
            char next = str[j + 1];
            if(current != next) {
                char strToAdd[4] = "";
                sprintf(strToAdd, "%c", count);
                fwrite(strToAdd, 1, sizeof(strToAdd), stdout);
                fwrite(&current, 1, sizeof(current), stdout);
                count = 1;
            }// end if
            else {
                count++;
            }// end else
        }// end for
    }// end else

    return 0;
}// end main
