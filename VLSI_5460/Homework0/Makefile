SOURCES+=$(wildcard *.cpp)

INCLUDES+=-I.

CFLAGS+=-Wall
CFLAGS+=$(INCLUDES)

LDFLAGS+=-Wall

EXECUTABLE:=homework0

OBJECTS:=$(SOURCES:.cpp=.o)
CC:=g++

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)
#	$(CC) -c $(SOURCES)
#	$(CC) ./*.o -o $(EXECUTABLE)
#	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

#.cpp.o: $(SOURCES)
#	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY:clean
clean:
	rm -f *.o
	rm -f $(EXECUTABLE)