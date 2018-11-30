#ifndef QUIZ8_H
#define QUIZ8_H

// Max number of unsigned ints we can use to represent a bit sequence.
// Note: this means the max number of bits is MAX_BIT_SEQ_LENGTH * 16.
#define MAX_BIT_SEQ_LENGTH 100

// Number of ASCII chars there are
#define NUM_ASCII_CHARS 128

// Max number of characters of the message to be 
// either encoded or decoded. 
#define MAX_MESSAGE_LENGTH 500



// ------------------- FREQ TABLE --------------

struct freqTable{
	int charCount[NUM_ASCII_CHARS];
};

typedef struct freqTable FreqTable;

/**
 * 	populate the array with the number of instances
 * 	of each character in the given input string.
 *
 */
FreqTable *createFreqTable(char *);

void destroyFreqTable(FreqTable *);

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
void printFreqTable(FreqTable *);





// ------------------- BIT SEQ --------------

struct bitSeq{
	unsigned short bits[MAX_BIT_SEQ_LENGTH];
	int numBitsInSeq;
};

typedef struct bitSeq BitSeq;


BitSeq* createBitSeq();

void destroyBitSeq(BitSeq*);

void printBitSeq(BitSeq *);

void packBits(BitSeq *bits, char* newBitsAsChars);

void displayBits(unsigned short value, int numBits);


// ------------------- HTREE --------------

typedef struct HTree HTree;

// An HTree is a node in a tree that supports Huffman
// encoding. The c field is the character. 
struct HTree{
	char c;
	int freq;
  
	HTree *p0;
	HTree *p1;
  //  int parent; //0 represents has no parent; 1 represents has parent
};

/**
 *	Given a Frequency Table,
 * 	create an Huffman tree.
 * 	Return a pointer to the root of the final tree.
 */
HTree* createEncodingTree(FreqTable *);


void destroyHTree(HTree *);

void printHTree(HTree *);


// ------------- MAIN FUNCTIONS ----------------

/**
 * Takes in a string and a pointer to a BitSeq to populate,
 * After encode, hTree should be pointing to the root of the final tree.
 */
HTree *encode(char *, BitSeq *);

/**
 *	Take in a BitSeq (sequence of bits),
 * 	and a Huffman tree.
 * 	Use the tree to decode the bit sequence.
 * 	Return a pointer to the decoded string.
 */
char *decode(BitSeq *, HTree *);



#endif
