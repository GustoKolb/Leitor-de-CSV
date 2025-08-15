CC = gcc         
CFLAGS = -Wall -Wextra 
TARGET = csvreader

SOURCES = main.c io.c
OBJECTS = $(SOURCES:.c=.o)

RM = rm -f

.PHONY: all clean purge

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS)

purge: clean
	$(RM) $(TARGET)
