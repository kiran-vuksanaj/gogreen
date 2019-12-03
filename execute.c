#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<unistd.h>

#include"parseargs.h"

void runchild(char *prompt){
  int f = fork();
  if(f){
    int status;
    wait(&status);
    printf("Child exited: signal %d, status %d\n",WEXITSTATUS(status),WTERMSIG(status));
  }else{
    char **args = parseargs(prompt);
    execvp(args[0],args);
  }
}
