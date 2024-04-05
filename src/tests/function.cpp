#include <tl/function.h>
#include <tl/console.h>
#include <functional>
using namespace tl;

int sum(int a, int b) { return a + b; }

void function_test() {
	with(ConsoleColor::red, println("function_test broken"));
	/*
	{
		auto f = create_function(sum);
		assert(f(2, 12) == 14);
	}
	{
		auto l = std::function{[](int a, int b) {
			return a + b;
		}};

		auto f = create_function([](int a, int b) {
			return a + b;
		});
		assert(f(2, 12) == 14);
	}
	{
		int b = 12;
		auto f = create_function([&](int a) {
			return a + b;
		});
		assert(f(2) == 14);
	}
	*/
}
