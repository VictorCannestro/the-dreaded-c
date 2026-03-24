# Tic-Tac-Toe v2 - Modular Implementation
# Multi-file Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
SRCDIR = src
INCDIR = include
TARGET = tictactoe

SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/types.c \
          $(SRCDIR)/board.c \
          $(SRCDIR)/game.c \
          $(SRCDIR)/display.c \
          $(SRCDIR)/input.c \
          $(SRCDIR)/ai.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

