#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<assert.h>
#include "quiz8.h"

#define DEBUG 0

/**
 *  populate the array with the number of instances
 *  of each character in the given input string.
 *
 */
FreqTable *createFreqTable(char * input){
  /* Allocate space for freqtable on the heap */
  FreqTable* freqtable = (FreqTable*)malloc(sizeof(FreqTable));
  /* Initialize freqtable */
  int i;
  int j = 0;
  for(i = 0; i < NUM_ASCII_CHARS; i++){
    freqtable->charCount[i] = 0;
  }
  /* Put non-empty char in the freqtable */
  while(*(input + j) != '\0'){
    freqtable->charCount[*(input + j)]++;
    j++;
  }
  return freqtable;
}

void destroyFreqTable(FreqTable * freqtable){
  free(freqtable);
}
/**
 * Print out the frequency table.
 * For conciseness, only print out entries that
 * have counts greater than 0.
 *
 * Example output:
 *
 * -----------
 * A: 1
 * D: 3
 * E: 2
 * ___________
 */
void printFreqTable(FreqTable * freqtable){
  int i;
  for(i = 0; i < NUM_ASCII_CHARS; i++){
    if(freqtable->charCount[i] > 0){
      printf("%c: %d\n", i, freqtable->charCount[i]);
    }
  }
  
}

// ------------------- BIT SEQ --------------


BitSeq* createBitSeq(){
  BitSeq *bitseq = malloc(sizeof(BitSeq));
  for(int i = 0; i < MAX_BIT_SEQ_LENGTH; i++){
    bitseq->bits[i] = 0;
  }
  bitseq->numBitsInSeq = 0;
  return bitseq;

}

void destroyBitSeq(BitSeq* bitseq){
  free(bitseq);
}

void printBitSeq(BitSeq* bitseq){
  int m = (bitseq->numBitsInSeq)/16;
  int n = (bitseq->numBitsInSeq)%16;
  for(int i = 0; i < m; i++){
     displayBits(bitseq->bits[i], 16);
     printf("\n");
    }
  displayBits(bitseq->bits[m], n);
    printf("\n");
    
}

void packBits(BitSeq *bitseq, char* newBitsAsChars){
  int i;
  unsigned short mask;
  for(i = 0; i < strlen(newBitsAsChars); i++){
    mask = 0;
    int m = (bitseq->numBitsInSeq)/16;
      int n = (bitseq->numBitsInSeq)%16;
      if(newBitsAsChars[i] == '1'){
        mask = 1 << (15-n);
      }
    bitseq->bits[m] = bitseq->bits[m] | mask;
        bitseq->numBitsInSeq++;
    }
}

void displayBits(unsigned short value, int numBits){
  unsigned short mask = 1 << 15;
    for(int i = 0;i < numBits; i++){
        putchar(value & mask? '1' : '0');
        mask = mask >> 1;
    }
}


// ------------------- HTREE --------------

/**
 *  Given a Frequency Table,
 *  create an Huffman tree.
 *  Return a pointer to the root of the final tree.
 */

int tm = 0;

HTree *newNode(){
  tm++;
  HTree *tree = malloc(sizeof(HTree));
  tree->c = 0;
  tree->freq = 0;
  tree->p0 = NULL;
  tree->p1 = NULL;
  return tree;
}

HTree *setNode(HTree *t, char c, int f, HTree *t1, HTree *t2){
  t->c = c;
  t->freq = f;
  t->p0 = t1;
  t->p1 = t2;
  return t;
}

HTree *newLeaf(char c, int f){
  HTree *tree = newNode();
  return setNode(tree, c, f, NULL, NULL);;
}

HTree *newInternal(HTree *t1, HTree *t2){
  HTree *tree = newNode();
  return setNode(tree, 0, (t1->freq + t2->freq), t1, t2);;  
}
int isLeaf(HTree *t){
  if(t->p0 == NULL){
    return 1;
  }
  else return 0;
}

void destroyNode(HTree *node){
  free(node);
  tm--; 
 }
void destroyHTree(HTree *t){
  if(!isLeaf(t)){
    destroyHTree(t->p0);
    destroyHTree(t->p1);
  }
  destroyNode(t);
}

//Method 1: Use global variables to implement stack

int nStack = 0;
HTree *nodeStack[NUM_ASCII_CHARS];


void pushStack(HTree *t){
  nodeStack[nStack++] = t;
}


HTree *popStack(){
  return nodeStack[--nStack];
}


void populateStack(FreqTable *ft){
  for(int i = 0; i < NUM_ASCII_CHARS; i++){
    if (ft->charCount[i] > 0){
      pushStack(newLeaf(i, ft->charCount[i]));
    }
  }
}


void swapStack (int i, int j){
  HTree *temp = nodeStack[i];
  nodeStack[i] = nodeStack[j];
  nodeStack[j] = temp;

}



void minToBack (){
  int n = nStack - 1;
  int i;
  for(i = 0; i < n; i++){
    if(nodeStack[i]->freq < nodeStack[n]->freq){
      swapStack(i, n);
    }
  }
}
HTree *getMin(){
  minToBack();
  return popStack();
}

HTree* createEncodingTree(FreqTable *table){
  nStack = 0;
  populateStack(table);
  while(nStack > 1){
    HTree *l = getMin();
    HTree *r = getMin();
    HTree *newNode = newInternal(l,r);
    pushStack(newNode);
  }
  return nodeStack[0];
}

void printHTree(HTree *htree){
  if(htree == NULL)
    return;
    printf("(");
    printHTree(htree->p0);
    printf("%c",htree->c);
    printHTree(htree->p1);
    printf(")");
}


//Mathod 2: Use a new struct stack 
/*
struct stack{
  int nStack; //represents the number of HtreeNode in the stack
  HTree *nodeStack[NUM_ASCII_CHARS]; //represents an array of HtreeNode
};

typedef struct stack Stack;

//create a stack 
Stack* createStack(){
  Stack* stack = malloc(sizeof(Stack));
  stack->nStack = 0;
  int i;
  for(i = 0; i < NUM_ASCII_CHARS; i++){
    stack->nodeStack[i] = NULL;
  }
  return stack;
}

// destory a stack 
void destoryStack(Stack *stack){
  int i;
  for(i = 0; i < NUM_ASCII_CHARS; i++){
    destroyNode(stack->nodeStack[i]);
  }
  free(stack); 
}

Stack *pushStack(HTree *t, Stack *stack){
  stack->nodeStack[stack->nStack] = t;
  stack->nStack++;
  return stack; 
}

HTree *popStack(Stack *stack){
  stack->nStack--;
  return (stack->nodeStack[stack->nStack]);
}

Stack *populateStack(FreqTable *ft){
  Stack *stack = createStack();
  for(int i = 0; i < NUM_ASCII_CHARS; i++){
    if (ft->charCount[i] > 0){
      pushStack(newLeaf(i, ft->charCount[i]), stack);
    }
  }
  return stack;
}

Stack *swapStack(Stack *stack, int i, int j){
  HTree *temp = stack->nodeStack[i];
  stack->nodeStack[i] = stack->nodeStack[j];
  stack->nodeStack[j] = temp;
  return stack;
}

Stack *minToBack (Stack *stack){
  int n = (stack->nStack) - 1;
  int i;
  for(i = 0; i < n; i++){
    if(stack->nodeStack[i]->freq < stack->nodeStack[n]->freq){
      swapStack(stack, i, n);
    }
  }
  return stack;
}

HTree *getMin(Stack *stack){
  return popStack(minToBack(stack));
}


HTree* createEncodingTree(FreqTable *table){
  Stack *stack = createStack();
  stack = populateStack(table);
  while(stack->nStack > 1){
    HTree *l = getMin(stack);
    HTree *r = getMin(stack);
    HTree *newNode = newInternal(l,r);
    pushStack(newNode, stack);
  }
  HTree *root = stack->nodeStack[0];
  free(stack);
  return root;
}



// Method 3: Use local variables 
HTree* createEncodingTree(FreqTable *table) {
  int letternum=0;
  HTree **node = malloc(128*sizeof(HTree));
  HTree *root;
  for(int i = 0; i < 128; i++) {
    node[i] = malloc(sizeof(HTree));
    if(table->charCount[i]!=0){
      letternum++;
      node[i]->c=i;
      node[i]->freq=table->charCount[i];
      node[i]->p0=NULL;
      node[i]->p1=NULL;
    }
    else {
    node[i]=NULL;
    }
  }
   
  for(int i = 0; i< letternum - 1; i++){
    int  min1= -1, min2;
    for(int j=0; j< 128 ;j++){
      if(node[j] !=NULL && min1 == -1){
	  min1 = j;
	  continue;
      }
      if(node[j] !=NULL){
	  min2 = j;
	  break;
      }
    }
    for(int j = min2; j<128; j++){
      if(node[j] != NULL){
	if(node[j]->freq < node[min1]->freq){
	  min2 = min1;
	  min1 = j;
	}
	else if (node[j]->freq < node[min2]->freq && j != min1) {
	  min2 = j;
	}
      }
    }
    root = malloc(sizeof(HTree));
    root->c = 0;
    root->freq = node[min1]->freq + node[min2]->freq;
    root->p0 = node[min1];
    root->p1 = node[min2];
    node[min1] = root;
    node[min2] = NULL;
  }
  free(node);
  return root;
}


void destroyHTree(HTree *tree) {
  if(tree==NULL){
    return;
  }
  destroyHTree(tree->p0);
  destroyHTree(tree->p1);
  free(tree);
}
*/

// ----------------Traverse HTree--------------------

/* different bit path for each of the 128 ASCII chars */
/*char *bitsForChar[NUM_ASCII_CHARS];
char path[50];
int nPath = 0;

void addCharToPath(char c){
    path[nPath++] = c;
    path[nPath] = 0;
}
void decPath(){
    path[--nPath] = 0;
}
char* copyPath(){
    char *str = malloc(strlen(path) + 1);
    path[nPath] = 0;
    strcpy(str, path);
    return str;
}


// recursive routine traverses tree and copys the path at each leaf
void rPaths(HTree *t){
    if(isLeaf(t)){
        bitsForChar[t->c] = copyPath();
        return;}
    addCharToPath('0');
    rPaths(t->p0);
    decPath();
    addCharToPath('1');
    rPaths(t->p1);
    decPath();
}

void constructEncodingArray(HTree *root){
    int nPath = 0;
    for(int i = 0; i < NUM_ASCII_CHARS; i++){
        bitsForChar[i] = NULL;
    }
    rPaths(root);
}*/

char path[50]; // no character is going to require more than 50 bits.
int nPath = 0; // keep track of the 0 that marks the end of the character string.

char *bitsForChar[128];// different bit path for each of the 128 ASCII chars.
 
void rPaths(HTree *t, int len){
  static char str[128];
  if (t != NULL){
    if(t->p0 == NULL && t->p1 == NULL){
      char *temp=malloc(len*sizeof(char));
      for(int i=0; i<len; i++){
	temp[i]=str[i];
      }
      bitsForChar[t->c] = temp;
    }
    else {
      str[len] = '0';
      rPaths(t->p0, len+1);
      str[len] = '1';
      rPaths(t->p1, len+1);
    }
  }
}
void constructEncodingArray(HTree *t){
  nPath = 0;
  for(int i=0; i<128; i++){
    bitsForChar[i] = malloc(sizeof(char));;
    bitsForChar[i] = NULL;
  }
  rPaths(t, 0);
 }


// ------------- MAIN FUNCTIONS ----------------

/**
 * Takes in a string and a pointer to a BitSeq to populate,
 * After encode, hTree should be pointing to the root of the final tree.
 */
HTree *encode(char *str, BitSeq *bits) {
  int i=0;
  FreqTable *table = createFreqTable(str);
  HTree *tree = createEncodingTree(table);
  constructEncodingArray(tree);
  while(str[i]!=0){
    packBits(bits, bitsForChar[str[i]]);
    i++;
  }
  return tree;
}

/**
 *  Take in a BitSeq (sequence of bits),
 *  and a Huffman tree.
 *  Use the tree to decode the bit sequence.
 *  Return a pointer to the decoded string.
 */
char *decode(BitSeq *bits, HTree *htree){
	HTree *temp = htree;
	char *string = (char*)malloc(sizeof(char) * MAX_MESSAGE_LENGTH);
	*string = '\0';
	unsigned short mask;
	int char_index = 0;
	for(int i = 0; i < bits->numBitsInSeq + 1; i++){		
		mask = 1;
		mask <<= (15 - (i % 16));
		if(bits->bits[i / 16] & mask){
			temp = temp->p1;
		}else {
			temp = temp->p0;
		}
		if(temp->c != 0){
			string[char_index++] = temp->c;
			string[char_index] = '\0';
			temp = htree;
		}
	}
	return string;
}

/*
void unitTest(){
  assert(tm == 0);
  HTree *t = newNode();
  assert(t != NULL);
  assert(tm == 1);
  HTree *a = setNode(t, 'a',37, NULL, NULL);
  assert(a == t);
  assert(a->c == 'a');
  assert(a->freq == 37);
  assert(a->p0 == NULL);
  assert(a->p1 == NULL);
  assert(tm == 1);
  HTree *b = newLeaf('b',5);
  assert(b->c == 'b');
  assert(b->freq == 5);
  assert(b->p0 == NULL);
  assert(b->p1 == NULL);
  assert(tm == 2);
  HTree *c = newInternal(a, b);
  assert(c->c == 0);
  assert(c->freq == 42);
  assert(c->p0 == a);
  assert(c->p1== b);
  assert(tm == 3);
  assert(isLeaf(a) == 1);
  assert(isLeaf(b) == 1);
  assert(isLeaf(c) == 0);
  destroyHTree(c);
  assert(tm == 0);

  FreqTable *table = createFreqTable("abbb");
  assert(table->charCount['a'] == 1);
  assert(table->charCount['b'] == 3);
  assert(table->charCount['c'] == 0);
    
  populateStack(table);
  assert(nodeStack[0]->c == 'a');
  assert(nodeStack[0]->freq == 1);
  assert(nodeStack[1]->c == 'b');
  assert(nodeStack[1]->freq == 3);

  swapStack(0,1);
  assert(nodeStack[0]->c == 'b');
  assert(nodeStack[1]->c == 'a');
  
  HTree* t1 = popStack();
  assert(nStack == 1);
  assert(t1->c == 'a');
  nodeStack[1] = NULL;
  pushStack(t1);
  assert(nStack == 2);
  assert(nodeStack[1]->c == 'a');

  minToBack();
  assert(nodeStack[1]->c = 'a');
  swapStack(0,1);
  minToBack();
  assert(nodeStack[1]->c = 'a');
  

  t1 = getMin();
  assert(nStack == 1);
  assert(t1->c == 'a');

  destroyNode(t1);
  destroyNode(nodeStack[0]);
  assert(tm == 0);

  
  t1 = createEncodingTree(table);
  assert(t1 == nodeStack[0]);
  assert(t1->p0->c == 'a');
  assert(t1->p1->c == 'b');

  destroyHTree(t1);
  assert(tm == 0);

  printFreqTable(table);
  destroyFreqTable(table);

  printf("passed unit test\n");


}
*/



