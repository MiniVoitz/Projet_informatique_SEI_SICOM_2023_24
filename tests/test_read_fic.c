#include "pyas/all.h"
#include "pyas/read_fic.h"
#include <stdio.h>
#include <stdlib.h>

void validate_args(int argc, char *argv[], const char *usage) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s %s\n", argv[0], usage);
    exit(EXIT_FAILURE);
  }
}

char* read_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory and read file content
    char *content = (char *)malloc(file_size + 1);
    if (!content) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';

    fclose(file);
    return content;
}

void print_content(const char *content) {
  if (content == NULL)
    return;

  while (*content != '\0') {
    putchar(*content);
    content++;
  }
}

int main(int argc, char *argv[]) {
  validate_args(argc, argv, "input_file_pys input_file_txt");
  char * c = read_file(argv[1]);
  print_content(c);
  free(c);
  c = read_file(argv[2]);
  print_content(c);
  free(c);
  exit(EXIT_SUCCESS); // Exiting with success as all operations were successful
}
