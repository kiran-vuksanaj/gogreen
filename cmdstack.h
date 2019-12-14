#ifndef DEFINE_CMDSTACK_H
#define DEFINE_CMDSTACK_H

struct cmd_node {
  struct cmd_node *next;
  struct cmd_node *prev;
  char cmd[512];
};

void init_cstack();
void clear_stack();

void push_cmd(char *cmd);
void put_cmd(char *buf);

void fwd_cstack();
void bk_cstack();

void free_back(struct cmd_node *node);
void free_fwd(struct cmd_node *node);

#endif
