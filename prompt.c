#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>

void print_prompt(){
  char cwd[256];
  getcwd(cwd,256);
  printf("[pp%d/p%d]%s$ ",getppid(),getpid(),cwd);
}