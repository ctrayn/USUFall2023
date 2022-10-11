CC = g++
EXEC = npe_cost
SOURCES = $(wildcard *.cpp)

$(EXEC): $(SOURCES) input_file.txt $(wildcard *.h)
	$(CC) -o $(EXEC) $(SOURCES)

.PHONY: run
run: $(EXEC)
	./$(EXEC)

.PHONY: clean
clean:
	-rm -f $(EXEC)