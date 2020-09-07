#include <stdio.h>    
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>   
#include <dirent.h>
#include <stdbool.h>   
#include <errno.h>
#include <time.h>   
#include <pwd.h>
#include <grp.h>
#include<fcntl.h>
#include<stdarg.h>
#include <string.h>


void rm(char* src){
    struct stat sb;
    DIR * dp = opendir(src);
    int res;
    if (dp==NULL){
        // failed to open dir
        fprintf(stderr,"rm: opendir failed on path %s : %s\n",src,strerror(errno));
        return;
    }
    struct dirent *d;
    size_t size;
    char *temp = NULL;
    while ((d = readdir(dp)) != NULL) { // iterate over all files/ sub-sir of src
        if (d->d_name[0]!='.'){ // don't consider "." and ".."
            size = strlen(d->d_name)+strlen(src)+2;
            temp = malloc(size*sizeof(char));
            strcpy(temp,src);
            strcat(temp,"/");
            strcat(temp,d->d_name); // temp has the path of the file/sub-dir of src
            if (stat(temp,&sb)==-1){
                // stat failed
                fprintf(stderr,"rm: stat failed on path %s : %s",src, strerror(errno));
                return;
            }
            if (S_ISDIR(sb.st_mode)){
                // temp corresponds to a directory
                rm(temp); // recure for the sub-dir
            }
            else{
                // temp corresponds to a file
                res = unlink(temp); // delete file
                if (res==-1){
                    // unlink returns -1 in case of a failure.
                    fprintf(stderr,"rm : couldn't remove %s : %s \n", temp, strerror(errno));
                    return;
                }
            }
        free(temp);
        temp = NULL;
        }
    }
    closedir(dp); // cllose directory
    // If we reach here then all the files and sub-dir of src are deleted.
    res = rmdir(src); // delete the empty directory src
    if (res ==-1){
        // unlink returns -1 in case of a failure.
        fprintf(stderr,"rm : couldn't remove %s : %s \n", src, strerror(errno));
        return;
    }
}



int main(int n, char** list){
    if (n==1){
        // user passed less arguements
        fprintf(stderr, "The sybtax for rm : rm [Option] [files/Dirs]\n");
        return 0;
    }
    int start = 1; // the position from where the file/dir start in list
    if (strcmp(list[1],"-r")==0){
        start = 2;
        if (n==2){
            // user passed less arguements
            fprintf(stderr, "The sybtax for rm : rm [Option] [files/Dirs]\n");
            return 0;
        }
    }
    char *src;
    int res;
    struct stat sb;
    for (int i =start ; n>i; i++){ // iterating over all files/dir passed
        src = list[i];
        if (stat(src,&sb)==-1){
            // stat failed
            fprintf(stderr,"rm: stat failed on path %s : %s\n",src,strerror(errno));
            continue;
        }
        if (S_ISDIR(sb.st_mode)) {
            // src is a directory
            if (start!=2){
                // -r was not passed
                fprintf(stderr,"rm : use -r to delete dir : %s \n",src);
                continue;
            }
            // recurive function to delete a directory
            rm(src);
        }
        else{
            // src is a file
            res = unlink(src); // delete file
            if (res==-1){
                // unlink returns -1 in case of a failure.
                fprintf(stderr, "rm: failed to delete file '%s': %s\n", src,strerror(errno));
                continue;
            }
        }
    }
    return 0;
}