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
   C Implementation of ls
*/

void ls(char* pathname){
    // prints contents in/at pathname
    struct stat sb;
    DIR *dp;
    if (stat(pathname, &sb) == -1){
        // stat failed on the given path
        fprintf(stderr, "ls: stat failed on the path '%s': %s\n", pathname,strerror(errno));
        return;
    }
    if (S_ISDIR(sb.st_mode)) { // If the path given is a directory
        if ((dp = opendir(pathname)) == NULL){
            // failed to open directory
            fprintf(stderr, "ls : cannot open directory '%s': %s\n", pathname,strerror(errno));
            return;
        }
        struct dirent *d;
        errno = 0; // A subsequent error will cause errno to become non-zero.
        while ((d = readdir(dp)) != NULL) { // Iterating over all files/directories of the current directory.
            // not printing the files "." and ".."
            if (d->d_name[0]!='.') fprintf(stderr,"%s ", d->d_name);
        }
        if (d==NULL && errno!=0){ // If errno is non-zero and d is null this means some error occured.
            fprintf(stderr, "ls: cannot read directory '%s': %s\n", pathname,strerror(errno));
            return;
        }
        closedir(dp); // closing directory
    } else {
        // the given path corresponds to a file.
        fprintf(stderr,"%s ", pathname);
    }
}

int main(int n, char** list){
    if (n==1){ // no arguements were passed with ls
        char* pathname = "."; // path is the current directory
        ls(pathname);
        fprintf(stderr, "\n");
    }
    else{
        for(int i =1; n>i;i++){ // iterating over all paths given as arguements.
            if (n>2){
                // If mutiple paths are given then also printing the path.
                fprintf(stderr, "%s : \n",list[i]);
            }
            ls(list[i]); // function ls takes as arguement a path
            fprintf(stderr, "\n");
        }
    }
    return 0;
}