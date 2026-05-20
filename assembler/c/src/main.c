#include "code-translator.c"
#include "parser.c"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *sym_to_a_inst(char *sym) {
  int n = atoi(sym);
  assert(n < 32768);
  char *binary = (char *)malloc(17 * sizeof(char));

  for (int i = 15; i >= 1; i--) {
    if ((n >> i) & 1) {
      binary[i] = '1';
    } else {
      binary[i] = '0';
    }
  }
  binary[0] = '0';
  binary[16] = '\0';
  return binary;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Error: could not open file %s\n", argv[1]);
    return 1;
  }

  char buffer[256];
  while (fgets(buffer, sizeof(buffer), file)) {
    clean_line(buffer);
    if (buffer[0] == '\0') {
      continue;
    }

    if (command_type(buffer) == A_COMMAND) {

      char *sym = symbol_str(buffer);
      char *a_inst = sym_to_a_inst(sym);
      free(sym);
    }

    if (command_type(buffer) == C_COMMAND) {

      char *dest = dest_str(buffer);
      char *jmp = jmp_str(buffer);
      char *comp = comp_str(buffer);

      free(dest);
      free(jmp);
      free(comp);
      putchar('\n');
    }
  }

  fclose(file);

  return 0;
}
