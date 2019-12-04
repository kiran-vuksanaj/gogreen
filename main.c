#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <signal.h>
#include <sys/stat.h>

#include"execute.h"

int main(){
  while(1){
    printf("> ");
    char input[256];
    fgets(input,256,stdin);
    exec_cmd(input);
  }
  return 0;
}
