#ifndef TRIE_H
  #define TRIE_H

  int parmV;
  
  //all functions return -1 if an error occurs
  int initTree();
  int insert(char *word);
  int prev(int number, int start, int end);
  int delete(int number);
  int find(char *word); 
  int clear();
  int clear_s();
#endif
