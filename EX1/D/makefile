TARGET = run
CFLAGS = -g -Wall -pthread -lrt
LDFLAGS = -g -lrt -pthread

SOURCES = $(shell find -name "*.c")
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
