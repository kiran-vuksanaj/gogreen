#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include"parseargs.h"
#include"execute.h"
#include"redirect.h"
#include"stry_util.h"

void printerr(){
  printf("errno [%d]: %s\n",errno,strerror(errno));
}

void exec_cmds(char *cmd){
  char **cmds = parseargs(cmd,";\n");
  int i = 0;
  while(cmds[i]){
    if(cmds[i][0]) exec_cmd(cmds[i++]); // inside this, args are malloc'd and freed; no memory leak
  }
  // this is NOT REACHED with exit(); there's a memory leak in that case!!
  free(cmds);
}

void exec_cmd(char *cmd){
  char **args = parseargs(cmd," ");
  if(!args || !args[0]) return;
  if(!strcmp(args[0],"cd")){
    if( chdir(args[1]) < 0 ){
      printerr();
    }
    free(args);
  }else if(!strcmp(args[0],"exit")){
    printf("logout\n");
    free(args);
    exit(0);
  }else{  
    prunchild(args);
    free(args);
  }
}

void prunchild(char **args){
  char **pipe_ptr = strrystr(args,"|");
  if(!pipe_ptr){
    runchild(args);
    return;
  }
  *(pipe_ptr++) = NULL;
  int pipefds[2];
  pipe(pipefds);
  int f = fork();
  if(f){
    close(pipefds[1]);
    int status;
    wait(&status);
    int in_backup = redirect(pipefds[0],STDIN_FILENO);
    runchild(pipe_ptr);
    dup2(in_backup,STDIN_FILENO);
  }else{
    close(pipefds[0]);
    int out_backup = redirect(pipefds[1],STDOUT_FILENO);
    prunchild(args);
    dup2(out_backup,STDOUT_FILENO);
    close(pipefds[1]);
    exit(0);
  }
}

void runchild(char **args){
  int std_backups[3];
  parse_redirects(std_backups,args);
  remove_blanks(args);
  int f = fork();
  if(f){
    int status;
    wait(&status);
    endredirect(std_backups);
    //    printf("Child exited: signal %d, status %d\n",WEXITSTATUS(status),WTERMSIG(status));
  }else{
    if( execvp(args[0],args) < 0 ){
      printerr();
      exit(errno);
    }
    printf("if it gets here something went wrong\n");
  }
}
