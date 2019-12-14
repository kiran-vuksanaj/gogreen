#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cmdstack.h"

static struct cmd_node *stack;

void init_cstack(){
  stack = malloc( sizeof(struct cmd_node) );
  stack->next = NULL;
  stack->prev = NULL;
  *(stack->cmd) = '\0'; 
}
void clear_stack(){
  free_back(stack->prev);
  free_fwd(stack->next);
  free(stack);
  stack = NULL;
}

void push_cmd(char *cmd){
  struct cmd_node *node = malloc( sizeof(struct cmd_node) );
  node->prev = stack;
  node->next = NULL;
  free_fwd(stack->next);
  strncpy(node->cmd,cmd,512);
  stack = node;
}

void put_cmd(char *buf){
  strncpy(buf,stack->cmd,512);
}

void fwd_cstack(){
  stack = stack->next;
}

void bk_cstack(){
  stack = stack->prev;
}

void free_back(struct cmd_node *node){
  if(node->prev) free_back(node->prev);
  free(node);
}
void free_fwd(struct cmd_node *node){
  if(node->next) free_fwd(node->next);
  free(node);
}
