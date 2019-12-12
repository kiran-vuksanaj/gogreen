#ifndef DEFINE_CMDSTACK_H
#define DEFINE_CMDSTACK_H

struct cmd_node {
  struct cmd_node *next;
  struct cmd_node *prev;
  char cmd[512];
};

struct cmd_node *push(struct cmd_node *node,char *cmd);

void free_back(struct cmd_node *node);

void free_fwd(struct cmd_node *node);

#endif
