#ifndef DEFINE_CMDSTACK_H
#define DEFINE_CMDSTACK_H

struct cmd_node {
  struct cmd_node *next;
  struct cmd_node *prev;
  char *cmd;
};

void init_cstack();
void clear_stack();

void push_cmd();
void link_cstack(char *buf);

void fwd_cstack(char *buf);
void bk_cstack(char *buf);

void free_node(struct cmd_node *node);
void free_back(struct cmd_node *node);
void free_fwd(struct cmd_node *node);

#endif
