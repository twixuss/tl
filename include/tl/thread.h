#pragma once
#include "common.h"
#include "optional.h"

#pragma warning(push)
#pragma warning(disable : 4582)
#pragma warning(disable : 4583)
#pragma warning(disable : 4625)
#pragma warning(disable : 4820)
#pragma warning(disable : 5220)

namespace TL {

FORCEINLINE void sleepMilliseconds(u32 milliseconds);
FORCEINLINE void sleepSeconds(u32 seconds);
#if OS_WINDOWS
FORCEINLINE void switchThread();
FORCEINLINE void spinIteration() { _mm_pause(); }

FORCEINLINE s8  lockAdd(s8  volatile *a, s8  b) { return _InterlockedExchangeAdd8((char *)a, (char)b); }
FORCEINLINE s16 lockAdd(s16 volatile *a, s16 b) { return _InterlockedExchangeAdd16(a, b); }
FORCEINLINE s32 lockAdd(s32 volatile *a, s32 b) { return (s32)_InterlockedExchangeAdd((long *)a, (long)b); }
FORCEINLINE s64 lockAdd(s64 volatile *a, s64 b) { return _InterlockedExchangeAdd64(a, b); }

template <class T>
FORCEINLINE T lockSet(T volatile *dst, T src) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedExchange64((long long*)dst, *(long long*)&src); 
	else if constexpr (sizeof(T) == 4) result = _InterlockedExchange  ((long     *)dst, *(long     *)&src);
	else if constexpr (sizeof(T) == 2) result = _InterlockedExchange16((short    *)dst, *(short    *)&src);
	else if constexpr (sizeof(T) == 1) result = _InterlockedExchange8 ((char     *)dst, *(char     *)&src);
	else static_assert(false, "lockSet is not available for this size");
	return *(T *)&result;
}

template <class T>
FORCEINLINE T lockSetIfEquals(T volatile *dst, T newValue, T comparand) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedCompareExchange64((long long*)dst, *(long long*)&newValue, *(long long*)&comparand);
	else if constexpr (sizeof(T) == 4) result = _InterlockedCompareExchange  ((long     *)dst, *(long     *)&newValue, *(long     *)&comparand);
	else if constexpr (sizeof(T) == 2) result = _InterlockedCompareExchange16((short    *)dst, *(short    *)&newValue, *(short    *)&comparand);
	else if constexpr (sizeof(T) == 1) result = _InterlockedCompareExchange8 ((char     *)dst, *(char     *)&newValue, *(char     *)&comparand);
	else static_assert(false, "lockSetIfEquals is not available for this size");
	return *(T *)&result;
}

TL_DECLARE_HANDLE(ThreadHandle);
extern ThreadHandle createThread(void (*fn)(void *), void *param);
extern void destroyThread(ThreadHandle handle);
extern u32 getCurrentThreadId();

#else
FORCEINLINE s8  lockAdd(s8  volatile *a, s8  b) { return __sync_fetch_and_add(a, b); }
FORCEINLINE s16 lockAdd(s16 volatile *a, s16 b) { return __sync_fetch_and_add(a, b); }
FORCEINLINE s32 lockAdd(s32 volatile *a, s32 b) { return __sync_fetch_and_add(a, b); }
FORCEINLINE s64 lockAdd(s64 volatile *a, s64 b) { return __sync_fetch_and_add(a, b); }

template <class T>
FORCEINLINE T lockSetIfEquals(T volatile *dst, T newValue, T comparand) {
	return __sync_val_compare_and_swap(dst, comparand, newValue);
}
#endif

FORCEINLINE u8  lockAdd(u8  volatile *a, u8  b) { return (u8 )lockAdd((s8  *)a, (s8 )b); }
FORCEINLINE u16 lockAdd(u16 volatile *a, u16 b) { return (u16)lockAdd((s16 *)a, (s16)b); }
FORCEINLINE u32 lockAdd(u32 volatile *a, u32 b) { return (u32)lockAdd((s32 *)a, (s32)b); }
FORCEINLINE u64 lockAdd(u64 volatile *a, u64 b) { return (u64)lockAdd((s64 *)a, (s64)b); }

FORCEINLINE s16 lockAdd(s16 volatile &a, s16 b) { return lockAdd(&a, b); }
FORCEINLINE s32 lockAdd(s32 volatile &a, s32 b) { return lockAdd(&a, b); }
FORCEINLINE s64 lockAdd(s64 volatile &a, s64 b) { return lockAdd(&a, b); }
FORCEINLINE u16 lockAdd(u16 volatile &a, u16 b) { return lockAdd(&a, b); }
FORCEINLINE u32 lockAdd(u32 volatile &a, u32 b) { return lockAdd(&a, b); }
FORCEINLINE u64 lockAdd(u64 volatile &a, u64 b) { return lockAdd(&a, b); }

#if 0
template <class T>
struct AtomicBase {
	T volatile value;

	AtomicBase() = default;
	AtomicBase(AtomicBase const &) = delete;
	AtomicBase(AtomicBase &&) = delete;
	AtomicBase(T that) : value(that) {}
	AtomicBase &operator=(AtomicBase const &) = delete;
	AtomicBase &operator=(AtomicBase &&) = delete;
	T store(T that) { return lockSet(&value, that), that; }
	T load() const { READ_WRITE_BARRIER; return value; }
	T operator=(T that) { return store(that); }
	operator T() const { return load(); }
};

template <class Int>
struct AtomicIntBase : AtomicBase<Int> {
	using Base = AtomicBase;
	using Base::Base;
	using Base::operator=;
	AtomicIntBase &operator++() { return lockIncrement(&value), *this; }
	AtomicIntBase &operator--() { return lockDecrement(&value), *this; }
	AtomicIntBase operator++(int) { return lockIncrement(&value) - 1; }
	AtomicIntBase operator--(int) { return lockDecrement(&value) + 1; }
	Int operator+=(Int that) { return lockAdd(value, that) + that; }
	Int operator-=(Int that) { return lockSubtract(value, that) - that; }
};

template <class T>
using SelectAtomicBase = std::conditional_t<std::is_integral_v<T>, AtomicIntBase<T>, AtomicBase<T>>;

template <class T>
struct Atomic : SelectAtomicBase<T> {
	using Base = SelectAtomicBase<T>;
	using Base::Base;
	using Base::operator=;
};

// single producer, single consumer
namespace SPSC {
template <class T, size_t capacity>
struct CircularQueue {
	CircularQueue()						 = default;
	CircularQueue(CircularQueue const &) = delete;
	template <class... Args>
	void emplace(Args &&... args) {
		getSpace();
		new (std::addressof(end->val)) T(std::forward<Args>(args)...);
		++poppableCount;
		increment(end);
	}
	void push(T &&val) { emplace(std::move(val)); }
	void push(T const &val) { emplace(val); }
	T pop() {
		ASSERT (poppableCount);
		--poppableCount;
		T result = std::move(begin->val);
		begin->val.~T();
		++pushableCount;
		increment(begin);
		return result;
	}
	Optional<T> try_pop() {
		Optional<T> result{};
		if (poppableCount) {
			--poppableCount;
			result.emplace(std::move(begin->val));
			begin->val.~T();
			++pushableCount;
			increment(begin);
		}
		return result;
	}
	umm size() const { return poppableCount; }

private:
	union Entry {
		T val;
		Entry() {}
		~Entry() {}
	};
	Entry entries[capacity];
	Entry *begin			  = entries;
	Entry *end				  = begin;
	Atomic<umm> poppableCount = 0;
	Atomic<umm> pushableCount = capacity;
	void increment(Entry *&e) {
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
#endif

#ifdef TL_IMPL

#if OS_WINDOWS

FORCEINLINE void sleepMilliseconds(u32 milliseconds) { Sleep(milliseconds); }
FORCEINLINE void sleepSeconds(u32 seconds) { Sleep(seconds * 1000); }
FORCEINLINE void switchThread() { SwitchToThread(); }

ThreadHandle createThread(void (*fn)(void *), void *param) {
	struct Data {
		void (*fn)(void *);
		void *param;
		bool volatile acquired;
	};
	Data data = {};
	data.fn = fn;
	data.param = param;
	data.acquired = false;
	HANDLE result = CreateThread(0, 0, [](void *param) -> DWORD {
		Data *pData = (Data *)param;
		Data data = *pData;
		pData->acquired = true;
		data.fn(data.param);
		return 0;
	}, &data, 0, 0);
	while (!data.acquired);
	return (ThreadHandle)result;
}
void destroyThread(ThreadHandle handle) {
	CloseHandle((HANDLE)handle);
}
u32 getCurrentThreadId() {
	return GetCurrentThreadId();
}
#else // ^^^ OS_WINDOWS ^^^ vvvvvv
#endif
#endif // TL_IMPL

template <class Predicate>
void loopUntil(Predicate &&predicate) {
	u32 tryCount = 0;
	while (!predicate()) {
		spinIteration();
		if (tryCount >= 64)
			switchThread();
		if (tryCount >= 4096)
			sleepMilliseconds(1);
		++tryCount;
	}
}

struct Mutex {
	bool volatile inUse = false;
};

inline void lock(Mutex &m) {
	loopUntil([&] {
		return !lockSetIfEquals(&m.inUse, true, false);
	});
}
inline void unlock(Mutex &m) {
	m.inUse = false;
}

struct RecursiveMutex {
	u32 volatile threadId = 0;
	u32 counter = 0;
};

inline void lock(RecursiveMutex &m) {
	u32 threadId = getCurrentThreadId();
	if (threadId == m.threadId) {
		++m.counter;
	} else {
		loopUntil([&] {
			return !lockSetIfEquals(&m.threadId, threadId, (u32)0);
		});
	}
}
inline void unlock(RecursiveMutex &m) {
	if (m.counter == 0) {
		m.threadId = 0;
	} else {
		--m.counter;
	}
}

#define SCOPED_LOCK(mutex) lock(mutex); DEFER { unlock(mutex); }
#define SCOPED_UNLOCK(mutex) unlock(mutex); DEFER { lock(mutex); }

} // namespace TL
#pragma warning(pop)
