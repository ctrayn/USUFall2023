SOURCES+=$(wildcard *.cpp)

INCLUDES+=-I.

CFLAGS+=-Wall
CFLAGS+=$(INCLUDES)
CFLAGS+=-g

EXECUTABLE:=homework0

CC:=g++

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

.PHONY:clean
clean:
	rm -f *.o
	rm -f $(EXECUTABLE)