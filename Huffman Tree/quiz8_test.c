#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz8.h"

#define assert(EXPRESSION) ((EXPRESSION) ? _assert_pass(#EXPRESSION, __FILE__, __LINE__) : \
                                           _assert_fail(#EXPRESSION, __FILE__, __LINE__))

char *test = "A DEAD DAD CEDED A BAD BABE A BEADED ABACA BED";
static void assert_tree_equals(HTree*, HTree*);

static void _assert_fail(const char* expression, const char* file, int line)
{
  fprintf(stderr, "Assertion '%s' FAILED, file '%s' line '%d'.\n", expression, file, line);
}

static void _assert_pass(const char *expression, const char *file, int line)
{
  fprintf(stdout, "Assertion '%s' passed, file '%s' line '%d'.\n", expression, file, line);
}

static void start_test(const char *test)
{
  static int count = 1;
  printf("Test #%d: %s\n\n", count++, test);
}

static void end_test()
{
  printf("\n--------------------------------\n\n");
}

static void test_encode()
{
  start_test("encode");

  BitSeq seq;
  memset(&seq, 0, sizeof(BitSeq));

  seq.bits[0] = (unsigned short)0b1000011101001000;
  seq.bits[1] = (unsigned short)0b1100100111011001;
  seq.bits[2] = (unsigned short)0b1100100100011111;
  seq.bits[3] = (unsigned short)0b0010011111011111;
  seq.bits[4] = (unsigned short)0b1000100011111101;
  seq.bits[5] = (unsigned short)0b0011100100101111;
  seq.bits[6] = (unsigned short)0b1011101000111111;
  seq.bits[7] = (unsigned short)0b0010000000000000;//0b001;
  seq.numBitsInSeq = 115;

  BitSeq actual;
  memset(&actual, 0, sizeof(BitSeq));

  HTree *encoded = encode(test, &actual);

  assert(actual.numBitsInSeq == seq.numBitsInSeq);

  int i = 0;
  for(; i < MAX_BIT_SEQ_LENGTH; i++) {
    assert(actual.bits[i] == seq.bits[i]);
  }

  destroyHTree(encoded);

  end_test();
}

static void test_decode()
{
  start_test("decode");

  BitSeq seq;
  memset(&seq, 0, sizeof(BitSeq));

  HTree *tree = encode(test, &seq);
  char *decoded = decode(&seq, tree);

  assert(strcmp(test, decoded) == 0);

  destroyHTree(tree);
  free(decoded);

  end_test();
}

static void test_create_encoding_tree()
{
  start_test("encoding tree");

  HTree root = {0, 46, NULL, NULL};
  HTree left = {0, 20, NULL, NULL};
  HTree leaf_ = {' ', 10, NULL, NULL};
  HTree leaf_d = {'D', 10, NULL, NULL};
  HTree right = {0, 26, NULL, NULL};
  HTree leaf_a = {'A', 11, NULL, NULL};
  HTree right_right = {0, 15, NULL, NULL};
  HTree leaf_e = {'E', 7, NULL, NULL};
  HTree right_right_right = {0, 8, NULL, NULL};
  HTree leaf_c = {'C', 2, NULL, NULL};
  HTree leaf_b = {'B', 6, NULL, NULL};

  root.p0 = &left;
  root.p1 = &right;
  left.p0 = &leaf_;
  left.p1 = &leaf_d;
  right.p0 = &leaf_a;
  right.p1 = &right_right;
  right_right.p0 = &leaf_e;
  right_right.p1 = &right_right_right;
  right_right_right.p0 = &leaf_c;
  right_right_right.p1 = &leaf_b;

  FreqTable *table = createFreqTable(test);
  HTree *tree = createEncodingTree(table);

  assert_tree_equals(&root, tree);

  destroyHTree(tree);
  destroyFreqTable(table);

  end_test();
}

static void assert_tree_equals(HTree *t1, HTree *t2)
{
  assert(t1->c == t2->c);

  assert(t1->freq == t2->freq);


  if(t1->p0 == NULL) {
    assert(t2->p0 == NULL);
  }
  else if(t2->p0 != NULL) {
    assert_tree_equals(t1->p0, t2->p0);
  }
  else {
    // this assertion is set up to always fail.
    // Control flow should not reach here.
    assert(t2->p0 != NULL);
  }

  if(t1->p1 == NULL) {
    assert(t2->p1 == NULL);
  }
  else if(t2->p1 != NULL) {
    assert_tree_equals(t1->p1, t2->p1);
  }
  else {
    assert(t2->p0 != NULL);
  }
}

static void test_create_freq_table()
{
  start_test("freq. table");

  FreqTable *table = createFreqTable(test);

  assert(table->charCount['A'] == 11);
  assert(table->charCount['C'] == 2);
  assert(table->charCount['B'] == 6);
  assert(table->charCount['E'] == 7);
  assert(table->charCount['D'] == 10);
  assert(table->charCount[' '] == 10);

  destroyFreqTable(table);

  end_test();
}

static void test_pack_bits()
{
    start_test("pack bits");

    BitSeq *out = createBitSeq(); 
    packBits(out, "1111"); 

    unsigned val = 61440; 

    assert(out->bits[0] == val);

    packBits(out, "110"); 
    val = 64512; 
    assert(out->bits[0] == val);

    packBits(out, "001");
    val = 64576;
    assert(out->bits[0] == val);

    packBits(out, "101"); 
    val = 64616; 
    assert(out->bits[0] == val);

    packBits(out, "1110"); 
    val = 64623; 
    assert(out->bits[0] == val);

    packBits(out, "000"); 
    packBits(out, "110"); 
    packBits(out, "100"); 
    packBits(out, "011"); 


    assert(out->bits[0] == val);
    assert(out->bits[1] == 3352);

    out->numBitsInSeq = 0; 
    out->bits[0] = 0;
    out->bits[1] = 0; 


    packBits(out, "1000011101001000");
    assert(out->bits[0] == 0b1000011101001000);

    packBits(out, "1100100111011001");
    assert(out->bits[1] == 0b1100100111011001);
     
    packBits(out, "1100100100011111"); 
    assert(out->bits[2] == 0b1100100100011111); 

    packBits(out, "0010011111011111");
    assert(out->bits[3] == 0b0010011111011111);

    packBits(out, "1000100011111101");
    assert(out->bits[4] == 0b1000100011111101);

    packBits(out, "0011100100101111");
    assert(out->bits[5] == 0b0011100100101111);

    packBits(out, "1011101000111111"); 
    assert(out->bits[6] == 0b1011101000111111);

    out->numBitsInSeq = 0; 
    out->bits[0] = 0;
    out->bits[1] = 0; 
    out->bits[2] = 0; 

    packBits(out, "100001110"); // 9 bits
    packBits(out, "1001000110010011101"); // 19 bits
    packBits(out, "1001110010010"); // 13 bits
    packBits(out, "0011111") ;

    assert(out->bits[0] == 34632);
    
    assert(out->bits[0] == 0b1000011101001000);
    assert(out->bits[1] == (unsigned short)0b1100100111011001);
    assert(out->bits[2] == (unsigned short)0b1100100100011111); 

    end_test(); 
}


int main(void)
{
  test_create_freq_table();
  test_create_encoding_tree();
  test_encode();
  test_decode();

  test_pack_bits(); 

  return EXIT_SUCCESS;
}
