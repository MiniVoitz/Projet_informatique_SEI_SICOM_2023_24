#include <assert.h>
#include <stdio.h>
#include <stdlib.h> /* NULL */
#include <string.h>

#include "pyas/all.h"

void print_operator(list_t q) {
  if (((binaryblock_t)(q->content))->operateur == '+') {
    printf("At least one time ");
  } else if (((binaryblock_t)(q->content))->operateur == '*') {
    printf("At least zero times ");
  } else if (((binaryblock_t)(q->content))->operateur == '?') {
    printf("One time Max ");
  } else if (((binaryblock_t)(q->content))->operateur == 'R' ||
             ((binaryblock_t)(q->content))->crochet) {
    printf("One time ");
  }
}

void reading_tab(list_t q) {
  int *tab = ((binaryblock_t)(q->content))->array;
  int i;
  if (((binaryblock_t)(q->content))->point) {
    printf("ASCII character");
  } else if (((binaryblock_t)(q->content))->negation) {
    printf("Every ASCCI without : (");
    for (i = 0; i < 128; i++) {
      if (!tab[i]) {
        printf("%c", i);
      }
    }
    printf(")");
  } else {
    for (i = 0; i < 128; i++) {
      if (tab[i] == 1) {
        printf("%c", i);
      }
    }
  }
  printf(" ");
  print_operator(q);
  printf("\n");
}

void reading(list_t l) {

  list_t q = l;
  do {
    reading_tab(q);
    q = q->next;
  } while (q != NULL);
}
