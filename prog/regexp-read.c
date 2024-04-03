/**
 * @file regexp.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  28 09:56:03 2022
 * @brief Regexp parsing and matching.
 *
 * Program for regexp parsing, and regexp matching.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pyas/all.h"

int main(int argc, char *argv[]) {

  action_t myFunctionPointer;
  myFunctionPointer = del_binary;

  if (argc != 2) {
    printf("Erreur d'argument \n");
    return 1;
  }

  list_t l;

  l = re_read(argv[1]);

  if(!l)
  {
    printf("The list is empty at the end \n");
    return 1;
  }

  if(((((binaryblock_t)(l->content)))->err))
  {
    list_delete(l, myFunctionPointer);
    exit(EXIT_FAILURE);
  }
  reading(l);
  list_delete(l, myFunctionPointer);

  return 0;
}
