#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char **parseargs(char *cmd,char * delim){
  int size = 16;
  char **out = malloc(size * sizeof(char *));
  int i = 0;
  while(cmd){
    char *newtok = strsep(&cmd,delim);
    if(*newtok) out[i++] = newtok;
    if(i >= size){
      size *= 2;
      out = realloc(out,size * sizeof(char *));
    }
  }
  out[i] = NULL;
  out = realloc(out, (i+1) * sizeof(char *));
  return out;
}
