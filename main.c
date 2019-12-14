#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <signal.h>
#include <sys/stat.h>
#include <termios.h>

#include"execute.h"
#include"prompt.h"
#include"entry_exit.h"

int main(){
  // continue loop of print prompt, execute command, repeat forever (until signal interrupts or exited from interior)
  config_exit();
  char preload[256] = "ls -al\n";
  while(1){
    print_prompt();
    char input[256];
    enable_rawmode();
    getcmd(input);
    reset_termios();
    printf("[%s]\n",input);
    if(exec_cmds(input) < 0) break;
  }
  return 0;
}
