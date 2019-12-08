#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#include"redirect.h"

int redirect(int fd,enum redirect_type mode) {
  int backup;
  switch(mode) {

  case R_STDIN:
    backup = dup(STDIN_FILENO);
    if( backup < 0 ) return -1;
    if( dup2(fd,STDIN_FILENO) < 0 ) return -1;
    return backup;
    break;
  case R_STDOUT:
    break;
  case R_OUTS:
    break;
  }
  return 0;
}
int endredirect(int backup_fd, enum redirect_type mode) {
  switch(mode) {

  case R_STDIN:
    if( dup2(backup_fd,STDIN_FILENO) < 0 ) return -1;
    close(backup_fd);
  case R_STDOUT: case R_OUTS:
    if( dup2(backup_fd,STDOUT_FILENO) < 0 ) return -1;
    close(backup_fd);
    
  }
  return 0;
}
