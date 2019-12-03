#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<unistd.h>
#include<sys/wait.h>
#include"parseargs.h"

void runchild(char *prompt){
  char **args = parseargs(prompt);
  int f = fork();
  if(f){
    int status;
    wait(&status);
    free(args);
    printf("Child exited: signal %d, status %d\n",WEXITSTATUS(status),WTERMSIG(status));
  }else{
    execvp(args[0],args);
  }
}
