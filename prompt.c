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

int restrictindex(char *s,int i){
  if(i<0) return 0;
  int len = strlen(s);
  if(i>len) return len;
  return i;
}

int handleansi(char *s,int i){
  char c = getchar();
  if(c != '[') return i;
  c = getchar();
  switch(c){
  case 'D':
    return i-1;
  case 'C':
    return i+1;
  default:
    return i;
  }
}

int insshift(char *s,int i,char c){
  strcpy(s+i+1,s+i);
  s[i] = c;
  return ++i;
}

int delshift(char *s,int i){
  if(i==0) return 0;
  strcpy(s+i-1,s+i);
  return --i;
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
      i = handleansi(buf,i);
      break;
    case '\t':
      i = insshift(buf,i,'%');
      break;
    case 127: // BACKSPACE
      // printf("del");
      i = delshift(buf,i);
      break;
    default:
      i = insshift(buf,i,c);
      break;
    }
    i = restrictindex(buf,i);
    printf("\e[u\e[K%s\e[u\e[%dC",buf,i);
    c = getchar();
  }
  putchar(c); // newline
}
