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
#include"cmdstack.h"
#include"shortcut.h"

int main(){
  // continue loop of print prompt, execute command, repeat forever (until signal interrupts or exited from interior)
  config_exit();
  init_cstack();
  while(1){
    print_prompt();
    char input[512];
    enable_rawmode();
    link_cstack(input);
    getcmd(input);
    push_cmd(input);
    reset_termios();
    insert_shortcuts(input);
    if(exec_cmds(input) < 0) break;
  }
  return 0;
}
