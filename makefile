CXXFLAGS= -std=c++11 -g -Wall -O3 -m64 -march=native
objects = src/loader.o src/newickParser.o src/simCalc.o src/structure.o src/main.o src/booster.o
build: $(objects)
	g++ $(CXXFLAGS) -lpthread -o bin/Meta-Prism_2.0 $(objects) -g
src/main.o: src/main.cpp
	g++ $(CXXFLAGS) -c  -o src/main.o src/main.cpp -g
src/simCalc.o: src/simCalc.hpp src/simCalc.cpp
	g++ $(CXXFLAGS) -c -lpthread -o src/simCalc.o src/simCalc.cpp -g
src/loader.o: src/loader.cpp src/loader.hpp
	g++ $(CXXFLAGS) -c -o src/loader.o src/loader.cpp -g
src/newickParser.o:src/newickParser.cpp src/newickParser.h
	g++ $(CXXFLAGS) -c -o src/newickParser.o src/newickParser.cpp -g
src/structure.o:src/structure.h src/structure.cpp
	g++ $(CXXFLAGS) -c -o src/structure.o src/structure.cpp -g
src/booster.o:src/booster.cpp src/booster.hpp
	g++ $(CXXFLAGS) -c -o src/booster.o src/booster.cpp -g
.PHONY: clean
clean:
	rm src/*.o
	rm bin/* 
