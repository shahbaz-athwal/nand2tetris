#include <stdlib.h>
#include <string.h>

typedef struct {
  char *mnemonic;
  char *code;
} Mapping;

static const Mapping jump_table[] = {
    {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"},
    {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};

static const Mapping comp_table[] = {
    // a=0 group
    {"0", "0101010"},
    {"1", "0111111"},
    {"-1", "0111010"},
    {"D", "0001100"},
    {"A", "0110000"},
    {"!D", "0001101"},
    {"!A", "0110001"},
    {"-D", "0001111"},
    {"-A", "0110011"},
    {"D+1", "0011111"},
    {"A+1", "0110111"},
    {"D-1", "0001110"},
    {"A-1", "0110010"},
    {"D+A", "0000010"},
    {"D-A", "0010011"},
    {"A-D", "0000111"},
    {"D&A", "0000000"},
    {"D|A", "0010101"},

    // a=1 group (Replace A with M)
    {"M", "1110000"},
    {"!M", "1110001"},
    {"-M", "1110011"},
    {"M+1", "1110111"},
    {"M-1", "1110010"},
    {"D+M", "1000010"},
    {"D-M", "1010011"},
    {"M-D", "1000111"},
    {"D&M", "1000000"},
    {"D|M", "1010101"}};

char *dest_code(const char *buffer) {
  char *code = (char *)malloc(4 * sizeof(char));
  strcpy(code, "000");

  if (buffer == NULL) {
    return code;
  }
  if (strchr(buffer, 'A')) {
    code[0] = '1';
  }
  if (strchr(buffer, 'D')) {
    code[1] = '1';
  }
  if (strchr(buffer, 'M')) {
    code[2] = '1';
  }
  return code;
}

char *jump_code(const char *buffer) {
  if (buffer == NULL) {
    return "000";
  }

  for (int i = 0; i < 8; i++) {
    if (strcmp(jump_table[i].mnemonic, buffer) == 0) {
      return jump_table[i].code;
    }
  }
  return "000";
}

char *comp_code(const char *buffer) {
  int num_entries = sizeof(comp_table) / sizeof(comp_table[0]);
  for (int i = 0; i < num_entries; i++) {
    if (strcmp(comp_table[i].mnemonic, buffer) == 0) {
      return comp_table[i].code;
    }
  }
  return "000000";
}
