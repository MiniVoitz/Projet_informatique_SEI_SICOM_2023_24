/**
 * @file all.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 19:08:12 2021
 * @brief General include.
 *
 * General include.
 */

#ifndef _ALL_H_
#define _ALL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pyas/list.h"
#include "pyas/queue.h"
#include "pyas/binaryblock.h"
#include "pyas/lexem.h"
#include "pyas/regread.h"
#include "pyas/regexp.h"
#include "pyas/regreadreading.h"
#include "pyas/parse_function.h"
#include "pyas/pyobj.h"
#include "pyas/read_fic.h"
#include "pyas/reading_pyobj.h"
#include "pyas/pyasm.h"
#include "pyas/write.h"


struct link_t {
  void          *content;
  struct link_t *next;
};


typedef struct link_t *queue_t;



#ifdef __cplusplus
}
#endif

#endif /* _ALL_H_ */
