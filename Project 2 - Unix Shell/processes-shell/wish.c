// Brandon Kmiec
// CSC 139-06
// 02-28-2024
// wish: implement a unix shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

void processCmd(char**, int, char[]);

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

            // continue looping if no command was entered
            // else process the command
            if(numArgs == 0) {
                continue;
            } // end if
            else {
                processCmd(args, numArgs, error_message);
            } // end else
        } // end while
    } // end if

    // run batch mode if two command line arguments
    else if(argc == 2) {
        // open the batch file and read line by line
        FILE *fp = fopen(argv[1], "r");
        while(getline(&userInputCmd, &size, fp) != -1) {
            // get the number of arguments (numArgs) and store the arguments
            // in args
            int numArgs = 0;
            char *delimeter = " \n\t";
            char *args[sizeof(userInputCmd)];
            args[numArgs] = strtok(userInputCmd, delimeter);
            while(args[numArgs] != NULL) {
                args[++numArgs] = strtok((char *) 0, delimeter);
            } // end while

            // continue looping if no command was entered
            // else process the command
            if(numArgs == 0) {
                continue;
            } // end if
            else {
                processCmd(args, numArgs, error_message);
            } // end else
        } // end while
    } // end else if

    // error for any other number of command line arguments
    else {
        fprintf(stderr, error_message, strlen(error_message));
        exit(1);
    } // end else
    return 0;
}//end main


void processCmd(char* args[], int numArgs, char error_message[]) {
    char path[] = "/bin";

    // exit the shell
    if(strcmp(args[0], "exit") == 0) {
        if(numArgs == 1) {
            exit(EXIT_SUCCESS);
        } // end if
        else {
            fprintf(stderr, error_message, strlen(error_message));
            exit(0);
        } // end else
    } // end if

    // change directory
    else if(strcmp(args[0], "cd") == 0) {
        char *dir;

        if(numArgs == 2) {
            dir = args[1];
        } // end if
        else {
            fprintf(stderr, error_message, strlen(error_message));
            exit(0);
        } // end else

        chdir(dir);
    } // end else if

    //path
    else if(strcmp(args[0], "path") == 0) {
        if(numArgs == 1) {
            strcpy(path, "");
        } // end if
        else {
            strcpy(path, "");
            for(int i = 1; i < numArgs; i++) {
                strncat(path, args[i], strlen(args[i]));    
            } // end for
        } // end else
    } // end else if

    // list directory contents
    else {
        //if(strcmp(args[0], "ls") == 0) {
        //char path[] = "/bin/";
        if(strcmp(args[0], "path") != 0) { 
            strncat(path, "/", 2);
            strncat(path, args[0], strlen(args[0]));
        } // end if        
            int status;
            pid_t pid;
            pid = fork();
            switch(pid) {
                case -1:
                    fprintf(stderr, error_message, strlen(error_message));
                    exit(0);
                case 0:
                    if(execv(path, args) != 0) {
                        fprintf(stderr, error_message, strlen(error_message));
                        exit(0);
                    } // end if
                    break;
                default:
                    if(wait(&status) == -1) {
                        fprintf(stderr, error_message, strlen(error_message));
                        exit(0);
                    } // end if
                    break;
            } // end switch
    } // end if


    //

    return;
    } // end processCmd
