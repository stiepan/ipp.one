#ifndef PARSE_H
  #define PARSE_H

  //returns string with lowercase word or NULL if **curr does not start 
  //with such word (omits ' ' before word if any). Needs to be freed
  char *getWord(char **curr);
  
  //returns nonnegative int if **curr starts with such or -1 otherwise
  int getNumber(char **curr);
  
  // returns 1 when **curr=<some ' ' or nothing><'\n'>
  int endOfLine(char **curr);
#endif
