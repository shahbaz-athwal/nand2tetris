#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void clean_line(char *buffer) {
  int read = 0;
  int write = 0;

  while (buffer[read] != '\0') {
    if (buffer[read] == '/' && buffer[read + 1] == '/') {
      break;
    }
    if (buffer[read] != ' ' && buffer[read] != '\n' && buffer[read] != '\r' &&
        buffer[read] != '\t') {
      buffer[write] = buffer[read];
      write++;
    }
    read++;
  }

  buffer[write] = '\0';
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
    printf("%s", buffer);
    if (buffer[0] != '\0') {
      putchar('\n');
    }
  }

  fclose(file);

  return 0;
}
