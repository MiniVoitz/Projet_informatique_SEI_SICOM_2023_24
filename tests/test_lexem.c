#include <unitest/unitest.h>

#include "pyas/all.h"

static void test_lexem_new(void) {

test_suite( "Interface robustness" );
lexem_t l = NULL;
l = lexem_new( NULL, NULL,0,0);
test_abort(l,"NULL when type is NULL" );
lexem_delete(l);

l = lexem_new( "aa", NULL,0,0);
test_abort(l,"NULL when value NULL" );
lexem_delete(l);

test_suite( "Well created");

l = lexem_new( "aa", "bb",0,0);
test_assert(l,"Created");
lexem_delete(l);
return;
}

static void test_lexem_print(void) {

test_suite( "Interface robustness" );

test_abort(lexem_print(NULL),"NULL when arg is NULL" );

void *l = lexem_new( "aa", NULL,0,0);
test_assert(lexem_print(l),"Affiche a part" );
test_oracle(  lexem_print(l), "[0:0:aa] (null) ", NULL,
         " Print ok" );
lexem_delete((lexem_t)l);

l = lexem_new( "aa", "bb",0,0);
test_assert(lexem_print(l),"Affiche a part" );
test_oracle(  lexem_print(l), "[0:0:aa] bb ", NULL,
         " Print ok" );
lexem_delete((lexem_t)l);
return;
}

static void test_lexem_delete(void) {
  test_suite( "Interface robustness" );

  test_assert(lexem_delete(NULL),"Nothing to do if NULL" );
  void *l = lexem_new( NULL, NULL,0,0);
  test_assert(lexem_delete(l),"Nothing to do if NULL" );
  l = lexem_new( "aa", NULL,0,0);
  test_assert(lexem_delete(l),"Need to free type" );
  l = lexem_new(NULL, "aa",0,0);
  test_assert(lexem_delete(l),"Need to free value" );

  test_suite( "Well deleted ?" );

  l = lexem_new( "aa", "bb",0,0);
  test_assert(lexem_delete(l),"Every thing has been deleted" );
  return;
}

static void test_lexem_peek(void)
{
  test_suite( "Interface robustness" );
  test_assert(!lexem_peek(NULL),"Nothing to peek if NULL" );
  list_t l = list_new();
  test_assert(!lexem_peek(&l),"Nothing to peek if NULL" );

  test_suite( "Blanck & Comment Skip" );


  char* type1 = "blank";
  char* value1 = " ";
  lexem_t lexem1 = lexem_new(type1,value1,0,0);

  char* type2="comment";
  char* value2="#v2";
  lexem_t lexem2 = lexem_new(type2,value2,1,1);

  char* type3 = "brack::Curlyright";
  char* value3 ="}";
  lexem_t lexem3 = lexem_new(type3,value3,2,2);

  queue_t q = queue_new();
  q = enqueue(q,lexem2);
  q = enqueue(q,lexem3);
  l = queue_to_list(q);
  lexem_t s = lexem_peek(&l);
  test_assert(s,"Blank & Comment are skip");
  test_oracle(lexem_print(s), "[2:2:brack::Curlyright] } ",NULL,"Good return");
  list_delete(l,&del_lexem);
  q = queue_new();
  q = enqueue(q,lexem1);
  l = queue_to_list(q);
  s = lexem_peek(&l); //s == NULL
  test_assert(!s,"Only blank or comment");
  list_delete(l,&del_lexem);

  return;
}

static void test_lexem_advance(void)
{
  test_suite( "Interface robustness" );
  test_assert(!lexem_advance(NULL),"Nothing to advance if NULL" );
  list_t l = list_new();
  test_assert(!lexem_advance(&l),"Nothing to advance if NULL" );


  test_suite( "Blanck & Comment Skip" );


  char* type1= "blank";
  char* value1 = " ";
  lexem_t lexem1 = lexem_new(type1,value1,0,0);

  queue_t q = queue_new();
  q = enqueue(q,lexem1);
  l = queue_to_list(q);
  list_print(l,&lexem_print);
  lexem_t s = lexem_advance(&l); //s == NULL
  test_assert(!s,"Just one blank => useless");
  lexem_delete(s); //Nothing to delete
  list_delete(l,&del_lexem); //Nothing to delete


  char* type2="comment";
  char* value2="#v2";
  lexem_t lexem2 = lexem_new(type2,value2,1,1);

  char* type3 = "brack::Curlyright";
  char* value3 ="}";
  lexem_t lexem3 = lexem_new(type3,value3,2,2);
  lexem1 = lexem_new(type1,value1,0,0);

  q = queue_new();
  q = enqueue(q,lexem1);
  q = enqueue(q,lexem3);
  q = enqueue(q,lexem2);
  l = queue_to_list(q);
  list_print(l,&lexem_print);
  s = lexem_advance(&l);
  test_assert(s,"Blank & Comment are skip");
  test_oracle(lexem_print(s), "[2:2:brack::Curlyright] } ",NULL,"Lexem return is the good one");
  test_oracle(lexem_print(l->content),"[1:1:comment] #v2 ",NULL,"List is correctly modified");
  lexem_delete(s);
  list_delete(l,&del_lexem);

  test_suite("sg fault TARCH");

  type1= "newline";
  value1 = "\\n";
  lexem1 = lexem_new(type1,value1,0,0);
  q = queue_new();
  q = enqueue(q,lexem1);
  l = queue_to_list(q);
  list_print(l,&lexem_print);
  s = lexem_advance(&l); //s == NULL
  test_assert(s,"return not null");
  test_oracle(lexem_print(s), "[0:0:newline] \\n ",NULL,"Lexem return is the good one");
  test_assert(!l,"End of list is null");
  lexem_delete(s); //Nothing to delete
  list_delete(l,&del_lexem); //Nothing to delete

  return;
}

static void test_next_lexem_is(void){
  test_suite( "Interface robustness" );

  test_assert(!next_lexem_is(NULL,NULL),"Nothing next if NULL" );

  list_t l = list_new();

  test_assert(!next_lexem_is(&l,NULL),"Nothing next if list empty");

  char* type1= "blank";
  char* value1 = " ";
  lexem_t lexem1 = lexem_new(type1,value1,0,0);

  char* type2="comment";
  char* value2="#v2";
  lexem_t lexem2 = lexem_new(type2,value2,1,1);

  char* type3 = "brack::Curlyright";
  char* value3 ="}";
  lexem_t lexem3 = lexem_new(type3,value3,2,2);



  queue_t q = queue_new();
  q = enqueue(q,lexem1);
  l = queue_to_list(q);

  test_assert(!next_lexem_is(&l,NULL),"Nothing to compare with");

  test_suite( "Blanck & Comment Skip" );

  test_assert(!next_lexem_is(&l,lexem3->type),"Not match");

  list_delete(l,&del_lexem);

  lexem1 = lexem_new(type1,value1,0,0);
  q = queue_new();
  q = enqueue(q,lexem1);
  q = enqueue(q,lexem2);
  q = enqueue(q,lexem3);
  l = queue_to_list(q);
  list_print(l,&lexem_print);

  test_assert(next_lexem_is(&l,lexem3->type),"Match");
  list_delete(l,&del_lexem);

  return;
}

// static void test_print_parse_error(void){
//   test_suite( "Interface robustness" );
//
//   test_abort(print_parse_error(NULL,NULL),"Error if NULL" );
//
//   char* m = calloc(64,sizeof(char));
//   m = "This is the cause";
//
//   test_abort(print_parse_error(m,NULL),"Pointer to list is NULL");
//
//   list_t l = list_new();
//   test_abort(print_parse_error(m,&l),"List empty");
//
//   test_suite( "Test Error");
//
//   char* type1= "blank";
//   char* value1 = " ";
//   lexem_t lexem1 = lexem_new(type1,value1,0,0);
//
//   char* type2="comment";
//   char* value2="#v2";
//   lexem_t lexem2 = lexem_new(type2,value2,1,1);
//
//   char* type3 = "brack::Curlyright";
//   char* value3 ="}";
//   lexem_t lexem3 = lexem_new(type3,value3,2,2);
//
//   queue_t q = queue_new();
//
//   q = enqueue(q,lexem1);
//   q = enqueue(q,lexem2);
//   q = enqueue(q,lexem3);
//   l = queue_to_list(q);
//   list_print(l,&lexem_print);
//
//   test_oracle(print_parse_error(m,&l),
//   "Error : This is the cause (Line,Columns) : (0,0)",
//   NULL,
//   "Error OK");
//
//   list_delete(l,&del_lexem);
//   free(m);
//
//   return;
// }





int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );
  test_lexem_new();
  test_lexem_print();
  test_lexem_delete();
  test_lexem_peek();
  test_lexem_advance();
  test_next_lexem_is();
  // test_print_parse_error();
  return 0;
}
