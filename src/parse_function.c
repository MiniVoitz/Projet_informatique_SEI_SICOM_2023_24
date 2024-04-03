#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "pyas/parse_function.h"

//---------------------------------------------------------//
//Error Case
int errorpys(list_t *lexems){lexem_t lex = lexem_peek(lexems);
	if(lex)
	{
		printf("Error at : (%d,%d) in type : %s value: %s \n", lex->line,lex->column,lex->type,lex->value);
	}
	else
	{
		printf("We're trying to read lexems but is NULL \n");
	}
	printf("-------PYS FILE NOT VALIDATED-------- \n");
	return 0;
}
//---------------------------------------------------------//


int parse_eol (list_t *lexems){
	if(next_lexem_is(lexems,"newline")) //Is there a eol ?
			{
				while(next_lexem_is(lexems,"newline")) //Skip each eol
				{
					del_lexem(lexem_advance(lexems));
				}
				return 1;
			}
	else
		{
			return 0 ; //No eof
		}
}



// -------------------------------------//


int parse_pys(list_t *lexems, py_codeblock* py_block)
{
	if(!lexems)
	{
		return -1;
	}
	if(next_lexem_is(lexems,"newline")) //Skip eof if there is any
	{
		 parse_eol(lexems);
	}

	if ( -1 == parse_prologue(lexems,py_block))
	{
		return -1;
	}


	if(-1==parse_code(lexems,py_block))
	{
		return -1;
	}
	printf("-------------------------------------------\n");
	printf("-------PYS FILE HAS BEEN VALIDATED---------\n");
	printf("-------------------------------------------\n");

	return 1;
}


//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//


//     PROLOGUE


int parse_prologue( list_t *lexems, py_codeblock* py_block){


	if ( -1 == parse_set_directives( lexems,py_block))
	{
		printf("Error in set-direcrtives \n");
		errorpys(lexems);
		return -1;
	}


	if ( -1 == parse_interned_strings( lexems,py_block)){
		return -1;
	}

	if ( -1 == parse_constants( lexems,py_block)){
		return -1;
	}


	if ( -1 == parse_names( lexems,py_block)){

	}

	return 0;
}

int parse_set_directives( list_t *lexems, py_codeblock* py_block){
	if (-1 == parse_set_version_pyvm(lexems,py_block))
	{
		return -1;
	}
	if ( -1 == parse_set_flags( lexems,py_block))
	{
		return -1;
	}

	if ( -1 == parse_set_filename( lexems,py_block)){
		return -1;
	}


	if ( -1 == parse_set_name( lexems,py_block)){
		return -1;
	}

	if ( -1 == parse_set_source_size( lexems,py_block)){

	}

	if ( -1 == parse_set_stack_size( lexems,py_block)){
		return -1;
	}

	if ( -1 == parse_set_arg_count( lexems,py_block)){
		return -1;
	}

	if ( -1 == parse_set_kwonly_arg_count( lexems,py_block)){

	}

	if ( -1 == parse_set_posonly_count( lexems,py_block)){

	}

	// ici peut-etre qqch pour set_kwonly_arg_count plus tard
	// ici peut-etre qqch pour set_posonly_arg_count plus tard
	return 0;
}


//----------------------------------------------------------//


int parse_set_version_pyvm ( list_t *lexems, py_codeblock* py_block)
{

	lexem_t lex;
	if ( next_lexem_is( lexems, "dir::set"))
	{
		del_lexem(lexem_advance ( lexems ));
	}

	else
	{
		printf("erreur sur le dir version_pyvm\n" );
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "symbol::version_pyvm")){
		lex = lexem_advance ( lexems );
		del_lexem(lex);
	}

	else {
		printf("erreur symbol::version_pyvm \n");
		errorpys(lexems);}

	if ( next_lexem_is( lexems, "integer::dec")){
		lex = lexem_advance ( lexems );
		py_block->version_pyvm = atoi((lex->value));
		del_lexem(lex);
	}

	else {
		printf("erreur dec version_pyvm \n");
		errorpys(lexems);
		return -1;}

	if ( 0 == parse_eol( lexems)){
		printf("erreur return at version_pyvm");
		errorpys(lexems);
		return -1;
	}

	return 0;
}


//----------------------------------------------------------//
int parse_set_flags ( list_t *lexems, py_codeblock* py_block){

	lexem_t lex;
	if ( next_lexem_is( lexems, "dir::set"))
	{
		del_lexem(lexem_advance (lexems));
	}
	else
	{
		errorpys(lexems);
		return -1;
	}

	if (next_lexem_is(lexems, "symbol::flags"))
	{
		del_lexem(lexem_advance ( lexems ));

	}

	else
	{
		printf("erreur symbol::flags \n");
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "integer::hex"))
	{
		lex = lexem_advance ( lexems );
		char *endptr;
    	unsigned long hexValue = strtoul(lex->value, &endptr, 16);
		(py_block->header).flags = (uint32_t)(hexValue);
		del_lexem(lex);
	}

	else
	{
		printf("erreur dec flags\n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at flags");
		errorpys(lexems);
		return -1;
	}

	return 0;
}

//----------------------------------------------------------//



int parse_set_filename ( list_t *lexems, py_codeblock* py_block){
	if ( next_lexem_is( lexems, "dir::set"))
	{
		del_lexem(lexem_advance ( lexems ));
	}

	else
	{
		printf("erreur sur le dir filename \n" );
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "symbol::filename"))
	{
		del_lexem(lexem_advance ( lexems ));
	}

	else
	{
		printf("erreur symbol::filename \n");
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "string"))
	{
		lexem_t lex = lexem_advance ( lexems ) ;
		pyobj_t _name = creat_pyobj_t_string_or_int(lex ,'s');
		((py_block->binary).trailer).filename = _name;


	}

	else
	{
		printf("erreur string filename  \n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at filename");
		errorpys(lexems);
		return -1;
	}

	return 0;
}

//----------------------------------------------------------//


int parse_set_name ( list_t *lexems, py_codeblock* py_block){
	if ( next_lexem_is( lexems, "dir::set"))
	{
		del_lexem(lexem_advance ( lexems ));
	}

	else
	{
		printf("erreur sur le dir name\n" );
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "symbol::name"))
	{
		del_lexem(lexem_advance ( lexems ));
	}

	else
	{
		printf("erreur symbol::name \n");
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "string"))
	{
		pyobj_t _name = creat_pyobj_t_string_or_int(lexem_advance ( lexems ) ,'s');
		((py_block->binary).trailer).name = _name;
	}

	else
	{
		printf("erreur dec \n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at name");
		errorpys(lexems);
		return -1;
	}
	return 0;
}

//----------------------------------------------------------//

int parse_set_source_size( list_t *lexems, py_codeblock* py_block){

	if ( next_lexem_is( lexems, "dir::set"))
	{
		if ( next_next_lexem_is( lexems, "symbol::source_size"))
		{
			del_lexem(lexem_advance ( lexems) );
			del_lexem(lexem_advance ( lexems) );
		}
		else {
			return 0;}
	}

	else
	{
		printf("erreur apres un .set il devrait y avoir quelque chose \n");
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "integer::dec"))
	{
		lexem_t lex = lexem_advance ( lexems  );//stack_size
		(py_block->header).stack_size = atoi(lex->value);
		del_lexem(lex);
	}

	else
	{
		printf("erreur dec source_size\n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at source_size");
		errorpys(lexems);
		return -1;
	}
	return 0;


}


//----------------------------------------------------------//


int parse_set_stack_size( list_t *lexems, py_codeblock* py_block){
	if ( next_lexem_is( lexems, "dir::set"))
	{
		del_lexem(lexem_advance ( lexems) );
	}

	else
	{
		printf("erreur sur le dir stack_size \n" );
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "symbol::stack_size"))
	{
		del_lexem(lexem_advance ( lexems) );
	}

	else
	{
		printf("erreur symbol::stack_size \n");
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "integer::dec"))
	{
		lexem_t lex = lexem_advance ( lexems  );//stack_size
		(py_block->header).stack_size = atoi(lex->value);
		del_lexem(lex);
	}

	else
	{
		printf("erreur dec stack_size\n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at stack_size");
		errorpys(lexems);
		return -1;
	}
	return 0;
}


//----------------------------------------------------------//


int parse_set_arg_count ( list_t *lexems, py_codeblock* py_block)
{
	if ( next_lexem_is( lexems, "dir::set"))
	{
		del_lexem(lexem_advance ( lexems) );
	}

	else
	{
		printf("erreur sur le dir arg_count\n" );
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "symbol::arg_count"))
	{
		del_lexem(lexem_advance ( lexems) );
	}

	else
	{
		printf("erreur symbol::arg_count \n");
		errorpys(lexems);
		return -1;
	}

	if ( next_lexem_is( lexems, "integer::dec"))
	{
		lexem_t lex = lexem_advance ( lexems );
		(py_block->header).arg_count = atoi((lex->value));
		del_lexem(lex);
	}

	else
	{
		printf("erreur dec arg_count \n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at arg_count");
		errorpys(lexems);
		return -1;
	}
	return 0;
}
//----------------------------------------------------------//

int parse_set_kwonly_arg_count( list_t *lexems, py_codeblock* py_block){

	if ( next_lexem_is( lexems, "dir::set"))
	{
		if ( next_next_lexem_is( lexems, "symbol::kwonly_arg_count"))
		{
			del_lexem(lexem_advance ( lexems) );
			del_lexem(lexem_advance ( lexems) );
		}
		else {
			return 0;}
	}

	else
	{
		return 0;
	}

	if ( next_lexem_is( lexems, "integer::dec"))
	{
		lexem_t lex = lexem_advance ( lexems  );//stack_size
		(py_block->header).stack_size = atoi(lex->value);
		del_lexem(lex);
	}

	else
	{
		printf("erreur dec kwonly_arg_count\n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at kwonly_arg_count");
		errorpys(lexems);
		return -1;
	}
	return 0;


}


//----------------------------------------------------------//

int parse_set_posonly_count( list_t *lexems, py_codeblock* py_block){

	if ( next_lexem_is( lexems, "dir::set"))
	{
		if ( next_next_lexem_is( lexems, "symbol::posonly_arg_count"))
		{
			del_lexem(lexem_advance ( lexems) );
			del_lexem(lexem_advance ( lexems) );
		}
		else {
			return 0;}
	}

	else
	{
		return 0;
	}

	if ( next_lexem_is( lexems, "integer::dec"))
	{
		lexem_t lex = lexem_advance ( lexems  );//stack_size
		(py_block->header).stack_size = atoi(lex->value);
		del_lexem(lex);
	}

	else
	{
		printf("erreur dec posonly_arg_count\n");
		errorpys(lexems);
		return -1;
	}

	if ( 0 == parse_eol( lexems))
	{
		printf("erreur return at posonly_arg_count");
		errorpys(lexems);
		return -1;
	}
	return 0;


}

//---------------------------------------------------------//

int parse_interned_strings ( list_t *lexems, py_codeblock* py_block)
{

	if ( next_lexem_is( lexems, "dir::interned"))
		{
			del_lexem(lexem_advance ( lexems) );
   		}

 	if ( 0 == parse_eol( lexems))
	{
		printf("erreur no return at interned_string \n ");
		errorpys(lexems);
		return -1;
	}

	lexem_t lex;
	queue_t q = queue_new();

	while(next_lexem_is(lexems , "string"))
	{
		lex = lexem_advance(lexems);
		q = enqueue(q,lex);

		if (0 == parse_eol( lexems))
			{
				printf("Missing return within interned_string\n");
				list_t l = queue_to_list(q); //List of useful lexem
				list_t liste_buffer = l;
				while(liste_buffer)
				{
					list_t out = liste_buffer->next;
					del_lexem(liste_buffer->content);

					liste_buffer=out;
				}
				del_lexem(lex);


				errorpys(lexems);
				return -1;
			}

	}

	list_t l = queue_to_list(q); //List of useful lexem

	size_t size = list_length(l);
	list_t liste_buffer = l;


	pyobj_t pyobj_list = calloc(1,sizeof(struct pyobj));
	pyobj_list->type = '[';
  	pyobj_list->py.list.size = (int)size;
  	pyobj_list->py.list.value = NULL;



  	pyobj_t *py_tbl = create_tab_pyobj(size);


	if (py_tbl == NULL)
	{
		 printf("Memory allocation failed\n");
		 return -1;
 	}

 	size_t i;
 	for (i = 0; i < size; i++)
 	{
 		pyobj_t maillon = creat_pyobj_t_string_or_int(l->content,'s');
	 	(py_tbl[i]) = maillon;
	 	l=l->next;
	}




	while(liste_buffer)
			{
			list_t out = liste_buffer->next;
			free(liste_buffer);
			liste_buffer=out;
			}
  	((py_block->binary).content).interned = pyobj_list;
	pyobj_list->py.list.value = py_tbl;



	if(next_lexem_is(lexems,"dir::consts"))
		{
			return 0;
		}

	else
	{
		return -1;
	}
}

//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//


//      		FIN DU PROLOGUE

// 				DEBUT DES CONSTANTES


//-----------------------------------------------------------------------//
//-----------CONSTANTS  & TUPLES---------------------------------------------------//

int parse_constant(list_t *lexems, pyobj_t py_block)
{
	lexem_t lex;



	if ( next_lexem_is( lexems, "integer::dec")||next_lexem_is( lexems, "integer::hex"))
	{
		lex = lexem_advance ( lexems );
		if (sizeof(int) == 4)
		{
			py_block->type = 'i';	//32bit
			py_block->py.number.integer = atoi(lex->value);
			del_lexem(lex);
		}

		else
		{
			py_block->type = 'I';//64bit
			py_block->py.number.integer64 = (int64_t)(lex->value); //atoi(lex->value); cast ?????
			del_lexem(lex);
		}
	}

	else if ( next_lexem_is( lexems, "float")||next_lexem_is( lexems, "float::exp"))
	{
		lex = lexem_advance ( lexems );
		py_block->type = 'f';	//32bit
		py_block->py.number.real = (double)*(lex->value);
		del_lexem(lex);

	}

	else if ( next_lexem_is( lexems, "string"))
	{
		lex = lexem_advance ( lexems );
		py_block->type = 's';
		py_block->refcount +=1;
		char* value = lex->value;
		int ln = strlen(lex->value);
		py_block->py.string.buffer = strdup(value);
		py_block->py.string.length = ln;
		del_lexem(lex);
	}

	else if ( next_lexem_is( lexems, "pycst::None"))
	{
		lex = lexem_advance ( lexems );
		del_lexem(lex);
		py_block->type = 'N';
	}

	else if ( next_lexem_is( lexems, "pycst::True"))
	{
		lex = lexem_advance ( lexems );
		del_lexem(lex);
		py_block->type = 'T';
	}

	else if ( next_lexem_is( lexems, "pycst::False"))
	{
		lex = lexem_advance ( lexems );
		del_lexem(lex);
		py_block->type = 'F';

	}
	else if ( parse_tuple(lexems,py_block))
	{
		lex = lexem_advance ( lexems );
		del_lexem(lex);
	}
	else
	{
		return 0;
	}

	return 1;
}


int parse_constants(list_t *lexems, py_codeblock* py_block)
	{

		if ( next_lexem_is( lexems, "dir::consts"))
		{
			del_lexem(lexem_advance ( lexems ));
   		}


		if ( 0 == parse_eol( lexems))
		{
			printf("erreur no return after .consts \n ");
			errorpys(lexems);
			return -1;
		}

		queue_t q = queue_new();
		pyobj_t cste;
		cste = (pyobj_t)malloc(sizeof(struct pyobj));


		while(parse_constant(lexems,cste))
		{
			q = enqueue(q,cste);

		 	cste = (pyobj_t)malloc(sizeof(struct pyobj));

		 	if ( 0 == parse_eol( lexems))
		 		{
					printf("erreur no return after const \n ");
					list_t l = queue_to_list(q); //List of useful lexem
					list_t liste_buffer = l;
					while(liste_buffer)
					{
						list_t out = liste_buffer->next;
						free_pyobj_t(liste_buffer->content);
						liste_buffer->content=NULL;
						free(liste_buffer);
						liste_buffer=out;
					}

					errorpys(lexems);
					free(cste);
					return -1;
				}

		}
		free(cste);

		list_t pyobj_liste = queue_to_list(q);
		list_t liste_buffer = pyobj_liste;
		size_t ln = list_length(pyobj_liste);

		pyobj_t py_obj_constantes = calloc(1,sizeof(struct pyobj));
	 	py_obj_constantes->type = '[';
  		py_obj_constantes->py.list.size = (int)ln;

		pyobj_t *py_tbl = create_tab_pyobj(ln);

		if (py_tbl == NULL)
		{
		 	printf("Memory allocation  at cosntantes failed\n");
		 	return -1;
 		}

 		size_t i;
 		for (i = 0; i < ln; i++)
 		{
				py_tbl[i] = pyobj_liste->content;
				//free(pyobj_liste->content);
				pyobj_liste = pyobj_liste->next;
		}
//free_queue(q);
		while(liste_buffer)
			{
			list_t out = liste_buffer->next;
			free(liste_buffer);
			liste_buffer=out;
			}
 		//py_obj_constantes->py.list.value = create_tab_pyobj(ln);
		((py_block->binary).content).consts = py_obj_constantes;
		py_obj_constantes->py.list.value = py_tbl;

return 0;
	}




int parse_tuple(list_t *lexems, pyobj_t py_block)
{
	py_block->type = '(';

	if ( next_lexem_is( lexems, "paren::left"))
	{
		del_lexem(lexem_advance ( lexems) );
  	}

	else
	{
		return 0; //Not a tupple
	}

	queue_t q = queue_new();

 	pyobj_t cste = calloc(1,sizeof(struct pyobj));

  	while(parse_constant(lexems,cste))
   		{
   			q = enqueue(q,cste);
		 	cste = calloc(1,sizeof(struct pyobj));
		 }


   	if (next_lexem_is( lexems, "paren::right"))
		{
			free_pyobj_t(cste);
			list_t pyobj_liste = queue_to_list(q);

			size_t ln = list_length(pyobj_liste);

			list_t liste_buffer = pyobj_liste;

	 		py_block->py.list.size = (int)ln;

			pyobj_t *py_tbl = create_tab_pyobj(ln); //alloc du tableau

			if (py_tbl == NULL)
			{
		 	printf("Memory allocation  at cosntantes failed\n");
		 	return -1;
 			}

 			size_t i;
	 		for (i = 0; i < ln; i++)
 				{
				py_tbl[i] = pyobj_liste->content;
				pyobj_liste = pyobj_liste->next;

				}
			while(liste_buffer)
			{
				list_t out = liste_buffer->next;
				free(liste_buffer);
				liste_buffer=out;
			}

		py_block->py.list.value = py_tbl;



		return 1;
   		}


   	else
   	{
   	printf("ERROR no ')' in tuple .const\n");
   	free(cste);

   	list_t pyobj_liste = queue_to_list(q);
   	list_t liste_buffer = pyobj_liste;
   	while(liste_buffer)
			{
				list_t out = liste_buffer->next;
				free_pyobj_t(liste_buffer->content);
				free(liste_buffer);
				liste_buffer=out;
			}
			py_block->type = '0';
		errorpys(lexems);
		py_block=NULL;
		return -1;
	}

return 0;
}





int parse_names(list_t *lexems, py_codeblock* py_block)
	{
		if ( next_lexem_is( lexems, "dir::names"))
		{
			del_lexem(lexem_advance ( lexems) );

   		}
   		else
   		{
					return 0;
   		}

   	if ( 0 == parse_eol( lexems))
		{
			printf("erreur no return after .name \n ");
			errorpys(lexems);
			return -1;
		}

	lexem_t lex;
	queue_t q = queue_new();

	while(next_lexem_is(lexems , "string"))
	{
		lex = lexem_advance(lexems);
		q = enqueue(q,lex);

		if (0 == parse_eol( lexems))
			{
				printf("Missing return within interned_string\n");
				errorpys(lexems);
				return -1;
			}
	}

	list_t l = queue_to_list(q); //List of useful lexem
list_t liste_buffer = l;
	size_t size = list_length(l);


	pyobj_t pyobj_list = calloc(1,sizeof(struct pyobj));
	pyobj_list->type = '[';
  	pyobj_list->py.list.size = (int)size;
  	pyobj_list->py.list.value = NULL;



  	pyobj_t *py_tbl = create_tab_pyobj(size);


	if (py_tbl == NULL)
	{
		 printf("Memory allocation failed\n");
		 return -1;
 	}

 	size_t i;
 	for (i = 0; i < size; i++)
 	{
 		pyobj_t maillon = creat_pyobj_t_string_or_int(l->content,'s');
	 	(py_tbl[i]) = maillon;
	 	l=l->next;
	}

	while(liste_buffer)
			{
			list_t out = liste_buffer->next;
			free(liste_buffer);
			liste_buffer=out;
			}
  	((py_block->binary).content).names = pyobj_list;
	pyobj_list->py.list.value = py_tbl;

	return 0;
	}
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//--------------------------------------------CODE-------------------------------------//

int parse_code(list_t *lexems, py_codeblock* py_block)
{
	if ( next_lexem_is( lexems, "dir::text"))
		{
			del_lexem(lexem_advance ( lexems ));
		}
	   	if ( 0 == parse_eol( lexems))
				{
					printf("erreur no return after .text \n ");
					errorpys(lexems);
					return -1;
				}

	queue_t q = queue_new();
	pyobj_t pyobj_line = (pyobj_t)malloc(sizeof(struct pyobj));

	while(lexem_peek(lexems) && parse_assembly_line(lexems,pyobj_line))
	{
		if (0 == parse_eol(lexems))
		{
		printf("erreur no return after instruction in a .line \n ");
		errorpys(lexems);
		return -1;
		}
		q=enqueue(q,pyobj_line);
		pyobj_line = (pyobj_t)malloc(sizeof(struct pyobj));
	}
free(pyobj_line);
	list_t l = queue_to_list(q);
	list_t liste_buffer = l;

	size_t size = list_length(l);


	pyobj_t bytecode = calloc(1,sizeof(struct pyobj));
	bytecode->type = '[';
	bytecode->py.list.size = size;
	bytecode->py.list.value = NULL;

  	pyobj_t *bytecode_list = (pyobj_t *)malloc(size * sizeof(struct pyobj));

  	if (bytecode_list == NULL)
	{
		 printf("Memory allocation failed\n");
		 return -1;
 	}


 	size_t i;

 	for (i = 0; i < size; i++)
 	{
			bytecode_list[i] = l->content;
	 	l=l->next;
	}
	while(liste_buffer)
			{
			list_t out = liste_buffer->next;
			free(liste_buffer);
			liste_buffer=out;
			}
	//bytecode->py.list.value = create_tab_pyobj(size);
	bytecode->py.list.size = size;
	bytecode->py.list.value = bytecode_list;
	py_block->binary.content.bytecode = bytecode;

return 0;
}



//--------------------------------//
int parse_assembly_line(list_t *lexems, pyobj_t py_block)
{

	if(!lexems)
	{
		return 0;
	}

	if(parse_source_lineno(lexems,py_block))
		{
			return 1;
		}

		if(parse_insn(lexems,py_block))
		{
			return 1;
		}


	else
		{

			return 0;

		}

	return 1;
}


int parse_insn(list_t *lexems,pyobj_t py_block)
{

	if(!lexems)
	{
		return 0;
	}


	if(next_insn_is(lexems,"insn::1"))
		{
			py_block->type = 's';

			lexem_t lex_ins = lexem_advance(lexems);

			if(next_lexem_is(lexems,"integer::dec"))
				{
					lexem_t lex_arg = lexem_advance(lexems);
					int ln = strlen(lex_arg->value);
					char * chaineStatique = calloc(3 + ln,sizeof(char));
			    *(chaineStatique) = ((lex_ins->type)[9]);
			    *(chaineStatique+1) = ((lex_ins->type)[10]);
			    py_block->py.string.line = -1;
					int i;
					for(i=0;i<ln;i++)
					{
						*(chaineStatique+2+i) =(lex_arg->value)[i];
					}
					py_block->type = 's';
					py_block->py.string.buffer = (chaineStatique);
					del_lexem(lex_arg);
					del_lexem(lex_ins);
				}


			else if(next_lexem_is(lexems,"symbol"))
			{
				lexem_t lex_arg = lexem_advance(lexems);
				int ln = strlen(lex_arg->value);
				char * chaineStatique = calloc(3 + ln ,sizeof(char));
				*(chaineStatique) = ((lex_ins->type)[9]);
				*(chaineStatique+1) = ((lex_ins->type)[10]);
				int i;
   				  py_block->py.string.line = -2;

				for(i=0;i<ln;i++)
				{
					*(chaineStatique+2+i) =(lex_arg->value)[i];
				}
				py_block->type = 's';
				py_block->py.string.buffer = (chaineStatique);
				del_lexem(lex_arg);
				del_lexem(lex_ins);
			}

			else
				{
					printf("erreur sur une ligne de .line\n");
					errorpys(lexems);
					return 0;
				}
		}

else if(next_insn_is(lexems,"insn::0"))
		{
			py_block->type = 's';
			py_block->py.string.line = -1;
			lexem_t lex = lexem_advance(lexems);
		 	char * chaineStatique = calloc(3,sizeof(char));
		  *(chaineStatique) = (lex->type)[9];
		  *(chaineStatique+1) = (lex->type)[10];
		  py_block->py.string.buffer = (chaineStatique);
		  py_block->py.string.line = -1;
		  del_lexem(lex);

			if(!next_lexem_is(lexems,"newline"))
				{
				printf("erreur sur une instruction avec 0 arg \n");
				errorpys(lexems);
				return 0;
				}
			}

else if(next_lexem_is(lexems,"symbol"))
	{
		lexem_t lex = lexem_advance(lexems);
		py_block->type = 's';
		py_block->py.string.line = -1;
		py_block->py.string.buffer = strdup(lex->value);
		//printf("regardez %s \n",py_block->py.string.buffer);
		del_lexem(lex);


		if(next_lexem_is(lexems,"colon"))
			{
				del_lexem(lexem_advance(lexems));
			}
	}
else if(next_lexem_is(lexems,"dir::line"))
	{
		return 0;
	}


else
	{
	printf("Error in insn or label \n");
	errorpys(lexems);
	return 0;
	}

return 1;
}



int parse_source_lineno(list_t *lexems, pyobj_t py_block)
{
	if(!lexems)
	{
		return 0;
	}


if ( next_lexem_is( lexems, "dir::line"))
	{
		del_lexem(lexem_advance ( lexems) );
  }

if ( next_lexem_is( lexems, "integer::dec"))
	{
		lexem_t lex = lexem_advance(lexems);
		py_block->type = 's';
		py_block->py.string.line = atoi(lex->value);
		py_block->py.string.buffer = strdup( "Line" );
		del_lexem(lex);
	}
  else
   		{
   			return 0;
			}

  return 1;
}
