#pragma once
#include "common.h"
#include <optional>
#include <atomic>
#include <thread>
// single producer, single consumer
namespace TL::SPSC {
template <class T, size_t capacity>
struct CircularQueue {
	CircularQueue() = default;
	CircularQueue(CircularQueue const&) = delete;
#define EMPLACE(name, param, assign) \
	void name(param) {               \
		getSpace();                  \
		assign;                      \
		++poppableCount;             \
		increment(end);              \
	}
	template <class... Args>
	EMPLACE(emplace, Args&&... args, new (std::addressof(end->val)) T(std::forward<Args>(args)...));
	EMPLACE(push, T&& val, new (std::addressof(end->val)) T(std::move(val)));
	EMPLACE(push, T const& val, new (std::addressof(end->val)) T(val));
#undef EMPLACE
	std::optional<T> pop() {
		std::optional<T> result{};
		if (poppableCount) {
			--poppableCount;
			result.emplace(std::move(begin->val));
			begin->val.~T();
			++pushableCount;
			increment(begin);
		}
		return result;
	}
	size_t size() const { return poppableCount; }

private:
	union Entry {
		T val;
#pragma warning(suppress : 4582)
		Entry() {}
#pragma warning(suppress : 4583)
		~Entry() {}
#pragma warning(suppress : 4625)
	};
	Entry entries[capacity];
	Entry* begin = entries;
	Entry* end = begin;
	std::atomic<size_t> poppableCount = 0;
	std::atomic<size_t> pushableCount = capacity;
	void increment(Entry*& e) {
		if (++e == entries + capacity) {
			e = entries;
		}
	}
	void getSpace() {
		while (pushableCount == 0) {
#if !COMPILER_GCC
			std::this_thread::sleep_for(std::chrono::microseconds(1));
#endif
		}
		--pushableCount;
	}
};
#if 0
void circularQueueTest() {
#if !COMPILER_GCC
	{
		struct Test {
			Test() { puts("con"); }
			Test(Test const&) { puts("cop"); }
			Test(Test&&) { puts("mov"); }
			~Test() { puts("des"); }
		};
		SPSC::CircularQueue<Test, 1> q;
		puts("emplace");
		q.emplace();
		puts("pop");
		q.pop();
		{
			puts("push &");
			Test t;
			q.push(t);
			puts("pop");
			q.pop();
		}
		puts("push &&");
		q.push({});
		puts("pop");
		q.pop();
	}

	CircularQueue<u32, 128> queue;
	bool done;
	auto popperWork = [&]() {
		u32 counter = 0;
		while (!done || queue.size()) {
			if (auto v = queue.pop(); v) {
				ASSERT(v.value() == counter++);
				// Sleep((DWORD)rand() & 1);
			}
		}
	};
	for (int j = 0; j < 2; ++j) {
		done = false;
		std::thread popper{popperWork};
		for (u32 i = 0; i < 1024 * 1024; ++i) {
			queue.push(i);
			// Sleep((DWORD)rand() & 1);
		}
		done = true;
		popper.join();
	}
#endif
}
#endif

} // namespace SPSC
