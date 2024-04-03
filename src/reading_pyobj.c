#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pyas/reading_pyobj.h"


void print_pyobjt_header(py_codeblock * py_block)
{

	printf("Version_pyvm : %d \n",py_block->version_pyvm);


	//header printing

	printf("Flags : %u en decimal \n",(py_block->header).flags);
	printf("arg_count : %d \n",(py_block->header).arg_count);
	printf("arg_count : %d \n",(py_block->header).stack_size);


}//fin de la lecture du header du codeblock


void print_pyobjt_object(pyobj_t p)
	{
		printf("Type : '%c', ",p->type);
		if(p->type == 's')
		{
			printf("Value : %s\n",(p->py).string.buffer);
		}



		 if(p->type == '[')
		{
			printf("je dois te voir ");
			int c = p->py.list.size;
			printf("voici la taille %d",c);
			int i;
			for(i=0;i<c;i++)
			{
				print_pyobjt_object(  *(p->py.list.value+i))  ;
			}
		}
		 if(p->type == '(')
		{
			printf("VOICI LE NOMBRE A AFFICHER %d \n",p->py.list.size);
			int j;
			for(j=0;j<p->py.list.size;j++)
			{
				print_pyobjt_object(  *(p->py.list.value+j))  ;
			}

		}


		 if(p->type == 'i')
		{
			printf(" int %d \n",(p->py).number.integer);
		}

		if(p->type == 'f'||p->type == 'g')
		{
			printf("%s \n",(p->py).string.buffer);


		}

		if(p->type == 'N'||p->type == 'F'||p->type == 'T')
		{
			printf("%s\n", (p->type == 'N') ? "None" : ((p->type == 'F') ? "False" : "True"));
		}


	}



void print_pyobjt_trailer(py_codeblock* py_block)
	{
	printf("filename : ");
	print_pyobjt_object( (py_block->binary).trailer.filename);

	printf("name : ");
	print_pyobjt_object( (py_block->binary).trailer.name);

	/*printf("lnotab : ");
	print_pyobjt_object( (py_block->binary).trailer.lnotab);*/

	}


void  print_pyobjt_binary_content(py_codeblock* py_block)
{


	printf("interned : \n");
	print_pyobjt_object((py_block->binary).content.interned);

	printf("constantes : \n");
	print_pyobjt_object((py_block->binary).content.consts);

	if(((py_block->binary).content.names!=NULL))
	{
		printf("names : \n");
		print_pyobjt_object((py_block->binary).content.names);
	}

	if(((py_block->binary).content.bytecode!=NULL))
	{
		printf("bytecode provisoire: \n");
		print_pyobjt_object((py_block->binary).content.bytecode);
	}

}

int main_print_py_codeblock(py_codeblock* py_block)
{
	printf("-------------------------------\n");
	printf("-----AFFICHAGE DU CODEBLOCK----\n");
	printf("-------------------------------\n");

	printf("-------------------------------\n");
	printf("------AFFICHAGE HEADER---------\n");
	printf("-------------------------------\n");
	print_pyobjt_header(py_block);

	printf("-------------------------------\n");
	printf("------AFFICHAGE Content--------\n");
	printf("-------------------------------\n");
	print_pyobjt_binary_content(py_block);

	printf("-------------------------------\n");
	printf("------AFFICHAGE TRAILER--------\n");
	printf("-------------------------------\n");
	print_pyobjt_trailer(py_block);

	printf("-------------------------------\n");
	printf("-------FIN DU CODEBLOCK--------\n");
	printf("-------------------------------\n");

	return 1;
}


void print_bytcode(pyobj_t bytecode)
{
	printf("-------------------------------\n");
	printf("-----Affichage du bytecode-----\n");
	printf("-------------------------------\n");
	printf("\n");
	int32_t i = 0;
	int32_t size = strlen(bytecode->py.string.buffer);
	for(i = 0; i<size;i++)
	{
			printf("%c%c ",  bytecode->py.string.buffer[i],bytecode->py.string.buffer[i+1]);
			i++;

	}
	printf("\n");
	printf("\n");

	return;
}

void print_lnotab(pyobj_t lnotab)
{
	printf("-------------------------------\n");
	printf("-----Affichage du lnotab-------\n");
	printf("-------------------------------\n");
	printf("\n");
	int32_t i = 0;
	int32_t size = lnotab->py.list.size;
	for(i = 0; i<size;i++)
	{
			printf("%d ",  lnotab->py.list.value[i]->py.number.integer);
	}
	printf("\n");
	printf("\n");

	return;
}



void print_pyobj_t(pyobj_t obj) {
    if (!obj) {
        printf("Object does not exist\n");
        return;
    }

    // printf("Raw dump: ");
		// int i;
		// for (i = 0; i < sizeof(struct pyobj); i++) {
    //     printf("%02x ", ((unsigned char *)obj)[i]);
    // }
  printf("\n ---------------------------------\n");
  printf("----------DISPLAY PYOBJ-----------\n");
  printf("------------------------------------\n");

  printf("Type: %c (%d)\n", obj->type, obj->type);
	//printf("Refcount: %d\n", obj->refcount);

    switch (obj->type) {
    case NULL_TYPE:
        printf("NULL\n");
        break;
    case NONE_TYPE:
        printf("None\n");
        break;
    case FALSE_TYPE:
        printf("False\n");
        break;
    case TRUE_TYPE:
        printf("True\n");
        break;
    case INT_TYPE:
        printf("Integer: %d\n", obj->py.number.integer);
        break;
    case INT64_TYPE:
        printf("Integer64: %lld\n", (long long int)(obj->py.number.integer64));
        break;
    case FLOAT_TYPE:
    case BINARY_FLOAT_TYPE:
        printf("Real: %f\n", obj->py.number.real);
        break;
    case COMPLEX_TYPE:
    case BINARY_COMPLEX_TYPE:
        printf("Complex: Real: %f, Imaginary: %f\n", obj->py.number.complex.real, obj->py.number.complex.imag);
        break;
    case STRING_TYPE:
        printf("String: %s (Length: %d)\n", obj->py.string.buffer,obj->py.string.length);
        break;
    case STRINGREF_TYPE:
        printf("String Reference: %s (Length: %d)\n", obj->py.string.buffer,
               obj->py.string.length);
        break;
    case TUPLE_TYPE:
        printf("Tuple Size: %d\n", obj->py.list.size);
        break;
    case LIST_TYPE:
        printf("List Size: %d\n", obj->py.list.size);
        break;
    case DICT_TYPE:
        printf("Dict Size: %d\n", obj->py.list.size);
        break;
    case SET_TYPE:
        printf("Set Size: %d\n", obj->py.list.size);
        break;
    case CODE_TYPE:
        printf("Code Type\n");
        break;
    case STOP_ITER_TYPE:
        printf("Stop ITER TYPE\n");
        break;
    case ELLIPSIS_TYPE:
        printf("Ellipsis\n");
        break;
    case LONG_TYPE:
        printf("Long Integer\n");
        break;
    case UNICODE_TYPE:
        printf("Unicode String");
        break;
    case INTERNED_TYPE:
        printf("Interned String: %s (Length: %d)\n", obj->py.string.buffer, obj->py.string.length);
        break;
    case UNKNOWN_TYPE:
        printf("Unknown Type\n");
        break;
    case FROZENSET_TYPE:
        printf("Frozen Set Size: %d\n", obj->py.list.size);
        break;
    default:
        printf("Other or Unknown Type\n");
        break;
    }
    return;
}
