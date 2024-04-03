/**
 * @file queue.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 19:02:17 2021
 * @brief Queue.
 *
 * Queue.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h> /* NULL */

#include "pyas/binaryblock.h"
#include "pyas/queue.h"

struct link_t {
  void *content;
  struct link_t *next;
};



queue_t queue_new( void ) {
  return NULL;
}

int queue_empty(queue_t q) {
  return queue_new() == q; //  the return of this function is boolean
  // 1 if the parameter q is a NULL pointer.
}

queue_t enqueue( queue_t q, void * object) // add a element to the queue
 {

  // pointer to structur
  struct link_t *new_q = queue_new();
  //free(new_q);
  // dynamic memory allocation for the structur
  new_q = calloc(1, sizeof(*new_q));

  // To use this pointer to access and manipulate members of the
  // structure, you need to allocate memory for the structure itself

  if (queue_empty(q)) {

    new_q->content = object;
    new_q->next = new_q;

  }

  else {
    new_q->next = q->next;
    new_q->content = object;
    q->next = new_q;
  }

  return new_q;
}

list_t queue_to_list(queue_t q) {
  list_t l = list_new();
  if(!q)
  {
      return l;
  }
  l = q->next;
  q->next = NULL;
  return l;
}

queue_t free_queue(queue_t q) {
  if (queue_empty(q)) {
    return NULL;
  }

  if (q->next == q) {
    printf("ceci est un testtttt\n");
    free(q);
    return NULL;
  }

  else {
    queue_t out = q->next;
    free(q);
    printf("ceci est un test\n");
    free_queue(out);
  }
  return q;
}

int del_binary(void * q)
{
if(!q)
{
  return 1;
}
binaryblock_t b = q;
if(b->array) free(b->array); //on libere le tableau
free(b); //on libere la structure
return 1;


}


int del_lexem(void * q)
{
lexem_t lex = q;

free(lex->value); //on libere le tableau
free(lex->type); //on libere le tableau

free(lex); //on libere la structure

return 1;
}
