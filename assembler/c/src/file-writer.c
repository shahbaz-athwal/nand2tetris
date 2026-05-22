#include <stdio.h>
int write_line(FILE *file, const char *line) {
  fputs(line, file);
  fputc('\n', file);
  return 0;
}
