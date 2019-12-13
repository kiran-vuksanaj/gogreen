#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

/**
 * void print_prompt() -- print prompt to precede command
 * -calls getcwd() so as to display current directory, as well as getpid() and getppid()
 * honestly when i wrote this i thought there might end up being more to it, this is kinda a sad little file
 */

void print_prompt(){
  char cwd[256];
  getcwd(cwd,256);
  printf("[pp%d/p%d]%s$ ",getppid(),getpid(),cwd);
}

void getcmd(char *buf){
  int c = getchar();
  while( c != '\n' ){
    *(buf++) = c;
    c = getchar();
  }
}

int has_escape(char *input){
  return strstr(input,"\e[A") || strstr(input,"\e[B");
}
