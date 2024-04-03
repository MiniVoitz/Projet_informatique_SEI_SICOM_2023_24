/**
 * @file regexp.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  28 09:56:03 2022
 * @brief regexp
 *
 * regexp code, as in the project's pdf document
 *
 */

#include <stdio.h>

#include "pyas/regexp.h"

int re_match(list_t regexp, char *source, char **end);


static int re_match_zero_or_more(binaryblock_t block, list_t regexp,char *source, char **end) {

  char *t = source;
  while ('\0' != *t && (block->array[(int)*t] || block->point)){ //While we're not at the end and . or matching char
    t++; //Go through the char*
  }

  do {
    if ( re_match( regexp, t, end ) ) return 1; //Try if it's better to not take every thing
  } while ( t-- > source );

  if (end) //if end != NULL
    *end = source;
  return 0;
}

static int re_match_one_or_more(binaryblock_t block, list_t regexp, char *source, char **end) {
  char *t = source;

  if (block->array[(int)*t] || block->point){
    t++;
  }
  else
    return 0;

  while ('\0' != *t && (block->array[(int)*t] || block->point))
    t++;


    do {
      if ( re_match( regexp, t, end ) ) return 1;
    } while ( --t > source );

  if (end)
    *end = source;
  return 0;
}



static int re_match_zero_or_one(binaryblock_t block, list_t regexp, char *source, char **end) {
  char *t = source;

  // Try matching one character
  if (block->array[(int)*t] || block->point) {
    t++;
  }

  // If the one character matched, try matching zero or more
  do {
    if (re_match(regexp, t, end)) return 1;
  } while (t-- > source);

  if (end)
    *end = source;

  return 0;
}






int re_match(list_t regexp, char *source, char **end) { // list_t is already a pointer to the list, so
                                                        // 'regexp' corresponds to the first link

  if((!list_empty(regexp))&&((((binaryblock_t)(regexp->content)))->err))
  {
    *end = source;
    return 0;
  }

  if (!source) { // if source is NULL
    if (end)
      *end = source;
    return 0;
  }

  if (list_empty(regexp) || NULL == (((binaryblock_t)(regexp->content)))->array) {
    if (end)
      *end = source; // to be sure that end is not NULL
    return 1;
  }

  if (((binaryblock_t)(regexp->content))->operateur == '*') {
    return re_match_zero_or_more((((binaryblock_t)(regexp->content))),
                                 regexp->next, source, end);
  }

  if (((binaryblock_t)(regexp->content))->operateur == '+') {
    return re_match_one_or_more(((binaryblock_t)(regexp->content)),
                                regexp->next, source, end);
  }

  if (((binaryblock_t)(regexp->content))->operateur == '?') {
    return re_match_zero_or_one(((binaryblock_t)(regexp->content)),
                                regexp->next, source, end); // if the character appears 0 times, we go to the next step of the regular expression but we stay at the same place for source
  }

  if (('\0' != *source) &&
      ((((binaryblock_t)(regexp->content))->point) ||
       ((binaryblock_t)(regexp->content))->array[(int)*source])) {
    return re_match(regexp->next, source + 1, end);
  }

  if (end)
    *end = source;
  return 0;
}
