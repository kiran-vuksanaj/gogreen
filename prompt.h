#ifndef INCLUDE_PROMPT_H
#define INCLUDE_PROMPT_H

void print_prompt();

void load_cmd();

int has_escape(char *input);

void reset_prompt();

void getcmd(char *buffer);

void reset_termios();

#endif
