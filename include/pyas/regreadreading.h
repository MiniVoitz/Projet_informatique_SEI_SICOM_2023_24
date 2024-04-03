#ifndef _REGREADREADING_H_
#define _REGREADREADING_H_


//fichier pour la fonction list_t re_read(char *re)

#include "pyas/all.h"
#include "pyas/list.h" /* list_t */
#include "pyas/queue.h"
#include "pyas/binaryblock.h"
#include "pyas/pyobj.h"

void reading( list_t l);
void reading_tab(list_t q);
void print_operator(list_t q);
void print_bytcode(pyobj_t bytecode);
void print_lnotab(pyobj_t lnotab);

#endif
