CC = g++
EXEC = sa.exe
SOURCES = $(wildcard *.cpp)
INPUT = input_file.txt
ZIP_NAME = Homework4
REPORT_MD = $(wildcard *.md)
REPORT_PDF = $(REPORT_MD:.md=.pdf)

$(EXEC): $(SOURCES) input_file.txt $(wildcard *.h)
	$(CC) -o $(EXEC) $(SOURCES)

.PHONY: run
run: $(EXEC)
	./$(EXEC) $(INPUT)

.PHONY: clean
clean:
	-rm -f $(EXEC)
	-rm -f *.zip

.PHONY: zip
zip:
	zip -o $(ZIP_NAME) $(wildcard *.cpp) $(wildcard *.h) Makefile $(INPUT) $(wildcard *.exe) $(REPORT_PDF)