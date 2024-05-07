// Brandon Kmiec
// CSC 139-06
// 4-26-2024
// Inode Inspector

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

void dispInodeInfo(int, char*[], struct stat, int);
void printPermissions(struct stat);
void dispHelpInfo(char*[]);

int main(int argc, char *argv[]) {
    struct stat fileInfo;

    if(argc == 1) {
        dispHelpInfo(argv);
    } // end if

    else if(argc == 2) {
        int ret = stat(argv[1], &fileInfo);
        if(ret == 0) {
            dispInodeInfo(argc, argv, fileInfo, 0);
        } // end if

        else if(ret != 0) {
            if(strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0) {
                dispHelpInfo(argv);
            } // end if
            else {
                fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                        argv[1], strerror(errno));
                return 1;
            } // end else
        } // end else if
    } // end else if

    // TODO add support for inspect without -i since -i/--inode is optional
    else if(argc == 3) {
        int ret = stat(argv[2], &fileInfo);
        if(ret == 0) {
            if(strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--inode") == 0) {
                dispInodeInfo(argc, argv, fileInfo, 0);
            } // end if
            else if(strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--all") == 0) {
                // TODO (display inode info for all files in directory)
            } // end else if
        } // end if

        else if(ret != 0) {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                    argv[2],strerror(errno));
            return 1;
        } // end else if
    } // end else if

    else if(argc == 4) {
        int ret = stat(argv[2], &fileInfo);
        if(ret == 0) {
            if((strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--all") == 0) &&
                    (strcmp(argv[3], "-r") == 0 || strcmp(argv[3], "--recursive") == 0)) {
                // TODO (display inode info for all files in directory) (unknown what -r, --recursive does)
            } // end if
            else if(strcmp(argv[3], "-h") == 0 || strcmp(argv[3], "--human") == 0) {
                dispInodeInfo(argc, argv, fileInfo, 1);
            } // end else if
            else if(strcmp(argv[3], "-f") == 0 || strcmp(argv[3], "--format") == 0) {
                dispInodeInfo(argc, argv, fileInfo, 0);
            } // end else if
        } // end if

        else if(ret != 0) {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                    argv[2],strerror(errno));
            return 1;
        } // end else if
    } // end else if

    else if(argc == 5) {
        int ret = stat(argv[2], &fileInfo);
        if(ret == 0) {
            if(strcmp(argv[3], "-f") == 0 || strcmp(argv[3], "--format") == 0) {
                if(strcmp(argv[4], "text") == 0) {
                    dispInodeInfo(argc, argv, fileInfo, 0);
                } // end if
                else if(strcmp(argv[4], "json") == 0) {
                    // TODO (display inode info in json)
                } // end else if
            } // end if
            else if(strcmp(argv[3], "-l") == 0 || strcmp(argv[3], "--log") == 0) {
                // TODO (log operations to specified file
            } // end else if
        } // end if

        else if(ret != 0) {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                    argv[2],strerror(errno));
            return 1;
        } // end else if
    } // end else if


    //	else {
    //	} // end else

    /*
    // display help info if command is "./inspect"
    if(argc < 2) {
    dispHelpInfo(argv);
    } // end if

    // display help info if command is "./inspect -?" or "./inspect --help"
    // else, error if unable to access file
    else if(argc == 2 && stat(argv[1], &fileInfo) != 0) {
    if(strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0) {
    dispHelpInfo(argv);
    } // end if
    else {
    fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
    argv[1], strerror(errno));
    return 1;
    } // end else
    } // end else if

    // error if unable to access file and command contains options
    else if(argc == 3 && stat(argv[2], &fileInfo) != 0) {
    fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
    argv[2],strerror(errno));
    return 1;
    } // end else if

    // Display inode information
    else if(argc == 2) {
    dispInodeInfo(argc, argv, fileInfo);
    } // end else if
    else if(argc == 3 && (strcmp(argv[1], "-i") == 0 
    || strcmp(argv[1], "--inode") == 0)) {

    } // end else if


    // TODO
    // Display inode information for all files in a directory
    else if(argc > 2 && (strcmp(argv[1], "-a") == 0 || 
    strcmp(argv[1], "--all") == 0)) {
    if(argc > 3 && (strcmp(argv[3], "-r") == 0 ||
    strcmp(argv[3], "--recursive") == 0)) {

    } // end if
    else {

    } // end else
    } // end else if

    // TODO
    // Display inode information with human-readable form
    else if(argc > 2 ) {
    }
    */
    return 0;
} // end main

void dispInodeInfo(int numArgs, char *args[], struct stat fileInfo, int human) {
    //struct stat fileInfo;

    if(numArgs == 2) {
        printf("\nInformation for %s:\n", args[1]);
    } // end if
    if(numArgs >= 3) {
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

    printf("File Permissions: ");
    printPermissions(fileInfo);

    printf("Number of Hard Links: %lu\n", fileInfo.st_nlink);
    
    printf("File Owner User ID: %d\n", fileInfo.st_uid);
    printf("File Owner Group ID: %d\n", fileInfo.st_gid);
    
    if(human == 0) {
        printf("File Size: %lu bytes\n", fileInfo.st_size);
        printf("Last Access Time: %ld\n", fileInfo.st_atime);
        printf("Last Modification Time: %ld\n", fileInfo.st_mtime);
        printf("Last Status Change Time: %ld\n\n", fileInfo.st_ctime);
    } // end if
    else if(human == 1) {
        float size = fileInfo.st_size;

        time_t atime = fileInfo.st_atime;
        time_t mtime = fileInfo.st_mtime;
        time_t ctime = fileInfo.st_ctime;
        struct tm ts;
        char buf[80];

        printf("File Size: ");
        if(size < 1024)
            printf("%0.0f bytes\n", size);
        else if(size >= 1024 && size < 1048576) {
            if((int)size % 1024 == 0)
                printf("%dK\n", ((int)size / 1024));
            else
                printf("%.2fK\n",(float)(size / 1024));
        } // end else if
        else if(size >= 1048576 && size < 1073741824) {
            if((int)size % 1048576 == 0)
                printf("%dM\n", ((int)size / 1048576));
            else
                printf("%.2fM\n", (float)(size / 1048576));
        } // end else if
        else {
            if((int)size % 1073741824 == 0)
                printf("%dG\n", ((int)size / 1073741824));
            else
                printf("%.2fG\n", (float)(size / 1073741824));
        } // end else

        // in strftime: might need to only be "%Y-%m-%d %H:%M:%S"
        ts = *localtime(&atime);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        printf("Last Access Time: %s\n", buf);
        
        ts = *localtime(&mtime);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        printf("Last Modification Time: %s\n", buf);
        
        ts = *localtime(&ctime);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        printf("Last Status Change Time: %s\n\n", buf);
    } // end else if
} // end dispInodeInfo

void printPermissions(struct stat fileInfo) { 
    /*
    // directory
    if(S_ISDIR(fileInfo.st_mode))
        printf("d");
    else
        printf("-");
    */
    // owner read
    if(S_IRUSR & fileInfo.st_mode)
        printf("r");
    else
        printf("-");
    // owner write
    if(S_IWUSR & fileInfo.st_mode)
        printf("w");
    else
        printf("-");
    // owner execute
    if(S_IXUSR & fileInfo.st_mode)
        printf("x");
    else
        printf("-");
    // group read
    if(S_IRGRP & fileInfo.st_mode)
        printf("r");
    else
        printf("-");
    // group write
    if(S_IWGRP & fileInfo.st_mode)
        printf("w");
    else
        printf("-");
    // group execute
    if(S_IXGRP & fileInfo.st_mode)
        printf("x");
    else
        printf("-");
    // other read
    if(S_IROTH & fileInfo.st_mode)
        printf("r");
    else
        printf("-");
    // other write
    if(S_IWOTH & fileInfo.st_mode)
        printf("w");
    else
        printf("-");
    // other execute
    if(S_IXOTH & fileInfo.st_mode)
        printf("x");
    else
        printf("-");

    printf("\n");
} // end printPermissions

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
