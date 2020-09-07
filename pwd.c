#include <stdio.h>    
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>   
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>   
#include <pwd.h>
#include <grp.h>
#include<fcntl.h>
#include<stdarg.h>

/* Author : Mihir Jain
   C Implementation of pwd
*/


int main(int n, char** list){
    char* path = getcwd(NULL,0); // getced return the path of the current directory
    if (path==NULL){
        // error handling
        fprintf(stderr, "pwd: failed to get path : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    fprintf(stderr,"%s\n",path);
    return 0;
}