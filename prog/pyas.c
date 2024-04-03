
#include <stdlib.h>
#include <stdio.h>
#include "pyas/all.h"
#include <stdint.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>


int main (int argc, char* argv[]) {

    //----------------Number arg OK ?----------------

	 if ( argc != 4 )
	 {
		printf("Needed: Path of the two files in ' ' \n");
    printf("Erreur d'argument \n");
    return 0;
  	}

    //------------Read arg----------------

	FILE *fic_lex, *fic_py;
	fic_lex = fopen(argv[1],"r");
	char * file_lex = read_fic(fic_lex);

	fic_py = fopen(argv[2],"r");
	char * file_py = read_fic(fic_py);


    //------------Lex----------------
	list_t liste = lex(file_lex,file_py);

	if(!liste)
	{
		exit(EXIT_FAILURE);
	}

  //------------Create the pointer to the lexems----------------

  list_t* l_point = (list_t*)malloc(sizeof(list_t));
  if(l_point != NULL)
		{
	    	*l_point = liste;
	  }

  //------------Create the pyobj_t modif by parse----------------

  pyobj_t py_obj_parent = calloc(1,sizeof(struct pyobj));

  if(py_obj_parent==NULL)
  	{
      printf("Error Alloc \n");
    }

	py_codeblock *py_block = calloc(1,sizeof(py_codeblock));
	py_obj_parent->type = 'c';
	init_codeblock(py_block);
	(py_obj_parent->py).codeblock = py_block ;


int res = parse_pys(l_point,py_block);

if(!res)
{
	printf("ERROOOOOR res = 0\n");
	exit(EXIT_FAILURE);
}
if(!py_block)
	{
		printf("py_block vide\n");
    	exit(EXIT_FAILURE);
	}


 pyasm(py_obj_parent);
	


FILE* pyc_file = fopen(argv[3],"wb");
//-------------------------------------------------------------------------------------//
// ON COMMENCE LE REMPLISSAGE DU FICHIER DE MORT

int final = pyobj_write(pyc_file,py_obj_parent);
if(-1 != final  )
{printf("---------FICHIER exemple.pyc DISPONIBLE ---------------\n");}
//fwrite(&coucou,sizeof(uint32_t),1,pyc_file);
	fclose(pyc_file);
	//free_pyobj_t(py_obj_parent->py.codeblock->binary.content.bytecode);
	free_pyobj_t(py_obj_parent);
	free(py_obj_parent);
	free(py_block);
  list_delete(*l_point,&del_lexem);
  free(l_point);
	fclose(fic_lex);
	fclose(fic_py);
	free(file_lex);
	free(file_py);
	return 0;
}
