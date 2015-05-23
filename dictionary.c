/*
  Kamil Tokarski kt361223
  Radix Tree implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "parse.h"
#define LINE_LENGTH_LIMIT 100000

int main(int argc, char* argv[]){
  char *parsed, *curr; //last parsed word, string of read line pending parsing
  ssize_t capacity = LINE_LENGTH_LIMIT + 7;
  char *curLine = malloc(capacity);
  int correct = 0; //==0 jeśli należy wypisać ignored
  int number, start, end;
  
  initTree();

  if(argc >= 2){
    if(argc == 2 && strcmp(argv[1], "-v") == 0)
      parmV = 1;
    else{
      printf("Nieprawidłowe parametry wywołania\n");
      exit(1);
    }
  }
  while (getline(&curLine, &capacity, stdin) != -1){
    correct = 0;
    curr = curLine;
    parsed = getWord(&curr);
    
    //matching commands
    if(parsed == NULL){ //there's no correct word at the beginning of curr
      correct = 0;
    }
    else if (strcmp(parsed, "insert") == 0){
      free(parsed);
      parsed = getWord(&curr);
      if(parsed != NULL && endOfLine(&curr)){
        correct = insert(parsed)>=0;
      }
    }
    else if (strcmp(parsed, "prev") == 0){ 
      number = getNumber(&curr);
      start = getNumber(&curr);
      end = getNumber(&curr);
      if(number >= 0 && start >= 0 && end >= start && endOfLine(&curr)){
	correct = prev(number, start, end)>=0;
      }
    }
    else if (strcmp(parsed, "delete") == 0){
      number = getNumber(&curr);
      if(number >= 0 && endOfLine(&curr)){
	correct = delete(number)>=0;
      }
    }
    else if (strcmp(parsed, "find") == 0){
      free(parsed);
      parsed = getWord(&curr);
      if(parsed != NULL && endOfLine(&curr)){
        correct = find(parsed)>=0;
      }
    }
    else if (strcmp(parsed, "clear") == 0 && endOfLine(&curr)){
      correct = clear()>=0;
    }
    
    if(!correct){ //unmatched command or incorrect arguments
      printf("ignored\n");
    }
    //end of matching

    free(parsed);
  }
  free(curLine);
  clear_s();
  return 0;
}
