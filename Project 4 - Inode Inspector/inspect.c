// Brandon Kmiec
// CSC 139-06
// 4-26-2024
// Inode Inspector

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

void dispInodeInfo(int, char*[], struct stat);
void dispHelpInfo(char*[]);

int main(int argc, char *argv[]) {
    struct stat fileInfo;
    
    // display help info if command is "./inspect"
    if(argc < 2) {
        dispHelpInfo(argv);
    } // end if

    // display help info if command if "./inspect -?" or "./inspect --help"
    // else, error if unable to access file
    else if(argc == 2 && stat(argv[1], &fileInfo) != 0) {
        if(strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0) {
            dispHelpInfo(argv);
        } // end if
        else {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", argv[1], 
                strerror(errno));
            return 1;
        } // end else
    } // end else if
    
    // error if unable to access file and command contains options
    else if(argc == 3 && stat(argv[2], &fileInfo) != 0) {
        fprintf(stderr, "\nError getting file info for %s: %s\n\n", argv[2],
                strerror(errno));
        return 1;
    } // end else if
    
    // Display inode information
    else if(argc == 2 || (argc == 3 && (strcmp(argv[1], "-i") == 0 
       || strcmp(argv[1], "--inode") == 0))) {
        dispInodeInfo(argc, argv, fileInfo);
    } // end else if

    return 0;
} // end main

void dispInodeInfo(int numArgs, char *args[], struct stat fileInfo) {
    //struct stat fileInfo;
    
    if(numArgs == 2) {
        printf("\nInformation for %s:\n", args[1]);
    } // end if
    if(numArgs == 3) {
        printf("\nInformation for %s:\n", args[2]);
    } // end if
    printf("File Inode: %lu\n", fileInfo.st_ino);
    printf("File Type: ");

    if(S_ISREG(fileInfo.st_mode))
        printf("regular file\n");
    else if(S_ISDIR(fileInfo.st_mode))
        printf("directory\n");
    else if(S_ISCHR(fileInfo.st_mode))
        printf("character device\n");
    else if(S_ISBLK(fileInfo.st_mode))
        printf("block device\n");
    else if(S_ISFIFO(fileInfo.st_mode))
        printf("FIFO (named pipe)\n");
    else if(S_ISLNK(fileInfo.st_mode))
        printf("symbolic link\n");
    else if(S_ISSOCK(fileInfo.st_mode))
        printf("socket\n");
    else
        printf("unknown?\n");

    printf("Number of Hard Links: %lu\n", fileInfo.st_nlink);
    printf("File Size: %lu bytes\n", fileInfo.st_size);
    printf("Last Access Time: %ld\n", fileInfo.st_atime);
    printf("Last Modification Time: %ld\n", fileInfo.st_mtime);
    printf("Last Status Change Time: %ld\n\n", fileInfo.st_ctime);
} // end dispInodeInfo

void dispHelpInfo(char *args[]) {
    // filler char[]'s for formatting
    char filler[] = "                                "; // 32 space characters
    char filler2[] = "                                                       "; // 55 space characters 
    //printf("%d %d", strlen(filler), strlen(filler2));

    printf("\nUsage: %s <FILE_PATH>\n", args[0]);
    printf("       %s <OPTION> <FILE_PATH>\n", args[0]);
    printf("Provide detailed inode information for specified files and directories.\n\n");
    

    printf("Options:\n");

    printf("    -?, --help                  ");
    printf("Display help information about the tool and its option.\n\n");

    printf("    -i, --inode <file_path>     ");
    printf("Display detailed inode information for the specified file.\n\n");

    printf("    -a, --all <directory_path>  ");
    printf("Display inode information for all files within the specified directory.\n");
    printf("%sIf no path is provided, default to the current directory.\n", filler);
    printf("%s(Optional Flag) -r, --recursive for recursive listing.\n\n", filler);

    printf("    -h, --human                 ");
    printf("Output all sizes in kilobytes (K), megabytes (M), or gigabytes (G)\n");
    printf("%sand all dates in a human-readable form.\n\n", filler);

    printf("    -f, --format [text|json]    ");
    printf("Specify the output format. If not specified, default to plain text.\n\n");

    printf("    -l, --log <log_file>        ");
    printf("Log operations to a specified file.\n\n\n");


    printf("Examples:\n");
    
    printf("    %s -?%s", args[0], "                                       ");
    printf("Display the help information.\n\n");

    printf("    %s -i /path/to/file%s", args[0], "                         ");
    printf("Display inode information for /path/to/file.\n\n");
    
    printf("    %s -a /path/to/directory -r%s", args[0], "                 ");
    printf("Display inode information for all files in\n");
    printf("%s/path/to/directory with recursive listing.\n\n", filler2);

    printf("    %s -i /path/to/file -h%s", args[0], "                      ");
    printf("Display inode information for /path/to/file\n");
    printf("%swith human-readable file sizes and dates.\n\n", filler2);

    printf("    %s -i /path/to/file -f json%s", args[0], "                 ");
    printf("Display inode information for /path/to/file\n");
    printf("%swith json format.\n\n", filler2);
    
    printf("    %s -i /path/to/file -l /path/to/logfile%s", args[0], "     ");
    printf("???.\n\n");
} // end dispHelpInfo
