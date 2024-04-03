/**
 * @file test-regexp.c
 * @author François Cayre <cayre@uvolante.org>
 * @date Sun Aug  7 21:22:53 2022
 * @brief Unit tests for regexps.
 *
 * Unit tests for regexps.
 */

#include <unitest/unitest.h>

#include "pyas/all.h"
/*

  The purpose of this file is to get you started for testing your regexp engine.

  From a purely combinatorial perspective, designing an exhaustive set of tests
  for regexp matching is impossible because (1) a regexp implements a
  deterministic finite automaton and (2) the size of the set of C-strings is:
  $$\sum_{i=1}^{2^48} 256^i$$
  (on an amd64 machine, just like the one you are probably using right now).

  That's the main reason why we shall focus on positive tests below.

  Yet, your little regexp engine will be constructed by *incrementally* adding
  support for various operators: we tell you how to match a constant regexp (one
  that does not contain any regexp operator) and a regexp containing `.` and/or
  `*` operators -- and you will add support for regexp operators `+` and `?`,
  etc.

  Also, remember that we are only supposed to match a regexp against the *start*
  of `source`!

  Now we need some systematic way of writing our tests for the operators. We
  propose the following strategy:
  - First we ensure core positive/negative properties of an operator (e.g., `*`
    operator must have a preceding char, etc.),
  - Then we ensure its behaviour is correct whether it appears (1) at the
    beginning, (2) in the middle, and (3) at the end of a regexp.

  Then it remains to test the behaviour of the legal combinations that were not
  covered by the above. In the present situation with the sole `.` and `*`
  operators available, the only remaining, uncovered legal combination is `.*`.

  That's because `*.` should have been covered in the tests above by "continuity
  by concatenation" (between a regexp that ends in `*` and another that starts
  in `.`), but `.*` was not tested already as the operators were tested
  separately and `*` refers to its preceding character.

  This gives us a blueprint for our testing strategy:
  1. pre/post-conditions;
  2. constant regexps;
  3. regexps containing `.`;
  4. regexps containing `*`;
  5. regexps containing `.*`.


  Obviously, any extension of these tests should be *justified* along the same
  lines. Similarly, the logic of any new group of tests for any new module
  should be *justified* in some way.

*/

static void re_pre_post( void ) {
  /* No relevant post-condition was identified, except return values. */

  char      *end;
  list_t l = list_new();

  test_suite( "Interface robustness" );

  l = re_read(NULL);
  test_assert(!re_match(l, NULL, &end),
	       "Matching fails when source is NULL");
  list_delete(l, &del_binary);

  l = re_read(NULL);
  test_assert(re_match(l, "a", &end),
		"The NULL regexp always matches");
  list_delete(l, &del_binary);

  l = re_read("");
  test_assert(re_match(l, "a", &end),
	        "The empty regexp always matches");
  list_delete(l, &del_binary);

  l = re_read("");
  test_assert(re_match(l, "a", NULL),
		"Parameter `end` is optional");
  list_delete(l, &del_binary);
}


static void re_constant( void ) {
  char *end;
  list_t l = NULL;  //

  test_suite( "Constant regexps" );

  l = re_read("abc");
  test_assert(!re_match(l, "ab", NULL),
    "The whole regexp must match");
  list_delete(l, &del_binary);

  l = re_read("a");
  test_assert(re_match(l, "abc", &end),
    "Can match a single char");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "bc", NULL,
    "  And suffix is OK");

  l = re_read("ab");
  test_assert(re_match(l, "abc", &end),
    "Can match prefix string");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "c", NULL,
    "  And suffix is OK");

  l = re_read("abc");
  test_assert(re_match(l, "abc", &end),
    "Can match last string");
  list_delete(l, &del_binary);

  test_oracle(!printf("%s", end), "", NULL,
    "  And suffix is empty");
}


static void re_dot( void ) {
  char *end;
  list_t l = list_new();  //

  test_suite( "`.` core properties" );

  l = re_read(".");
  test_assert(!re_match(l, "", NULL),
    "The empty source does *not* match `.`");
  list_delete(l, &del_binary);

  l = re_read(".");
  test_assert(re_match(l, "z", NULL) && re_match(l, "abc", &end),
    "Any character matches `.`");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "bc", NULL,
    "  And suffix is OK");

  test_suite( "`.` continuity" );

  l = re_read(".ip");
  test_assert(re_match(l, "zipend", &end),
    "Can match `.` at the start of regexp");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "end", NULL,
    "  And suffix is OK");

  l = re_read("z.p");
  test_assert(re_match(l, "zipend", &end),
    "Can match `.` inside regexp");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "end", NULL,
    "  And suffix is OK");

  l = re_read("zi.");
  test_assert(re_match(l, "zipend", &end),
    "Can match `.` at the end of regexp");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "end", NULL,
    "  And suffix is OK");

  return;
}


static void re_star( void ) {
  char *end;
  list_t l = list_new();  //

  test_suite( "`*` core properties" );

  l = re_read("*");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match `*` at the start of regexp");
  list_delete(l, &del_binary);

  l = re_read("a**");
  test_assert(!re_match(l, "aaaabb", &end),
    "Cannot match `*` without following char");
  list_delete(l, &del_binary);

  l = re_read("a*");
  test_assert(re_match(l, "", NULL),
    "Can match the empty source");

  test_assert(re_match(l, "baaaaa", &end),
    "Can match zero character");

  test_assert(re_match(l, "abb", &end),
    "Can match one character");

  test_assert(re_match(l, "aaaaaaabb", &end),
    "Can match several characters");
  list_delete(l, &del_binary);

  test_suite( "`*` continuity" );

  l = re_read("z*ip");
  test_assert(re_match(l, "zipend", &end),
    "Can match `*` at the start of regexp");
  list_delete(l, &del_binary);

  l = re_read("zi*p");

  test_assert(re_match(l, "zipend", &end),
    "Can match `*` inside regexp");
  list_delete(l, &del_binary);

  l = re_read("zip*");

  test_assert(re_match(l, "zipend", &end),
    "Can match `*` at the end of regexp");
  list_delete(l, &del_binary);

  return;
}

static void re_dot_star( void ) {
  char *end;
  list_t l = list_new();   //

  test_suite( "`.*` properties" );

  l = re_read(".*");
  test_assert(re_match(l, "", &end) && !*end,
    "`.*` matches the empty string");
  list_delete(l, &del_binary);

  l = re_read(".*");
  test_assert(re_match(l, "any string ;-)", &end) && !*end,
    "[Theorem] `.*` is the set of finite strings");
  list_delete(l, &del_binary);

  l = re_read("abc.*");
  test_assert(re_match(l, "abcdef", &end) && !*end,
    "`prefix.*` can match any prefix");
  list_delete(l, &del_binary);

  l = re_read(".*def");
  test_assert(re_match(l, "abcdef", &end) && !*end,
    "`.*suffix` can match any suffix");
  list_delete(l, &del_binary);

  l = re_read("abc.*def");
  test_assert(re_match(l, "abcABSORBMEdefend", &end),
    "[Corollary] `.*` is absorbent");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "end", NULL,
    "  And suffix is OK");
}

static void question_mark( void ) {
  char *end;
  list_t l = list_new();   //

  test_suite( "`?` core properties" );

  l = re_read("?");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match `?` at the start of regexp");
  list_delete(l, &del_binary);

  l = re_read("a??");
  test_assert(!re_match(l, "aaaabb", &end),
    "Cannot match `?` without following char");
  list_delete(l, &del_binary);

  l = re_read("a?");
  test_assert(re_match(l, "", NULL),
    "Can match the empty source");

  test_assert(re_match(l, "baaaaa", &end),
    "Can match zero character");

  test_assert(re_match(l, "abb", &end),
    "Can match one character");

  test_assert(re_match(l, "aaaaaaabb", &end),
    "Can't match several characters");
  list_delete(l, &del_binary);

  test_suite( "`?` continuity" );

  l = re_read("z?ip");
  test_assert(re_match(l, "zipend", &end),
    "Can match `?` at the start of regexp");
  list_delete(l, &del_binary);

  l = re_read("zi?p");
  test_assert(re_match(l, "zipend", &end),
    "Can match `?` inside regexp");
  list_delete(l, &del_binary);

  l = re_read("zip?");
  test_assert(re_match(l, "zipend", &end),
    "Can match `*?` at the end of regexp");
  list_delete(l, &del_binary);
}

static void neg( void ) {
  char *end;
  list_t l = list_new();   //

  test_suite( "`^` core properties" );

  l = re_read("^");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match `^` alone");
  list_delete(l, &del_binary);

  l = re_read("^a");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match `^` at the start of regexp");
  list_delete(l, &del_binary);

  l = re_read("a^");
  test_assert(!re_match(l, "aaaabb", &end),
    "Cannot match `^` without preceding char");
  list_delete(l, &del_binary);

  l = re_read("^a");
  test_assert(!re_match(l, "", NULL),
    "Can't match the empty source");

  test_assert(!re_match(l, "aaaaa", &end),
    "Not matching character");
  list_delete(l, &del_binary);

  l = re_read("^a");
  test_assert(re_match(l, "baa", &end),
    "Can match one character");
  list_delete(l, &del_binary);

  test_oracle(printf("%s", end), "aa", NULL,
    "  And suffix is OK");

  test_suite( "`^` continuity" );

  l = re_read("^z");
  test_assert(re_match(l, "aend", &end),
    "Can match `^` at the start of regexp");
  list_delete(l, &del_binary);

  l = re_read("zi^p");
  test_assert(re_match(l, "zieend", &end),
    "Can match `^` inside regexp");
  list_delete(l, &del_binary);
}

static void braket( void ) {
  char *end;
  list_t l = list_new();  //

  test_suite( "`[ ]` core properties" );

  l = re_read("[]");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match []");
  list_delete(l, &del_binary);

  l = re_read("[ ]");
  test_assert(re_match(l, " abc", &end),
    "Can match ' '");
  list_delete(l, &del_binary);

  l = re_read("[abc]");
  test_assert(re_match(l, "abc", &end),
    "Can match the first char in braket");
  list_delete(l, &del_binary);

  l = re_read("[abc]");
  test_assert(re_match(l, "bbc", &end),
    "Can match the middle char in braket");
  list_delete(l, &del_binary);

  l = re_read("[abc]");
  test_assert(re_match(l, "cbc", &end),
    "Can match the last char in braket");
  list_delete(l, &del_binary);

  test_suite( "`[] & -` core properties");

  l = re_read("[-]");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match [-]");
  list_delete(l, &del_binary);

  l = re_read("[-a]");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match [-a]");
  list_delete(l, &del_binary);

  l = re_read("[a-]");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match [a-]");
  list_delete(l, &del_binary);

  l = re_read("[a-A]");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match [a-A]");
  list_delete(l, &del_binary);

  l = re_read("[a-0]");
  test_assert(!re_match(l, "aaaa", &end),
    "Cannot match [a-0]");
  list_delete(l, &del_binary);

  l = re_read("[a-z]");
  test_assert(re_match(l, "abc", &end),
    "Can match '[a-z]'");
  list_delete(l, &del_binary);

  l = re_read("[A-Z]");
  test_assert(re_match(l, "ABC", &end),
    "Can match '[A-Z]'");
  list_delete(l, &del_binary);

  l = re_read("[0-9]");
  test_assert(re_match(l, "012", &end),
    "Can match '[0-9]'");
  list_delete(l, &del_binary);

  test_suite( "`[] & opperator` core properties");

  l = re_read("[0-9]*");
  test_assert(re_match(l, "0123456789a", &end),
    "Can match several");
  list_delete(l, &del_binary);

  l = re_read("[0-9]*");
  test_assert(re_match(l, "a", &end),
    "Can match 0");
  list_delete(l, &del_binary);

  l = re_read("[0-9]+");
  test_assert(!re_match(l, "a", &end),
    "Can't match 0");
  list_delete(l, &del_binary);

  l = re_read("[0-9]+");
  test_assert(re_match(l, "0123456789a", &end),
    "Can match several");
  list_delete(l, &del_binary);

  l = re_read("[0-9]?");
  test_assert(re_match(l, "12", &end),
    "Can't match more than 1");
  list_delete(l, &del_binary);

  l = re_read("[0-9]?");
  test_assert(re_match(l, "a", &end),
    "Can match 0");
  list_delete(l, &del_binary);

  l = re_read("[0-9]?");
  test_assert(re_match(l, "1a", &end),
    "Can match 1");
  list_delete(l, &del_binary);

  l = re_read("^[0-9]");
  test_assert(!re_match(l, "1", &end),
    "Can't match with number");
  list_delete(l, &del_binary);

  l = re_read("[^0]");
  test_assert(!re_match(l, "0", &end),
    "Can't match 0");
  list_delete(l, &del_binary);
}


static void back_slash( void )
{
  char * end = NULL;
  list_t l = NULL;  //

  test_suite( "`\\` core properties" );

  l = re_read("\\a");
  test_assert(!re_match(l, "0", &end),
    "Can't use '\\' with every char");
  list_delete(l, &del_binary);

  l = re_read("\\n");
  test_assert(re_match(l, "\na", &end),
    "Can match '\\n'");
  list_delete(l, &del_binary);

  l = re_read("\\t");
  test_assert(re_match(l, "\ta", &end),
    "Can match '\\t'");
  list_delete(l, &del_binary);

  l = re_read("\\+");
  test_assert(re_match(l, "+a", &end),
    "Can match '\\+'");
  list_delete(l, &del_binary);

  l = re_read("\\-");
  test_assert(re_match(l, "-a", &end),
    "Can match '\\-'");
  list_delete(l, &del_binary);

  l = re_read("\\\\");
  test_assert(re_match(l, "\\a", &end),
    "Can match '\\'");
  list_delete(l, &del_binary);

  l = re_read("\\*");
  test_assert(re_match(l, "*a", &end),
    "Can match '\\*'");
  list_delete(l, &del_binary);

  l = re_read("\\?");
  test_assert(re_match(l, "?a", &end),
    "Can match '\\?'");
  list_delete(l, &del_binary);

  l = re_read("\\[");
  test_assert(re_match(l, "[a", &end),
    "Can match '\\['");
  list_delete(l, &del_binary);

  l = re_read("\\]");
  test_assert(re_match(l, "]a", &end),
    "Can match '\\]'");
  list_delete(l, &del_binary);

  l = re_read("\\^");
  test_assert(re_match(l, "^a", &end),
    "Can match '\\^'");
  list_delete(l, &del_binary);
}

static void specific_test( void ){
  char * end = NULL;
  list_t l = list_new();

  test_suite("Recursive for *");

  l = re_read("a*a+");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  l = re_read("a*a");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  l = re_read("a*a?");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  test_suite("Recursive for +");

  l = re_read("a+a+");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  l = re_read("a+a");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  l = re_read("a+a?");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  test_suite("Recursive for +");

  l = re_read("a?a+");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  l = re_read("a?a");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);

  l = re_read("a?a?");
  test_assert(re_match(l, "aab", &end),
    "Can match 'aa'");
  list_delete(l, &del_binary);
}




int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  re_pre_post();
  re_constant();
  re_dot();
  re_star();
  re_dot_star();
  question_mark();
  neg();
  braket();
  back_slash();
  specific_test();

  exit( EXIT_SUCCESS );
}
