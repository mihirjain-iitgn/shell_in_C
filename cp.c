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


/* Author : Mihir Jain
   C Implementation of cp
*/




int makedir(char* path){
    // function to create a directory at path with mode 664.
    mode_t mode = 664;
    int res;
    res = mkdir(path,mode);
    return res;
}


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

void cp(char* srcp, char* destp){
    // function that copies the file at srcp to file at destp
     int src = open(srcp,O_RDONLY); // open src file
     // dest file is created with mode 664 if it doesn't exist
     int dest = open(destp,O_WRONLY | O_CREAT,664);

     //open returns -1 incase of a failure
     if (src==-1){
         fprintf(stderr, "cp: failed to open file %s : %s \n", srcp,strerror(errno));
         return;
     }
     if (dest==-1){
         fprintf(stderr, "cp: failed to open file %s : %s \n", destp,strerror(errno));
         return;
     }
     int bytes;
     char buffer[BUFSIZ]; // check this size
     while ((bytes = read(src, buffer, BUFSIZ)) > 0){
         if (bytes==-1){
             // read returns -1 on a failure
             fprintf(stderr, "cp: read failed on the path %s: %s\n", srcp,strerror(errno));
             return;
         }
         if (write(dest, buffer, bytes) != bytes){
             // write returns number of bytes written. If this is less than n then an error has occured.
             fprintf(stderr, "cp: write failed on the path %s: %s\n", destp,strerror(errno));
             return;
         }
     }
     // closing files
     close(src); 
     close(dest);
}

int main(int n, char** list){
    struct stat sb;
    struct dirent *d;
    DIR * d_src;
    DIR *d_dest;
    char *dest = list[n-1];
    char *src;
    char *filename;
    size_t size;
    char * temp = NULL;
    char * temp1 = NULL;
    char * temp2 = NULL;
    if (n==1 || n==2){
        // User did not give sufficient arguements
        fprintf(stderr,"The syntax of cp : cp [Options] [Src] Destination\n");
        return 0;
    }
    int dir_des, dir_src; // dir_des is 1 when destination is a directory. Same with dir_src
    if (stat(dest, &sb) == -1){
        // stat failed
        fprintf(stderr, "cp: stat failed on the path '%s': %s\n", dest,strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (S_ISDIR(sb.st_mode)){
        // destination is a directory
        dir_des = 1;
    }
    int start = 1; // the position in list from where the path names start
    if (strcmp(list[1],"-r")==0){
        // -r is passed
        start = 2; // start must be incremented
        if (dir_des==0){
            // -r is passed by destination is not a directory
            fprintf(stderr,"Destination %s is not a dir\n",dest);
            exit(EXIT_FAILURE);
        }
        if (n==3){
            // User did not give sufficient arguements
            fprintf(stderr,"The syntax of cp : cp [Options] [Src] Destination\n");
            return 0;
        }
    }
    for (int i =start ; n-1>i;i++){ // Iterating over all source paths.
        src = list[i];
        dir_src = 0;
        if (stat(src, &sb) == -1){
            // stat failed
            fprintf(stderr, "cp: stat failed on the path '%s': %s\n", src,strerror(errno));
            continue;
        }
        if (S_ISDIR(sb.st_mode)){
            // src is a directory
            dir_src = 1;
        }
        if (dir_src==1 && dir_des==1){
            if (start == 1){
                fprintf(stderr, "cp : use -r to copy dir %s to dir %s\n", src,dest);
                continue;
            }
            // source and destination both are directories
            if ((d_src = opendir(src)) == NULL){
                // failed to opem src directory
                fprintf(stderr, "cp : cannot open directory '%s': %s\n", src,strerror(errno));
                continue;
            }
            // the next few lines create the path of src folder in dest folder
            size = strlen(src)+strlen(dest)+2;
            temp = malloc(size*sizeof(char)); 
            temp = strcpy(temp, dest);
            temp = strcat(temp,"/");
            filename = getfilename(src); // name of the folder at src
            temp = strcat(temp,filename); // temp is appended with the name of the folder at src
            if ((d_dest = opendir(temp)) == NULL){
                if (errno==ENOENT){
                    // directory does not exist. Make the directory.
                    if (makedir(temp)==-1){
                        // makedir failed
                        fprintf(stderr, "cp : could not create dir on path %s",temp);
                        continue;
                    }
                }
                else{
                    // some other error
                    fprintf(stderr, "cp : cannot make dir %s in directory '%s': %s\n", filename, temp,strerror(errno));
                    continue;
                }
            }
            while ((d = readdir(d_src)) != NULL) { // Iterating over all files/ directories of the src directory
                size = strlen(d->d_name) + strlen(temp) + 2;
                temp1 = malloc(size * sizeof(char));
                temp1 = strcpy(temp1, temp);
                temp1 = strcat(temp1, "/");
                temp1 = strcat(temp1, d->d_name); // temp1 has the path to which a file from the src directory will go in the destination directory.
                size = strlen(d->d_name) + strlen(src) + 2;
                temp2 = malloc(size * sizeof(char));
                temp2 = strcpy(temp2, src);
                temp2 = strcat(temp2, "/");
                temp2 = strcat(temp2, d->d_name); // temp2 has the path of a file in the src directory
                if (stat(temp2, &sb) == -1)
                {
                    // stat failed
                    fprintf(stderr, "cp: stat failed on the path '%s': %s\n", temp2, strerror(errno));
                    continue;
                }
                if (S_ISREG(sb.st_mode)){
                    // copy only files, not directories.
                    cp(temp2, temp1);
                }
            }
            // freeing memory
            free(temp);
            free(temp1);
            free(temp2);
            temp = NULL;
            temp1 = NULL;
            temp2 = NULL;
        }
        else if (dir_src==0 && dir_des==1){
            // src is a file and dest is a directory
            size = strlen(src)+strlen(dest)+2;
            temp = malloc(size*sizeof(char));
            temp = strcpy(temp, dest);
            temp = strcat(temp,"/");
            temp = strcat(temp,getfilename(src)); //temp contains the path of the copied src file in dest folder
            cp(src, temp);
            // freeing memory
            free(temp);
            temp = NULL;
        }
        else if (dir_src==1 && dir_des==0){
            // src is a directory but dest is file
            fprintf(stderr,"cp : cannot copy dir %s to file %s \n",src,dest);
            continue;
        }
        else{
            // src and dest are both files
            cp(src,dest);
        }
    }
    return 0;
}