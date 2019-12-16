#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include"parseargs.h"
#include"execute.h"

// cd !$

void strinsert(char *dest,char *src){
  char tmpbuf[strlen(dest)]; // -1 for ~, +1 for \0
  strcpy(tmpbuf,dest+1); // the +1 cuts off the ~
  strcpy(dest,src);
  strcat(dest,tmpbuf);
}

void insert_shortcuts(char *buf){
  char *tilda = strchr(buf,'~');
  char *home = getenv("HOME");
  while(tilda){
    strinsert(tilda,home);
    tilda = strchr(buf,'~');
  }
}

/*
void interpert(char ** args){
	int x = 0;
	char * line = malloc(100 * sizeof(char));
	char * home = getenv("HOME");
	while(x < sizeof(args)){
		if(strchr(args[x], '~')){
			line = strchr(args[x], '~');
			char * placeholder = malloc((strlen(line)+ strlen(home) - 1) * sizeof(char)); // Exact size and -1 to remove extra null
			strncpy(placeholder,args[x], strlen(args[x]) - strlen(line));// CHECK -1
			strcat(placeholder, home);
			strcat(placeholder, line);

			args[x] = placeholder; 
		}
		x++;
	}
	//STRCHR
}

int main(){
  char buf[256];
  fgets(buf,256,stdin);
  insert_shortcuts(buf);
  printf("[%s]\n",buf);
}
*/
