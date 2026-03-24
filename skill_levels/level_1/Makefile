# Tic-Tac-Toe v1 - Naive Implementation
# Simple Makefile for single-file build

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = tictactoe

all: $(TARGET)

$(TARGET): tictactoe.c
	$(CC) $(CFLAGS) -o $(TARGET) tictactoe.c

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

