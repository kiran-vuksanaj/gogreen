#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

void enable_rawmode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void reset_termios() {
  struct termios normal;
  tcgetattr(STDIN_FILENO,&normal);
  normal.c_lflag &= (ECHO | ICANON); 
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &normal);
}

void config_exit() {
  atexit(reset_termios);
}
