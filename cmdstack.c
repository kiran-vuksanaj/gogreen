#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cmdstack.h"

static struct cmd_node *stack;


void init_cstack(){
  //printf("malloc stack init\n");
  stack = malloc( sizeof(struct cmd_node) );
  stack->prev = NULL;
  stack->next = NULL;
  stack->cmd = NULL;
}
void clear_stack(){
  free_fwd(stack->next);
  free_back(stack->prev);
  free(stack);
}

void push_cmd(){
  //printf("malloc permabuf %lu\n",strlen(stack->cmd)+1);
  char *permabuf = malloc( strlen(stack->cmd) + 1 );
  //printf("permabuf malloc'd\n");
  strcpy(permabuf,stack->cmd);
  stack->cmd = permabuf;
  free_fwd(stack->next);
  //printf("malloc new node");
  struct cmd_node *node = malloc( sizeof(struct cmd_node) );
  node->prev = stack;
  stack->next = node;
  node->next = NULL;
  node->cmd = NULL;

  stack = node;
}
void link_cstack(char *buf){
  stack->cmd = buf;
}

void fwd_cstack(char *buf){
  if(stack->next){
    // printf("malloc fdwrite\n");
    stack->cmd = malloc( strlen(buf) + 1);
    strcpy(stack->cmd,buf);
    stack = stack->next;
    strcpy(buf,stack->cmd);
    free(stack->cmd);
    stack->cmd = buf;
  }
}
void bk_cstack(char *buf){
  if(stack->prev){
    //printf("malloc bkwrite\n");
    stack->cmd = malloc( strlen(buf) + 1);
    strcpy(stack->cmd,buf);
    stack = stack->prev;
    strcpy(buf,stack->cmd);
    free(stack->cmd);
    stack->cmd = buf;
  }
}

void free_node(struct cmd_node *node){
  //printf("free node (%p)\n",node);
  free(node->cmd);
  free(node);
}
void free_back(struct cmd_node *node){
  if(node && node->prev) free_back(node->prev);
  if(node) free_node(node);
}
void free_fwd(struct cmd_node *node){
  if(node && node->next) free_fwd(node->next);
  if(node) free_node(node);
}
