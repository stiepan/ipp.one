/*
  Kamil Tokarski kt361223
  Dictionary on radix tree
  Radix tree implementation
*/

#ifdef ASSERT
#include <assert.h>
#else
#define assert(x)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT_SIZE 2*1024*1024+7
#define DEFAULT_EDGE_ARRAY_SIZE 2

struct Node;
struct Edge;
typedef struct Node *Tree;
typedef struct Edge *Edge;

struct Node{
  Tree parent;
  Edge *edges;
  int capacity, size;
  int isWord;
};

struct Edge{
  Tree target;
  char *beg, *end;
};

Tree root;
static char strings[MAX_INPUT_SIZE];
static char *wordsNumbers[MAX_INPUT_SIZE];
static int wordsLengths[MAX_INPUT_SIZE];
static int stringsIt; //iterator for strings[]
static int wordsCounter;
static int nodesCounter;
int parmV; //=1 if -v parm of execution


static Tree newTree(int isWord, Tree ancestor){
  Tree nT = malloc(sizeof *nT);
  nT->edges = malloc(sizeof *(nT->edges) * DEFAULT_EDGE_ARRAY_SIZE);
  nT->size = 0;
  nT->capacity = DEFAULT_EDGE_ARRAY_SIZE;
  nT->isWord = isWord;
  nT->parent = ancestor;
  return nT;
}

static Edge newEdge(Tree target, char *beg, char *end){
  Edge nE = malloc (sizeof *nE);
  nE->target = target;
  nE->beg = beg;
  nE->end = end;
  return nE;
}

static void addEdge(Tree t, Edge e){
  if(t->size == t->capacity){
    t->capacity *= 2;
    t->edges = realloc(t->edges, (sizeof *(t->edges)) * t->capacity);
  }
  (t->edges)[(t->size)++] = e;
}

static void removeEdge(Tree t, int it){
  if(it < t->size && t->size > 0){
    free(t->edges[it]);
    t->edges[it] = t->edges[t->size-1];
    --(t->size);
  }
}

static void destroy(Tree t){
  int i;
  for(i=0; i < t->size; i++){
    destroy((t->edges)[i]->target);
    free((t->edges)[i]);
  }
  free(t->edges);
  free(t);
}

static void stderrwr(){
  int n = (nodesCounter == 1)? 0 : nodesCounter;
  if(parmV == 1) 
    fprintf(stderr, "nodes: %d\n", n);
}

//merges supr with edge following supr->target (must be exacly one)
//supr->target will be destroyed
static void mergeWithNextEdge(Edge supr){
  Tree victim = supr->target;
  assert(victim->size == 1);
  supr->beg = victim->edges[0]->beg - (supr->end + 1 - supr->beg);
  supr->end = victim->edges[0]->end;
  supr->target = victim->edges[0]->target;
  supr->target->parent = victim->parent;
  free(victim->edges[0]);
  victim->size = 0;
  destroy(victim);
}

static int directEdge(Tree ancs, Tree desc){
  int i;
  for(i=0; i<ancs->size; i++){
    if(ancs->edges[i]->target == desc)
      return i;
  }
  return -1;
}

//returns position of last char in common [edge] and [beg...end] prefix or
//-1 if 0 position doesn't match 
static int cmprLabels(Edge e, char *beg, char *end){
  assert(beg <= end);
  int i = 0;
  while ((e->beg + i <= e->end) && (beg + i <= end) && beg[i] == (e->beg)[i]){
    ++i;
  }
  return i - 1;
}

//returns node representing the longest common prefix with Tree and *beg
//moves *beg to the first 'protruding' position
static Tree lastMatchedNode(char **beg, char **end){
  Tree t = root;
  int i=0;
  while (i < t->size){
    if(cmprLabels((t->edges)[i], *beg, *end) == t->edges[i]->end - t->edges[i]->beg){
      *beg = *beg + ((t->edges[i])->end + 1  - (t->edges[i])->beg);
      t = (t->edges)[i]->target;
      i = -1;
      if(*beg > *end) 
	return t;
    }
    ++i;
  }
  return t;
}

//0 - not found, 1 - found with isWord=0 , 2 - found with isWord=1
static int exists(char *beg, char *end){
  Tree node;
  int i;
  node = lastMatchedNode(&beg, &end);
  if(beg<=end) //not whole word matched
    return 0;
  else
    return node->isWord + 1;
}

static char *endform(char *word){
  char *end = word;
  while(*(end+1) != '\0') 
    ++end;
  return end;
}

static int insertion(char *beg, char *end){
  int i, howFarFitted;
  Tree parent, child, hub;
  Edge fromParent = NULL, nEdge, fromHubToOld;
  parent = lastMatchedNode(&beg, &end);
  if(beg > end){
    parent->isWord = 1;
    printf("word number: %d\n", wordsCounter-1);
    stderrwr();
    return 0;
  }
  for(i=0; i<parent->size; ++i){ //looking for partially matching edge
    howFarFitted=cmprLabels((parent->edges)[i], beg, end);
    if(howFarFitted != -1){
      assert(howFarFitted <= end-beg);
      fromParent = (parent->edges)[i];
      break;
    }
  }
  if(fromParent == NULL){ //there's no edge with common prefix
    child = newTree(1, parent);
    ++nodesCounter;
    nEdge = newEdge(child, beg, end);
    addEdge(parent, nEdge);
  }
  else if(howFarFitted == end-beg){ //inserting node between parent and fromParent->target
    child = newTree(1, parent);
    ++nodesCounter;
    nEdge = newEdge(fromParent->target, fromParent->beg + howFarFitted + 1, fromParent->end);
    addEdge(child, nEdge);
    fromParent->target->parent = child;
    fromParent->end = fromParent->beg + howFarFitted;
    fromParent->target = child;
  }
  else{ //splitting fromParent edge
    hub = newTree(0, parent);
    child = newTree(1, hub);
    nodesCounter+=2;
    fromHubToOld = newEdge(fromParent->target, fromParent->beg+howFarFitted+1, fromParent->end);
    fromParent->target->parent = hub;
    addEdge(hub, fromHubToOld);
    fromParent->end = fromParent->beg + howFarFitted;
    fromParent->target = hub;
    nEdge = newEdge(child, beg+howFarFitted+1, end);
    addEdge(hub, nEdge);
  }
  printf("word number: %d\n", wordsCounter-1);
  stderrwr();
  return 0;
}

int initTree(){
  root = newTree(0, root);
  nodesCounter = 1;
}

int insert(char *word){
  int length;
  char *beg, *end;
  if(root == NULL) 
    initTree();
  beg = word; end = endform(word);
  if(exists(beg, end) == 2) return -1; //ignore an attempt to insert already inserted word
  //insert word to global array
  length=0;
  beg = strings + stringsIt;
  while(word[length] != '\0'){
    beg[length] = word[length];
    ++length;
  }
  stringsIt = stringsIt + length;
  wordsNumbers[wordsCounter] = beg;
  wordsLengths[wordsCounter++] = length;
  end = beg + length - 1;
  assert(beg <= end);
  return insertion(beg, end);
}

int prev(int number, int start, int end){
  char *begS, *endS;
  if(root == NULL) 
    initTree();
  if(number >= wordsCounter || start > end || end >= wordsLengths[number])
    return -1;
  begS = wordsNumbers[number]+start; 
  endS = wordsNumbers[number]+end;
  if(exists(begS, endS) == 2) 
    return -1;
  wordsNumbers[wordsCounter] = begS;
  wordsLengths[wordsCounter++] = end - start + 1;
  insertion(begS, endS);
}

int find(char *word){
  Tree nearest;
  char *beg = word, *end = endform(word);
  int i;
  if(root == NULL) 
    initTree();
  nearest = lastMatchedNode(&beg, &end);
  if(beg>end){
    printf("YES\n");
    return 1;
  }
 for(i=0; i<nearest->size; ++i){ //looking for partially matching edge
   if(cmprLabels((nearest->edges)[i], beg, end) == end - beg){
     printf("YES\n");
     return 1;
   }
 }
 printf("NO\n");
 return 0;
}

int delete(int number){
  Tree grandparent, parent, victim;
  Edge parentToVictim; 
  int iterE;
  char *beg, *end;
  if(root == NULL) 
    initTree();
  if(number >= wordsCounter || wordsNumbers[number] == NULL)
    return -1;
  beg = wordsNumbers[number]; end = beg + wordsLengths[number] - 1;
  wordsNumbers[number] = NULL;
  wordsLengths[number] = 0;
  assert(exists(beg, end) == 2);
  victim = lastMatchedNode(&beg, &end);
  parent = victim->parent;
  iterE = directEdge(parent, victim);
  assert(iterE != -1); 
  parentToVictim = parent->edges[iterE];
  
  if(victim->size > 1){//breadwinner, make it hub
    victim->isWord = 0;
  }
  else if(victim->size == 1){ //only child, merge edges, remove victim
    mergeWithNextEdge(parentToVictim);
    --nodesCounter;
  }
  else{//no children, look up if hub
    destroy(victim);
    removeEdge(parent, iterE);
    --nodesCounter;
    if(parent->size == 1 && parent->isWord == 0 && parent != root){
      grandparent = parent->parent;
      iterE = directEdge(grandparent, parent);
      assert(iterE != -1);
      mergeWithNextEdge(grandparent->edges[iterE]);
      --nodesCounter;
    }
  }
  printf("deleted: %d\n", number);
  stderrwr();
  return 0;
}

int clear(){
  clear_s();
  printf("cleared\n");
  stderrwr();
  return 0;
}

int clear_s(){
  if(root == NULL) 
    initTree();
  destroy(root);
  root = NULL;
  stringsIt = wordsCounter = nodesCounter = 0;
  return 0;
}
