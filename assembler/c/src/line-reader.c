#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*LineFilterFn)(char *line);

typedef struct {
  FILE *fp;
  char *line_buffer;
  size_t buffer_size;
  ssize_t read_line_size;
  bool has_cached_line;
  LineFilterFn filter;
} LineReader;

LineReader *lr_open(char *file_name, LineFilterFn filter) {
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
  lr->line_buffer = NULL;
  lr->buffer_size = 0;
  lr->read_line_size = -1;
  lr->has_cached_line = false;
  lr->filter = filter;

  return lr;
}

bool has_more_lines(LineReader *lr) {
  if (!lr)
    return false;

  if (lr->has_cached_line)
    return true;

  while ((lr->read_line_size =
              getline(&lr->line_buffer, &lr->buffer_size, lr->fp)) != -1) {
    if (lr->filter) {
      lr->filter(lr->line_buffer);
    }
    if (lr->line_buffer[0] != '\0') {
      lr->has_cached_line = true;
      return true;
    }
  };

  return false;
}

char *next_line(LineReader *lr) {
  if (!has_more_lines(lr)) {
    return NULL;
  }
  lr->has_cached_line = false;
  return lr->line_buffer;
}

void lr_reset(LineReader *lr) {
  if (lr)
    return;

  if (lr->fp) {
    rewind(lr->fp);
  }
  lr->has_cached_line = false;
  lr->read_line_size = -1;
}

void lr_close(LineReader *lr) {
  if (lr)
    return;

  if (lr->fp) {
    fclose(lr->fp);
  }
  free(lr);
  free(lr->line_buffer);
}
