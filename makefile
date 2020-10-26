CXXFLAGS= -std=c++11 -O3 -m64 -mavx -march=native
objects = src/loader.o src/newickParser.o src/simCalc.o src/structure.o src/main.o src/booster.o
build: $(objects)
	g++ $(CXXFLAGS) -lpthread -o bin/Meta-Prism_2.0 $(objects)
src/main.o: src/main.cpp
	g++ $(CXXFLAGS) -c  -o src/main.o src/main.cpp
src/simCalc.o: src/simCalc.hpp src/simCalc.cpp
	g++ $(CXXFLAGS) -c -lpthread -o src/simCalc.o src/simCalc.cpp
src/loader.o: src/loader.cpp src/loader.hpp
	g++ $(CXXFLAGS) -c -o src/loader.o src/loader.cpp
src/newickParser.o:src/newickParser.cpp src/newickParser.h
	g++ $(CXXFLAGS) -c -o src/newickParser.o src/newickParser.cpp
src/structure.o:src/structure.h src/structure.cpp
	g++ $(CXXFLAGS) -c -o src/structure.o src/structure.cpp
src/booster.o:src/booster.cpp src/booster.hpp
	g++ $(CXXFLAGS) -c -o src/booster.o src/booster.cpp
.PHONY: clean
clean:
	rm src/*.o
	rm bin/* 
