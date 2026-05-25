#include "code-translator.c"
#include "file-writer.c"
#include "line-reader.c"
#include "parser.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ends_with_asm(const char *filename) {
  if (filename == NULL)
    return 0;
  const char *suffix = ".asm";
  size_t fn_len = strlen(filename);
  size_t suf_len = strlen(suffix);
  if (fn_len < suf_len)
    return 0;
  return strcmp(filename + fn_len - suf_len, suffix) == 0;
}

char *output_file_name(const char *input_file_name) {
  int size = strlen(input_file_name);
  char *name = (char *)malloc((size + 2) * sizeof(char));
  strncpy(name, input_file_name, size - 3);
  strcpy(name + size - 3, "hack");
  return name;
}

#ifndef TEST_MODE
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  if (!ends_with_asm(argv[1])) {
    printf("Error: input file must have a .asm extension: %s\n", argv[1]);
    return 1;
  }

  LineReader *lr = lr_open(argv[1], clean_line);

  char *out_name = output_file_name(argv[1]);
  FILE *dest_file = fopen(out_name, "w");
  free(out_name);

  char *instruction[17];

  // First pass: find L instructions
  while (has_more_lines(lr)) {
    const char *line = next_line(lr);

    if (command_type(line) != L_COMMAND) {
      continue;
    }
    // if seen then skip otherwise add to symbol table.
  }
  // Second pass: find variables
  lr_reset(lr);
  while (has_more_lines(lr)) {
  }
  // Third pass: writer
  lr_reset(lr);
  while (has_more_lines(lr)) {

    const char *line = next_line(lr);

    if (command_type(line) == A_COMMAND) {
      cpy_a_inst(instruction, line);
    }

    if (command_type(line) == C_COMMAND) {
      cpy_c_inst(instruction, line);
    }

    fputs((const char *)instruction, dest_file);
    if (has_more_lines(lr)) {
      fputc('\n', dest_file);
    }
  }

  lr_close(lr);
  fclose(dest_file);

  return 0;
}
#endif
