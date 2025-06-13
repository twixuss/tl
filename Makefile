#bin/tl: src/tests.cpp src/tests/*.cpp
#	g++ -o ./bin/tl $^ -I./include -std=c++23 -w -fmax-errors=1 -mpopcnt
bin/tl: src/tests/definitions.cpp
	g++ -o ./bin/tl $^ -I./include -std=c++23 -w -fmax-errors=999 -mpopcnt
