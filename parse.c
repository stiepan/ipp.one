/*
  Kamil Tokarski kt361223
  Dictionary on radix tree
  Parser
*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define DEFAULT_STRING_SIZE 32
#define MAX_INT 2147483647LL


//Returns pointer to first non-space char in string
char *beg(char *curr){ 
  while (*curr==' ' && *curr!='\n')
    ++curr;
  return curr;
}


//If [**curr]=<some' ' or nothing><word><some' ' or '\n'> where <word> contains
//only chars for which [pred] is true, getSubString returns word otherwise NULL.
//Moves [*curr] to the next after last copied char. Needs to be freed
char *getSubString(char **curr, int (*pred)(int)){ 
  int i = 0, capacity = DEFAULT_STRING_SIZE;
  char *string;
  *curr = beg(*curr);
  if(!(*pred)(**curr)) //first non-space char must be valid...
   return NULL;
  string =  malloc(sizeof (*string) * capacity);
  while((*pred)(**curr)){ //.. and so does all the rest
    string[i++] = **curr;
    ++(*curr);
    if(i+1 == capacity){
      capacity *= 2;
      string = realloc(string, sizeof (*string) * capacity);
    }
  }
  if(**curr != ' ' && **curr != '\n'){ //no other separators allowed
    free(string);
    return NULL;
  }
  string[i] = '\0';
  return string;
}


int endOfLine(char **curr){
  *curr = beg(*curr);
  return **curr == '\n';
}


char *getWord(char **curr){
  return getSubString(curr, islower);
}


//returns non-negative int if input is valid or -1 otherwise
int getNumber(char **curr){
  int i, length = 0, nonZero = 0, correct = 1;
  long long int conv = 0LL; //number converted from string
  char *string = getSubString(curr, isdigit);
  if(string == NULL){
    correct = 0;
  }
  else{
    while(string[length] != '\0' && correct){
      if(string[length] != '0')
	nonZero = 1;
      else if (nonZero == 0 && string[length+1] != '\0') //leading zero 
	correct = 0; 
      ++length;
    }
    if((correct) && (correct=length <= 10)){
      for(i=0; i<length; ++i){
	conv = conv*10 + string[i] - '0';
      }
      if(conv > MAX_INT)
	correct = 0;
    }
  }
  free(string);
  if(correct)
    return (int) conv;
  else
    return -1;
}
