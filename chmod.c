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
   C Implementation of chmod
*/


int main(int n , char** list){
    if (n==1 || n==2){
        fprintf(stderr, "The syntax for chmod : chmod mode File\n");
        return 0;
    }
    int res;
    char * ptr = NULL;
    mode_t mode = strtol(list[1],&ptr,8);
    for (int i =2; n>i;i++ ){
        res = chmod(list[i], mode);
        if (res == -1){
            fprintf(stderr, "chmod : failed to change mode of %s : %s\n", list[i], strerror(errno));
        }
    }
    return 0;
}