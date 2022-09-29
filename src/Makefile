OS := $(shell uname -s)
ARCH := $(shell uname -m)

calc: main.cpp codegen.o parse.o tok.o
	g++ -std=c++17 main.cpp codegen.o parse.o tok.o -O1 -o calc

linux: codegen.cpp calc.hpp
	g++ -std=c++17 -c codegen.cpp -D LINUX -O1 -o codegen.o

mac: codegen.cpp calc.hpp
	g++ -std=c++17 -c codegen.cpp -D MAC -O1 -o codegen.o

parse.o: parse.cpp calc.hpp
	g++ -std=c++17 -c parse.cpp -O1 -o parse.o

tok.o: tok.cpp calc.hpp
	g++ -std=c++17 -c tok.cpp -O1 -o tok.o


.PHONY: clean
# clean all output program
clean:
	rm -rf calc
	rm -f *.o
