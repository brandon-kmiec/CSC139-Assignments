// Brandon Kmiec
// CSC 139-06
// 4-26-2024
// Inode Inspector

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

static int helpFlag = 0;	// flag for if argv contains -?/--help
static int iFlag = 0;		// flag for if argv contains -i/--inspect
static int aFlag = 0;		// flag for if argv contains -a/--all
static int rFlag = 0;		// flag for if argv contains -r/--recursive
static int hFlag = 0;		// flag for if argv contains -h/--human
static int fFlag = 0;		// flag for if argv contains -f/--format
static int textFlag = 0;	// flag for if argv contains text
static int jsonFlag = 0;	// flag for if argv contains json
static int lFlag = 0;		// flag for if argv contains -l/--log
static int lOptionPos = 0;	// position of -l/--log option in argv
static int aOptionPos = 0;  // position of -a/--all option in argv

void dispInodeAll(struct stat, char[]);
void dispInodeInfo(struct stat, char[]);
void printPermissions(struct stat);
void dispHelpInfo(char*[]);

int main(int argc, char *argv[]) {
    struct stat fileInfo;

    // Display help info in argv only contains ./inspect
    if(argc == 1) {
        dispHelpInfo(argv);
        return 0;
    } // end if

    // Set flags based on contents of argv
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
            helpFlag = 1;
        } // end if
        else if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--inspect") == 0) {
            iFlag = 1;
        } // end else if
        else if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {
            aFlag = 1;
            aOptionPos = i;
        } // end else if
        else if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--recursive") == 0) {
            rFlag = 1;
        } // end else if
        else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--human") == 0) {
            hFlag = 1;
        } // end else if
        else if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) {
            fFlag = 1;
        } // end else if
        else if(strcmp(argv[i], "text") == 0) {
            textFlag = 1;
        } // end else if
        else if(strcmp(argv[i], "json") == 0) {
            jsonFlag = 1;
        } // end else if
        else if(strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log") == 0) {
            lFlag = 1;
            lOptionPos = i;
        } // end else if
    } // end for

    // Redirect output to logfile
    if(lFlag == 1) {
        int fd;
        fd = open(argv[lOptionPos + 1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH | S_IRGRP | S_IWGRP);

        // fd error checks
        if(fd == -1) {
            // Error check if /path/to/logfile not specified
            if(argv[lOptionPos + 1] == NULL) {
                fprintf(stderr, "\nError creating log file: /path/to/logfile not specified\n\n");
                return 1;
            } // end if
            // Error check if unable to create logfile
            else {
                fprintf(stderr, "\nError creating log file\n\n");
                return 1;
            } // end else
        } // end if

        // Redirect output
        dup2(fd, 1);

        // Error check if error closing fd 
        if(close(fd) == -1) {
            fprintf(stderr, "\nError closing file descriptor %d\n\n", fd, strerror(errno));
            return 1;
        } // end if
    } // end if

    // Display help info if helpFlag is set
    if(helpFlag == 1) {
        dispHelpInfo(argv);
        return 0;
    } // end if

    // Obtain return value from stat(/path/to/file, &fileInfo)
    int ret;
    // Display inode info if iFlag is set
    if(iFlag == 1) {
        ret = stat(argv[2], &fileInfo);
        
        if(ret == 0) {
            // Display inode info if aFlag is not set
            if(aFlag == 0) {
                dispInodeInfo(fileInfo, argv[2]);
            } // end if
            
            // Error check if aFlag and iFlag are both set
            else {
                fprintf(stderr, "\nError: options -a and -i cannot be used at the same time\n\n");
                return 1;
            } // end else
        } // end if
        
        // Error check if error getting file
        else {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                    argv[2], strerror(errno));
            return 1;
        } // end else
    } // end if

    // Display inode info for all files in a directory if aFlag is set
    else if(aFlag == 1) {
        // Output formatting
        //if(rFlag == 1)
            printf("\n");
        if(jsonFlag == 1)
            printf("[\n");
        
        ret = stat(argv[aOptionPos + 1], &fileInfo);
        
        // Display inode info for files in specified directory
        if(ret == 0) {
            // Error checking if /path/to/directory is not a directory
            if(!S_ISDIR(fileInfo.st_mode)) {
                fprintf(stderr, "\nError getting directory %s: %s\n\n", argv[aOptionPos + 1], "Not a directory");
                return 1;
            } // end if 
                
            dispInodeAll(fileInfo, argv[aOptionPos + 1]);
        } // end if
        
        // Display inode info for files in current directory if no directory specified
        else {
            dispInodeAll(fileInfo, ".");
        } // end else
        
        // Output formatting
        if(jsonFlag == 1)
            printf("]\n\n");
    } // end else if

    // Display inode info if iFlag is not set
    else {
        ret = stat(argv[1], &fileInfo);

        if(ret == 0) {
            // Display inode info if aFlag is not set
            if(aFlag == 0) {
                dispInodeInfo(fileInfo, argv[1]);
            } // end if
        } // end if

        // Error check if error getting file
        else {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                    argv[1], strerror(errno));
            return 1;
        } // end else
    } // end else

    return 0;
} // end main


// Function for processing -a/--all and -r/--recursive options
void dispInodeAll(struct stat fileInfo, char dirPath[]) {
    DIR *dir = opendir(dirPath);
    DIR *next = opendir(dirPath);
    struct dirent *dp;
    struct dirent *nextDP;

    // Process recursive option
    if(rFlag == 1) {
        nextDP = readdir(next);

        // Navigate through files in directory dir
        while((dp = readdir(dir)) != NULL) {
            nextDP = readdir(next);

            // Ignore "." and ".." directories
            if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
                // Create temp and allocate memory
                char *temp;
                temp = (char *)malloc(100);
                
                // Format dirPath and store in temp
                strcpy(temp, dirPath);
                strcat(temp, "/");
                if(strchr(dp->d_name, ' ') != NULL) {
                    strcat(temp, "\'");
                    strcat(temp, dp->d_name);
                    strcat(temp, "\'");
                } // end if
                else {
                    strcat(temp, dp->d_name);
                } // end else

                // Get file info for filePath temp
                stat(temp, &fileInfo);
                
                // If current file is a directory, display inode info and call dispInodeAll() with new directory
                if(S_ISDIR(fileInfo.st_mode)) {
                    dispInodeInfo(fileInfo, temp);
                    dispInodeAll(fileInfo, temp);
                } // end if
                // Else, display inode info
                else {
                    dispInodeInfo(fileInfo, temp);
                } // end else
                
                // Output formatting
                if(jsonFlag == 0)
                    printf("\n");
                else {
                    if(nextDP != NULL)
                        printf(",\n");
                    else
                        printf("\n");
                } // end else

                // Free memory occupied by temp
                free(temp);
            } // end if
        } //end while
    } // end if

    // Process without recursive listing
    else {
        nextDP = readdir(next);

        // Navigate through files in directory dir
        while((dp = readdir(dir)) != NULL) {
            nextDP = readdir(next);

            // Ignore "." and ".." directories
            if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
                // Create temp and allocate memory
                char *temp;
                temp = (char *)malloc(100);
                
                // Format dirPath and store in temp
                strcpy(temp, dirPath);
                strcat(temp, "/");        
                if(strchr(dp->d_name, ' ') != NULL) {
                    strcat(temp, "\'");
                    strcat(temp, dp->d_name);
                    strcat(temp, "\'"); 
                } // end if
                else {
                    strcat(temp, dp->d_name);
                } // end else

                // Get file info for filePath temp
                stat(temp, &fileInfo);  

                // Display inode info for filePath temp  
                dispInodeInfo(fileInfo, temp);
                
                // Output formatting
                if(jsonFlag == 0)
                    printf("\n");
                else { 
                    if(nextDP != NULL)
                        printf(",\n");
                    else
                        printf("\n");
                } // end else
                
                // Free memory occupied by temp
                free(temp);
            } // end if
        } // end while
    } // end else
} // end dispInodeAll


// Function for displaying inode information
void dispInodeInfo(struct stat fileInfo, char filePath[]) {
    // Display inode info in json format
    if(fFlag == 1 && jsonFlag == 1) {
        printf("  {\n");
            // Obtain file path
		printf("    \"filePath\": \"%s\",\n", filePath);
            // Obtain file inode number
        printf("    \"inode\": {\n");
        printf("      \"number\": %lu,\n", fileInfo.st_ino);
            // Obtain file type
        printf("      \"type\": ");
        if(S_ISREG(fileInfo.st_mode))
            printf("\"regular file\",\n");
        else if(S_ISDIR(fileInfo.st_mode))
            printf("\"directory\",\n");
        else if(S_ISCHR(fileInfo.st_mode))
            printf("\"character device\",\n");
        else if(S_ISBLK(fileInfo.st_mode))
            printf("\"block device\",\n");
        else if(S_ISFIFO(fileInfo.st_mode))
            printf("\"FIFO (named pipe)\",\n");
        else if(S_ISLNK(fileInfo.st_mode))
            printf("\"symbolic link\",\n");
        else if(S_ISSOCK(fileInfo.st_mode))
            printf("\"socket\",\n");
        else
            printf("\"unknown?\",\n");
            // Obtain file permissions
        printf("      \"permissions\": \"");
        printPermissions(fileInfo);
        printf("\",\n");
            // Obtain file link count
        printf("      \"linkCount\": %lu,\n", fileInfo.st_nlink);
            // Obtain file uid and gid
        printf("      \"uid\": %d,\n", fileInfo.st_uid);
        printf("      \"gid\": %d,\n", fileInfo.st_gid);
  
        // Display in human-readable values if hFlag is set
        if(hFlag == 1) {
            float size = fileInfo.st_size;

            time_t atime = fileInfo.st_atime;
            time_t mtime = fileInfo.st_mtime;
            time_t ctime = fileInfo.st_ctime;
            struct tm ts;
            char buf[80];
            
            // Obtain and format file size 
            printf("      \"size\": ");
                // Bytes
            if(size < 1024)
                printf("\"%0.0f bytes\",\n", size);
                // Kilobytes
            else if(size >= 1024 && size < 1048576) {
                if((int)size % 1024 == 0)
                    printf("\"%dK\",\n", ((int)size / 1024));
                else
                    printf("\"%.2fK\",\n",(float)(size / 1024));
            } // end else if
                // Megabytes
            else if(size >= 1048576 && size < 1073741824) {
                if((int)size % 1048576 == 0)
                    printf("\"%dM\",\n", ((int)size / 1048576));
                else
                    printf("\"%.2fM\",\n", (float)(size / 1048576));
            } // end else if
                // Gigabytes
            else {
                if((int)size % 1073741824 == 0)
                    printf("\"%dG\",\n", ((int)size / 1073741824));
                else
                    printf("\"%.2fG\",\n", (float)(size / 1073741824));
            } // end else

            // Obtain and format file access time
            ts = *localtime(&atime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("      \"accessTime\": \"%s\",\n", buf);
            
            // Obtain and format file modification time
            ts = *localtime(&mtime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("      \"modificationTime\": \"%s\",\n", buf);
            
            // Obtain and format file status change time
            ts = *localtime(&ctime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("      \"statusChangeTime\": \"%s\"\n", buf);
        } // end if

        // Display non-human-readable values
        else {
            // Obtain file size, access time, modification time, and status change time
            printf("      \"size\": \"%lu bytes\",\n", fileInfo.st_size);
            printf("      \"accessTime\": \"%ld\",\n", fileInfo.st_atime);
            printf("      \"modificationTime\": \"%ld\",\n", fileInfo.st_mtime);
            printf("      \"statusChangeTime\": \"%ld\"\n", fileInfo.st_ctime);
        } // end else

        // Output formatting
        if(aFlag == 0) {
            printf("    }\n");
            printf("  }\n\n");
        } // end if
        else {
            printf("    }\n");
            printf("  }");
        } // end else
    } // end if

    // Display inode info in text format
    else {
        // Output formatting
        if(aFlag == 0)
            printf("\n");

            // Obtain file path
        printf("Information for %s:\n", filePath);
            // Obtain file inode number
        printf("File Inode: %lu\n", fileInfo.st_ino);
            // Obtain file type
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
            // Obtain file permissions
        printf("File Permissions: ");
        printPermissions(fileInfo);
            // Obtain file link count
        printf("\nNumber of Hard Links: %lu\n", fileInfo.st_nlink);
            // Obtain file uid and gid
        printf("File Owner User ID: %d\n", fileInfo.st_uid);
        printf("File Owner Group ID: %d\n", fileInfo.st_gid);

        // Display non-human-readable values
        if(hFlag == 0) {
            // Obtain file size, access time, modification time, and status change time
            printf("File Size: %lu bytes\n", fileInfo.st_size);
            printf("Last Access Time: %ld\n", fileInfo.st_atime);
            printf("Last Modification Time: %ld\n", fileInfo.st_mtime);
            printf("Last Status Change Time: %ld\n", fileInfo.st_ctime);
        } // end if

        // Display in human-readable values if hFlag is set
        else{
            float size = fileInfo.st_size;

            time_t atime = fileInfo.st_atime;
            time_t mtime = fileInfo.st_mtime;
            time_t ctime = fileInfo.st_ctime;
            struct tm ts;
            char buf[80];

            // Obtain and format file size
            printf("File Size: ");
                // Bytes
            if(size < 1024)
                printf("%0.0f bytes\n", size);
                // Kilobytes
            else if(size >= 1024 && size < 1048576) {
                if((int)size % 1024 == 0)
                    printf("%dK\n", ((int)size / 1024));
                else
                    printf("%.2fK\n",(float)(size / 1024));
            } // end else if
                // Megabytes
            else if(size >= 1048576 && size < 1073741824) {
                if((int)size % 1048576 == 0)
                    printf("%dM\n", ((int)size / 1048576));
                else
                    printf("%.2fM\n", (float)(size / 1048576));
            } // end else if
                // Gigabytes
            else {
                if((int)size % 1073741824 == 0)
                    printf("%dG\n", ((int)size / 1073741824));
                else
                    printf("%.2fG\n", (float)(size / 1073741824));
            } // end else

            // Obtain and format file access time
            ts = *localtime(&atime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("Last Access Time: %s\n", buf);

            // Obtain and format file modification time
            ts = *localtime(&mtime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("Last Modification Time: %s\n", buf);

            // Obtain and format file status change time
            ts = *localtime(&ctime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("Last Status Change Time: %s\n", buf);

            // Output formatting
            if(aFlag == 0)
                printf("\n");
        } // end else if
    } // end else
} // end dispInodeInfo


// Function to output file permissions
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
} // end printPermissions


// Function to display help info
void dispHelpInfo(char *args[]) {
    // filler char[]'s for formatting
    char filler[] = "                                "; // 32 space characters
    char filler2[] = "                                                       "; // 55 space characters 

    // Usage for ./inspect
    printf("\nUsage: %s <FILE_PATH>\n", args[0]);
    printf("       %s <OPTION> <FILE_PATH>\n", args[0]);
    printf("Provide detailed inode information for specified files and directories.\n\n");


    // Options for ./inspect
    printf("Options:\n");
        // -?/--help
    printf("    -?, --help                  ");
    printf("Display help information about the tool and its option.\n\n");
        // -i/--inode
    printf("    -i, --inode <file_path>     ");
    printf("Display detailed inode information for the specified file.\n\n");
        // -a/--all
    printf("    -a, --all <directory_path>  ");
    printf("Display inode information for all files within the specified directory.\n");
    printf("%sIf no path is provided, default to the current directory.\n", filler);
    printf("%s(Optional Flag) -r, --recursive for recursive listing.\n\n", filler);
        // -h/--human
    printf("    -h, --human                 ");
    printf("Output all sizes in kilobytes (K), megabytes (M), or gigabytes (G)\n");
    printf("%sand all dates in a human-readable form.\n\n", filler);
        // -f/--format
    printf("    -f, --format [text|json]    ");
    printf("Specify the output format. If not specified, default to plain text.\n\n");
        // -l/--log
    printf("    -l, --log <log_file>        ");
    printf("Log operations to a specified file.\n\n\n");

    // Example for ./inspect
    printf("Examples:\n");
        // Display help info
    printf("    %s -?%s", args[0], "                                       ");
    printf("Display the help information.\n\n");
        // Display inode info
    printf("    %s -i /path/to/file%s", args[0], "                         ");
    printf("Display inode information for /path/to/file.\n\n");
        // Display inode info for files in directory
    printf("    %s -a /path/to/directory -r%s", args[0], "                 ");
    printf("Display inode information for all files in\n");
    printf("%s/path/to/directory with recursive listing.\n\n", filler2);
        // Display inode info with human-readable values
    printf("    %s -i /path/to/file -h%s", args[0], "                      ");
    printf("Display inode information for /path/to/file\n");
    printf("%swith human-readable file sizes and dates.\n\n", filler2);
        // Display inode info in json format
    printf("    %s -i /path/to/file -f json%s", args[0], "                 ");
    printf("Display inode information for /path/to/file\n");
    printf("%swith json format.\n\n", filler2);
        // Redirect inode output to logfile
    printf("    %s -i /path/to/file -l /path/to/logfile%s", args[0], "     ");
    printf("Redirect output to /path/to/logfile.\n\n");
} // end dispHelpInfo
