CC = clang
CFLAGS = -Wall -Wextra -g
TARGET = build/assembler
TEST_TARGET = build/test_sym_to_a_inst
SRCS = $(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c -o $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): test/test_sym_to_a_inst.c test/unity/unity.c src/code-translator.c
	mkdir -p build
	$(CC) $(CFLAGS) -DTEST_MODE test/test_sym_to_a_inst.c test/unity/unity.c -o $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean test
