#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/**
 * char **parseargs(char *cmd, char *delim) -- convert string into array of substrings
 * @param char *cmd; string to be split
 * @param char *delim; array of characters on which string should be split
 * -repeatedly runs strsep() using the given delimiter, returns null-terminated array of char * pointers to null-terminated tokens
 * -automatically skips over empty strings
 * RETURN VALUE: malloc'd, null-terminated array of char *s, pointing to each token(arg) in the original cmd
 */

char **parseargs(char *cmd,char * delim){
  // initialize out with sample size of 16
  int size = 16;
  char **out = malloc(size * sizeof(char *));
  // iterate through cmd using strsep() until it is set to NULL
  int i = 0;
  while(cmd){
    char *newtok = strsep(&cmd,delim);
    // if returned string from strsep() is not "", add it to the output array and increment the counter
    if(*newtok) out[i++] = newtok;
    // if the counter has reached the last allocated size, realloc to an array twice the previous size
    if(i >= size){
      size *= 2;
      out = realloc(out,size * sizeof(char *));
    }
  }
  // terminate the array with a NULL pointer
  out[i] = NULL;
  // reallocate memory to shorter size, freeing the unused space (i+1 so as to include the terminating null pointer)
  out = realloc(out, (i+1) * sizeof(char *));
  return out;
}
