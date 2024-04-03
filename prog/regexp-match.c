/**
 * @file regexp.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  28 09:56:03 2022
 * @brief Regexp parsing and matching.
 *
 * Program for regexp parsing, and regexp matching.
 */

#include "pyas/all.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  char *end = NULL;
  int is_match;
  action_t myFunctionPointer;
  myFunctionPointer = del_binary;

  if (argc < 3) {
    fprintf(stderr, "Usage :\n\t%s regexp text\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  list_t l = list_new();
  l = re_read(argv[1]);

  if(((((binaryblock_t)(l->content)))->err))
  {
    exit(EXIT_FAILURE);
  }

  is_match = re_match(l, argv[2], &end);

  if (is_match) {
    printf("The start of '%s' is %s, %s: '%s'.\n", argv[2], argv[1],
           *end ? "next" : "END", end);
  } else {
    printf("The start of '%s' is *NOT* %s.\n", argv[2], argv[1]);
  }
  list_delete(l, myFunctionPointer);
  exit(EXIT_SUCCESS);
}
