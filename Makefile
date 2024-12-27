bin/tl: src/tests.cpp src/tests/*.cpp
	gcc -o ./bin/tl $^ -I./include -std=c++23 -w -fmax-errors=1
