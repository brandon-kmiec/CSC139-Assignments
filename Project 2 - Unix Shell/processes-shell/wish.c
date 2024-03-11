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
void throwErrorMsg(int);

int main(int argc, char* argv[]) {

    char path[] = "/bin/";

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
                processCmd(args, numArgs, path);
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
                processCmd(args, numArgs, path);
            } // end else
        } // end while
        fclose(fp);
    } // end else if

    // error for any other number of command line arguments
    else {
        throwErrorMsg(1);
    } // end else
    return 0;
}//end main


void processCmd(char* args[], int numArgs, char path[]) {
    // exit the shell
    if(strcmp(args[0], "exit") == 0) {
        if(numArgs == 1) {
            exit(EXIT_SUCCESS);
        } // end if
        else {
            throwErrorMsg(0);
        } // end else
    } // end if

    // change directory
    else if(strcmp(args[0], "cd") == 0) {
        char *dir;

        if(numArgs == 2) {
            dir = args[1];
        } // end if
        else {
            throwErrorMsg(0);
        } // end else

        chdir(dir);
    } // end else if

    // change path
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

    // non built in commands
    else {
        
        for(int i = 0; i < numArgs - 1; i++) {
            if(strcmp(args[i], ">") == 0) {
            } // end if
        } // end for

        if(strcmp(args[0], "path") != 0) { 
            strncat(path, "/", 2);
            strncat(path, args[0], strlen(args[0]));
        } // end if        

        int status;
        pid_t pid;
        pid = fork();
        switch(pid) {
            case -1:
                throwErrorMsg(0);
            case 0:
                if(execv(path, args) != 0) {
                    throwErrorMsg(-1);
                } // end if
                break;
            default:
                if(wait(&status) == -1) {
                    throwErrorMsg(0);
                } // end if
                break;
        } // end switch
    } // end if


    //

    return;
} // end processCmd


void throwErrorMsg(int errCode) {
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, error_message, strlen(error_message));
    if(errCode == -1) {
        _exit(0);
    } // end if
    else {
        exit(errCode);
    } // end else
} // end throwErrorMsg
