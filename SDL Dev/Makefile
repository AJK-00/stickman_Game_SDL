CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lSDL2 -lSDL2_mixer -lm

SRCDIR = .
SOURCES = main.c audio.c renderer.c game_logic.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = running_man

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean