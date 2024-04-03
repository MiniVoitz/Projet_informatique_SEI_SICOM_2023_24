#ifndef _READINGPYOBJ_H_
#define _READINGPYOBJ_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "pyas/all.h"


void print_pyobjt_header(py_codeblock* py_block);
int main_print_py_codeblock(py_codeblock* py_block);
void print_pyobjt_trailer(py_codeblock* py_block);
void  print_pyobjt_binary_content(py_codeblock* py_block);
void print_pyobjt_object(pyobj_t p);
void print_pyobj_t(pyobj_t obj);







#ifdef __cplusplus
}
#endif

#endif /* _CALLBACKS_H_ */
