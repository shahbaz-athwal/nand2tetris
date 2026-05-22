#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *mnemonic;
  char *code;
} Mapping;

static const Mapping dest_table[] = {
    {"", "000"},  {"M", "001"},  {"D", "010"},  {"MD", "011"},
    {"A", "100"}, {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}};

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

char *dest_code(char *buffer) {
  if (buffer == NULL) {
    return "000";
  }

  int num_entries = sizeof(dest_table) / sizeof(dest_table[0]);
  for (int i = 0; i < num_entries; i++) {
    if (strcmp(dest_table[i].mnemonic, buffer) == 0) {
      free(buffer);
      return dest_table[i].code;
    }
  }

  free(buffer);
  return "000";
}

char *jmp_code(char *buffer) {
  if (buffer == NULL) {
    return "000";
  }
  int num_entries = sizeof(jump_table) / sizeof(jump_table[0]);

  for (int i = 0; i < num_entries; i++) {
    if (strcmp(jump_table[i].mnemonic, buffer) == 0) {
      free(buffer);
      return jump_table[i].code;
    }
  }
  free(buffer);
  return "000";
}

char *comp_code(char *buffer) {
  int num_entries = sizeof(comp_table) / sizeof(comp_table[0]);
  for (int i = 0; i < num_entries; i++) {
    if (strcmp(comp_table[i].mnemonic, buffer) == 0) {
      free(buffer);
      return comp_table[i].code;
    }
  }
  free(buffer);
  return "0000000";
}

char *sym_to_a_inst(char *sym) {
  int n = atoi(sym);
  assert(n < 32768);
  char *binary = (char *)malloc(17 * sizeof(char));

  for (int i = 15; i >= 0; i--) {
    binary[15 - i] = ((n >> i) & 1) ? '1' : '0';
  }
  binary[16] = '\0';
  return binary;
}

char *build_c_inst(char *dest, char *jump, char *comp) {
  char *binary = (char *)malloc(17 * sizeof(char));

  strncpy(binary, "111", 3);
  strncpy(binary + 3, comp, 7);
  strncpy(binary + 10, dest, 3);
  strncpy(binary + 13, jump, 3);
  binary[16] = '\0';
  return binary;
}
