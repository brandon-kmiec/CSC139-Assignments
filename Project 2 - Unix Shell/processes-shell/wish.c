// Brandon Kmiec
// CSC 139-06
// 02-28-2024
// wish: implement a unix shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    char error_message[30] = "An error has occurred\n";

    char *userInputCmd = NULL;
    size_t size = 0;
    
    // run interactive mode if one command line argument
    if(argc == 1) {
        while(true) {

            printf("\nwish> ");
            getline(&userInputCmd, &size, stdin);

            // get the number of arguments (numArgs) and store the arguments
            // in args 
            int numArgs = 0;
            char *delimeter = " \n\t";
            char *args[sizeof(userInputCmd)];
            args[numArgs] = strtok(userInputCmd, delimeter);
            while(args[numArgs] != NULL) {
                args[++numArgs] = strtok((char *) 0, delimeter);
            } // end while

            // continue looping no command was entered
            if(numArgs == 0) {
                continue;
            } // end if


            // exit the shell
            if(strcmp(args[0], "exit") == 0) {
                exit(EXIT_SUCCESS);
            } // end if


        } // end while
    } // end if

    // run batch mode if two command line arguments
    else if(argc == 2) {
    
    } // end else if

    // error for any other number of command line arguments
    else {
        fprintf(stderr, error_message, strlen(error_message));
        exit(1);
    } // end else
    return 0;
}//end main
