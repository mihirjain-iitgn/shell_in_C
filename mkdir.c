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
   C Implementation of mkdir
*/


int main(int n, char**list){
    if (n==1){
        fprintf(stderr, "The syntax of mkdir : mkdir [paths]\n");
        return 0;
    }
    int res;
    mode_t mode = 664;
    for(int i = 1; n>i; i++){
        res = mkdir(list[i],mode);
        if (res == -1){
            fprintf(stderr, "mkdir : failed to create directory %s : %s \n",list[i],strerror(errno));
        }
    }
    return 0;
}