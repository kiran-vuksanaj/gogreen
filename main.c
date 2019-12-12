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
  // continue loop of print prompt, execute command, repeat forever (until signal interrupts or exited from interior)
  while(1){
    print_prompt();
    char input[256];
    fgets(input,256,stdin);
    // special thing i looked up; previously, when input was redirected into ./shash by <, no newline separated prompt from print
    // isatty() checks whether stdin is a terminal input, and if not it reprints the command inputted
    if(!isatty(STDIN_FILENO)) printf("%s",input);
    exec_cmds(input);
  }
  return 0;
}
