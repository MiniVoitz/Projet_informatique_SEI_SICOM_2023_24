#ifndef _PARSE_H_
#define _PARSE_H_


//fichier pour la fonction list_t re_read(char *re)

#include "pyas/all.h"
#include "pyas/pyobj.h"
#include "pyas/lexem.h"


int parse_eol (list_t *lexems);
int parse_pys( list_t *lexems, py_codeblock* py_block);
int parse_prologue( list_t *lexems, py_codeblock* py_block);
int parse_set_directives( list_t *lexems, py_codeblock* py_block);
int parse_set_version_pyvm ( list_t *lexems, py_codeblock* py_block);
int parse_set_flags ( list_t *lexems, py_codeblock* py_block);
int parse_set_filename ( list_t *lexems, py_codeblock* py_block);
int parse_set_name ( list_t *lexems, py_codeblock* py_block);
int parse_set_stack_size ( list_t *lexems, py_codeblock* py_block);
int parse_set_arg_count ( list_t *lexems, py_codeblock* py_block);
int parse_interned_strings ( list_t *lexems, py_codeblock* py_block);
int parse_constants(list_t *lexems, py_codeblock* py_block);
int parse_constant(list_t *lexems, pyobj_t py_block);
int parse_tuple(list_t *lexems, pyobj_t py_block);
int parse_names(list_t *lexems, py_codeblock* py_block);
int parse_code(list_t *lexems, py_codeblock* py_block);
int parse_assembly_line(list_t *lexems, pyobj_t py_block);
int parse_insn(list_t *lexems, pyobj_t py_block);
int parse_source_lineno(list_t *lexems, pyobj_t py_block);
int errorpys(list_t *lexems);
int parse_label(list_t *lexems, py_codeblock* py_block);
int parse_set_kwonly_arg_count( list_t *lexems, py_codeblock* py_block);
int parse_set_posonly_count( list_t *lexems, py_codeblock* py_block);
int parse_set_source_size( list_t *lexems, py_codeblock* py_block);
#endif
