#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include"parseargs.h"
#include"execute.h"

// cd !$


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
	char array[2][20] = {"~/BRYAN",0};
	char ** test = array;
	int x = 0;
	while(test[x]) { 
		printf ("\tEntry: %d: %d\n", x, test[x]);
		x++; 
	}
	interpert(&test);
	x =0;
	while(test[x]){ 
		printf ("\tEntry: %d: %d\n", x, test[x]); 
		x++;
	}
	return 0;
}