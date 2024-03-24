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
#include <fcntl.h>
#include <errno.h>

bool pathChanged = false;

void processCmd(char**, int, char[]);
void redirect(int, char*[]);
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

            if(strstr(userInputCmd, ">") != NULL) {
                char redirectStr[4] = " > ";

                int temp = 0;
                char *tempStr[strlen(userInputCmd)];
                tempStr[temp] = strtok(userInputCmd, ">");
                while(tempStr[temp] != NULL) {
                    tempStr[++temp] = strtok(NULL, ">");
                } // end while

                size_t newInCmdSize = strlen(tempStr[0]) + 1;
                for(int i = 0; i < temp; i++) {
                    newInCmdSize += strlen(redirectStr) + strlen(tempStr[i]);
                } // end for

                char *newInCmd = malloc(newInCmdSize);
                if(newInCmd == NULL) {
                    throwErrorMsg(0);
                } // end if

                strcpy(newInCmd, tempStr[0]);
                for(int i = i; i < temp; i++) {
                    strcat(newInCmd, redirectStr);
                    strcat(newInCmd, tempStr[i]);
                } // end for

                userInputCmd = newInCmd;
            } // end if

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
        if(fp == NULL) {
            throwErrorMsg(1);
        } // end if

        while(getline(&userInputCmd, &size, fp) != -1) {
            if(strstr(userInputCmd, ">") != NULL) {
                char redirectStr[4] = " > ";

                int temp = 0;
                char *tempStr[strlen(userInputCmd)];
                tempStr[temp] = strtok(userInputCmd, ">");
                while(tempStr[temp] != NULL) {
                    tempStr[++temp] = strtok(NULL, ">");
                } // end while

                size_t newInCmdSize = strlen(tempStr[0]) + 1;
                for(int i = 1; i < temp; i++) {
                    newInCmdSize += strlen(redirectStr) + strlen(tempStr[i]);
                } // end for

                char *newInCmd = malloc(newInCmdSize);
                if(newInCmd == NULL) {
                    throwErrorMsg(0);
                } // end if

                strcpy(newInCmd, tempStr[0]);
                for(int i = 1; i < temp; i++) {
                    strcat(newInCmd, redirectStr);
                    strcat(newInCmd, tempStr[i]);
                } // end for

                userInputCmd = newInCmd;              
            } // end if

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
        pathChanged = true;
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

    // display files
    else if(strcmp(args[0], "cat") == 0) {
        int i;
        for(i = 1; i < numArgs; i++) {
            FILE *fp2 = fopen(args[i], "r");
            char *line = NULL;
            size_t size = 0;

            if(fp2 == NULL) {
                throwErrorMsg(0);
            } // end if
            else {
                while(getline(&line, &size, fp2) != -1) {
                    fprintf(stdout, line, strlen(line));
                } // end while
            } // end else
        } // end for
    } // end else if

    // non built in commands
    else {
        //char tempPath[strlen(path)];
        //strcpy(tempPath, path);

        redirect(numArgs, args);
        
        if(/*strcmp(args[0], "path") != 0*/ !pathChanged) { 
            //strncat(path, "/", 2);
            strcpy(path, "/bin/");
            strncat(path, args[0], strlen(args[0]));
        } // end if
        else {
            strcat(path, "/");
            strncat(path, args[0], strlen(args[0]));
        } // end else
        
        //strncat(path, "\n", 2);
        //fprintf(stderr, path, strlen(path));

        int status;
        pid_t pid;
        pid = fork();
        switch(pid) {
            case -1:
                throwErrorMsg(0);
            case 0:
                if(execv(path, args) != 0) {
                    //fprintf(stderr, path, strlen(path));
                    //fprintf(stderr, args[0], strlen(args[0]));
                    //fprintf(stderr, "execv failed: %s\n", strerror(errno));
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

    return;
} // end processCmd


void redirect(int numArgs, char* args[]) {
    int outPos = 0; // position of out redirection (>)
    int fd;         // file descriptor

    for(int i = 0; i < numArgs; i++) {
        if(strcmp(args[i], ">") == 0) {
            if(outPos != 0 || i == 0 || i == numArgs - 1) {
                throwErrorMsg(-1);
            } // end if

            if(strstr(args[i + 1], ".") && strstr(args[i + 2], ".") && i + 2 < numArgs) {
                throwErrorMsg(0);
            } // end if

            outPos = i;
        } // end if
    } // end for

    if(outPos != 0) {
        if(args[outPos + 1] == NULL) {
            throwErrorMsg(-1);
        } // end if


        fd = open(args[outPos + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

        if(fd == -1) {
            throwErrorMsg(-1);
        } // end if

        dup2(1, fd);

        if(close(fd) == -1) {
            throwErrorMsg(0);
        } // end if

        args[outPos] = NULL;
    } // end if

    /*
       for(int i = 0; i < numArgs; i++) {
       if(strcmp(args[i], ">") == 0) {
       if(i == 0 || i == numArgs - 1) {
       throwErrorMsg(-1); // might need to be 0
       } // end if

       if(strstr(args[i + 1], ".") && strstr(args[i + 2], ".") && i + 2 < numArgs) {
       throwErrorMsg(0);
       } // end if

       outPos = i;

       if(outPos != 0 && outPos != numArgs - 1) {
       fd = open(args[outPos + 1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
       if(fd == -1) {
       throwErrorMsg(-1); // might need to be 0
       } // end if

       dup2(fd, 1);

       if(close(fd) == -1) {
       throwErrorMsg(0);
       } // end if

       args[outPos] = NULL;
       } // end if
       } // end if

       } // end for
       */
    return;
} // end redirect


void throwErrorMsg(int errCode) {
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, error_message, strlen(error_message));
    if(errCode == -1) {
        _exit(0);
    } // end if
    else {
        exit(errCode);
    } // end else

    return;
} // end throwErrorMsg
