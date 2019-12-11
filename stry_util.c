#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void remove_blanks(char **args){
  int i = 0;
  int j = 0;
  while(args[i]){
    args[j] = args[i];
    if(args[j][0]) j++;
    i++;
  }
  args[j] = NULL;
}

char **strrystr(char **stry,char *str){
  int i = 0;
  while(stry[i]) i++;
  while( i-- > 0 && strcmp(stry[i],str));
  return i<0 ? NULL : stry+i;
}
