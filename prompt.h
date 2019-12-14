#ifndef INCLUDE_PROMPT_H
#define INCLUDE_PROMPT_H

void print_prompt();

void getcmd(char *buffer);

int handleansi(char *buf,int i);

int restrictindex(char *buf, int i);
int delshift(char *buf, int i);
int insshift(char *buf, int i, char c);
#endif
