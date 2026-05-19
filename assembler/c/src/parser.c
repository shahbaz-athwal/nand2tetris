#include <assert.h>
#include <stdlib.h>
#include <string.h>
typedef enum { A_COMMAND, C_COMMAND, L_COMMAND } CommandType;

CommandType command_type(const char *buffer) {
  if (buffer[0] == '@') {
    return A_COMMAND;
  } else if (buffer[0] == '(') {
    return L_COMMAND;
  } else {
    return C_COMMAND;
  }
}

char *symbol(const char *buffer) {
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
