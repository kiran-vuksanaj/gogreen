#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<unistd.h>
#include<sys/wait.h>
#include"parseargs.h"
#include"execute.h"

void printerr(){
  printf("errno [%d]: %s\n",errno,strerror(errno));
}

void exec_cmd(char *cmd){
  char **args = parseargs(cmd);
  if(!strcmp(args[0],"cd")){
    if( chdir(args[1]) < 0 ){
      printerr();
    }
    free(args);
  }else if(!strcmp(args[0],"exit")){
    printf("logout\n");
    free(args);
    exit(0);
  }else{
    runchild(args);
    free(args);
  }
}

void runchild(char **args){
  int f = fork();
  if(f){
    int status;
    wait(&status);
    printf("Child exited: signal %d, status %d\n",WEXITSTATUS(status),WTERMSIG(status));
  }else{
    if( execvp(args[0],args) < 0 ){
      printerr();
      exit(errno);
    }
    printf("if it gets here something went wrong\n");
  }
}
