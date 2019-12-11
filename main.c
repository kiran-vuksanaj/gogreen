#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <signal.h>
#include <sys/stat.h>

#include"execute.h"
#include"prompt.h"

int main(){
  while(1){
    print_prompt();
    char input[256];
    fgets(input,256,stdin);
    if(!isatty(STDIN_FILENO)) printf("%s",input);
    exec_cmds(input);
  }
  return 0;
}
