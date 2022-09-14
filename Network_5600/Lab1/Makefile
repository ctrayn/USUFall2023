SOURCES:=$(wildcard *.cpp)
SOURCES+=../Sample_codes/frameio.cpp
SOURCES+=../Sample_codes/util.cpp

INCLUDES+=-I.
INCLUDES+=-I../Sample_codes/

CFLAGS+=$(INCLUDES)
CFLAGS+=-Wall

CC:=g++

EXEC=out

$(EXEC): $(SOURCES)
	$(CC) -o $(EXEC) $(CFLAGS) $(SOURCES)