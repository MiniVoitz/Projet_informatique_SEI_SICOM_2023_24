#include "pyas/all.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int free_pyobj_t(pyobj_t p)
{
  int free_ok = 1;
  int i;
  if(!p)
  {
    return 1;
  }

  if(p->type == '[' || p->type == '(' || p->type == 't')
    {
      // printf("liste :\n");
      for(i = 0; i < p->py.list.size;i++)
        {
          free_ok = free_ok && free_pyobj_t(p->py.list.value[i]);
        }
        free(p->py.list.value);
        free(p);
        return free_ok;

    }
/*
  if(p->type == 's')
  {
    free(p->py.string.buffer);

  }
  if(p->type == 'c')
  {
    free_ok = free_ok && free_codeblock(p->py.codeblock);
  }
  free(p);
*/
    if(p->type == 'c')  // C'est le codeblock làà
  {
    // printf("constate le codeblock dans le quel on rentre la \n");
    free_ok = free_ok && free_codeblock(p->py.codeblock);
  }
   if(p->type == '0')  // C'est le codeblock làà
  {
    free(p);
    return free_ok;
  }

  if(p->type == 's')
  {
    // printf("On libère un string %s \n",p->py.string.buffer);
    free(p->py.string.buffer);
    free(p);
    return free_ok;
  }

  if(p->type == 'i')
  {
    // printf("On libère un int %d \n",p->py.number.integer);
    free(p);
    return free_ok;
  }

   if(p->type == 'N'||p->type=='T'||p->type=='F')
  {
    // printf("On libère un boolean  \n");
    //free(p->py.number.integer);
    free(p);
    return free_ok;
  }

  return free_ok;
}

int free_codeblock(py_codeblock *cb)
{
  int i = 1;
  if(!cb)
  {
    return 1;
  }
/*
  if(cb->parent)
  {
    pyobj_t py= cb->parent;
    cb->parent = NULL;
    free_pyobj_t(py);
  }
  */
  // printf("\n");
  i = i&&free_pyobj_t(cb->binary.content.interned);
    // printf("\n");

  i = i&&free_pyobj_t(cb->binary.content.bytecode);
  i = i&&free_pyobj_t(cb->binary.content.consts);
    // printf("\n");

  i = i&&free_pyobj_t(cb->binary.content.names);
  // printf("\n");

  i = i&&free_pyobj_t(cb->binary.content.varnames);
  i = i&&free_pyobj_t(cb->binary.content.freevars);
  i = i&&free_pyobj_t(cb->binary.content.cellvars);

  // printf("\n");
  i = i&&free_pyobj_t(cb->binary.trailer.filename);

  i = i&&free_pyobj_t(cb->binary.trailer.name);
  //i = i&&free_pyobj_t(cb->binary.trailer.lnotab);
  return i;
}


void init_codeblock(py_codeblock *cb)
{
cb->binary.content.interned = NULL;
cb->binary.content.bytecode = NULL;
cb->binary.content.consts = NULL;
cb->binary.content.names = NULL;
cb->binary.content.varnames = NULL;
cb->binary.content.freevars = NULL;
cb->binary.content.cellvars = NULL;
cb->binary.trailer.filename = NULL;
cb->binary.trailer.name = NULL;
cb->binary.trailer.lnotab = NULL;
return;
}


pyobj_t creat_pyobj_t_string_or_int(lexem_t lex ,char type)
{
  pyobj_t p = calloc(1,sizeof(struct pyobj));



    if(type =='s')
    {

      p->type = 's';
      p->refcount += 1;
      char* _value = lex->value;
      int ln = strlen(_value);
      (p->py).string.buffer = _value;
      (p->py).string.length = ln;
    }

    // else if (type == 'i' || type == 'I' || type == 'g')//merttre en plusieurs else if
    // {
    //   printf("A FZIRE EN FAIT cf clement \n");
    //
    // }
free(lex->type);
free(lex);
    return p;
}

//--------------------------------------------------//






pyobj_t create_pyobj_t_list(size_t size,pyobj_t pyobj_list)
{
  pyobj_list = calloc(1,sizeof(struct pyobj));

  if (pyobj_list == NULL) {
        fprintf(stderr, "Échec de l'allocation mémoire.\n");
        return NULL;
    }


  pyobj_list->type = '[';
  pyobj_list->py.list.size = (int)size;
  pyobj_list->py.list.value = NULL;

  printf("seg fault \n");

  if(pyobj_list->type == '[')
  {
  }

  return pyobj_list;
}








//--------------------------------------------------//
pyobj_t* create_tab_pyobj(size_t size)
{


  return (pyobj_t *)malloc(size * sizeof(struct pyobj));

}
