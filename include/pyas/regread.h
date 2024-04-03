#ifndef _REGREAD_H_
#define _REGREAD_H_


//fichier pour la fonction list_t re_read(char *re)

#include "pyas/list.h" /* list_t */
#include "pyas/queue.h"
#include "pyas/binaryblock.h"

binaryblock_t fonction_crochet(char* expr);
list_t re_read(char *regexp);

#endif
