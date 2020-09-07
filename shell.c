#include <stdio.h>    
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
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
   Shell in C
*/

char * binarypath; // a global variable that will be given the path of the executable. Using this variable, the path to the bin folder is obtained.

void Execute(char** list, int n, int des, int binary){
    pid_t pid, wpid;
    pid = fork();
    int res;
    if (0>pid){
        // fork failed
        fprintf(stderr, "fork failed : %s \n", strerror(errno));
    }
    else if (pid==0) {
        // child process
        char * temp = malloc(sizeof(char)*(strlen(binarypath)+strlen(list[0])+6));
        if (binary){
            // run custom binaries
            // generating path to the binary folder
            strcat(temp,binarypath);
            strcat(temp,"/bin/");
            strcat(temp,list[0]); 
            res = execvp(temp,list);
        }
        else{
            // run inbuilt binaries
            res = execvp(list[0],list);
        }
        // execvp only returns on a failure
        fprintf(stderr, "execvp failed on %s : %s\n",temp, strerror(errno));
        exit(EXIT_FAILURE); // terminate child process
    }
    else{
        // parent process
        if (des) return; // if des is 1 then don't wait for child i.e run child in background
        wpid = wait(NULL);
    }
}


char** GetArgs(char* line){
    // This function extracts the arguements from the line into list
    int buffersize = 5; // number of items in your list
    char** list = (char**)malloc(sizeof(char*)*buffersize); 
    int index = 0;
    char* arg = strtok(line," \n"); // strok gives pointers from line as per delimiters passed as 2nd arguement.
    while(arg!=NULL){
        list[index]=arg;
        index++;
        if (index==buffersize){
            // the alloted buffer got full, more memory should be allocated
            buffersize++;
            list = realloc(list,buffersize*(sizeof(char*)));
        }
        arg = strtok(NULL," \n"); // next pointer after a delimiter (or NULL)
    }
    if (index==buffersize){
        // more memory should be allocated for the NULL that will come at the end of list
       buffersize++;
       list = realloc(list,buffersize*(sizeof(char*)));
    }
    list[index]=NULL; // list is NULL teriminated
    return list;	
}

char* GetLine(){
    char * line = NULL;
    size_t buffersize = 0;
    line = (char*)malloc(sizeof(char)*buffersize);
    int bytes = getline(&line,&buffersize,stdin);
    if (bytes==-1){
        // getline failed to read the line.
        fprintf(stderr, "%s\n" ,strerror(errno));
        return line;
    }
    return line;
}

void cd(int n,char**list){
    if (n==1){
        // user gave less arguments
        fprintf(stderr,"cd : couldn't change dir : arguement missing\n");
        return;
    }
    if (n>2){
        // user gave extra arguements
        fprintf(stderr,"cd : couldn't change dir : too many arguements given\n");
        return;
    }
    // chdir changes directory
    if (chdir(list[1])!=0){
        // childir failed as it returns 0 on success.
        fprintf(stderr,"cd : couldn't change dir %s : %s\n",list[1],strerror(errno));
    }
}



char* pwd(){
    // getcwd returns the current path
    char* path = getcwd(NULL,0);
    if (path==NULL){
        // error handling
        fprintf(stderr, "pwd: failed to get path : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return path;
}

int background(char *line){
    // This function finds the last charcter of line and checks if it's "&"
    int i = 0;
    char c;
    while (line[i]!='\0'){ // iterate till NULL is encountered
        c = line[i];
        i++;
    }
    if (c=='&'){
        line[i-1]='\0'; // removed the "&"
        return 1;
    }
    return 0;
}

int binary(char* command){
    // This function decices whether to use an inbuilt binary or custom binary
    // the below is a list of custom binaries
    char* custom[]= { "ls", "mkdir", "grep" , "pwd" , "rm", "cp", "cat", "mv","\0"};
    int i =0;
    while (custom[i]!="\0"){ // iterating over list of custom binaries
        if (strcmp(command, custom[i])==0){
            return 1; // custom binary
        }
        i++;
    }
    return 0; // inbuilt binary
}

void shell_loop(){
    char* line;
    char** list;
    int n;
    while (1){ // loop of the shell
        n = 0;
        fprintf(stderr,"%s >>",pwd());
        line = GetLine(); // get line input by the user

        if (line[0]=='\n') continue; // empty string is ignored

        if (line == NULL){
            // could not fetch line
            exit(EXIT_FAILURE);
        }
        list = GetArgs(line); // tokenise the line into arguements
        while (list[n]!=NULL){
            // count the number of arguements
            n++; 
        }
        if (strcmp(list[0],"cd")==0){
            // cd is executed by the shell not a child process
            cd(n,list);
        }
        else{
            // execute a command 
            // background(line) = 1 -> run in background else wait to child to finish
            // binary(list[0]) = 1 -> run custom binary else run inbuilt binary
            Execute(list, n, background(line), binary(list[0]));
        }
        free(line);
        free(list);
    }
}
int main(){
    binarypath = pwd();
    // shell loop
    shell_loop();
    return 0;
}