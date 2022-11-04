SOURCES:=$(wildcard *.cpp)

CFLAGS+=-lpthread -std=c++11

EXEC=out

.PHONY: $(EXEC)
$(EXEC): $(SOURCES)
# This was listed as traceroute usu.edu in the lab assignment, ip r is more general
	ip r > gateway_output
	g++ $(CFLAGS) -o $(EXEC) $(SOURCES)
	

.PHONY: clean
clean:
	rm -f $(EXEC)

.PHONY: run
run: $(EXEC)
	sudo ./out 2> output.log
