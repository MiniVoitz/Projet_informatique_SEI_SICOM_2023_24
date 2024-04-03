#include <stdio.h>
#include <stdlib.h>

#include "pyas/all.h"
#include "pyas/read_fic.h"

char* read_fic(FILE* file) {
    char* buffer = NULL;
    int len;
    if (file == NULL) {
        perror("Error opening the file");
        return NULL;
    }

    // Move the cursor to the end of the file to get its size
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);  // Reset the cursor to the beginning

    // Allocate memory for the file's content
    buffer = (char*)malloc(len + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    // Read the file's content into the buffer
    fread(buffer, 1, len, file);
    // Add a null character at the end to make it a valid string
    buffer[len] = '\0';
    return buffer;
}

void print_file(const char* content) {
    if (content == NULL) return;

    while (*content != '\0') {
        putchar(*content);
        content++;
    }
}