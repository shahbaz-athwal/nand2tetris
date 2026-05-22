#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  FILE *fp;
  char *line_buffer;
  size_t buffer_size;
  ssize_t read_line_size;
  bool had_cached_line;
} LineReader;

LineReader *lr_open(char *file_name) {
  FILE *fp = fopen(file_name, "r");
  if (!fp) {
    return NULL;
  }

  LineReader *lr = malloc(sizeof(LineReader));
  if (!lr) {
    printf("Failed to allocate memory to LineReader.");
    return NULL;
  }

  lr->fp = fp;
  lr->buffer_size = 0;
  lr->read_line_size = -1;
  lr->read_line_size = false;

  return lr;
}
