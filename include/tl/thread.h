#pragma once
#include "common.h"
#include <atomic>
#include <optional>
#include <thread>
#pragma warning(push)
#pragma warning(disable : 4582)
#pragma warning(disable : 4583)
#pragma warning(disable : 4625)
// single producer, single consumer
namespace TL {
namespace SPSC {
template <class T, size_t capacity>
struct CircularQueue {
	CircularQueue()						= default;
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
		Entry() {}
		~Entry() {}
	};
	Entry entries[capacity];
	Entry* begin					  = entries;
	Entry* end						  = begin;
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
} // namespace SPSC
} // namespace TL
#pragma warning(pop)
