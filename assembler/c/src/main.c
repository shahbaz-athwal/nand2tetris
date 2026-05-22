#include "code-translator.c"
#include "file-writer.c"
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

  FILE *src_file = fopen(argv[1], "r");
  if (src_file == NULL) {
    printf("Error: could not open file %s\n", argv[1]);
    return 1;
  }

  char *out_name = output_file_name(argv[1]);
  FILE *dest_file = fopen(out_name, "w");
  free(out_name);

  char buffer[256];
  while (fgets(buffer, sizeof(buffer), src_file)) {
    clean_line(buffer);
    if (buffer[0] == '\0') {
      continue;
    }

    if (command_type(buffer) == A_COMMAND) {
      char *sym = symbol_str(buffer);
      char *a_inst = sym_to_a_inst(sym);
      write_line(dest_file, a_inst);
      free(a_inst);
      free(sym);
    }

    if (command_type(buffer) == C_COMMAND) {

      char *jmp = jmp_code(jmp_str(buffer));
      char *comp = comp_code(comp_str(buffer));
      char *dest = dest_code(dest_str(buffer));

      char *c_inst = build_c_inst(dest, jmp, comp);
      write_line(dest_file, c_inst);
      free(c_inst);
    }
  }

  fclose(src_file);

  return 0;
}
#endif
