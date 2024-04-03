#include "pyas/all.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char type1[]="blank";
    char value1[2]=" ";
    lexem_t lexem1 = lexem_new(type1,value1,1,3);
    /*lexem_print(lexem1);*/

    char type2[]="comment";
    char value2[]="#coucou";
    lexem_t lexem2 = lexem_new(type2,value2,1,1);
    /*lexem_print(lexem2);*/

    char type3[]="brack::Curlyright";
    char value3[]="}";
    lexem_t lexem3 = lexem_new(type3,value3,2,2);
    /*lexem_print(lexem3);*/

    struct link_t link1 = {lexem1, NULL};
    struct link_t link2 = {lexem2, NULL};
    struct link_t link3 = {lexem3, NULL};
    link1.next=&link2;
    link2.next=&link3;

    list_t l = &link1;
    list_t* l_point = (list_t*)malloc(sizeof(list_t));
    if (l_point != NULL) {
        *l_point = l;
    }
    /*list_print(*l_point,lexem_print);*/

    lexem_t lexem_final;
    lexem_final = lexem_advance(l_point);
    lexem_print(lexem_final);
    list_print(*l_point,lexem_print);

    /*printf("next_lexem_is=%d\n",next_lexem_is(l_point,"brack::Curlyright"));*/

    /*print_parse_error("ohlala quelle erreur!",l_point);*/

    free(l_point);

    return 0;
}
