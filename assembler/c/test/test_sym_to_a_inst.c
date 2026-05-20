#include "unity/unity.h"
#include "../src/code-translator.c"
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

static void assert_sym_to_a_inst(char *sym, char *expected) {
  char *actual = sym_to_a_inst(sym);
  TEST_ASSERT_EQUAL_STRING(expected, actual);
  free(actual);
}

void test_0(void) { assert_sym_to_a_inst("0", "0000000000000000"); }

void test_1(void) { assert_sym_to_a_inst("1", "0000000000000001"); }

void test_32767(void) { assert_sym_to_a_inst("32767", "0111111111111111"); }

void test_100(void) { assert_sym_to_a_inst("100", "0000000001100100"); }

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_0);
  RUN_TEST(test_1);
  RUN_TEST(test_32767);
  RUN_TEST(test_100);
  return UNITY_END();
}
