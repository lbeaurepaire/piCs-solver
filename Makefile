# Name of the executable
TARGET = nonogram

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -std=c99

all: $(TARGET)

$(TARGET): nonogram.c
	$(CC) $(CFLAGS) -o $(TARGET) nonogram.c

clean:
	rm -f $(TARGET)

