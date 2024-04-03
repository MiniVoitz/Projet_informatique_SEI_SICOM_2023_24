#ifndef _BINARYBLOCKS_H_
#define _BINARYBLOCKS_H_

struct binaryblock {
  int* array;
  char operateur; // * or + or ? => every post fixe R if nothing
  int crochet;  // if there is a [or not
  int negation; // 1 if there is a ^ before the block
  int point; //1 if '.' => so we don't have do fill the table of 1
  int err; //if 1 there is a syntaxe error
};

typedef struct binaryblock *binaryblock_t;

void print_binaryblock(binaryblock_t bb);

#endif
