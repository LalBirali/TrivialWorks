CXX = g++
CXXFLAGS = -std=c++11	-Wall	-g

PROGRAMS = sorer

build:
	g++ -o sorer src/main.cpp	$(CXXFLAGS)
	g++ -o test/test1 test/test1.cpp	$(CXXFLAGS)
	g++ -o test/test2 test/test2.cpp	$(CXXFLAGS)
	g++ -o test/test3 test/test3.cpp	$(CXXFLAGS)
	g++ -o test/test4 test/test4.cpp	$(CXXFLAGS)
	g++ -o test/test5 test/test5.cpp	$(CXXFLAGS)

run:
	./sorer data/test.sor 3
	

tests:	
	./test/test1 data/test.sor 3
	./test/test2 data/test.sor 3
	./test/test3 data/test.sor 3
	./test/test4 data/test.sor 3
	./test/test5 data/test.sor 3


val: 
	valgrind --leak-check=full ./sorer data/test.sor 3

clean:
	rm test/test1 test/test2 test/test3 test/test4 test/test5
