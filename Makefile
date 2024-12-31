#bin/tl: src/tests.cpp src/tests/*.cpp
#	gcc -o ./bin/tl $^ -I./include -std=c++23 -w -fmax-errors=1 -mpopcnt
bin/tl: src/tests/definitions.cpp
	gcc -o ./bin/tl $^ -I./include -std=c++23 -w -fmax-errors=999 -mpopcnt
