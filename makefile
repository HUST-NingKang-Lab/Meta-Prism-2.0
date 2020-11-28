CXXFLAGS= -std=c++11 -O3 -m64 -march=native
objects = build/loader.o build/newickParser.o build/simCalc.o build/structure.o build/main.o build/booster.o
build: $(objects)
	mkdir -p bin
	g++ $(CXXFLAGS) -lpthread -o bin/Meta-Prism_2.0 $(objects)
build/main.o: src/main.cpp
	g++ $(CXXFLAGS) -c  -o build/main.o src/main.cpp
build/simCalc.o: src/simCalc.hpp src/simCalc.cpp mkbuild
	g++ $(CXXFLAGS) -c -lpthread -o build/simCalc.o src/simCalc.cpp
build/loader.o: src/loader.cpp src/loader.hpp mkbuild
	g++ $(CXXFLAGS) -c -o build/loader.o src/loader.cpp
build/newickParser.o:src/newickParser.cpp src/newickParser.h mkbuild
	g++ $(CXXFLAGS) -c -o build/newickParser.o src/newickParser.cpp
build/structure.o:src/structure.h src/structure.cpp mkbuild
	g++ $(CXXFLAGS) -c -o build/structure.o src/structure.cpp
build/booster.o:src/booster.cpp src/booster.hpp mkbuild
	g++ $(CXXFLAGS) -c -o build/booster.o src/booster.cpp
mkbuild:
	mkdir -p build
.PHONY: clean
clean:
	rm build/*
	rm bin/* 

