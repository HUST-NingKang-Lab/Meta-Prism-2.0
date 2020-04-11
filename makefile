CXXFLAGS= -std=c++11 -g -Wall -O3 -m64
objects = src/loader.o src/newickParser.o src/simCalc.o src/structure.o src/test.o src/booster.o
build: $(objects)
	g++ -std=c++11 -lpthread -o bin/test $(objects) -g
test: src/test.cpp
	g++ -std=c++11 -c  -o src/test.o src/test.cpp -g
simCalc: src/simCalc.hpp src/simCalc.cpp
	g++ -std=c++11 -c -O3 -lpthread -o src/simCalc.o src/simCalc.cpp -g
loader: loader.cpp loader.hpp
	g++ -std=c++11 -c -o src/loader.o src/loader.cpp -g
newickParser:src/newickParser.cpp src/newickParser.h
	g++ -std=c++11 -c -o src/newickParser.o src/newickParser.cpp -g
structure:src/structure.h src/structure.cpp
	g++ -std=c++11 -c -o src/structure.o src/structure.cpp -g
booster:stc/booster.cpp src/booster.hpp
	g++ -std=c++11 -c -o src/booster.o src/booster.cpp -g
.PHONY: clean
clean:
	rm src/*.o
	rm bin/* 
