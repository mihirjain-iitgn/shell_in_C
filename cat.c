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
   C Implementation of cat
*/

int main(int n, char** list){
    if (n==1){ // no arguements were passed by the user
        fprintf(stderr,"The syntax for cat is : cat [Files]\n");
        return 0;
    }
    FILE* fp;
    int m;
    size_t len = 0;
    char* line =NULL;
    int bytes;
    for (int i=1; n>i;i++){ // Iterating over all file paths passed as arguments.
        fp = fopen(list[i],"r"); // opening a file
        if (fp!=NULL){
            if (n>2){ 
                // multiple files are passed passed as arguments.
                fprintf(stderr, "%s : \n", list[i]); // printing the  path of the file
            }
            bytes = getline(&line, &len, fp); // reading the first line of a file
            if (bytes==-1 && !feof(fp)){ 
                // getline returns -1 on a failure or EOF. Ensuring that the -1 is not because of EOF.
                fprintf(stderr, "cat : %s : %s\n", list[i], strerror(errno));
                fclose(fp); // closing file
                free(line); // memory is freed
                line = NULL;
                continue; // Moving to next file.
            }
            while (bytes >= 0){ 
                // Iterating till EOF.
                fprintf(stderr,"%s",line);
                bytes = getline(&line, &len, fp);
                if (bytes==-1 && !feof(fp)){
                    // getline returns -1 on a failure or EOF. Ensuring that the -1 is not because of EOF.
                    fprintf(stderr, "cat : %s : %s\n", list[i], strerror(errno));
                    fclose(fp);
                    free(line); // memory is freed
                    line = NULL;
                    continue; // Moving to next file.
                }
            }
            
            fclose(fp); // closing file
        }
        else{
            // failed to open the file 
            fprintf(stderr, "cat : %s : %s\n", list[i], strerror(errno));
            continue; // moving to next file
        }
        fprintf(stderr,"\n");
        free(line); // freeing memory
        line = NULL;
    }
    return 0;
}