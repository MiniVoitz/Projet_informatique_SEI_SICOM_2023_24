
/**
 * @file lexem.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 17:59:27 2021
 * @brief Lexems.
 *
 * Lexems.
 */

#define _POSIX_C_SOURCE 200809L /* strdup(3) */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


#include "pyas/all.h"
#include "pyas/list.h"



//-----------------------------------------------------------------------------------------------------//

lexem_t lexem_new( char *type, char *value, int line, int column ) {
  lexem_t lex = calloc( 1, sizeof( *lex ) );

  assert( lex );

  if ( type  && *type  ) lex->type  = strdup( type ); //strdup = alloc
  if ( value && *value ) lex->value = strdup( value );

  lex->line   = line;
  lex->column = column;

  return lex;
}
//-----------------------------------------------------------------------------------------------------//

int     lexem_print( void *_lex ) {
  assert(_lex);
  assert(_lex);
  lexem_t lex = _lex;

  return printf( "[%d:%d:%s] %s ",
		 lex->line,
		 lex->column,
		 lex->type,
		 lex->value );
}
//-----------------------------------------------------------------------------------------------------//

int     lexem_delete( void *_lex ) {
  lexem_t lex = _lex; // Casting of the void pointer

  if ( lex ) {
    free( lex->type );
    free( lex->value );
  }

  free( lex );

  return 1;
}

//-----------------------------------------------------------------------------------------------------//

lexem_t lexem_peek( list_t *lexems) {
    if(!lexems) //lexems == NULL => ERROR
    {
      return NULL;
    }
    list_t l=*lexems;
  	if(list_empty(l)) // the list of lexem in paramter can't be empty
    {
      return NULL;
    }
    lexem_t lexem = l->content;
    while ((!strcmp(lexem->type,"blank")||!strcmp(lexem->type,"comment"))&&list_empty(list_next(l))!=1){ // while we do not find any useful lexem, we advance in the list of lexem
        l=list_next(l);
        lexem=l->content;
    }
    if (!strcmp(lexem->type,"blank")||!strcmp(lexem->type,"comment")){ // tests if we have reached the end of the list without finding a useful lexem
        printf("No useful lexem left.\n");
        lexem = NULL;
    }
    return lexem;
}

//-----------------------------------------------------------------------------------------------------//

lexem_t lexem_advance( list_t *lexems ){ // this function returns the first lexem useful of the list and removes it from the list
  if(!lexems) //lexems == NULL => ERROR
  {
    return NULL;
  }
  list_t l=*lexems;
	if(list_empty(l)) // the list of lexem in paramter can't be empty
  {
    return NULL;
  }
  lexem_t lexem = l->content;
	while ((!strcmp(lexem->type,"blank")||!strcmp(lexem->type,"comment"))&&!list_empty(list_next(l)))
  {
    l=list_del_first(l,&del_lexem); //Need to use this for delete the useless lex
		lexem = l->content;
	}

  if (!strcmp(lexem->type,"blank")||!strcmp(lexem->type,"comment")){ // tests if we have reached the end of the list without finding a useful lexem
        printf("No useful lexem left.\n");
        lexem_delete(lexem);
        lexem = NULL;}

  if(list_empty(list_next(l))){ //Just 1 lexem in the list
    free(l); //We have access to the only element with lexem so we can free list of lexem
    *lexems = NULL;

	}
	else {
		*lexems=list_next(l); //We don't wan't to delete the information of the first element
    free(l);
	}
	return lexem;
}

//-----------------------------------------------------------------------------------------------------//

int next_lexem_is( list_t *lexems, char *type){
  if(!lexems) //lexems == NULL => ERROR
  {
    return 0;
  }
  list_t l=*lexems;
	if(list_empty(l)) // the list of lexem in paramter can't be empty
  {
    return 0;
  }
  if(!type)
  {
    printf("String: 'type' is empty \n");
    return 0;
  }
	lexem_t next_useful_lex = lexem_peek(lexems);
  if(!next_useful_lex)
  {
    return 0;
  }
	if (!strcmp(next_useful_lex->type,type)){
		return 1; // the next useful lexem IS of expected type
	}
	return 0; // the next useful lexem IS NOT of expected type
}

//-----------------------------------------------------------------------------------------------------//

int next_next_lexem_is( list_t *lexems, char *type){
  if(!lexems) //lexems == NULL => ERROR
  {
    return 0;
  }
  list_t l=*lexems;
  l=l->next;

  //lexem_advance();
  if(list_empty(l)) // the list of lexem in paramter can't be empty
  {
    return 0;
  }
  if(!type)
  {
    printf("String: 'type' is empty \n");
    return 0;
  }
  lexem_t next_useful_lex = lexem_peek(&l);
  if(!next_useful_lex)
  {
    return 0;
  }
  if (!strcmp(next_useful_lex->type,type)){
    return 1; // the next useful lexem IS of expected type
  }
  return 0; // the next useful lexem IS NOT of expected type
}

//-----------------------------------------------------------------------------------------------------//


int next_insn_is(list_t *lexems , char *type)
{
  if(!lexems) //lexems == NULL => ERROR
  {
    return 0;
  }
  list_t l=*lexems;
  if(list_empty(l)) // the list of lexem in paramter can't be empty
  {
    return 0;
  }
  if(!type)
  {
    printf("String: 'type' is empty \n");
    return 0;
  }
  lexem_t next_useful_lex = lexem_peek(lexems);

  if(!next_useful_lex)
  {
    return 0;
  }

  if (NULL!=(strstr(next_useful_lex->type,type))){
    // printf("oui %s est dans %s \n",type,next_useful_lex->type);
    return 1; // the next useful lexem IS of expected type
  }
  return 0; // the next useful lexem IS NOT of expected type
}


//----------------------------------------------------------------//
void print_parse_error( char *msg, list_t *lexems ){
  assert(lexems);
  assert(msg);
  list_t l = *lexems;
	assert(!list_empty(l)); // the list of lexem in paramter can not be empty

  lexem_t lexem = l->content;
	printf("Error : %s (Line,Columns) : (%d,%d)\n",msg, lexem->line, lexem->column);
}

//-----------------------------------------------------------------------------------------------------//

list_t lex(char *regexp_file, char *source_file) {

  if(!regexp_file||!source_file)
  {
    printf("Argument error : in %s \n", !regexp_file ? "regexp_file (Arg 1)" : "source_file (Arg 2)");
    return NULL;
  }

  if(!strcmp(regexp_file, "") ||!strcmp(source_file, ""))
  {
    printf("Argument error : in %s \n", !strcmp(regexp_file, "") ? "regexp_file (Arg 1)" : "source_file (Arg 2)");
    return NULL;
  }

  queue_t queueHead = queue_new();
  int i = 0 ; // Will allow to navigate througt the char*
  int x = 0;  //col
  int y = 1;  //Li in subject line start at 1
  int count_for_tab;

while(*(regexp_file+i) != '\0' ) // Read of regexp_file.txt
  {

      if(*(regexp_file+i)=='\n') //mean we have a return here
        {
          x=0;
          y++;
          i++;
        }

      if(*(regexp_file+i) == '#' && x==0) // means we have a comment here
        {
          while(*(regexp_file+i) != '\n')
            {
              i++;
            }

            x = 0 ;
            y++;
            i++;
        }

        // in this case the information at the line we are is usefull
        else if(*(regexp_file+i) != '#' && x==0)
        {

            lexem_t lex = calloc( 1, sizeof( *lex ) );


            char *tableau_type = (char *)calloc(100, sizeof(char));


            char *tableau_value = (char *)calloc(100, sizeof(char));


            count_for_tab = 0;

          while(*(regexp_file+i) != ' ' && *(regexp_file+i-1) != ' ') //copy of the name of the lexem
            {

              *(tableau_type+count_for_tab) = *(regexp_file+i); // caracter by caracter
              count_for_tab++;
              i++;
            }

          lex->type = tableau_type; // and then push it into the lexem struct


          while(*(regexp_file+i) == ' ')  // we navigate between the name and the value
            {
              i++;
            }

          if(*(regexp_file+i) != ' ') //copy the value of the lexem here
          {
            count_for_tab = 0;

              while(*(regexp_file+i) != ' ' || *(regexp_file+i+1) == '\\')

                {
                  *(tableau_value + count_for_tab) = *(regexp_file+i); //push the value inside tableau
                    count_for_tab++;
                  i++;
                }
          }

          lex->value = tableau_value;



          while(*(regexp_file+i) != '\n')           //Goes to the end of the line
            {
              i++;
            }




        i++;
        y++;


        queueHead = enqueue(queueHead,lex); //put the lexem in the queue
        }
  } //END OF THE FILE

list_t listHead = queue_to_list(queueHead); //list of lexem
list_t listHead_buffer = listHead; //buffer lexem
queue_t queue_binary = queue_new();
while(!list_empty(listHead_buffer))
{

  queue_binary = enqueue(queue_binary,re_read((((lexem_t)(listHead_buffer->content))->value)));
  listHead_buffer = listHead_buffer->next;
}

list_t l_binary = queue_to_list(queue_binary); //List of list of binary block.
list_t l_binary_buffer = l_binary; //buffer BB;

//-----------------------------------------------------------------------------------------------------//

char     *end = NULL;
char *source_file_buffer = source_file; //keep acces to source_file first char
int  is_match = 0;


queue_t q_lex = queue_new();
i = 0 ; // Will allow to navigate througt the char*
x = 0;  //col
y = 1;  //Li in subject line start at 1

while(*source_file_buffer != '\0') // While there is still some char in source_file
{
  l_binary_buffer = l_binary;
  listHead_buffer = listHead;
  while (!is_match && !list_empty(l_binary_buffer))
  {
      is_match = re_match(l_binary_buffer->content , source_file_buffer, &end);
      l_binary_buffer = l_binary_buffer->next; //if is_match we don't want do go further in l_binary_buffe
      if(!is_match){
      listHead_buffer = listHead_buffer->next;
    }
  }
  // printf("%c", *end);
  if(!is_match)//Case not matched at all not a Syntax Error !
  {
    printf("Not matched, Character : %c, Line : %d, Column: %d \n", *source_file_buffer,y,x);
    return NULL;
  }

  else //We matched => enqueue
  {
char *val = calloc(64,sizeof(char));
  i=0;
  int x_buffer = x;
  int y_buffer = y;
  while((source_file_buffer+i) != end ) //Create a copy of source_file_buffer but without the end. + Coord of pointer end:
  {
    val[i] =  source_file_buffer[i];
    x++;
      if(*(source_file_buffer+i)=='\n') //mean we have a return here
        {
          x=0;
          y++;
        }
    i++;
  }
  lexem_t lexem =  lexem_new((((lexem_t)(listHead_buffer->content))->type),val, y_buffer, x_buffer); //Creat lexem we match
  q_lex = enqueue(q_lex,lexem);
  source_file_buffer = end;
  end = NULL;
  is_match = 0;
  free(val);

  }


} //No more char in the source file


list_t list_lex = queue_to_list(q_lex);
list_t buffer;
while(l_binary)
{
  list_delete(l_binary->content,&del_binary);
  buffer = l_binary;
  l_binary = l_binary->next;
  free(buffer);
}
list_delete(listHead,&del_lexem);

//We're only returning list_lex.

  return list_lex;
} // END OF THE FUNCTION
