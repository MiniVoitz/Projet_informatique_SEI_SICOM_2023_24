#include <stdlib.h>
#include <stdio.h>
#include "pyas/all.h"
#include <stdint.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>

int pyobj_write(FILE *pyc_file,pyobj_t obj)
{
	int i;
py_codeblock *py_block = obj->py.codeblock;
uint16_t versionpyvm = py_block->version_pyvm;
uint16_t magic = 2573;
uint32_t CODE=99 ;
int content = 40;
int int_string = 115;
int taille_str =0;



fwrite(&versionpyvm,2,1,pyc_file);


fwrite(&magic,2,1,pyc_file);
time_t timestamp = time( NULL );
unsigned long seconds = difftime( timestamp, 0 );
//fwrite(&coucou,sizeof(uint32_t),1,pyc_file);
fwrite(&seconds,4,1,pyc_file);
//fseek(pyc_file,-6,SEEK_END);
fwrite(&CODE,1,1,pyc_file);

fwrite(&(py_block->header.arg_count),4,1,pyc_file);
fwrite(&(py_block->header.local_count),4,1,pyc_file);
fwrite(&(py_block->header.stack_size),4,1,pyc_file);
fwrite(&(py_block->header.flags),4,1,pyc_file);
//-LA ON DOIT AFFICHER LE .TEXT DU COUP MAIS A FAIRE APRES



//-------------------BYTECODE------------------------------------------

char tab_bytecode[3];
int debut_text = 115;
fwrite(&debut_text,1,1,pyc_file);
int taille_text = strlen(py_block->binary.content.bytecode->py.string.buffer);
taille_text = ((taille_text)/2)   ;
fwrite(&taille_text,4,1,pyc_file);
int compteur_bytecode;
for(compteur_bytecode = 0;compteur_bytecode<(intptr_t)strlen(py_block->binary.content.bytecode->py.string.buffer);compteur_bytecode++)
{
	tab_bytecode[0] = (py_block->binary.content.bytecode->py.string.buffer)[compteur_bytecode];
	tab_bytecode[1] = (py_block->binary.content.bytecode->py.string.buffer)[compteur_bytecode+1];
	tab_bytecode[2] = '\0';

	int res;
    sscanf(tab_bytecode, "%X", &res);
    
	fwrite(&res,1,1,pyc_file);
	compteur_bytecode++;
}
//free((py_block->binary.content.bytecode->py.string.buffer));
//free_pyobj_t(py_block->binary.content.bytecode);




//-------------------CONSTANTES--------------------------

int taille = py_block->binary.content.consts->py.list.size;
fwrite(&(content),1,1,pyc_file);
fwrite(&taille,4,1,pyc_file);

int compteur;
int int_buffer = 0;
pyobj_t pyob_content = py_block->binary.content.consts;
for(compteur=0;compteur<taille;compteur++)
{
	pyobj_t *actuel = pyob_content->py.list.value;


	if(actuel[compteur]->type== 'i')
		{
			int code_int = 105;
			fwrite(&code_int,1,1,pyc_file);
			int_buffer = actuel[compteur]->py.number.integer;
			fwrite(&int_buffer,4,1,pyc_file);
			//printf("deja un bon debut %d",actuel[compteur]->py.number.integer);
		}
		else if(actuel[compteur]->type== 'N')
		{
			int code_int = 78;
			fwrite(&code_int,1,1,pyc_file);
		}


		else if(actuel[compteur]->type== 's')
		{

			int code_int =116 ;
			fwrite(&code_int,1,1,pyc_file);
			int taille_cst = (strlen(actuel[compteur]->py.string.buffer));
			taille_cst = taille_cst-2;
			//printf("voici la constante string %s",(actuel[compteur]->py.string.buffer));
			fwrite(&taille_cst,4,1,pyc_file);
			//fwrite(&(actuel[compteur]->py.string.buffer),strlen((actuel[compteur]->py.string.buffer)),1,pyc_file);




			char tab_cst[taille_cst];
			for( i=0;i<taille_cst;i++)
			{

					tab_cst[i]= (actuel[compteur]->py.string.buffer)[i+1];

			}
				tab_cst[taille_cst]='\0';
				fwrite(&(tab_cst),taille_cst,1,pyc_file);
					//printf("\nvoici la constante string %s\n",tab_cst);




		}

}


//print des names que des strings ici

if (py_block->binary.content.names != NULL ) {
  taille = py_block->binary.content.names->py.list.size;
  fwrite(&(content),1,1,pyc_file);
  fwrite(&taille,4,1,pyc_file);

  //printf("regardez la taille de names %d\n",taille);
  pyob_content = py_block->binary.content.names;
  int code_int = 116;
  for(compteur=0;compteur<taille;compteur++)
  {




    pyobj_t *actuel = pyob_content->py.list.value;




        fwrite(&code_int,1,1,pyc_file);

        //int_buffer = actuel[compteur]->py.string.buffer;
         taille_str = strlen(actuel[compteur]->py.string.buffer);
        taille_str = taille_str-2;
        int j;
        for(j=0;j<taille_str;j++)
        {
        fwrite(&taille_str,4,1,pyc_file);
        char tab_names[taille_str];
        int k;
        for( k=0;k<taille_str;k++)
        {

            tab_names[k]= (actuel[compteur]->py.string.buffer)[k+1];

        }
          tab_names[taille_str]='\0';
  fwrite(&(tab_names),taille_str,1,pyc_file);
        // printf("REGARDE %s %d \n",tab_names,taille_str);
      }


  }
}

//---------------//
	// AFFICHAGE DES VARNAMES FREEVARS ET CELLVAR QUI NE SONT PAS PRIS EN COMPTE POUR LE MOMENT
taille = 0;
for(compteur =0;compteur<3;compteur++)
	{

		fwrite(&(content),1,1,pyc_file);
		fwrite(&taille,4,1,pyc_file);
	}

//------// FILENAME ICI


fwrite(&int_string,1,1,pyc_file);
taille_str = strlen(py_block->binary.trailer.filename->py.string.buffer)-2;
//printf("voici la taille du filename %d \n",taille_str);
				//nom du filename mtn
fwrite(&taille_str,4,1,pyc_file);
//Je met au bon format les strings
printf("\n");
char tab[taille_str];
for(i=0;i<taille_str;i++)
{

	tab[i]= (py_block->binary.trailer.filename->py.string.buffer)[i+1];

}
tab[taille_str]='\0';


fwrite(&(tab),taille_str,1,pyc_file);
//printf("voici le filename %s \n",py_block->binary.trailer.filename->py.string.buffer);

//------// NAME ICI

int int_str = 116;
fwrite(&(int_str),1,1,pyc_file);
taille_str = strlen(py_block->binary.trailer.name->py.string.buffer)-2;
//printf("voici la taille du name %d \n",taille_str);

				//nom du filename mtn
fwrite(&taille_str,4,1,pyc_file);

 char tab_name[taille_str];
for(i =0;i<taille_str;i++)
{

	tab_name[i]= (py_block->binary.trailer.name->py.string.buffer)[i+1];

}
tab_name[taille_str]='\0';
//printf("voici le name %s",tab_name);

fwrite(&(tab_name),taille_str,1,pyc_file);

//firstlineno
fwrite(&(py_block->binary.trailer.firstlineno),4,1,pyc_file);





// ON AFFICHE CE QUI SE TROUVE APRES LE .TEXT

fwrite(&int_string,1,1,pyc_file);
taille_str = py_block->binary.trailer.lnotab->py.list.size ;
taille_str = taille_str-2 ; // on ne prend jamais en compte les 2 premiers qui sont toujours identique en fait

//printf("voici la taille de lnotab %d \n",taille_str);
				//nom du filename mtn
fwrite(&taille_str,4,1,pyc_file);

pyob_content = py_block->binary.trailer.lnotab;
pyobj_t buffer = pyob_content->py.list.value[2];
free_pyobj_t(pyob_content->py.list.value[1]);
free_pyobj_t(pyob_content->py.list.value[0]);

for(compteur =2;compteur<taille_str+2;compteur++)
{
pyobj_t *actuel = pyob_content->py.list.value;

	
*actuel = *(actuel+2);
	int_buffer = actuel[compteur]->py.number.integer;
	//printf("\nREGARFDEZ MOI    %d \n",int_buffer);
	free_pyobj_t(pyob_content->py.list.value[compteur]);
	fwrite(&int_buffer,1,1,pyc_file);

}
 // /free(*(pyob_content->py.list.value));

return 1;
}
