#ifndef _PYOBJ_H_
#define _PYOBJ_H_

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pyas/all.h"

//typedef unsigned int pyobj_type ;

typedef enum {
    NULL_TYPE = '0',            // 2.7 Absence d'objet
    NONE_TYPE = 'N',            // 2.7 None
    FALSE_TYPE = 'F',           // 2.7 False
    TRUE_TYPE = 'T',            // 2.7 True
    INT_TYPE = 'i',             // 2.7 Entier signé sur 4 octets
    INT64_TYPE = 'I',           // 2.7 Entier signé sur 8 octets, plus généré
    FLOAT_TYPE = 'f',           // 2.7 Chaîne d'un réel (max : 17 caractères)
    BINARY_FLOAT_TYPE = 'g',    // 2.7 Réel binaire sur 8 octets (double)
    COMPLEX_TYPE = 'x',         // 2.7 Deux chaînes pour un complexe
    BINARY_COMPLEX_TYPE = 'y',  // 2.7 Deux réels pour un complexe
    STRING_TYPE = 's',          // 2.7 Chaîne
    STRINGREF_TYPE = 'R',       // 2.7 Référence à une chaîne internée
    TUPLE_TYPE = '(',           // 2.7 Tuple
    LIST_TYPE = '[',            // 2.7 Liste
    DICT_TYPE = '{',            // 2.7 Dictionnaire
    SET_TYPE = '<',             // 2.7 Ensemble
    CODE_TYPE = 'c',            // 2.7 Objet de code
    STOP_ITER_TYPE = 'S',       // 2.7 Arrêt d'un itérateur
    ELLIPSIS_TYPE = '.',        // 2.7 ...
    LONG_TYPE = 'l',            // 2.7 Entier signé en base 15
    UNICODE_TYPE = 'u',         // 2.7 Chaîne Unicode
    INTERNED_TYPE = 't',        // 2.7 idem, à interner
    UNKNOWN_TYPE = '?',        // 2.7 Objet de type inconnu
    FROZENSET_TYPE = '>',       // 2.7 Ensemble en lecture seule
} pyobj_type;

// #define ASCII 'a' //3.x Chaîne ASCII → Unicode
// #define ASCII_INTERNED 'A' //3.x idem mais à interner
// #define SHORT_ASCII 'z' //3.x Petite chaîne ASCII → Unicode
// #define SHORT_ASCII_INTERNED 'Z' //3.x idem, à interner
// #define SMALL_TUPLE ')' //3.x Petit tuple
// #define REF 'r' //3.x Référence à un objet

typedef struct pyobj * pyobj_t ;

typedef struct {
  int version_pyvm ;
  struct
    {
      uint32_t arg_count ;
      uint32_t local_count ;
      uint32_t stack_size ;
      uint32_t flags ;
    } header ;
pyobj_t parent ;
  struct
  {
    struct
        {
          uint32_t magic ;
          time_t timestamp ;
          uint32_t source_size ;
        } header ;
    struct
      {
          pyobj_t interned ;
          pyobj_t bytecode ;
          pyobj_t consts ;
          pyobj_t names ;
          pyobj_t varnames ;
          pyobj_t freevars ;
          pyobj_t cellvars ;
        } content ;
      struct
      {
          pyobj_t filename ;
          pyobj_t name ;
          uint32_t firstlineno ;
          pyobj_t lnotab ;
        } trailer ;
    } binary ;
} py_codeblock ;






struct pyobj {

  pyobj_type type ;
  unsigned int refcount ;

  union
  {

   struct
   {
     pyobj_t * value ;
     int32_t size ;
   } list ;

   struct
   {
     char * buffer ;
     int length ;
     int line;
   } string ;

   py_codeblock* codeblock ;

   union
   {
     int32_t integer ;
     int64_t integer64 ;
     double real ;

     struct
     {
       double real ;
       double imag ;
      } complex ;

    } number ;



  } py;
};


int free_pyobj_t(pyobj_t p);
int free_codeblock(py_codeblock *cb);
void init_codeblock(py_codeblock *cb);

pyobj_t creat_pyobj_t_string_or_int(lexem_t lex,char type);
pyobj_t* create_tab_pyobj(size_t size);

#endif
