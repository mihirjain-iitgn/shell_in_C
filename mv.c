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
   C Implementation of mv
*/


char * getfilename(char* path){
    // function to get filename or directory name from a path.
    int i = 0;
    int index =0;
    while (path[i]!='\0'){
        // Itearte in path till NULL is encountered
        if (path[i]=='/'){
            // If a / is encountered store it's index
            index = i;
        }
        i++;
    }
    // final value of index corresponds to the last "/"
    return &path[index];
}

int main(int n, char** list){
    if (n==1 || n==2){
        // user didn't give source and/or destination
        fprintf(stderr, "The syntax for mv is : mv [src] dest");
        return 0;
    }
    char * dest = list[n-1]; // last index is the source path
    char * src;
    int res;
    size_t size;
    struct stat sb;
    struct stat sb1;
    char * temp = NULL;
    int dir = 0; // dir is 1 when destination path is a directory
    if (stat(dest, &sb) == -1){
        // stat failed
        fprintf(stderr, "ls: stat failed on the path '%s': %s\n", dest,strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (S_ISDIR(sb.st_mode)) {
        // Destination path is a directory
        dir = 1;
    }
    if (dir==0 && n>3){
        // multiple src paths are given but destination is not a directory
        fprintf(stderr, "mv : target %s is not a directory", dest);
        exit(EXIT_FAILURE);
    }
    for (int i =1; n-1>i; i++){ // Iterating over all source paths
        src = list[i];
        if (stat(src, &sb1) == -1){
            // stat failed
            fprintf(stderr, "mv: stat failed on the path '%s': %s\n", src ,strerror(errno));
            continue;
        }
        if (S_ISDIR(sb1.st_mode)) {
            // src is a directory
            fprintf(stderr, "mv : failed src %s is a directory", src);
            continue;
        }
        if (dir){
            // The below lines create the path for the source file in the destination folder.
            size = strlen(src)+strlen(dest)+1;
            temp = malloc(size*sizeof(char));
            temp = strcpy(temp, dest); // temp initially has destination path
            temp = strcat(temp,"/"); 
            temp = strcat(temp,getfilename(src)); // appending the filename of src to temp

            res = rename(src, temp); 

            free(temp); // freeing memory
            temp = NULL;
        }
        else{
            res = rename(src,dest);
        }
        if (res==-1){
            // rename returns -1 on failure
            fprintf(stderr, "mv: failed to move %s to %s : %s\n", src , dest,strerror(errno));
        }
    }
    return 0;
}
