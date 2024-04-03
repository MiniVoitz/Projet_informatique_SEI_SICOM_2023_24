#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pyas/pyasm.h"

// VERIFIEZ COMMENT FREE + COMPARER DEUX CHAINES DE CHAR

int pyasm (pyobj_t code){
    pyobj_t liste_elements = code->py.codeblock->binary.content.bytecode; // liste de pyobj_t de type string du type Liste,op,0,op,op,label_0... numéro de liste dans pyobj_t de type int_liste
    pyobj_t element = calloc(1,sizeof(struct pyobj)); // rempli à la fin
    element->type = 's';
    pyobj_t vrai_bytecode = calloc(1,sizeof(struct pyobj)); // rempli à la fin
    vrai_bytecode->type = 's';
    pyobj_t vrai_lnotab = calloc(1,sizeof(struct pyobj)); // rempli à la fin
    vrai_lnotab->type = '[';
    pyobj_t element_lnotab = calloc(1,sizeof(struct pyobj)); // stocke les labels pour 2ème passe
    element_lnotab->type = 'i';
    queue_t queue_t_label = queue_new(); // rempli à la fin
    queue_t queue_t_line = queue_new(); // rempli à la fin
    pyobj_t label_string = calloc(1,sizeof(struct pyobj));
    label_string->type = 's';
    int32_t compteur_octets=0;
    int32_t compteur_octets_pre_lines = 0;
    int pre_line = 0;
    int compteur_lnotab=0;
    int compteur_labels=0;
    int parametre_int;
    int j=0;
    int k;
    //int parametre_value;
    char* opcode_string = calloc(3,sizeof(char));
    char* parametre_string = calloc(5,sizeof(char)); //PQ CES TAILLES ???
    char* label_vrai_string = calloc(256,sizeof(char));
    size_t i;
    size_t size_list_ele = liste_elements->py.list.size;
    vrai_lnotab->py.list.value =  (pyobj_t *)malloc(size_list_ele * sizeof(struct pyobj));
    vrai_bytecode->py.string.buffer = calloc(1024,sizeof(char)); //Taille Provisoire
    vrai_bytecode->py.string.length = 1024;
    label_string->py.string.buffer = calloc(1024,sizeof(char)); //Taille Provisoire
    label_string->py.string.length = 1024;

    char * suite;
   int valeur_suite;
    char * suite_hexa = calloc(10,sizeof(char));
    //  AFFICHAGE LISTE ENTREE

    //
    for (i=0;i<size_list_ele;i++)
    {
      element = liste_elements->py.list.value[i]; // le i-ème pyobj_t de type string*
/*      printf("On est là : %s\n", element->py.string.buffer);
*/  }

      printf("\n");
    for (i=0;i<size_list_ele;i++)
    {
        element = liste_elements->py.list.value[i]; // le i-ème pyobj_t de type string*
//        printf("for %d : %s\n", i, element->py.string.buffer);
        if ((!strcmp(element->py.string.buffer,"Line")) && (element->py.string.line!=0))
        { // ligne car line est indiqué
            //printf("Line: %d\n",element->py.string.line);
            element_lnotab->py.number.integer = compteur_octets - compteur_octets_pre_lines  ;
            compteur_octets_pre_lines = compteur_octets;
            vrai_lnotab->py.list.value[compteur_lnotab]=element_lnotab;
            compteur_lnotab+=1;
            //free(element_lnotab);
            element_lnotab = calloc(1,sizeof(struct pyobj)); // nouvelle adresse pour la prochaine ligne
            element_lnotab->type = 'i';
            element_lnotab->py.number.integer = element->py.string.line - pre_line;
            pre_line = element->py.string.line;
            vrai_lnotab->py.list.value[compteur_lnotab]=element_lnotab;
            compteur_lnotab+=1;
            //free_pyobj_t(element_lnotab);
            //free(element_lnotab);
            element_lnotab = calloc(1,sizeof(struct pyobj)); // nouvelle adresse pour la prochaine ligne
            element_lnotab->type = 'i';

        }
        
        else if (isdigit(element->py.string.buffer[0]))
        { // le premier char est un nombre donc instruction
            // printf("Ici donc insn\n");
            opcode_string[0]=element->py.string.buffer[0];
            opcode_string[1]=element->py.string.buffer[1];
            int opcode_value;
            // printf("opcode_string %s\n", opcode_string );
            sscanf(opcode_string,"%x",&opcode_value);

            // printf("opcode_value: %x \n",opcode_value );
            // printf("opcode_value: laaaaaaaaaaaaaaa %d \n",opcode_value );
            if (opcode_value<90){ // pas de paramètre inf à 0x5a (hexa) = 90 (base 10)
                // printf("LAAAAAAA : opcode_value<90 (%s) \n",opcode_string);
                vrai_bytecode->py.string.buffer[j]=opcode_string[0];
                vrai_bytecode->py.string.buffer[j+1]=opcode_string[1];
                j+=2;
                compteur_octets+=1;
            }
            else if(strcmp(opcode_string,"5d")==0||strcmp(opcode_string,"8f")==0||strcmp(opcode_string,"7a")==0||strcmp(opcode_string,"79")==0||strcmp(opcode_string,"78")==0||strcmp(opcode_string,"6e")==0||strcmp(opcode_string,"77")==0||strcmp(opcode_string,"6f")==0||strcmp(opcode_string,"70")==0||strcmp(opcode_string,"71")==0||strcmp(opcode_string,"72")==0||strcmp(opcode_string,"73")==0)
            {
              // printf("LAAAAAAA => label en argument\n");
                    vrai_bytecode->py.string.buffer[j]= opcode_string[0];
                    vrai_bytecode->py.string.buffer[j+1]= opcode_string[1];
                    vrai_bytecode->py.string.buffer[j+2]='0'; // place pour marquer l'emplacement du label lors de la deuxième passe
                    vrai_bytecode->py.string.buffer[j+3]='0';
                    vrai_bytecode->py.string.buffer[j+4]='0';
                    vrai_bytecode->py.string.buffer[j+5]='0';
                    // printf("%s\n", vrai_bytecode->py.string.buffer);
                    j+=6;
                    compteur_octets+=3;
                }
            else if (opcode_value>=90) 
                { // paramètre entier codé sur deux octets
                  // printf("%d\n", opcode_value);
                  // printf("LAAAAAAA opcode_value>=90\n");  //Pas compris ici pcq fin de l'argument doit être mis en hexa ?

                    if(strlen(element->py.string.buffer)==2)
                    {//Normalement impossible car tester dans parse
                        printf("Missing argument\n");
                        return -1;
                    }

                    suite = element->py.string.buffer + 2;
                    //taille_suite = element->py.string.length - 2;
                    valeur_suite = atoi(suite);

                    if(valeur_suite>65535)
                    {
                      printf("Error too many args\n");
                      return -1;
                    }
                    int tt = sizeof(suite_hexa);
                    snprintf(suite_hexa, tt, "%04X", valeur_suite);
                    // printf("VALEUR 1 EN HEXA CEST LAAAAAA %s\n",suite_hexa );


                    vrai_bytecode->py.string.buffer[j]=opcode_string[0];
                    vrai_bytecode->py.string.buffer[j+1]=opcode_string[1];
                    vrai_bytecode->py.string.buffer[j+2]=suite_hexa[2];
                    vrai_bytecode->py.string.buffer[j+3]=suite_hexa[3];
                    vrai_bytecode->py.string.buffer[j+4]=suite_hexa[0];
                    vrai_bytecode->py.string.buffer[j+5]=suite_hexa[1];
                    j+=6;
                    compteur_octets+=3;
                }
                //free_pyobj_t(element_lnotab);
        }

        else if (!isdigit(element->py.string.buffer[0]) && strcmp(element->py.string.buffer,"Line"))
        { // le premier char n'est pas un nombre donc label (cas ligne déjà traité)
              queue_t_label = enqueue(queue_t_label,element->py.string.buffer); //queue de char*
            // int copy_octet = compteur_octets;
           // printf(" r---------------------------------------- %s ",queue_t_label->content);

            queue_t_line = enqueue(queue_t_line,(void*)(intptr_t)compteur_octets); //queue line (int)
            // printf("%s\n", label_string->py.string.buffer);
            compteur_labels+=1;
        }
        else {
                printf("Cannot determine element\n");
                return -1;
        }
    }

    list_t liste_label = queue_to_list(queue_t_label);
    list_t liste_label_buffer = liste_label;
    list_t liste_line = queue_to_list(queue_t_line);
    list_t liste_line_buffer = liste_line;



free_pyobj_t(element_lnotab);

    // 2ème passe
    // printf(" --------------------2ème passe----------------------\n");
    int l;
    int opcode_value;
    j=0;
    for (i=0;(int)i<liste_elements->py.list.size;i++)
    { // ai-je accès à size ?
        // printf(" 2ème passe: %s\n",element->py.string.buffer );

        element = liste_elements->py.list.value[i]; // le i-ème pyobj_t de type string
        //printf("%s\n", element->py.string.buffer);
        //printf("debut for j; %d\n",j );

        if (isdigit(element->py.string.buffer[0])){ // le premier char est un nombre donc instruction
            // printf("isdigit oui: %s\n",element->py.string.buffer );
            opcode_string[0]=element->py.string.buffer[0];
            opcode_string[1]=element->py.string.buffer[1];
            sscanf(opcode_string,"%x",&opcode_value);
            if(opcode_value<90){ // pas de paramètre
                // printf("isdigit + pas de para (%s)\n",element->py.string.buffer);
                j+=2;
            }
            else if(strcmp(opcode_string,"5d")==0)
            { // label en paramètre
              //printf("LAAAAAAAAAAAAAAAAAAA 6E\n");
                j+=2; // pour écrire l'emplacement du label au bon endroit == num octet
                k=0;
                int ln = strlen(element->py.string.buffer);
                while ((k+2)<ln)
                {
                    if (k>256){
                        printf("Label name must be under 256 characters\n");
                        return -1;
                    }
                    label_vrai_string[k]=element->py.string.buffer[k+2];
                    k+=1;
                }

                liste_label_buffer = liste_label;
                liste_line_buffer = liste_line;
                for(l=0;l<compteur_labels;l++)
                {
                    label_string->py.string.buffer = liste_label_buffer->content;
                    label_string->py.string.line = (int)(intptr_t)(liste_line_buffer->content);
                   // printf("label_line: %d\n",label_string->py.string.line);

                    if (!strcmp(label_string->py.string.buffer,label_vrai_string))
                    { //Si label_string = label_vrai_string qu'on cherche
                        parametre_int = label_string->py.string.line - ((j/2)+2); //On récup la ligne
                       /* printf("j+1 car FOR_ITER ---------------------------------- %d\n", (j/2)+2 );
                        printf("parametre_int ---------------------------------- %d\n",label_string->py.string.line );
                        printf("parametre_int ---------------------------------- %d\n",parametre_int );
                        */if ( parametre_int >= 0 && parametre_int <= 65535)//0xFFFF
                        {
                            sprintf(parametre_string, "%04X", parametre_int);
                            printf("parametre_string %s\n",parametre_string);
                            vrai_bytecode->py.string.buffer[j]=parametre_string[2]; //on écrit là où on avait laissé de la place
                            vrai_bytecode->py.string.buffer[j+1]=parametre_string[3]; //little Endian ??????
                            vrai_bytecode->py.string.buffer[j+2]=parametre_string[0];
                            vrai_bytecode->py.string.buffer[j+3]=parametre_string[1];
                            j+=6;
                            // printf("vrai_bytecode %s\n", vrai_bytecode->py.string.buffer);
                        }
                        else
                        {
                            printf("Label position must be under 65535 octets\n");
                            return -1;
                        }
                    }
                }
            }





            else if(strcmp(opcode_string,"8f")==0||strcmp(opcode_string,"7a")==0||strcmp(opcode_string,"79")==0||strcmp(opcode_string,"78")==0||strcmp(opcode_string,"6e")==0)
            { // label en paramètre
              //printf("LAAAAAAAAAAAAAAAAAAA 78n");
                j+=2; // pour écrire l'emplacement du label au bon endroit == num octet
                k=0;
                int ln = strlen(element->py.string.buffer);
                while ((k+2)<ln)
                {
                    if (k>256){
                        printf("Label name must be under 256 characters\n");
                        return -1;
                    }
                    label_vrai_string[k]=element->py.string.buffer[k+2];
                    k+=1;
                }

                liste_label_buffer = liste_label;
                liste_line_buffer = liste_line;
                for(l=0;l<compteur_labels;l++)
                {
                    label_string->py.string.buffer = liste_label_buffer->content;

                    /*printf("label_string->py.string.buffer  %s ", label_string->py.string.buffer );
                    printf("label_string->py.string.buffer  %s \n", label_vrai_string );
*/
                    label_string->py.string.line = (int)(intptr_t)(liste_line_buffer->content);
                    // printf("label_line: %d\n",label_string->py.string.line);

                    if (!strcmp(label_string->py.string.buffer,label_vrai_string))
                    { //Si label_string = label_vrai_string qu'on cherche
                        parametre_int = label_string->py.string.line - ((j/2)+1); //On récup la ligne
                        // printf("LUUUUUUUUUUUUUUUUUUUUUUUIIIIIIIIIIIIIIIIIIIIIIIIIIIII j ---------------------------------- %d\n", (j/2)+1 );
//                        printf("parametre_int ---------------------------------- %d\n",label_string->py.string.line );
  //                      printf("parametre_int ---------------------------------- %d\n",parametre_int );
                        if ( parametre_int >= 0 && parametre_int <= 65535)//0xFFFF
                        {
                            sprintf(parametre_string, "%04X", parametre_int);
    //                        printf("parametre_string %s\n",parametre_string);
                            vrai_bytecode->py.string.buffer[j]=parametre_string[2]; //on écrit là où on avait laissé de la place
                            vrai_bytecode->py.string.buffer[j+1]=parametre_string[3]; //little Endian ??????
                            vrai_bytecode->py.string.buffer[j+2]=parametre_string[0];
                            vrai_bytecode->py.string.buffer[j+3]=parametre_string[1];
                            j+=6;
                            // printf("vrai_bytecode %s\n", vrai_bytecode->py.string.buffer);
                        }
                        else
                        {
                            printf("Label position must be under 65535 octets\n");
                            return -1;
                        }
                    }
                    liste_label_buffer = liste_label_buffer->next;
                }
            }









            else if(strcmp(opcode_string,"77")==0||strcmp(opcode_string,"6f")==0||strcmp(opcode_string,"70")==0||strcmp(opcode_string,"71")==0||strcmp(opcode_string,"72")==0||strcmp(opcode_string,"73")==0)
            { // label en paramètre
                j+=2; // pour écrire l'emplacement du label au bon endroit == num octet
                k=0;
                // printf("isdigit + arg = label \n");
                int ln = strlen(element->py.string.buffer);
                while ((k+2)<ln)
                {
                    if (k>256){
                        printf("Label name must be under 256 characters\n");
                        return -1;
                    }
                    label_vrai_string[k]=element->py.string.buffer[k+2];
                    // printf("label_vrai_string 1 : %s\n", label_vrai_string);
                    k+=1;
                }

                liste_label_buffer = liste_label;
                liste_line_buffer = liste_line;
                for(l=0;l<compteur_labels;l++)
                {
                  // printf("liste label boucle for\n");
                    label_string->py.string.buffer = liste_label_buffer->content;
                    label_string->py.string.line = (int)(intptr_t)(liste_line_buffer->content);
                    // printf("label_string: %s\n",label_string->py.string.buffer);
                    parametre_int = label_string->py.string.line; //On récup la ligne
                    /*printf("label_line: %d\n",label_string->py.string.line);
                    printf("j ---------------------------------- %d\n", j/2+1 );
                    printf("parametre_int ---------------------------------- %d\n",parametre_int );
*/
                    if (!strcmp(label_string->py.string.buffer,label_vrai_string))
                    { //Si label_string = label_vrai_string qu'on cherche
                        //printf("parametre_int =  %d\n", parametre_int);
                        if ( parametre_int >= 0 && parametre_int <= 65535)//0xFFFF
                        {
                            sprintf(parametre_string, "%04X", parametre_int);
                            //printf("parametre_string %s\n",parametre_string);
                            vrai_bytecode->py.string.buffer[j]=parametre_string[2]; //on écrit là où on avait laissé de la place
                            vrai_bytecode->py.string.buffer[j+1]=parametre_string[3]; //little Endian ??????
                            vrai_bytecode->py.string.buffer[j+2]=parametre_string[0];
                            vrai_bytecode->py.string.buffer[j+3]=parametre_string[1];
                            j+=4;
                            // printf("vrai_bytecode %s\n", vrai_bytecode->py.string.buffer);
                        }
                        else
                        {
                            printf("Label position must be under 65535 octets\n");
                            return -1;
                        }
                    }
                }
            }
            else if (opcode_value>=90) { // paramètre entier codé sur deux octets
                j+=6;
            }
           // printf("jjjjjjjjjjjjjjj %d\n", j);
        }
         //free_pyobj_t(liste_elements->py.list.value[i]);
    }

    free(code->py.codeblock->binary.content.bytecode);
    code->py.codeblock->binary.content.bytecode=vrai_bytecode;
    vrai_lnotab->py.list.size = compteur_lnotab;
    code->py.codeblock->binary.trailer.lnotab=vrai_lnotab;
    code->py.codeblock->binary.trailer.firstlineno = (vrai_lnotab->py.list.value[1])->py.number.integer;
    

   free(suite_hexa );
    free(opcode_string);
    free(parametre_string);
    free(label_vrai_string);
    free_pyobj_t(element);




    return 0;
}