#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  /*
  char *str = "abcdefg";
  char *newStr[strlen(str)];
  
  //strcpy(&newStr, &str[0]);
  
  for(int i = 0; i < strlen(str); i++) {
  	*newStr[i] = &str[i];
  }
  
  printf("%s\n", &str[0]);
  printf("%s\n", &newStr[0]);
  */

  char readLine[] = "aaaabbbbbbbcccdaaa";
  size_t length = strlen(readLine);
  char compressedStr[] = "";
  int count = 1;
  for(int i = 0; i < length; i++) {
  	char current = *(readLine + i);
    char next = *(readLine + i + 1);
    //count++;
    
    printf("%c %c %d\n", current, next, count);
    if(current != next) {
    	//char strToAdd[80] = "";
        //sprintf(strToAdd, "%d%s", count, &current);
        //size_t addLength = strlen(strToAdd);
        //strcat(compressedStr, strToAdd);
        char temp[80];
        sprintf(temp,"%d%c", count, current);
        strcat(compressedStr, temp);
        count = 1;
        printf("%s\n",compressedStr);
    }
    else {
    	count++;
    }
  }
  
  //printf("%s", readLine);
  //for(int i = 0; i < strlen(readLine); i++)
    //printf("%s", readLine[i]);
  printf("\n\n");
  //for(int i = 0; i < strlen(compressedStr); i++)
    printf("%s", compressedStr);
  //printf("%p\n%s", readLine, compressedStr);
  
  return 0;
}