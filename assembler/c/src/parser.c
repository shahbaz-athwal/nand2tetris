#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef enum { A_COMMAND, C_COMMAND, L_COMMAND, SKIP = -1 } CommandType;

CommandType command_type(const char *buffer) {
  if (buffer == NULL || buffer[0] == '\0') {
    return SKIP;
  }

  if (buffer[0] == '@') {
    return A_COMMAND;
  } else if (buffer[0] == '(') {
    return L_COMMAND;
  } else {
    return C_COMMAND;
  }
}

char *symbol_str(const char *buffer) {
  int type = command_type(buffer);
  assert(type != C_COMMAND);

  int str_len;

  if (type == A_COMMAND) {
    str_len = strlen(buffer) - 1; // removing @
  } else {
    str_len = strlen(buffer) - 2; // removing ( and )
  }

  char *sub_string = (char *)malloc((str_len + 1) * sizeof(char));

  strncpy(sub_string, buffer + 1, str_len + 1);
  if (type == A_COMMAND) {
    sub_string[str_len + 1] = '\0';
  } else {
    sub_string[str_len] = '\0';
  }
  return sub_string;
}

char *dest_str(const char *buffer) {
  assert(command_type(buffer) == C_COMMAND);
  char *des_end = strchr(buffer, '=');
  if (!des_end) {
    return NULL;
  }
  int len = des_end - buffer;
  char *dest = (char *)malloc((len + 1) * sizeof(char));
  strncpy(dest, buffer, len);
  dest[len] = '\0';
  return dest;
}

char *jmp_str(const char *buffer) {
  assert(command_type(buffer) == C_COMMAND);
  char *jmp_start = strchr(buffer, ';');
  if (!jmp_start) {
    return NULL;
  }

  int len = strlen(jmp_start + 1);
  char *jmp = (char *)malloc((len + 1) * sizeof(char));
  strncpy(jmp, jmp_start + 1, len);
  jmp[len] = '\0';
  return jmp;
}

char *comp_str(const char *buffer) {
  assert(command_type(buffer) == C_COMMAND);

  char *des_end = strchr(buffer, '=');
  char *jmp_start = strchr(buffer, ';');

  const char *start;
  const char *end;
  if (des_end == NULL) {
    start = buffer;
  } else {
    start = des_end + 1;
  }

  if (jmp_start == NULL) {
    end = buffer + strlen(buffer);
  } else {
    end = jmp_start;
  }

  int size = end - start;

  char *comp = (char *)malloc((size + 1) * sizeof(char));
  strncpy(comp, start, size);
  comp[size] = '\0';
  return comp;
}
