#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include<sys/wait.h> 
#include <sys/stat.h>


bool getC(char *fName){//gets a file name and checks if it has .c at the end of its name
    bool isC=false;
    const char stop='.';
    char *ans;
    ans=strchr(fName,stop);
    if(strcmp(ans,".c")==0){
        isC=true;
    }
    return isC;
}

bool compFile(char *path){//compiles the file
    bool didComp=false;
    int stat;
    char *args[]={"gcc", path, NULL};
    pid_t pid=fork();
    if(pid<0){
        perror("Error, could not fork");
        exit(1);
    }
    if(!pid){//child
        execvp("gcc",args);//should compile the student file
    }
    wait(&stat);
    if(stat==0){//if it compiled
        //printf("compiled!\n");
        didComp=true;
    }
    return didComp;
}
//pid==0 -> child
void inputFile(char *input){
    //this function takes the second line and runs it with the student compiled file and then put the result in a string
    char * args[] = {"./a.out",input, 0};
    int stat;
    int fd = open("StudentOutput.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd < 0) {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid=fork();
    if(pid<0){
        perror("could not fork");
        exit(1);
    }
    if(pid==0){
        close(STDOUT_FILENO);
        dup2(fd, STDOUT_FILENO);
        execvp(args[0], args);
    }
    else{
        wait(&stat);
        //if(stat==0){
        //    printf("ayyyy");
        //}
    }
    close(fd);
}

bool cmpToExpected(char *studentOutput, char *textToCompare){
    bool isSame=false;
    char *cmd[]={"./comp.out", studentOutput, textToCompare, NULL};
    int stat;
    pid_t pid =fork();
    if(pid<0){
        perror("could not fork");
        exit(1);
    }
    if(pid==0){
        execvp(cmd[0],cmd);
    }
    else{
        waitpid(pid,&stat,0);
        int i=WEXITSTATUS(stat);
        //printf("i= %d stat= %d",i,stat);
        if(i==2){
            printf("files are the same!");
            isSame=true;
        }
    }
    return isSame;
}


int main(int argc, char *argv[]){
    if(argc!=2){
        fprintf(stderr,"Need 2 args \n");
        return 1;
    }
    int fileD;
    char Fline[80];//folder path
    char Sline[80];//input file
    char Tline[80];//correct output for line Sline
    char testResults[256];//to write into the CSV
    fileD=open(argv[1],O_RDONLY);
    if(fileD<1){
        fprintf(stderr,"Cant open the file\n");
        return 1;
    }
    if((fileD=dup2(fileD,0))<0){
        fprintf(stderr,"Error with file!\n");
        return 1;
    }
    scanf("%s",Fline);//first line of the text file - folder path
    scanf("%s",Sline);//second line of the text file - input path
    scanf("%s",Tline);//third line of the text file - output path
    DIR *pDir;
    if ((pDir = opendir("/")) == NULL){
        fprintf(stderr,"Error! could not open directory!\n");
        exit(1);
    }
    DIR *dir=opendir(Fline);
    DIR *dirIn;
    struct dirent *curr;
    struct dirent *currIn;
    char inDirName[80];//path for student folder
    curr=readdir(dir);
    while(curr!=NULL){
        //printf(" name=%s\n", curr->d_name);//prints the folder name
        strcpy(inDirName,Fline);
        strcat(inDirName,"/");
        strcat(inDirName,curr->d_name);
        if((strcmp(curr->d_name,".")==0) || (strcmp(curr->d_name,"..")==0)){//skip '.' and ".." folders
            curr=readdir(dir);
            continue;
        }
        //printf(" %s\n",inDirName);//should print the path
        dirIn=opendir(inDirName);
        currIn=readdir(dirIn);
        bool haveC=false;//flag to see if folder has .c file
        while(currIn!=NULL){
            if((strcmp(currIn->d_name,".")==0) || (strcmp(currIn->d_name,"..")==0)){//skip '.' and ".." folders
                currIn=readdir(dirIn);
                continue;
            }
            //printf(" inside=%s\n",currIn->d_name);//prints whats inside the folder
            if(getC(currIn->d_name)==true){
                //printf("file: %s\n",currIn->d_name);//print the .c file
                strcat(inDirName,"/");
                strcat(inDirName,currIn->d_name);//add the .c file to path
                haveC=true;
            }
            currIn=readdir(dirIn);
        }
        if(haveC==false){//doesnt have c file, give 0 to student
            //STUDENTNAME,0,NO_C_FILE INTO CSV
            strcat(testResults,curr->d_name);
            strcat(testResults,",0,NO_C_FILE\n");
        }
        else{//HAS C FILE, NEED TO COMPILE
            if(compFile(inDirName)==false){//could not compile
                strcat(testResults,curr->d_name);
                strcat(testResults,",0,COMPILATION_ERROR\n");
            }
            else{//compiled, now to give input and compare
                inputFile(Sline);
                if(cmpToExpected("StudentOutput.txt",Tline)==true){//files are the same
                    strcat(testResults,curr->d_name);
                    strcat(testResults,",100,GREAT_JOB\n");
                }
                else{//output and expected are not the same
                    strcat(testResults,curr->d_name);
                    strcat(testResults,",0,BAD_OUTPUT\n");
                }
            }
        }
        curr=readdir(dir);
        unlink("a.out");
        unlink("StudentOutput.txt");
    }
    int csvFile,writeToCsv;
    csvFile=open("results.csv",O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    writeToCsv=write(csvFile,testResults,strlen(testResults));
    close(writeToCsv);
    close(csvFile);
    closedir(dirIn);
    closedir(dir);
    close(fileD);
    closedir(pDir);
    return 0;
}