#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

struct termios orig_ios;

void enable_rawmode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  orig_ios = raw;
  raw.c_lflag &= ~(ECHO | ICANON);
  //  printf("%x,%x\n",raw.c_lflag&ECHO,raw.c_lflag&ICANON);
  //  printf("%x,%x\n",orig_ios.c_lflag&ECHO,orig_ios.c_lflag&ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void reset_termios() {
  //  printf("%x,%x\n",orig_ios.c_lflag&ECHO,orig_ios.c_lflag&ICANON);
  int n = tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_ios);
  //  printf("%d\n",n);
}

void config_exit() {
  atexit(reset_termios);
}
