all: SimpleGR.exe mapper.exe benchmark

CXX = g++

OFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG
#OFLAGS = -g

LINKFLAGS = 

SimpleGR.exe: SimpleGR.o IO.o Utils.o main.o MazeRouter.o
	/bin/rm -f SimpleGR.exe
	$(CXX) $(LINKFLAGS) SimpleGR.o IO.o Utils.o main.o MazeRouter.o -o SimpleGR.exe

mapper.exe: IO.o Utils.o mapper.o
	/bin/rm -f mapper.exe
	$(CXX) $(LINKFLAGS) IO.o Utils.o mapper.o -o mapper.exe

SimpleGR.o: SimpleGR.h SimpleGR.cpp
	/bin/rm -f SimpleGR.o
	$(CXX) $(OFLAGS) SimpleGR.cpp -c

Utils.o: SimpleGR.h Utils.cpp
	/bin/rm -f Utils.o
	$(CXX) $(OFLAGS) Utils.cpp -c

IO.o: SimpleGR.h IO.cpp
	/bin/rm -f IO.o
	$(CXX) $(OFLAGS) IO.cpp -c
	
MazeRotuer.o: SimpleGR.h MazeRouter.cpp
	/bin/rm -f MazeRouter.o
	$(CXX) $(OFLAGS) MazeRouter.cpp -c

main.o: SimpleGR.h main.cpp
	/bin/rm -f main.o
	$(CXX) $(OFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c

mapper.o: SimpleGR.h mapper.cpp
	/bin/rm -f mapper.o
	$(CXX) $(OFLAGS) mapper.cpp -c

	
#setup the symbolic links of benchmark files under this folder
benchmark: simple.gr adaptec1.simple.gr adaptec2.simple.gr adaptec3.simple.gr

simple.gr:
	ln -s ../benchmarks/simple.gr

adaptec1.simple.gr:
	ln -s ../benchmarks/adaptec1.simple.gr
	
adaptec2.simple.gr:
	ln -s ../benchmarks/adaptec2.simple.gr

adaptec3.simple.gr:
	ln -s ../benchmarks/adaptec3.simple.gr

clean:
	/bin/rm -f SimpleGR.o IO.o Utils.o main.o MazeRouter.o SimpleGR.exe mapper.o mapper.exe
	/bin/rm -f simple.gr adaptec1.simple.gr adaptec2.simple.gr adaptec3.simple.gr
