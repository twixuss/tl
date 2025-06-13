#bin/tl: src/tests.cpp src/tests/*.cpp
#	g++ -o ./bin/tl $^ -I./include -std=c++23 -w -fmax-errors=1 -mpopcnt

LIBS = -ldl -lpthread
CFLAGS = -I./include -std=c++23 -w -fmax-errors=999 -mpopcnt -mlzcnt -mavx2 -mfma $(LIBS)

headers = $(wildcard include/*.h)
sources = src/tests.cpp $(wildcard src/tests/*.cpp)
objects = $(sources:cpp=o)

./bin/tl: src/tests.o $(objects)
	g++ -o $@ $^ $(CFLAGS)

%.o: %.cpp $(headers)
	g++ -c -o $@ $< $(CFLAGS)
