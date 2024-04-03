


#include "pyas/all.h"
#include "pyas/read_fic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int no_blanks = 0;
int no_newline = 0;
int no_comments = 0;

void parse_arguments(int argc, char *argv[]) {
  int i;
  for(i = 0; i < argc; i++) { // Skip the program name and file arguments
    if (strcmp(argv[i], "--no-blanks")) {
      no_blanks = 1;
    } else if (strcmp(argv[i], "--no-newline")) {
      no_newline = 1;
    } else if (strcmp(argv[i], "--no-comments")) {
      no_comments = 1;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf("Needed: Path of the two files in ' ' \n");
    printf("Optional: --no-blanks --no-newline, --no-comments \n");
    return 0;
  }

  parse_arguments(argc, argv);

  FILE *fic_lex, *fic_py;

  fic_lex = fopen(argv[1], "r");
  char *file_lex = read_fic(fic_lex);

  fic_py = fopen(argv[2], "r");
  char *file_py = read_fic(fic_py);

  list_t liste = lex(file_lex, file_py);
  if (!liste) {
    exit(EXIT_FAILURE);
  }

  list_t l_buffer = liste;

  while (l_buffer) {
    lexem_t current_lexem = list_first(l_buffer);

    // Check if we should skip this lexeme, if so, continue to the next
    // iteration early
    if ((no_blanks && strcmp(current_lexem->type, "blank") == 0) ||
        (no_newline && strcmp(current_lexem->type, "newline") == 0) ||
        (no_comments && strcmp(current_lexem->type, "comment") == 0)) {
      l_buffer = l_buffer->next;
      continue; // Skip the rest of the loop for this lexeme
    }

    lexem_print(current_lexem);
    l_buffer = l_buffer->next;
  }

  list_delete(liste, &del_lexem);
  fclose(fic_lex);
  fclose(fic_py);
  free(file_lex);
  free(file_py);
  return 0;
}
