CC = g++
EXEC = sa.exe
SOURCES = $(wildcard *.cpp)
INPUT = input_file.txt

$(EXEC): $(SOURCES) input_file.txt $(wildcard *.h)
	$(CC) -o $(EXEC) $(SOURCES)

.PHONY: run
run: $(EXEC)
	./$(EXEC) $(INPUT)

.PHONY: clean
clean:
	-rm -f $(EXEC)