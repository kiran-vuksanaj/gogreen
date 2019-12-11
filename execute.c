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

#define READ 0
#define WRITE 1

void printerr(){
  printf("errno [%d]: %s\n",errno,strerror(errno));
}


/**
 * void exec_cmds(char *cmd)
 * @param cmd: null-terminated string of commands, as would be entered in a shell.
 * -parses input by semicolons and newlines to separate into a series of commands to be run individually
 * -runs each commands (using exec_cmd)
 * potential memory leak of commands that follow `exit;`, but all memory is freed immediately afterwards upon exit of the program.
 */

void exec_cmds(char *cmd){
  char **cmds = parseargs(cmd,";\n");
  int i = 0;
  while(cmds[i]){
    if(cmds[i][0]) exec_cmd(cmds[i++]); // inside this, args are malloc'd and freed; no memory leak
  }
  // this is NOT REACHED with exit(); there's a memory leak in that case!!
  free(cmds);
}

/**
 * void exec_cmd(char *cmd)
 * @param cmd: null-terminated single command string (no semicolons\newlines), as would be entered in a shell
 * -parses input by spaces and interprets whether a "special case" (`cd` or `exit`) is necessary
 * -executes chdir and exit where necessary, and otherwise runs prunchild() to handle pipes and children
 */

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

/**
 * void prunchild(char *cmd) - recursively handle forks and pipes for | commands
 * @param args: null-terminated array of null-terminated args which should be executed in an execvp() call
 * -base case: if there is no pipe arg present, passes args on to runchild()
 * -recursive case: 
 *   -find last instance of | arg, create a child which runs prunchild() on all args before "|" and has a pipe to the parent carrying stdout outputs
 *   -call runchild() on the args after the last "|", with the earlier pipe redirected into stdin
 * -relies on "|" being separated by whitespace on either side! otherwise the pipe won't be found
 */

void prunchild(char **args){
  // locate last instance of "|" in the arg array (see stry_util.c)
  char **pipe_ptr = strrystr(args,"|");
  // base case: no pipe exists; call runchild() as normal
  if(!pipe_ptr){
    runchild(args);
    return;
  }
  // recursive case: separate arg array at pipe pointer by turning it into NULL, move pointer to immediately after the former "|"
  *(pipe_ptr++) = NULL;
  // create pipe
  int pipefds[2];
  pipe(pipefds);
  // fork, manage processes
  int f = fork();
  if(f){
    // PARENT (child code, below, runs first) -- execute post pipe
    // close unnecessary write end of pipe
    close(pipefds[WRITE]);
    // wait for child to finish
    int status;
    wait(&status);
    // redirect read end of pipe into stdin (see redirect.c), and run child (post-final pipe) as normal
    int in_backup = redirect(pipefds[READ],STDIN_FILENO);
    runchild(pipe_ptr);
    // restore stdin from its backup
    dup2(in_backup,STDIN_FILENO);
    // close backup and read end of pipe (it's now ready to be destroyed)
    close(in_backup);
    close(pipefds[READ]);
  }else{
    // CHILD -- execute pre pipe
    // close unneccesary read end of pipe
    close(pipefds[READ]);
    // redirect write end of pipe into stdout (see redirect.c), and recursively run pre-pipe
    // by executing recursively like this, commands run first to last
    int out_backup = redirect(pipefds[WRITE],STDOUT_FILENO);
    prunchild(args);
    // restore stdout from its backup
    dup2(out_backup,STDOUT_FILENO);
    // close backup and write end of pipe
    close(pipefds[WRITE]);
    close(out_backup);
    // exit, return to parent
    exit(0);
  }
}

/**
 * void runchild(char *cmd) - handle redirects and forks for single command (no pipes no semicolons)
 * @param args: null-terminated array of null-terminated args which should be executed in an execvp() call
 * -call parse_redirects() to set up redirects as specified by >,>>,<,&>,&>> (see redirect.c)
 * -fork and call execvp() on remaining args
 * -restore redirects to what they were previously (not necessarily to normal std, if a pipe is in place)
 * -relies on any redirect symbol being separated by some amount of whitespace from the filename and command
 */


void runchild(char **args){
  // location where backups will be stored by parse_redirects()
  int std_backups[3];
  parse_redirects(std_backups,args);
  // parse_redirects() leaves empty strings in place of used args; remove_blanks() corrects this (see stry_util.c)
  remove_blanks(args);
  int f = fork();
  if(f){
    // PARENT (runs after child code, below) -- clean up post execvp()
    // wait for child to complete
    int status;
    wait(&status);
    // use stored backups to restore previous state of redirects
    endredirect(std_backups);
    //    printf("Child exited: signal %d, status %d\n",WEXITSTATUS(status),WTERMSIG(status));
  }else{
    // CHILD -- execvp call; execs, and only continues if error
    if( execvp(args[0],args) < 0 ){
      // if there's an error, child still exits, just on the errno
      printerr();
      exit(errno);
    }
    printf("if it gets here something went wrong\n");
  }
}
