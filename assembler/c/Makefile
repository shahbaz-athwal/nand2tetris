CC = clang
CFLAGS = -Wall -Wextra -g
TARGET = build/assembler
SRCS = $(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) src/main.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
