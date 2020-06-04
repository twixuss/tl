#pragma once
#include "common.h"
#include <atomic>
#include <optional>
#include <thread>
#pragma warning(push)
#pragma warning(disable : 4582)
#pragma warning(disable : 4583)
#pragma warning(disable : 4625)
#if OS_WINDOWS
#include <intrin.h>
#endif
namespace TL {

#if OS_WINDOWS
extern "C" __declspec(dllimport) int __stdcall SwitchToThread();
FORCEINLINE void switchThread() {
	SwitchToThread();
}

FORCEINLINE u32 lockIncrement(u32 volatile *v) {
	return (u32)_InterlockedIncrement((long *)v);
}
#endif

template <class Pred>
inline void waitUntil(Pred pred) {
	u32 miss = 0;
	while (!pred()) {
		++miss;
		if (miss >= 256) {
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		} else 
		if (miss >= 64) {
			switchThread();
		} else if (miss >= 16) {
			_mm_pause();
		}
	}
}

inline void waitUntil(bool const volatile &condition) {
	waitUntil([&]{ return condition; });
}

struct SyncPoint {
	u32 const threadCount;
	u32 volatile counter;
	inline SyncPoint(u32 threadCount) : threadCount(threadCount), counter(0) {}
};

inline void sync(SyncPoint& sp) {
	lockIncrement(&sp.counter);
	waitUntil([&]{return sp.counter==sp.threadCount;});
}

// single producer, single consumer
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
