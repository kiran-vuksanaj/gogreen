#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>

#include"redirect.h"

int redirect(int fd1,int fd2){
  int out = dup(fd2);
  if(out < 0) return -1;
  if( dup2(fd1,fd2) < 0 ) return -1;
  return out;
}

int redirect_filename(char **args,int i,int fd_std,char *filename,int flags){
  int fd_file = open(filename,flags,0644);
  if(fd_file < 0) return -1;
  int out = redirect(fd_file,fd_std);
  if(out < 0) return -1;
  if( close(fd_file) < 0 ) return -1;
  args[i][0] = '\0';
  args[i+1][0] = '\0';
  return out;
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
      bk_pointer[STDOUT_FILENO] = redirect_filename(args,i,STDOUT_FILENO,args[i+1],O_WRONLY|O_TRUNC|O_CREAT);
      if( bk_pointer[STDOUT_FILENO] < 0 ) return -1;
    }
    else if(!strcmp(args[i],"<")) {
      bk_pointer[STDIN_FILENO] = redirect_filename(args,i,STDIN_FILENO,args[i+1],O_RDONLY);
      if( bk_pointer[STDIN_FILENO] < 0 ) return -1;
    }
    else if(!strcmp(args[i],">>")) {
      bk_pointer[STDOUT_FILENO] = redirect_filename(args,i,STDOUT_FILENO,args[i+1],O_WRONLY|O_CREAT);
      if( bk_pointer[STDOUT_FILENO] < 0 ) return -1;
    }
    else if(!strcmp(args[i],"&>")) {
      bk_pointer[STDOUT_FILENO] = redirect_filename(args,i,STDOUT_FILENO,args[i+1],O_WRONLY|O_TRUNC|O_CREAT);
      if( bk_pointer[STDOUT_FILENO] < 0 ) return -1;
      bk_pointer[STDERR_FILENO] = redirect(STDOUT_FILENO,STDERR_FILENO);
      if( bk_pointer[STDERR_FILENO] < 0 ) return -1;
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
