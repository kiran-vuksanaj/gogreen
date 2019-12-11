#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>

#include"redirect.h"

/**
 * int redirect(int fd1,int fd2) -- redirect one file descriptor into another
 * @param int fd1 - 'source' descriptor of the redirect
 * @param int fd2 - 'destination' descriptor of the redirect
 * -using dup2(), source descriptor's file is copied into destination fd
 * -RETURN VALUE: on success, a 'backup' file descriptor, containing the former contents of destination, is returned. otherwise, -1 is returned.
 * (this is a really simple function but its really convenient for usage in several other functions)
 */

int redirect(int fd1,int fd2){
  int out = dup(fd2);
  if(out < 0) return -1;
  if( dup2(fd1,fd2) < 0 ) return -1;
  return out;
}

/**
 * int redirect_filename(int fd_std,char *filename,int flags) -- configure redirect of a given filename
 * 
 */

int redirect_filename(int fd_std,char *filename,int flags){
  int fd_file = open(filename,flags,0644);
  if(fd_file < 0) return -1;
  int out = redirect(fd_file,fd_std);
  if(out < 0) return -1;
  if( close(fd_file) < 0 ) return -1;
  return out;
}

/**
 * void clear_used_args(char **args,int i) -- set args[i] and args[i+1] to be empty strings (helper method)
 */

void clear_used_args(char **args,int i){
  args[i][0] = '\0';
  args[i+1][0] = '\0';
}

int parse_redirects(int *bk_pointer, char **args){
  // return value 0/-1
  // TODO: expand beyond just single args
  int i;
  for(i=0;i<3;i++) bk_pointer[i] = -1;
  i = 0;
  while(args[i]){
    // very temporary checking structure! feel free to tear this to shreds!
    if(!strcmp(args[i],">")) {
      bk_pointer[STDOUT_FILENO] = redirect_filename(STDOUT_FILENO,args[i+1],O_WRONLY|O_TRUNC|O_CREAT);
      if( bk_pointer[STDOUT_FILENO] < 0 ) return -1;
      clear_used_args(args,i);
    }
    else if(!strcmp(args[i],"<")) {
      bk_pointer[STDIN_FILENO] = redirect_filename(STDIN_FILENO,args[i+1],O_RDONLY);
      if( bk_pointer[STDIN_FILENO] < 0 ) return -1;
      clear_used_args(args,i);
    }
    else if(!strcmp(args[i],">>")) {
      bk_pointer[STDOUT_FILENO] = redirect_filename(STDOUT_FILENO,args[i+1],O_WRONLY|O_CREAT);
      if( bk_pointer[STDOUT_FILENO] < 0 ) return -1;
      clear_used_args(args,i);
    }
    else if(!strcmp(args[i],"&>")) {
      bk_pointer[STDOUT_FILENO] = redirect_filename(STDOUT_FILENO,args[i+1],O_WRONLY|O_TRUNC|O_CREAT);
      if( bk_pointer[STDOUT_FILENO] < 0 ) return -1;
      clear_used_args(args,i);
      bk_pointer[STDERR_FILENO] = redirect(STDOUT_FILENO,STDERR_FILENO);
      if( bk_pointer[STDERR_FILENO] < 0 ) return -1;
      clear_used_args(args,i);
    }
    i++;
  }
}

int endredirect(int *backups){
  int i;
  for(i=0;i<3;i++){
    if(backups[i] != -1){
      if(dup2(backups[i],i)<0) return -1;
      if(close(backups[i])) return -1;
    }
  }
  return 0;
}
