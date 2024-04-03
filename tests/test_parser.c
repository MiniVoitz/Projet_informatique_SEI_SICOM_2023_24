#include "pyas/all.h"
#include "pyas/read_fic.h"
#include <stdio.h>
#include <stdlib.h>
#include <unitest/unitest.h>

int errors = 0;

void validate_args(int argc, char *argv[], const char *usage) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s %s\n", argv[0], usage);
    exit(EXIT_FAILURE);
  }
}

void suite_parser_tests(void) {
  test_suite("Parser Tests");

  // // Test 1: Check if valid Python assembler code is correctly parsed
  // list_t *valid_lexems = get_valid_lexems();  // You'd need a function to
  // generate or fetch valid lexems for testing test_assert(parse(valid_lexems)
  // != NULL, "Valid Python assembler code is correctly parsed");

  // // Test 2: Check if an error in the Python assembler code results in NULL
  // list_t *invalid_lexems = get_invalid_lexems();  // Similarly, you'd need a
  // function to generate or fetch invalid lexems for testing
  // test_assert(parse(invalid_lexems) == NULL, "Error in Python assembler code
  // results in NULL");

  // if a test fails exit with EXIT_FAILURE
}

// method for parser test with parameter of source file
void parser_test(int argc, char *argv[]) {
  unit_test(argc, argv);
}

int main(int argc, char *argv[]) {
  // Validate the number of arguments
  validate_args(argc, argv, "source_file");

  // Read the file's content
  char *content = read_fic(fopen(argv[1], "r"));

  // Parse the file's content
  parser_test(argc, argv);

  // Print the file's content
  print_file(content);

  exit(errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
