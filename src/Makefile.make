TARGET = airport-sim
LIBS = -lm -lpthread
CC = c99
CFLAGS = -g -Wall -std=c99 -D _POSIX_C_SOURCE=200809L

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
		-rm -f *.o
		-rm -f $(TARGET)
