TARGET = run
CFLAGS = -g -Wall -std=c99
LDFLAGS = -g -std=c99

SOURCES = $(shell find -name "*.c")
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
