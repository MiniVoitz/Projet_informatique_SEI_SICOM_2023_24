#include <unitest/unitest.h>

#include "pyas/all.h"


static void extrem(void) {
  /* No relevant post-condition was identified, except return values. */

  test_suite( "Interface robustness" );

  test_assert( !lex( NULL, NULL),
	       "Fails when regexp_file and source are NULL" );

  test_assert( !lex( NULL, "a"),
  "Fails when regexp_file or source are NULL" );

  test_assert(  !lex( "a", NULL),
  "Fails when regexp_file or source are NULL" );

  test_assert(  !lex( "", ""),
  "Fails when regexp_file or source are empty" );

  test_assert(  !lex( "a", ""),
  "Fails when regexp_file or source are empty" );

}

static void read_pys(void) {

  test_suite("read pys ok");

  FILE* file = fopen("regexp_file.txt", "r");
  char * file_lex = read_fic(file);
  FILE* test1 = fopen("tests/test_lex/test1.pys","r");
  char *source_file = read_fic(test1);

  list_t l = list_new();
  l = lex(file_lex, source_file);

  test_assert( l, "Test1 list not empty");
  test_oracle(list_print(l,&lexem_print),"/tests/test_lex/res_test1.txt",NULL,"TEST  ORACLE 1");

  list_delete(l,&del_lexem);
  // free(source_file);
  // free(file_lex);
  fclose(test1);

  test_suite("read pys not ok");

  FILE* test_err1 = fopen("tests/test_lex/test_err1.pys","r");
  char *source_file_err1 = read_fic(test_err1);

  l = list_new();
  l = lex(file_lex, source_file_err1);
  test_assert(!l, "Test error 1");
  list_delete(l,&del_lexem);
  // free(source_file);
  // free(file_lex);
  fclose(test_err1);

  FILE* test_err2 = fopen("tests/test_lex/test_err2.pys","r");
  char *source_file_err2 = read_fic(test_err2);

  l = list_new();
  l = lex(file_lex, source_file_err2);
  test_assert(!l, "Test error 2");
  //list_print(l,&lexem_print);

  printf("\n");
  printf("----------------------------------------------------------\n");
  printf("Error which will be taken into accout in the parsing part\n");
  printf("----------------------------------------------------------\n");
  printf("\n");

  list_delete(l,&del_lexem);
  // free(source_file);
  // free(file_lex);
  fclose(test_err2);

  FILE* test_err3 = fopen("tests/test_lex/test_err3.pys","r");
  char *source_file_err3 = read_fic(test_err3);

  l = list_new();
  l = lex(file_lex, source_file_err3);
  test_assert(!l, "Test error  3");
  list_delete(l,&del_lexem);
  // free(source_file);
  // free(file_lex);
  fclose(test_err3);

  FILE* test_err4 = fopen("tests/test_lex/test_err4.pys","r");
  char *source_file_err4 = read_fic(test_err4);

  l = list_new();
  l = lex(file_lex, source_file_err4);
  test_assert(!l, "Test error 4");
  //list_print(l,&lexem_print);

  printf("\n");
  printf("----------------------------------------------------------\n");
  printf("Error which will be taken into accout in the parsing part\n");
  printf("----------------------------------------------------------\n");
  printf("\n");

  list_delete(l,&del_lexem);
  // free(source_file);
  // free(file_lex);
  fclose(test_err4);

  FILE* test_err5 = fopen("tests/test_lex/test_err5.pys","r");
  char *source_file_err5 = read_fic(test_err5);

  l = list_new();
  l = lex(file_lex, source_file_err5);
  test_assert(!l, "Test error 5");
  //list_print(l,&lexem_print);

  printf("\n");
  printf("----------------------------------------------------------\n");
  printf("Error which will be taken into accout in the parsing part\n");
  printf("----------------------------------------------------------\n");
  printf("\n");

  list_delete(l,&del_lexem);
  // free(source_file_err5);
  // free(file_lex);
  fclose(test_err5);

  fclose(file);
  return;

}



int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  extrem();
  read_pys();

  exit(EXIT_SUCCESS);
}
