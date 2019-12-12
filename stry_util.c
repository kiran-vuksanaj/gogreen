#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/**
 * void remove_blanks(char **args) -- remove ""s from args, shift forward subsequent args
 * @param char **args; string array on which to operate
 * -when completed, args is guaranteed to have no empty strings (used in conjunction with redirect)
 */

void remove_blanks(char **args){
  int i = 0; // counter for "old" array
  int j = 0; // counter for "new" array
  while(args[i]){
    // assign new array position to old array position (always, j<=i)
    args[j] = args[i];
    // j increments only when whatever just written is not ""; if it is, j does not increment and so the "" gets overwritten
    if(*args[j]) j++;
    // i always increments, as it simply represents the old array
    i++;
  }
  // add new terminating null to args
  args[j] = NULL;
}

/**
 * char **strrystr(char **stry, char *str) -- find, from right, exact match of string in a string array
 * @param char **stry; array of strings in which to search
 * @param char *str; `needle` to find in haystack of stry
 * RETURN VALUE: pointer to char* which precisely matches str; if not found, NULL is returned
 */

char **strrystr(char **stry,char *str){
  int i = 0;
  //increment i until it reaches NULL, the end of the array
  while(stry[i]) i++;
  // decrement i (avoiding NULL) until the start of the array is reached or a match is found
  while( i-- > 0 && strcmp(stry[i],str));
  // if i reaches the start of the array, return NULL (no match found), and otherwise return pointer to this string
  return i<0 ? NULL : stry+i;
}
