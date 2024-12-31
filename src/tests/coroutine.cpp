#include <tl/console.h>
#include <setjmp.h>

using namespace tl;

struct Coroutine {
	jmp_buf buf;
	int (*function)(Coroutine *) = 0;
	bool initialized = false;
	bool yielded_at_least_once = false;
	bool yielding = false;
	bool finished = false;
	int operator()() {
		yielding = false;
		assert(!finished);
		defer {
			if (!yielding)
				finished = true;
		};
		return function(this);
	}
};

bool callable(Coroutine &c) {
	return !c.finished;
}

Coroutine create_coroutine(int (*function)(Coroutine *)) {
	return {.function = function};
}

#define start_coroutine() \
do { \
	the_coroutine->initialized = true; \
	if (the_coroutine->yielded_at_least_once) \
		longjmp(the_coroutine->buf, 1); \
} while (0)

// It's like resumable return
#define yield(x) \
do { \
	assert(the_coroutine->initialized); \
	if (setjmp(the_coroutine->buf) == 0) { \
		/*We are yielding*/ \
		the_coroutine->yielded_at_least_once = true; \
		the_coroutine->yielding = true; \
		return x; \
	} else { \
		/*We are resuming*/ \
	} \
} while (0)

int simple_coro(Coroutine *the_coroutine) {
	start_coroutine();

	yield(1);
	yield(2);
	yield(3);

	// Use return instead of yield to indicate that coroutine has finished and is not resumable anymore
	return 4;
}

void simple_coro_test() {
	auto c = create_coroutine(simple_coro);

	assert(c() == 1);
	assert(c() == 2);
	assert(c() == 3);
	assert(c() == 4);
	// c(); // will assert
}
int multi_coro_1(Coroutine *the_coroutine) {
	start_coroutine();

	yield(1);
	yield(2);
	return 3;
}

int yield_6(Coroutine *the_coroutine) {
	yield(6);
}

int multi_coro_2(Coroutine *the_coroutine) {
	start_coroutine();

	yield(4);
	yield(5);
	yield_6(the_coroutine);
	return 7;
}

void multi_coro_test() {
	Coroutine coros[2];
	coros[0] = create_coroutine(multi_coro_1);
	coros[1] = create_coroutine(multi_coro_2);

	while (1) {
		bool finished = true;
		for (int i = 0; i < 2; ++i) {
			auto &coro = coros[i];
			if (callable(coro)) {
				print("coros[%] yielded %\n", i, coro());
			}
			finished &= coro.finished;
		}
		if (finished) {
			break;
		}
	}
}


void newcoro_test() {

}


void coroutine_test() {
	newcoro_test();
	//simple_coro_test();
	//multi_coro_test();
}
