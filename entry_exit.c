#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

struct termios orig_termios;

void enable_rawmode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void reset_termios() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void config_exit() {
  atexit(reset_termios);
}

void exit_clean(int val){
  reset_termios();
  exit(val);
}
