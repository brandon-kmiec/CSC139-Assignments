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

static int helpFlag = 0;	// flag for if argv contains -?/--help
static int iFlag = 0;		// flag for if argv contains -i/--inspect
static int aFlag = 0;		// flag for if argv contains -a/--all
static int rFlag = 0;		// flag for if argv contains -r/--recursive
static int hFlag = 0;		// flag for if argv contains -h/--human
static int fFlag = 0;		// flag for if argv contains -f/--format
static int textFlag = 0;	// flag for if argv contains text
static int jsonFlag = 0;	// flag for if argv contains json
static int lFlag = 0;		// flag for if argv contains -l/--log

void dispInodeInfo(struct stat, char[]);
void printPermissions(struct stat);
void dispHelpInfo(char*[]);

int main(int argc, char *argv[]) {
    struct stat fileInfo;

    // display help info in argv only contains ./inspect
    if(argc == 1) {
        dispHelpInfo(argv);
        return 0;
    } // end if

    // set flags based on contents of argv
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
            helpFlag = 1;
        } // end if
        else if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--inspect") == 0) {
            iFlag = 1;
        } // end if
        else if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {
            aFlag = 1;
        } // end if
        else if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--recursive") == 0) {
            rFlag = 1;
        } // end if
        else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--human") == 0) {
            hFlag = 1;
        } // end if
        else if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) {
            fFlag = 1;
        } // end if
        else if(strcmp(argv[i], "text") == 0) {
            textFlag = 1;
        } // end if
        else if(strcmp(argv[i], "json") == 0) {
            jsonFlag = 1;
        } // end if
        else if(strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log") == 0) {
            lFlag = 1;
        } // end if
    } // end for

    // display help info if helpFlag is set
    if(helpFlag == 1) {
        dispHelpInfo(argv);
        return 0;
    } // end if

    // obtain return value from stat(/path/to/file, &fileInfo)
    int ret;
    if(iFlag == 1) {
        ret = stat(argv[2], &fileInfo);
        if(ret == 0) {
            // display inode info if aFlag and lFlag are not set
            if(aFlag == 0 && lFlag == 0) {
                dispInodeInfo(fileInfo, argv[2]);
            } // end if
            else if(aFlag == 1) {

            } // end else if
            else if(lFlag == 1) {

            } // end else if
        } // end if
        else {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                    argv[2], strerror(errno));
            return 1;
        } // end else
    } // end if
    else {
        ret = stat(argv[1], &fileInfo);
        if(ret == 0) {
            // display inode info if aFlag and lFlag are not set
            if(aFlag == 0 && lFlag == 0) {
                dispInodeInfo(fileInfo, argv[1]);
            } // end if
            else if(aFlag == 1) {

            } // end else if
            else if(lFlag == 1) {

            } // end else if
        } // end if
        else {
            fprintf(stderr, "\nError getting file info for %s: %s\n\n", 
                    argv[1], strerror(errno));
            return 1;
        } // end else
    } // end else

    /*	
    // Does -l/--log redirect output to a file???
    // Still not sure what -r/--recursive does

    // TODO add support for inspect without -i since -i/--inode is optional
    // TODO add support for mixed options (such as ./inspect -i /file/path -h -f json)
    // search contents of argv and set flags based on if argv contains -h, -f, json, text, etc.
    // process command based on what flags are set (print human readable if -h flag set)
    // help info should only occur as ./inspect -? or ./inspect --help or ./inspect
    // -i/--inode is optional, should process command as if it was present

    // if argv contains -i/--inode
    // int ret = stat(argv[2], &fileInfo);
    // else
    // int ret = stat(argv[1], &fileInfo);

    // if argv contains -a/--all, loop through contents of directory printing inode info
    // while (condition)
    //	dispInodeInfo()

    // in dispInodeInfo
    // if argv contains -f/--format
    // if argv contains json
    // if argv contains -h/--human
    // display inode info with human readable numbers with json format
    // else
    // display default inode info with json format
    // if argv contains text	// could do: if(textFlag = true || (textFlag = false && jsonflag = false)) then ignore following else statement
    // if argv contains -h/--human
    // display inode info with human readable numbers with text format
    // else
    // display default inode info with text format
    // else default to text format
    // if argv contains -h/--human
    // display inode info with human readable numbers with text format
    // else
    // display default inode info with text format
    // else
    // if argv contains -h/--human
    // display inode info with human readable numbers with text format
    // else
    // display default inode info with text format
    */

    return 0;
} // end main

void dispInodeInfo(struct stat fileInfo, char filePath[]) {
    // display inode info in json format
    // else display in text format
    if(fFlag == 1 && jsonFlag == 1) {
        printf("\n{\n");
        printf("  \"filePath\": \"%s\",\n", filePath);
        printf("  \"inode\": {\n");
        printf("    \"number\": %lu,\n", fileInfo.st_ino);
        printf("    \"type\": ");
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

        printf("    \"permissions\": \"");
        printPermissions(fileInfo);
        printf("\",\n");

        printf("    \"linkCount\": %lu,\n", fileInfo.st_nlink);

        printf("    \"uid\": %d,\n", fileInfo.st_uid);
        printf("    \"gid\": %d,\n", fileInfo.st_gid);

        if(hFlag == 1) {
            float size = fileInfo.st_size;

            time_t atime = fileInfo.st_atime;
            time_t mtime = fileInfo.st_mtime;
            time_t ctime = fileInfo.st_ctime;
            struct tm ts;
            char buf[80];

            printf("    \"size\": ");
            if(size < 1024)
                printf("\"%0.0f bytes\",\n", size);
            else if(size >= 1024 && size < 1048576) {
                if((int)size % 1024 == 0)
                    printf("\"%dK\",\n", ((int)size / 1024));
                else
                    printf("\"%.2fK\",\n",(float)(size / 1024));
            } // end else if
            else if(size >= 1048576 && size < 1073741824) {
                if((int)size % 1048576 == 0)
                    printf("\"%dM\",\n", ((int)size / 1048576));
                else
                    printf("\"%.2fM\",\n", (float)(size / 1048576));
            } // end else if
            else {
                if((int)size % 1073741824 == 0)
                    printf("\"%dG\",\n", ((int)size / 1073741824));
                else
                    printf("\"%.2fG\",\n", (float)(size / 1073741824));
            } // end else

            ts = *localtime(&atime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("    \"accessTime\": \"%s\",\n", buf);

            ts = *localtime(&mtime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("    \"modificationTime\": \"%s\",\n", buf);

            ts = *localtime(&ctime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("    \"statusChangeTime\": \"%s\"\n", buf);
        } // end if
        else {
            printf("    \"size\": \"%lu bytes\",\n", fileInfo.st_size);
            printf("    \"accessTime\": \"%ld\",\n", fileInfo.st_atime);
            printf("    \"modificationTime\": \"%ld\",\n", fileInfo.st_mtime);
            printf("    \"statusChangeTime\": \"%ld\"\n", fileInfo.st_ctime);
        } // end else

        printf("  }\n");
        printf("}\n\n");
    } // end if
    else {
        printf("\nInformation for %s:\n", filePath);
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

        printf("\nNumber of Hard Links: %lu\n", fileInfo.st_nlink);

        printf("File Owner User ID: %d\n", fileInfo.st_uid);
        printf("File Owner Group ID: %d\n", fileInfo.st_gid);

        if(hFlag == 0) {
            printf("File Size: %lu bytes\n", fileInfo.st_size);
            printf("Last Access Time: %ld\n", fileInfo.st_atime);
            printf("Last Modification Time: %ld\n", fileInfo.st_mtime);
            printf("Last Status Change Time: %ld\n\n", fileInfo.st_ctime);
        } // end if
        else{
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

            ts = *localtime(&atime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("Last Access Time: %s\n", buf);

            ts = *localtime(&mtime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("Last Modification Time: %s\n", buf);

            ts = *localtime(&ctime);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
            printf("Last Status Change Time: %s\n\n", buf);
        } // end else if
    } // end else

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

    //printf("\n");
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
