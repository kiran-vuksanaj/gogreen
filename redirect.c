#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>

#include"redirect.h"

int redirect(int *bk_pointer, char **args){
  // return value 0/-1
  // TODO: expand beyond just single args
  int i;
  for(i=0;i<3;i++) bk_pointer[i] = -1;
  i = 0;
  while(args[i]){
    // very temporary checking structure! feel free to tear this to shreds!
    int fd_std = -1;
    int options;
    char *filename;
    if(!strcmp(args[i],">")) {
      fd_std = STDOUT_FILENO;
      options = O_WRONLY|O_TRUNC;
      filename = args[i+1];
    }
    else if(!strcmp(args[i],"<")) {
      fd_std = STDOUT_FILENO;
      options = O_RDONLY;
      filename = args[i+1];
    }
    if(fd_std > 0) {
      bk_pointer[fd_std] = dup(fd_std);
      if(bk_pointer[fd_std] < 0) return -1;
      int fd_file = open(filename,options);
      if(fd_file < 0) return -1;
      if( dup2(fd_file,fd_std) < 0 ) return -1;
      if( close(fd_file) < 0 ) return -1;
      args[i][0] = '\0';
      args[i+1][0] = '\0';
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
