CXXFLAGS= -std=c++11 -O3 -m64 -march=native -pthread
objects = build/DataProcessor.o build/newickParser.o build/simCalc.o build/structure.o build/main.o build/BoostCalculator.o
build: $(objects)
	mkdir -p bin
	g++ $(CXXFLAGS) -lpthread -o bin/Meta-Prism_2.0 $(objects)
build/main.o: src/main.cpp
	g++ $(CXXFLAGS) -c  -o build/main.o src/main.cpp
build/simCalc.o: src/simCalc.hpp src/simCalc.cpp mkbuild
	g++ $(CXXFLAGS) -c -o build/simCalc.o src/simCalc.cpp
build/DataProcessor.o: src/DataProcessor.cpp src/DataProcessor.hpp mkbuild
	g++ $(CXXFLAGS) -c -o build/DataProcessor.o src/DataProcessor.cpp
build/newickParser.o:src/newickParser.cpp src/newickParser.h mkbuild
	g++ $(CXXFLAGS) -c -o build/newickParser.o src/newickParser.cpp
build/structure.o:src/structure.h src/structure.cpp mkbuild
	g++ $(CXXFLAGS) -c -o build/structure.o src/structure.cpp
build/BoostCalculator.o:src/BoostCalculator.cpp src/BoostCalculator.hpp mkbuild
	g++ $(CXXFLAGS) -c -o build/BoostCalculator.o src/BoostCalculator.cpp
mkbuild:
	mkdir -p build
.PHONY: clean
clean:
	rm build/*
	rm bin/* 

