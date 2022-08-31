SOURCES+=$(wildcard *.cpp)

CFLAGS+=-Wall
LDFLAGS+=-Wall

EXECUTABLE=homework0

OBJECTS=$(SOURCES.c=.0)

CC=g++

$(EXECUTABLE): $(OBJECTS)
	$(CC) -c $(SOURCES)
	$(CC) -o $(EXECUTABLE) $(LDFLAGS) */.o

.c.o: $(SOURCES)
	$(CC) $(CFLAGS) $? -c $<