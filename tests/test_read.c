#include <unitest/unitest.h>

#include "pyas/all.h"


static void lim_read( void ) {
  /* No relevant post-condition was identified, except return values. */

  test_suite( "Interface robustness" );

  test_assert( !re_read(NULL),
	       "Read error when regexp is NULL" );

  test_assert( !re_read(""),
	        "The empty regexp => nothing to print");
}

static void read_constant( void ) {

  list_t l = list_new();
  test_suite( "Constant regexps" );

  l = re_read("abc");
  test_assert(l, "Simple 'abc'" );
  list_delete(l,&del_binary);

  l = re_read("012");
  test_assert(l, "Simple '012'" );
  list_delete(l,&del_binary);

  l = re_read("abc");
  test_assert(l, "Simple 'ABC'" );
  list_delete(l,&del_binary);

}

static void re_dot_read( void ) {

  test_suite( "`.` core properties" );

  list_t l = list_new();
  l = re_read(".");
  test_assert( l, " `.` = Every ASCII char " );
  list_delete(l,&del_binary);

  test_suite( "`.` continuity" );

  l = re_read(".ip");
  test_assert(l, "Can read `.` at start of regexp" );
  list_delete(l,&del_binary);


  l =re_read("z.p");
  test_assert(l,
	        "Can read `.` inside regexp" );
          list_delete(l,&del_binary);

  l = re_read("zi.");
  test_assert(l,
	        "Can read `.` at end of regexp" );
          list_delete(l,&del_binary);

  return;
}

static void re_star_read( void ) {

  test_suite( "`*` core properties" );

  list_t l = list_new();
  l = re_read("*");
  test_assert(((binaryblock_t)(l->content))->err,"Cannot read `*` at start of regexp" );
  list_delete(l,&del_binary);

  l = re_read("a**");
  test_assert(  l,"Cannot read `*` without following char" );
  list_delete(l,&del_binary);

  test_suite( "`*` continuity" );

  l = re_read("*ip");
  test_assert(((binaryblock_t)(l->content))->err, "Can't read `*` at start of regexp" );
  list_delete(l,&del_binary);

  l = re_read("z*p");
  test_assert(l, "Can read `*` inside regexp" );
  list_delete(l,&del_binary);

  l = re_read("zi*");
  test_assert(l, "Can read `*` at end of regexp" );
  list_delete(l,&del_binary);

  return;
}

static void re_dot_star_read( void ) {

  test_suite( "`.*` properties" );
  list_t l = list_new();
  l = re_read(".*");
  test_assert(l, " can read `.*` the empty string" );
  list_delete(l,&del_binary);
}

static void question_mark_read( void ) {

    test_suite( "`?` core properties" );

    list_t l = list_new();
    l = re_read("?");
    test_assert(  l,
  	        "Cannot read `?` at start of regexp" );
            list_delete(l,&del_binary);
    l = re_read("a??");
    test_assert(  l,
  	        "Cannot read `?` without following char" );
            list_delete(l,&del_binary);

    test_suite( "`?` continuity" );

    l = re_read("?ip");

    test_assert(((binaryblock_t)(l->content))->err, "Can't read `?` at start of regexp" );
    list_delete(l,&del_binary);

    l = re_read("z?p");
    test_assert(l,
  	        "Can read `?` inside regexp" );
            list_delete(l,&del_binary);

    l = re_read("zi?");
    test_assert(l,
  	        "Can read `?` at end of regexp" );
            list_delete(l,&del_binary);

    return;
  }


static void neg_read( void ) {

      test_suite( "`^` core properties" );

      list_t l = list_new();
      l = re_read("^");
      test_assert(  l,
    	        "Cannot neg `^` nothing" );
              list_delete(l,&del_binary);
      l = re_read("^^a");
      test_assert(  l,
    	        "Cannot read `^` without preceding char" );
              list_delete(l,&del_binary);

      test_suite( "`^` continuity" );

      l = re_read("^ip");

      test_assert(l, "Can read `^` at start of regexp" );
      list_delete(l,&del_binary);

      l = re_read("z^p");
      test_assert(l,
    	        "Can read `^` inside regexp" );
              list_delete(l,&del_binary);

      l = re_read("zi^");
      test_assert(((binaryblock_t)(l->content))->err,"Can't read `^` at end of regexp" );
              list_delete(l,&del_binary);

      return;
    }

static void braket_read( void ) {
  list_t l = list_new();

  test_suite( "`[ ]` core properties" );
  l = re_read("[]");
  test_assert(((binaryblock_t)(l->content))->err, "Cannot read []");
  list_delete(l, &del_binary);

  l = re_read("[ ]");
  test_assert(l, "Can read ' '");
  list_delete(l, &del_binary);

  l = re_read("[abc]");
  test_assert(l, "Can match the first char in bracket");
  list_delete(l, &del_binary);

  test_suite( "`[] & -` core properties" );

  l = re_read("[-]");
  test_assert(((binaryblock_t)(l->content))->err, "Cannot read [-]");
  list_delete(l, &del_binary);

  l = re_read("[-a]");
  test_assert(((binaryblock_t)(l->content))->err, "Cannot read [-a]");
  list_delete(l, &del_binary);

  l = re_read("[a-]");
  test_assert(((binaryblock_t)(l->content))->err, "Cannot read [a-]");
  list_delete(l, &del_binary);

  l = re_read("[a-A]");
  test_assert(((binaryblock_t)(l->content))->err, "Cannot read [a-A]");
  list_delete(l, &del_binary);

  l = re_read("[a-0]");
  test_assert(((binaryblock_t)(l->content))->err, "Cannot read [a-0]");
  list_delete(l, &del_binary);

  l = re_read("[a-z]");
  test_assert(l, "Can read '[a-z]'");
  list_delete(l, &del_binary);

  l = re_read("[A-Z]");
  test_assert(l, "Can read '[A-Z]'");
  list_delete(l, &del_binary);

  l = re_read("[0-9]");
  test_assert(l, "Can read '[0-9]'");
  list_delete(l, &del_binary);

  test_suite( "`[] & operator` core properties" );

  l = re_read("[0-9]*");
  test_assert(l, "Can read []*");
  list_delete(l, &del_binary);

  l = re_read("[0-9]+");
  test_assert(l, "Can read []+");
  list_delete(l, &del_binary);

  l = re_read("[0-9]?");
  test_assert(l, "Can read []*");
  list_delete(l, &del_binary);

  l = re_read("^[0-9]");
  test_assert(l, "Can read ^[]");
  list_delete(l, &del_binary);

  l = re_read("^[0-9]*");
  test_assert(l, "Can read ^[]*]");
  list_delete(l, &del_binary);

  l = re_read("[^0]");
  test_assert(l, "Can't read 0");
  list_delete(l, &del_binary);
}

static void back_slash_read( void )
{
  list_t l = list_new();  // Déclarer la variable en dehors de la boucle test_suite

  test_suite( "`\\` core properties" );

  l = re_read("\\a");
  test_assert(l, "Can't use '\\' with every char");
  list_delete(l, &del_binary);

  l = re_read("\\n");
  test_assert(l, "Can read '\\n'");
  list_delete(l, &del_binary);

  l = re_read("\\t");
  test_assert(l, "Can read '\\t'");
  list_delete(l, &del_binary);

  l = re_read("\\+");
  test_assert(l, "Can read '\\+'");
  list_delete(l, &del_binary);

  l = re_read("\\-");
  test_assert(l, "Can read '\\-'");
  list_delete(l, &del_binary);

  l = re_read("\\");
  test_assert(l, "Can read '\\'");
  list_delete(l, &del_binary);

  l = re_read("\\*");
  test_assert(l, "Can read '\\*'");
  list_delete(l, &del_binary);

  l = re_read("\\?");
  test_assert(l, "Can read '\\?'");
  list_delete(l, &del_binary);

  l = re_read("\\[");
  test_assert(l, "Can read '\\['");
  list_delete(l, &del_binary);

  l = re_read("\\]");
  test_assert(l, "Can read '\\]'");
  list_delete(l, &del_binary);

  return;
}


int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  lim_read();
  read_constant();
  re_dot_read();
  re_star_read();
  re_dot_star_read();
  question_mark_read();
  neg_read();
  braket_read();
  back_slash_read();

  exit( EXIT_SUCCESS );
}
