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
bool multPaths = false;
int numPaths = 0;

void processCmd(char**, int, char[], char[][2048]);
void processParallelCmd(char**, int, char[], char[][2048]);
void redirect(int, char*[]);
void throwErrorMsg(int);

int main(int argc, char* argv[]) {
    char path[] = "/bin/";
    char multiplePath[2048][2048];

    char *userInputCmd = NULL;
    size_t size = 0;
    
    // create file descriptor for "history.txt"
    int historyFD;
    historyFD = open("history.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    // run interactive mode if one command line argument
    if(argc == 1) {
        // coninue looping until exit command entered
        while(true) {
            printf("\nwish> ");
            getline(&userInputCmd, &size, stdin);
            
            // store userInputCmd in temp for history command            
            char temp[strlen(userInputCmd)];
            strcpy(temp, userInputCmd);
            
            // re-execute a previous command
            if(temp[0] == '!') {
                // get the lineNum of the command to re-execute
                int lineNum;
                temp[0] = '0';
                sscanf(temp, "%d", &lineNum);
                
                // error if lineNum is negative
                if(lineNum - 1 < 0) {
                    throwErrorMsg(0);
                } // end if
                
                // open "history.txt" and error check
                FILE *histFP = fopen("history.txt", "r");
                if(histFP == NULL) {
                    throwErrorMsg(0);
                } // end if

                // scan each line of the file until curLine = lineNum - 1
                else {
                    int curLine = 0;
                    while(getline(&userInputCmd, &size, histFP)) {
                        if(curLine == lineNum - 1) {
                            break;
                        } // end if
                        curLine++;
                    } // end while
                } // end else
            } // end if

            // write the command to "history.txt"
            else {
                write(historyFD, temp, sizeof(temp));
            } // end else

            // add space on both sides of ">"
            if(strstr(userInputCmd, ">") != NULL) {
                char redirectStr[4] = " > ";

                // strtok userInputCmd based on delimeter ">"
                int temp = 0;
                char *tempStr[strlen(userInputCmd)];
                tempStr[temp] = strtok(userInputCmd, ">");
                while(tempStr[temp] != NULL) {
                    tempStr[++temp] = strtok(NULL, ">");
                } // end while

                // allocate memory
                size_t newInCmdSize = strlen(tempStr[0]) + 1;
                for(int i = 0; i < temp; i++) {
                    newInCmdSize += strlen(redirectStr) + strlen(tempStr[i]);
                } // end for
                char *newInCmd = malloc(newInCmdSize);
                if(newInCmd == NULL) {
                    throwErrorMsg(0);
                } // end if

                // reassemble userInputCmd with spaces on each side of ">"
                strcpy(newInCmd, tempStr[0]);
                for(int i = i; i < temp; i++) {
                    strcat(newInCmd, redirectStr);
                    strcat(newInCmd, tempStr[i]);
                } // end for
                userInputCmd = newInCmd;
            } // end if

            // if userrInputCmd contains "&", process parallel commands
            if(strstr(userInputCmd, "&") != NULL) {
                // strtok userInputCmd based on delimeter "&"
                int numCmds = 0;
                char *commands[strlen(userInputCmd)];
                commands[numCmds] = strtok(userInputCmd, "&\n\t");
                while(commands[numCmds] != NULL) {
                    commands[++numCmds] = strtok(NULL, "&\n\t");
                } // end while

                pid_t pid[numCmds];

                // strtok each command in commands based on
                // delimeter " \n\t", then process the command
                for(int i = 0; i < numCmds; i++) {
                    int numArgs = 0;
                    char *delimeter = " \n\t";
                    char *args[strlen(commands[i])];
                    args[numArgs] = strtok(commands[i], delimeter);
                    while(args[numArgs] != NULL) {
                        args[++numArgs] = strtok(NULL, delimeter);
                    } // end while
                    
                    // create a new process
                    int status;
                    pid[i] = fork();
                    if(pid[i] < 0) {
                        throwErrorMsg(0);
                    } // end if
                    else if(pid[i] == 0) {
                        processParallelCmd(args, numArgs, path, multiplePath);
                    } // end else if
                    else {
                        waitpid(pid[i], &status, 0);
                        if(status == 1) {
                            throwErrorMsg(0);
                        } // end if
                    } // end else
                } // end for
            } // end if

            // get the number of arguments (numArgs) and store the arguments
            // in args
            // strtok userInputCmd based on delimeter " \n\t" 
            else {
                int numArgs = 0;
                char *delimeter = " \n\t";
                char *args[strlen(userInputCmd)];
                args[numArgs] = strtok(userInputCmd, delimeter);
                while(args[numArgs] != NULL) {
                    args[++numArgs] = strtok(NULL, delimeter);
                } // end while

                // continue looping if no command was entered
                // else process the command
                if(numArgs == 0) {
                    continue;
                } // end if
                else {
                    processCmd(args, numArgs, path, multiplePath);
                } // end else
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

        // process each line in batch file
        while(getline(&userInputCmd, &size, fp) != -1) {
            // store userInputCmd in temp for history command
            char temp[strlen(userInputCmd)];
            strcpy(temp, userInputCmd);

            // re-execute a previous command
            if(temp[0] == '!') {
                // get the lineNum of the command to re-execute
                int lineNum;
                temp[0] = '0';
                sscanf(temp, "%d", &lineNum);

                // error if lineNum is negative
                if(lineNum - 1 < 0) {
                    throwErrorMsg(0);
                } // end if

                // open "history.txt" and error check
                FILE *histFP = fopen("history.txt", "r");
                if(histFP == NULL) {
                    throwErrorMsg(0);
                } // end if

                // scan each line of the file until curLine = lineNum - 1
                else {
                    int curLine = 0;
                    while(getline(&userInputCmd, &size, histFP)) {
                        if(curLine == lineNum - 1) {
                            break;
                        } // end if
                        curLine++;
                    } // end while
                } // end else
            } // end if

            // write the command to "history.txt"
            else {
                write(historyFD, temp, sizeof(temp));
            } // end else

            // add space on both sides of ">"
            if(strstr(userInputCmd, ">") != NULL) {
                char redirectStr[4] = " > ";

                // strtok userInputCmd based on delimeter ">"
                int temp = 0;
                char *tempStr[strlen(userInputCmd)];
                tempStr[temp] = strtok(userInputCmd, ">");
                while(tempStr[temp] != NULL) {
                    tempStr[++temp] = strtok(NULL, ">");
                } // end while

                // allocate memory
                size_t newInCmdSize = strlen(tempStr[0]) + 1;
                for(int i = 1; i < temp; i++) {
                    newInCmdSize += strlen(redirectStr) + strlen(tempStr[i]);
                } // end for
                char *newInCmd = malloc(newInCmdSize);
                if(newInCmd == NULL) {
                    throwErrorMsg(0);
                } // end if

                // reassemble userInputCmd with spaces on each side of ">" 
                strcpy(newInCmd, tempStr[0]);
                for(int i = 1; i < temp; i++) {
                    strcat(newInCmd, redirectStr);
                    strcat(newInCmd, tempStr[i]);
                } // end for
                userInputCmd = newInCmd;              
            } // end if

            // if userInputCmd contains "&", process parallel commands
            if(strstr(userInputCmd, "&") != NULL) {
                // strtok userInputCmd based on delimeter "&"
                int numCmds = 0;
                char *commands[strlen(userInputCmd)];
                commands[numCmds] = strtok(userInputCmd, "&\n\t");
                while(commands[numCmds] != NULL) {
                    commands[++numCmds] = strtok(NULL, "&\n\t");
                } // end while

                pid_t pid[numCmds];

                // strtok each command in commands based on 
                // delimeter " \n\t", then process the command
                for(int i = 0; i < numCmds; i++) {
                    int numArgs = 0;
                    char *delimeter = " \n\t";
                    char *args[strlen(commands[i])];
                    args[numArgs] = strtok(commands[i], delimeter);
                    while(args[numArgs] != NULL) {
                        args[++numArgs] = strtok(NULL, delimeter);
                    } // end while

                    // create a new process 
                    int status;
                    pid[i] = fork();
                    if(pid[i] < 0) {
                        throwErrorMsg(0);
                    } // end if
                    else if(pid[i] == 0) {
                        processParallelCmd(args, numArgs, path, multiplePath);
                    } // end else if
                    else {
                        waitpid(pid[i], &status, 0);
                        if(status == 1) {
                            throwErrorMsg(0);
                        } // end if
                    } // end else
                } // end for
            } // end if

            // else, get the number of arguments (numArgs) and store the 
            // arguments in args
            else {
                // strtok userInputCmd based on delimeter " \n\t"
                int numArgs = 0;
                char *delimeter = " \n\t";
                char *args[strlen(userInputCmd)];
                args[numArgs] = strtok(userInputCmd, delimeter);
                while(args[numArgs] != NULL) {
                    args[++numArgs] = strtok(NULL, delimeter);
                } // end while

                // continue looping if no command was entered
                // else process the command
                if(numArgs == 0) {
                    continue;
                } // end if
                else {
                    processCmd(args, numArgs, path, multiplePath);
                } // end else
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


// process commands
void processCmd(char *args[], int numArgs, char path[], char multiplePath[][2048]) {
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
        multPaths = false;
        if(numArgs == 1) {
            strcpy(path, "");
        } // end if
        else if(numArgs == 2) {
            strcpy(path, args[1]);
        } // end else if
        else {
            multPaths = true;
            numPaths = 0;
            for(int i = 1; i < numArgs; i++) {
                strcpy(multiplePath[i - 1], args[i]);
                numPaths++;
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

    // output the contents of "history.txt"
    else if(strcmp(args[0], "history") == 0) {
        FILE *fp2 = fopen("history.txt", "r");
        char *line = NULL;
        size_t size = 0;

        if(fp2 == NULL) {
            throwErrorMsg(0);
        } // end if
        else {
            int i = 1;
            while(getline(&line, &size, fp2) != -1) {
                fprintf(stdout, "%d ", i);
                fprintf(stdout, line, strlen(line));
                i++;
            } // end while
        } // end else
    } // end else if

    // non built in commands
    else {
        // copy path to tempPath to avoid issues with path maintaining
        // the set path from a previous command
        char tempPath[strlen(path)];
        strcpy(tempPath, path);

        // process redirection
        redirect(numArgs, args);

        // modify path for execv
        if(!pathChanged) { 
            strcpy(tempPath, "/bin/");
            strcat(tempPath, args[0]);
            if(access(tempPath, X_OK) != 0) {
                strcpy(tempPath, "/usr/bin/");
                strcat(tempPath, args[0]);
                if(access(tempPath, X_OK) != 0) {
                    throwErrorMsg(0);
                } // end if
            } // end if
        } // end if
        else {
            // when multiple paths, check which path exists and is executable, then use that path
            if(multPaths) {
                for(int i = 0; i < numPaths; i++) {
                    strcpy(tempPath, multiplePath[i]);
                    strcat(tempPath, "/");
                    strcat(tempPath, args[0]);
                    if(access(tempPath, X_OK) == 0) {
                        break;
                    } // end if
                } // end for
            } // end if
            else {
                strcat(tempPath, "/");
                strncat(tempPath, args[0], strlen(args[0]));
            } // end else
        } // end else

        // create a new process 
        int status;
        pid_t pid;
        pid = fork();
        switch(pid) {
            case -1:        
                throwErrorMsg(0);
            case 0:
                if(execv(tempPath, args) != 0) {
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


// process parallel commands
void processParallelCmd(char *args[], int numArgs, char path[], char multiplePath[][2048]) {
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
        multPaths = false;
        if(numArgs == 1) {
            strcpy(path, "");
        } // end if
        else if(numArgs == 2) {
            strcpy(path, args[1]);
        } // end else if
        else {
            multPaths = true;
            numPaths = 0;
            for(int i = 1; i < numArgs; i++) {
                strcpy(multiplePath[i - 1], args[i]);
                numPaths++;
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

    // output the contents of "history.txt"
    else if(strcmp(args[0], "history") == 0) {
        FILE *fp2 = fopen("history.txt", "r");
        char *line = NULL;
        size_t size = 0;

        if(fp2 == NULL) {
            throwErrorMsg(0);
        } // end if
        else {
            int i = 1;
            while(getline(&line, &size, fp2) != -1) {
                fprintf(stdout, "%d ", i);
                fprintf(stdout, line, strlen(line));
                i++;
            } // end while
        } // end else
    } // end else if

    // non built in commands
    else {
        // copy path to tempPath to avoid issues with path maintaining
        // the set path from a previous command
        char tempPath[strlen(path)];
        strcpy(tempPath, path);

        // process redirection
        redirect(numArgs, args);

        // modify path for execv
        if(!pathChanged) { 
            strcpy(tempPath, "/bin/");
            strcat(tempPath, args[0]);
            if(access(tempPath, X_OK) != 0) {
                strcpy(tempPath, "/usr/bin/");
                strcat(tempPath, args[0]);
                if(access(tempPath, X_OK) != 0) {
                    throwErrorMsg(0);
                } // end if
            } // end if
        } // end if
        else {
            // when multiple paths, check which path exists and is executable, then use that path
            if(multPaths) {
                for(int i = 0; i < numPaths; i++) {
                    strcpy(tempPath, multiplePath[i]);
                    strcat(tempPath, "/");
                    strcat(tempPath, args[0]);
                    if(access(tempPath, X_OK) == 0) {
                        break;
                    } // end if
                } // end for
            } // end if
            else {
                strcat(tempPath, "/");
                strncat(tempPath, args[0], strlen(args[0]));
            } // end else
        } // end else

        // create a new process 
        /*int status;
          pid_t pid;
          pid = fork();
          switch(pid) {
          case -1:        
          throwErrorMsg(0);
          case 0:
          if(execv(tempPath, args) != 0) {
          throwErrorMsg(-1);
          } // end if
          break;
          default:
          if(wait(&status) == -1) {
          throwErrorMsg(0);
          } // end if
          break;
          } // end switch
          */
        if(execv(tempPath, args) != 0) {
            throwErrorMsg(-1);
        } // end if
    } // end if

    return;
} // end processParallelCmd


// process redirection
void redirect(int numArgs, char* args[]) {
    int outPos = 0; // position of out redirection (>)
    int fd;         // file descriptor

    // loop through arguments to find position of redirection ">"
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

    // open file for redirected output, assign a file descriptor (fd),
    // redirect stdout to fd for redirected output
    if(outPos != 0) {
        if(args[outPos + 1] == NULL) {
            throwErrorMsg(-1);
        } // end if

        fd = open(args[outPos + 1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

        if(fd == -1) {
            throwErrorMsg(-1);
        } // end if

        dup2(1, fd);

        if(close(fd) == -1) {
            throwErrorMsg(0);
        } // end if

        args[outPos] = NULL;
    } // end if

    return;
} // end redirect


// print error message to stderr
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
