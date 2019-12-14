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

void handleansi(){
  
}

void insshift(char *s,int i,char c){
  if(i<0) return;
  strcpy(s+i+1,s+i);
  s[i] = c;
}

void delshift(char *s,int i){
  if(i<=0) return;
  strcpy(s+i-1,s+i);
}

void getcmd(char *buf){
  int i = 0;
  *buf = '\0';
  printf("\e[s");
  int c = getchar();
  while( c != '\n' ){
    // printf("/%d/",c);
    switch(c){
    case '\e':
      // printf("esc");
      handleansi();
      break;
    case 127: // BACKSPACE
      // printf("del");
      delshift(buf,i--);
      break;
    default:
      insshift(buf,i++,c);
      break;
    }
    printf("\e[u\e[K%s",buf);
    c = getchar();
  }
  buf[i] = '\0';
  putchar(c); // newline
}
