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
   C Implementation of grep
*/

int main(int n, char** list){
    FILE* fp;
    size_t len = 0;
    char* line =NULL;
    int bytes;
    if (n==1){  // no arguements were passed by the user
        fprintf(stderr, "The syntax for grep is : grep PATTERN [FILES]\n");
        return 0;
    }
    if (n==2){ // destination file was not given by the user. The next line input is taken as the destination for pattern search.
        line = (char*)malloc(sizeof(char)*len);
        int bytes = getline(&line,&len,stdin); // reading line from stdin
        if (bytes==-1){
            // error handling
            fprintf(stderr, "%s\n" ,strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (strstr(line, list[1])){ // searching for the pattern in the input string
            fprintf(stderr,"%s",line);
        }
        free(line); // memory is freed
        return 0;
    }
    for (int i=2; n>i;i++){ // Iterating over all files passed as arguement.
        fp = fopen(list[i],"r");
        if (fp!=NULL){
            bytes = getline(&line, &len, fp); // reading the first line of the file
            if (bytes==-1 && !feof(fp)){ 
                // getline returns -1 on a failure or EOF. Ensuring that the -1 is not ecause of EOF.
                fprintf(stderr, "grep : %s : %s\n", list[i], strerror(errno));
                fclose(fp);
                free(line); // memory is freed
                line = NULL;
                continue; // Moving to next file.
            }
            while (bytes >= 0){
                // Iterating till EOF.
                if (strstr(line,list[1])!=NULL){ // searching for the pattern in a line of the file
                    if (n>3){ // if we have more than one file then we also print the name of the file along with the match.
                        fprintf(stderr,"%s>>%s",list[i],line);
                    }
                    else{
                        fprintf(stderr,"%s",line);
                    }
                }
                bytes = getline(&line, &len, fp); // reading the next line of the file.
                if (bytes==-1 && !feof(fp)){ // getline returns -1 on a failure or EOF. Ensuring that the -1 is not ecause of EOF.
                    fprintf(stderr, "grep : %s : %s\n", list[i], strerror(errno));
                    fclose(fp);
                    free(line); // memory is freed
                    line = NULL;
                    continue; // Moving to next file.
                }
            }
            fprintf(stderr,"\n");
            fclose(fp); // current file closed
        }
        else{
            // couldn't open file.
            fprintf(stderr, "grep : %s : %s\n", list[i], strerror(errno));
            continue;
        }
        free(line); // memory is freed
        line = NULL;
    }
    return 0;
}