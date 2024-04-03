
#include <stdlib.h>
#include <stdio.h>
#include "pyas/all.h"



int main (int argc, char* argv[]) {

	 if ( argc != 3 )
	 {
		printf("Needed: Path of the two files in ' ' \n");
    printf("Erreur d'argument \n");
    return 0;
  	}

	FILE *fic_lex, *fic_py;
	fic_lex = fopen(argv[1],"r");
	char * file_lex = read_fic(fic_lex);

	fic_py = fopen(argv[2],"r");
	char * file_py = read_fic(fic_py);

	list_t liste = lex(file_lex,file_py);

	if(!liste)
	{
		exit(EXIT_FAILURE);
	}

	list_t* l_point = (list_t*)malloc(sizeof(list_t));
  	if(l_point != NULL)
		{
	    	*l_point = liste;
		}

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
	//printf("REGARDEZ LE RESULTAT : %d\n",res);
	if(!res)
	{
		printf("ERROOOOOR\n");
		exit(EXIT_FAILURE);
	}
	
	if(py_block ==NULL)
		{
			printf("ah merde \n");
		}

		if(res==1)
		{
			print_pyobj_t(py_obj_parent);
			if(main_print_py_codeblock(py_block))
			{
				printf("\n FIN DU PROGRAMME \n");
			}

		
		}

	free_pyobj_t(py_obj_parent);
	free(py_block);
	free(py_obj_parent);
  	list_delete(*l_point,&del_lexem);
  	free(l_point);
	fclose(fic_lex);
	fclose(fic_py);
	free(file_lex);
	free(file_py);
	return 0;

}
