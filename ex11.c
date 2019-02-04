#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
files are similar = return 2
otherwise = return 1
compile as comp.out
*/

int main(int argc, char *argv[]){ 
    int firstF,secondF;
    char buf1,buf2;
    if(argc != 3){//not enough arguments (less than 2 files)
        fprintf(stderr,"argc error\n");
        return 1;
    }
    firstF = open(argv[1], O_RDONLY);//first file
    secondF = open(argv[2], O_RDONLY);//second file
    if(firstF < 1 || secondF < 1){ //checks if we could open the files
        fprintf(stderr,"Error! Could not open one or both of the files!");
        return 1;
    }
    int r1,r2;
    r1=read(firstF,&buf1,1);
    r2=read(secondF,&buf2,1);
    while((r1==1) || (r2==1)){
        if(r1==0 && r2!=0){//checks to see if file 1 is opened and file2 is not
            return 1;
        }
        if(r1!=0 && r2==0){//reverse^^
            return 1;
        }
        if(buf1!=buf2){//checks the char
           return 1;
        }
        r1=read(firstF,&buf1,1);
        r2=read(secondF,&buf2,1);
    }
    close(r1);
    close(r2);
    close(firstF);
    close(secondF);
    return 2;
}