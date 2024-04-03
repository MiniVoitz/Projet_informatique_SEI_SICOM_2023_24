#include <stdio.h>
#include <stdlib.h>

#include "pyas/all.h"


int test_function_match(char *test_name, char *regexp_char, char *source,
                        char *exp_res) {
  printf("The expected result for the test '%s' with the regular expression "
         "'%s' and the source '%s' is '%s'\n",
         test_name, regexp_char, source, exp_res);
  printf("The actual result is\n");
  list_t l = re_read(regexp_char);
  char *end = NULL;
  printf("%d\n", re_match(l, source, &end));
  return 0;
}

int main() {
  char *test_name;
  char *regexp_char;
  char *source;
  char *exp_res;

  test_name = "Parentheses";
  regexp_char = "[a-d]";
  source = "c";
  exp_res = " 1 therefore regular expression recognized";
  test_function_match(test_name, regexp_char, source, exp_res);

  test_name = "Parentheses";
  regexp_char = "[a-d]";
  source = "e";
  exp_res = " 0 therefore regular expression not recognized";
  test_function_match(test_name, regexp_char, source, exp_res);

  test_name = "Parentheses and +";
  regexp_char = "[a-d]+";
  source = "abbddc";
  exp_res = " 1 therefore regular expression recognized";
  test_function_match(test_name, regexp_char, source, exp_res);

  test_name = "Parentheses and *, empty source";
  regexp_char = "[a-d]*";
  source = "";
  exp_res = " 1 therefore regular expression recognized";
  test_function_match(test_name, regexp_char, source, exp_res);

  test_name = "Parentheses and ^";
  regexp_char = "^[a-d]+";
  source = "ef123";
  exp_res = " 1 therefore regular expression recognized";
  test_function_match(test_name, regexp_char, source, exp_res);

  test_name = "More complicated expression";
  regexp_char = "[a-d]+[1-9]*";
  source = "abbd3847";
  exp_res = " 1 therefore regular expression recognized";
  test_function_match(test_name, regexp_char, source, exp_res);

  test_name = "More complicated expression";
  regexp_char = "[a-d]?[1-9]+";
  source = "0";
  exp_res = " 0 therefore regular expression not recognized";
  test_function_match(test_name, regexp_char, source,
                      exp_res); // here the test is not conclusive / now it is

  test_name = "test simple";
  regexp_char = "[\\+\\-]?[0-9]+";
  source = "aaac";
  exp_res = " 0 therefore regular expression recognized";
  test_function_match(test_name, regexp_char, source, exp_res);
  return 1;
}
