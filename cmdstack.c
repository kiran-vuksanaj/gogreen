#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cmdstack.h"

static struct cmd_node *stack;
static struct cmd_node *top;
void init_cstack(){
  stack = malloc( sizeof(struct cmd_node) );
  stack->next = NULL;
  stack->prev = NULL;
  *(stack->cmd) = '\0';
  top = malloc( sizeof(struct cmd_node) );
  top->prev = stack;
  top->next = NULL;
  *(top->cmd) = '\0';
  stack->next = top;
}
void clear_stack(){
  free_back(stack->prev);
  free_fwd(stack->next);
  free(stack);
  free(top);
  stack = NULL;
  top = NULL;
}

void push_cmd(char *cmd){
  struct cmd_node *node = malloc( sizeof(struct cmd_node) );
  node->prev = stack->prev;
  node->next = top;
  free_fwd(stack);
  strncpy(node->cmd,cmd,512);
  stack = node;
}

void put_cmd(char *buf){
  strncpy(buf,stack->cmd,512);
}

void fwd_cstack(){
  if(stack->next) stack = stack->next;
}

void bk_cstack(){
  if(stack->prev) stack = stack->prev;
  printf("[[%s]]",stack->cmd);
}

void free_back(struct cmd_node *node){
  if(node->prev) free_back(node->prev);
  free(node);
}
void free_fwd(struct cmd_node *node){
  if(node->next) free_fwd(node->next);
  if(node != top) free(node);
}
