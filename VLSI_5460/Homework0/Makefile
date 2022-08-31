SOURCES+=$(wildcard *.cpp)

INCLUDES+=-I.

CFLAGS+=-Wall
CFLAGS+=$(INCLUDES)

LDFLAGS+=-Wall

EXECUTABLE=homework0

OBJECTS:=$(SOURCES:.c=.o)
CC=g++

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -c $? -o $@

.PHONY:clean
clean:
	rm -f *.0
	rm -f $(EXECUTABLE)