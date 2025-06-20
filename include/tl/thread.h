#pragma once
#include "function.h"
#include "list.h"
#include "queue.h"
#include "sleep.h"

#include <emmintrin.h>

#pragma warning(push)
#pragma warning(disable: TL_DISABLED_WARNINGS)
#pragma warning(disable: 4582)
#pragma warning(disable: 4583)
#pragma warning(disable: 4625)
#pragma warning(disable: 4820)
#pragma warning(disable: 5220)

namespace tl {

template <class T>
concept AInterlockExchangeable = sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8;

forceinline void yield_smt() { _mm_pause(); }

#if OS_WINDOWS

TL_API void switch_thread();

forceinline s16 atomic_increment(s16 volatile *a) { return _InterlockedIncrement16((short *)a); }
forceinline s32 atomic_increment(s32 volatile *a) { return _InterlockedIncrement((long *)a); }
forceinline s64 atomic_increment(s64 volatile *a) { return _InterlockedIncrement64((long long *)a); }

forceinline s16 atomic_decrement(s16 volatile *a) { return _InterlockedDecrement16((short *)a); }
forceinline s32 atomic_decrement(s32 volatile *a) { return _InterlockedDecrement((long *)a); }
forceinline s64 atomic_decrement(s64 volatile *a) { return _InterlockedDecrement64((long long *)a); }

forceinline s8  atomic_add(s8  volatile *a, s8  b) { return _InterlockedExchangeAdd8((char *)a, (char)b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return _InterlockedExchangeAdd16(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return (s32)_InterlockedExchangeAdd((long *)a, (long)b); }
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return _InterlockedExchangeAdd64(a, b); }

forceinline s8  atomic_and(s8  volatile *a, s8  b) { return _InterlockedAnd8((char *)a, (char)b); }
forceinline s16 atomic_and(s16 volatile *a, s16 b) { return _InterlockedAnd16((short *)a, (short)b); }
forceinline s32 atomic_and(s32 volatile *a, s32 b) { return _InterlockedAnd((long *)a, (long)b); }
forceinline s64 atomic_and(s64 volatile *a, s64 b) { return _InterlockedAnd64((long long *)a, (long long)b); }

forceinline s8  atomic_or(s8  volatile *a, s8  b) { return _InterlockedOr8((char *)a, (char)b); }
forceinline s16 atomic_or(s16 volatile *a, s16 b) { return _InterlockedOr16((short *)a, (short)b); }
forceinline s32 atomic_or(s32 volatile *a, s32 b) { return _InterlockedOr((long *)a, (long)b); }
forceinline s64 atomic_or(s64 volatile *a, s64 b) { return _InterlockedOr64((long long *)a, (long long)b); }

forceinline s8  atomic_xor(s8  volatile *a, s8  b) { return _InterlockedXor8((char *)a, (char)b); }
forceinline s16 atomic_xor(s16 volatile *a, s16 b) { return _InterlockedXor16((short *)a, (short)b); }
forceinline s32 atomic_xor(s32 volatile *a, s32 b) { return _InterlockedXor((long *)a, (long)b); }
forceinline s64 atomic_xor(s64 volatile *a, s64 b) { return _InterlockedXor64((long long *)a, (long long)b); }

template <AInterlockExchangeable T>
forceinline T atomic_set(T volatile *dst, T src) {
	s64 result;
	     if constexpr (sizeof(T) == 1) result = _InterlockedExchange8 ((char     *)dst, *(char     *)&src);
	else if constexpr (sizeof(T) == 2) result = _InterlockedExchange16((short    *)dst, *(short    *)&src);
	else if constexpr (sizeof(T) == 4) result = _InterlockedExchange  ((long     *)dst, *(long     *)&src);
	else if constexpr (sizeof(T) == 8) result = _InterlockedExchange64((long long*)dst, *(long long*)&src);
	else static_error_t(T, "lockSet is not available for this size");
	return *(T *)&result;
}

template <AInterlockExchangeable T>
forceinline T atomic_compare_exchange(T volatile *dst, T new_value, T comparand) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedCompareExchange64((long long*)dst, *(long long*)&new_value, *(long long*)&comparand);
	else if constexpr (sizeof(T) == 4) result = _InterlockedCompareExchange  ((long     *)dst, *(long     *)&new_value, *(long     *)&comparand);
	else if constexpr (sizeof(T) == 2) result = _InterlockedCompareExchange16((short    *)dst, *(short    *)&new_value, *(short    *)&comparand);
	else if constexpr (sizeof(T) == 1) result = _InterlockedCompareExchange8 ((char     *)dst, *(char     *)&new_value, *(char     *)&comparand);
	else static_error_t(T, "atomic_compare_exchange is not available for this size");
	return *(T *)&result;
}

#elif OS_LINUX

forceinline void switch_thread() {}

forceinline s8  atomic_increment(s8  volatile *a) { return __sync_fetch_and_add(a, 1); }
forceinline s16 atomic_increment(s16 volatile *a) { return __sync_fetch_and_add(a, 1); }
forceinline s32 atomic_increment(s32 volatile *a) { return __sync_fetch_and_add(a, 1); }
forceinline s64 atomic_increment(s64 volatile *a) { return __sync_fetch_and_add(a, 1); }

forceinline s8  atomic_decrement(s8  volatile *a) { return __sync_fetch_and_sub(a, 1); }
forceinline s16 atomic_decrement(s16 volatile *a) { return __sync_fetch_and_sub(a, 1); }
forceinline s32 atomic_decrement(s32 volatile *a) { return __sync_fetch_and_sub(a, 1); }
forceinline s64 atomic_decrement(s64 volatile *a) { return __sync_fetch_and_sub(a, 1); }

forceinline s8  atomic_add(s8  volatile *a, s8  b) { return __sync_fetch_and_add(a, b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return __sync_fetch_and_add(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return __sync_fetch_and_add(a, b); }
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return __sync_fetch_and_add(a, b); }

forceinline s8  atomic_sub(s8  volatile *a, s8  b) { return __sync_fetch_and_sub(a, b); }
forceinline s16 atomic_sub(s16 volatile *a, s16 b) { return __sync_fetch_and_sub(a, b); }
forceinline s32 atomic_sub(s32 volatile *a, s32 b) { return __sync_fetch_and_sub(a, b); }
forceinline s64 atomic_sub(s64 volatile *a, s64 b) { return __sync_fetch_and_sub(a, b); }

forceinline s8  atomic_and(s8  volatile *a, s8  b) { return __sync_fetch_and_and(a, b); }
forceinline s16 atomic_and(s16 volatile *a, s16 b) { return __sync_fetch_and_and(a, b); }
forceinline s32 atomic_and(s32 volatile *a, s32 b) { return __sync_fetch_and_and(a, b); }
forceinline s64 atomic_and(s64 volatile *a, s64 b) { return __sync_fetch_and_and(a, b); }

forceinline s8  atomic_or(s8  volatile *a, s8  b) { return __sync_fetch_and_or(a, b); }
forceinline s16 atomic_or(s16 volatile *a, s16 b) { return __sync_fetch_and_or(a, b); }
forceinline s32 atomic_or(s32 volatile *a, s32 b) { return __sync_fetch_and_or(a, b); }
forceinline s64 atomic_or(s64 volatile *a, s64 b) { return __sync_fetch_and_or(a, b); }

forceinline s8  atomic_xor(s8  volatile *a, s8  b) { return __sync_fetch_and_xor(a, b); }
forceinline s16 atomic_xor(s16 volatile *a, s16 b) { return __sync_fetch_and_xor(a, b); }
forceinline s32 atomic_xor(s32 volatile *a, s32 b) { return __sync_fetch_and_xor(a, b); }
forceinline s64 atomic_xor(s64 volatile *a, s64 b) { return __sync_fetch_and_xor(a, b); }

template <AInterlockExchangeable T>
forceinline T atomic_set(T volatile *dst, T src) {
	return __atomic_exchange_n(dst, src, __ATOMIC_ACQ_REL);
}

template <class T>
forceinline T atomic_compare_exchange(T volatile *dst, T new_value, T comparand) {
	return __sync_val_compare_and_swap(dst, comparand, new_value);
}
#endif

template <AInterlockExchangeable T>
forceinline bool atomic_replace(T volatile *dst, T new_value, T condition) {
	return atomic_compare_exchange(dst, new_value, condition) == condition;
}

struct Thread {
	Allocator allocator;
	void (*function)(Thread *self) = 0;
	void *param = 0;
	void *handle = 0;
	#ifdef TL_USE_CONTEXT
	Context *context = 0;
	#endif
};

TL_API void run_thread(Thread *thread);
inline Thread *create_thread(void (*function)(Thread *self), void *param) {
	auto allocator = TL_GET_CURRENT(allocator);
	auto thread = allocator.allocate<Thread>();
	thread->allocator = allocator;
	thread->function = function;
	thread->param = param;
	run_thread(thread);
	return thread;
}


// These are used only once in `create_thread`, but sadly they can't be made local...
template <class Tuple, umm... indices>
static void thread_invoke(Thread *thread) noexcept {
	Tuple &tuple = *(Tuple *)thread->param;
	std::invoke(std::move(std::get<indices>(tuple))...);
}
template <class Tuple, umm... indices>
static constexpr auto get_thread_invoke(std::index_sequence<indices...>) noexcept {
	return &thread_invoke<Tuple, indices...>;
}

template <class Fn, class ...Args>
inline Thread *create_thread(Fn &&fn, Args &&...args) {
	auto allocator = TL_GET_CURRENT(allocator);
	auto thread = allocator.allocate<Thread>();
	thread->allocator = allocator;

	using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
	auto params = allocator.allocate_uninitialized<Tuple>();
	new(params) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);
	thread->function = get_thread_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
	thread->param = params;

	run_thread(thread);
	return thread;
}

namespace td {
template <class Fn, class Tuple, umm... indices>
inline static decltype(auto) invoke(Fn &&fn, Tuple &tuple, std::index_sequence<indices...>) noexcept {
	return fn(get<indices>(tuple)...);
}
}

//template <class Fn, class ...Args>
//Thread create_thread(Fn &&fn, Args &&...args) {
//	return create_thread(create_function(fn, args...));
//}

TL_API void join(Thread *thread);

TL_API bool finished(Thread *thread);
TL_API void free(Thread *thread);

inline bool valid(Thread *thread) { return thread->handle != 0; }

TL_API u32 get_current_thread_id();
TL_API u32 get_thread_id(Thread *thread);

inline void join_and_free(Thread *thread) {
	join(thread);
	free(thread);
}

forceinline u16 atomic_increment(u16 volatile *a) { return (u16)atomic_increment((s16 *)a); }
forceinline u32 atomic_increment(u32 volatile *a) { return (u32)atomic_increment((s32 *)a); }
forceinline u64 atomic_increment(u64 volatile *a) { return (u64)atomic_increment((s64 *)a); }

forceinline u16 atomic_decrement(u16 volatile *a) { return (u16)atomic_decrement((s16 *)a); }
forceinline u32 atomic_decrement(u32 volatile *a) { return (u32)atomic_decrement((s32 *)a); }
forceinline u64 atomic_decrement(u64 volatile *a) { return (u64)atomic_decrement((s64 *)a); }

forceinline u8  atomic_add(u8  volatile *a, u8  b) { return (u8 )atomic_add((s8  *)a, (s8 )b); }
forceinline u16 atomic_add(u16 volatile *a, u16 b) { return (u16)atomic_add((s16 *)a, (s16)b); }
forceinline u32 atomic_add(u32 volatile *a, u32 b) { return (u32)atomic_add((s32 *)a, (s32)b); }
forceinline u64 atomic_add(u64 volatile *a, u64 b) { return (u64)atomic_add((s64 *)a, (s64)b); }

forceinline u8  atomic_and(u8  volatile *a, u8  b) { return (u8 )atomic_and((s8  *)a, (s8 )b); }
forceinline u16 atomic_and(u16 volatile *a, u16 b) { return (u16)atomic_and((s16 *)a, (s16)b); }
forceinline u32 atomic_and(u32 volatile *a, u32 b) { return (u32)atomic_and((s32 *)a, (s32)b); }
forceinline u64 atomic_and(u64 volatile *a, u64 b) { return (u64)atomic_and((s64 *)a, (s64)b); }

forceinline u8  atomic_or(u8  volatile *a, u8  b) { return (u8 )atomic_or((s8  *)a, (s8 )b); }
forceinline u16 atomic_or(u16 volatile *a, u16 b) { return (u16)atomic_or((s16 *)a, (s16)b); }
forceinline u32 atomic_or(u32 volatile *a, u32 b) { return (u32)atomic_or((s32 *)a, (s32)b); }
forceinline u64 atomic_or(u64 volatile *a, u64 b) { return (u64)atomic_or((s64 *)a, (s64)b); }

forceinline u8  atomic_xor(u8  volatile *a, u8  b) { return (u8 )atomic_xor((s8  *)a, (s8 )b); }
forceinline u16 atomic_xor(u16 volatile *a, u16 b) { return (u16)atomic_xor((s16 *)a, (s16)b); }
forceinline u32 atomic_xor(u32 volatile *a, u32 b) { return (u32)atomic_xor((s32 *)a, (s32)b); }
forceinline u64 atomic_xor(u64 volatile *a, u64 b) { return (u64)atomic_xor((s64 *)a, (s64)b); }

forceinline bool atomic_and(bool volatile *a, bool b) { return (bool)atomic_and((u8 *)a, (u8)b); }
forceinline bool atomic_or (bool volatile *a, bool b) { return (bool)atomic_or ((u8 *)a, (u8)b); }
forceinline bool atomic_xor(bool volatile *a, bool b) { return (bool)atomic_xor((u8 *)a, (u8)b); }

extern TL_API FatFunctionPointer thread_initter;
extern TL_API FatFunctionPointer thread_deinitter;

#ifdef TL_IMPL

TL_API FatFunctionPointer thread_initter = +[]{
	init_allocator();
	current_printer = standard_output_printer;
};
TL_API FatFunctionPointer thread_deinitter = +[]{
	deinit_allocator();
};

#if OS_WINDOWS

void switch_thread() { SwitchToThread(); }

void run_thread(Thread *thread) {
	#ifdef TL_USE_CONTEXT
	thread->context = clone_context();
	#endif
	thread->handle = CreateThread(0, 0, [](void *param) noexcept -> DWORD {
		auto thread = (Thread *)param;
		#ifdef TL_USE_CONTEXT
		registrate(thread->context);
		#endif

		thread_initter();
		defer { thread_deinitter(); };
		thread->function(thread);
		return 0;
	}, thread, 0, 0);
}
void join(Thread *thread) {
	WaitForSingleObjectEx((HANDLE)thread->handle, INFINITE, false);
}
bool finished(Thread *thread) {
	return WaitForSingleObjectEx((HANDLE)thread->handle, 0, false) == WAIT_OBJECT_0;
}
void free(Thread *thread) {
	CloseHandle((HANDLE)thread->handle);
	thread->handle = 0;
}

u32 get_current_thread_id() {
	return GetCurrentThreadId();
}
u32 get_thread_id(Thread *thread) {
	return GetThreadId((HANDLE)thread->handle);
}

#else // ^^^ OS_WINDOWS ^^^ vvvvvv
#endif
#endif // TL_IMPL

template <class T>
concept ASpinner = requires(T t) { t.spin(); };

struct BasicSpinner {
	inline void spin() {
		yield_smt();
		switch_thread();
	}
};

struct SleepySpinner {
	u32 threshold = 256;

	inline void spin() {
		yield_smt();
		switch_thread();
		if (threshold == 0) {
			sleep_milliseconds(1);
		} else {
			--threshold;
		}
	}
};

template <AInterlockExchangeable T, ASpinner Spinner = BasicSpinner>
forceinline T atomic_update(T volatile *a, Callable<T, T> auto fn, Spinner spinner = {}) {
	using Int = TypeAt<log2(sizeof(T)), u8, u16, u32, u64>;
	while (1) {
		auto old_value = *a;
		auto new_value = fn(old_value);
		if (atomic_compare_exchange((Int volatile *)a, *(Int *)&new_value, *(Int *)&old_value) == *(Int *)&old_value) {
			return old_value;
		}
		spinner.spin();
	}
}

template <ASpinner Spinner = SleepySpinner>
void loop_while(std::predicate<> auto &&predicate, Spinner spinner = {}) {
	while (predicate()) {
		spinner.spin();
	}
}
template <ASpinner Spinner = SleepySpinner>
void loop_until(std::predicate<> auto &&predicate, Spinner spinner = {}) {
	return loop_while([&]{return!predicate();}, spinner);
}

struct SyncPoint {
	u32 target_thread_count = 0;
	u32 volatile synced_thread_count = 0;
};

inline SyncPoint create_sync_point(u32 thread_count) {
	return {
		.target_thread_count = thread_count,
		.synced_thread_count = 0,
	};
}

template <ASpinner Spinner = SleepySpinner>
inline void sync(SyncPoint &point, Spinner spinner = {}) {
	atomic_add(&point.synced_thread_count, 1);
	loop_until([&] { return point.synced_thread_count == point.target_thread_count; }, spinner);
}

struct SpinLock {
	TL_MAKE_FIXED(SpinLock);

	u32 volatile locker_id = 0;

	SpinLock() = default;
};

inline bool try_lock(SpinLock &m) {
	auto current = get_current_thread_id();
	return !atomic_compare_exchange(&m.locker_id, current, (u32)0);
}
template <ASpinner Spinner = SleepySpinner>
inline void lock(SpinLock &m, Spinner spinner = {}) {
	auto current = get_current_thread_id();
	assert(m.locker_id != current, "single thread deadlock");
	while (!atomic_replace(&m.locker_id, current, (u32)0)) {
		spinner.spin();
	}
}
inline void unlock(SpinLock &m) {
	atomic_set(&m.locker_id, (u32)0);
}

template <>
struct Scoped<SpinLock> {
	SpinLock *mutex;
	void enter(SpinLock &mutex) {
		this->mutex = &mutex;
		lock(mutex);
	}
	void exit() {
		unlock(*mutex);
	}
};

// TODO: why indirection?
struct TL_API OsLock {
	TL_MAKE_FIXED(OsLock);

	void *handle;

	OsLock();
};

TL_API bool try_lock(OsLock &m);
TL_API void lock(OsLock &m);
TL_API void unlock(OsLock &m);

#ifdef TL_IMPL
#if OS_WINDOWS
OsLock::OsLock() {
	handle = DefaultAllocator{}.allocate<CRITICAL_SECTION>();
	InitializeCriticalSection((CRITICAL_SECTION *)handle);
}
bool try_lock(OsLock &m) {
	return TryEnterCriticalSection((CRITICAL_SECTION *)m.handle);
}
void lock(OsLock &m) {
	EnterCriticalSection((CRITICAL_SECTION *)m.handle);
}
void unlock(OsLock &m) {
	LeaveCriticalSection((CRITICAL_SECTION *)m.handle);
}
#elif OS_LINUX
OsLock::OsLock() {
	handle = DefaultAllocator{}.allocate<pthread_mutex_t>();
	pthread_mutex_init((pthread_mutex_t*)handle, 0);
}
bool try_lock(OsLock &m) {
	return pthread_mutex_trylock((pthread_mutex_t *)m.handle) == 0;
}
void lock(OsLock &m) {
	pthread_mutex_lock((pthread_mutex_t *)m.handle);
}
void unlock(OsLock &m) {
	pthread_mutex_unlock((pthread_mutex_t *)m.handle);
}
#endif
#endif


template <>
struct Scoped<OsLock> {
	OsLock *mutex;
	void enter(OsLock &mutex) {
		this->mutex = &mutex;
		lock(mutex);
	}
	void exit() {
		unlock(*mutex);
	}
};

template <class T>
concept ALock = requires (T t) {
	lock(t); 
	{ try_lock(t) } -> std::same_as<bool>;
	unlock(t); 
};

// There are two ways to use LockProtected.
// 1) Using a lambda.
//    protected.use([&](Item &item) {
//        item.modify();
//    });
//    You call `use` with lambda, it calls lock, invokes the lambda, calls unlock.
//    Problem with this is unability to return values from mid lambda.
//    For that I implemented a second way.
// 2) Foreach.
//    for (Item &item : protected) {
//        item.modify();
//        return item.value; // can return here, lock is unlocked automatically.
//    }
//    With this way you can return from the body. But it looks confusing at first glance.
//    There is `use_locked` macro that is more understandable.
//
// locked_use macros reduce some typing. See below.
//
// https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxCCSAJykAA6oCoRODB7evnrJqY4CQSHhLFEx8baY9vkMQgRMxASZPn5cFVXptfUEhWGR0bEJCnUNTdmtQ109xaUDAJS2qF7EyOwc5gDMwcjeWADUJutuQ/ioAHQIB9gmGgCC1zcAbqh46Lu0qMgA1hCz%2BwDsFl2iWIwQIVAg5jM7y%2BJgArG4GJDZgdASY/gARe5PF67LwMaHfX5owHA0HgyF4glwhFIlH/TG3e5DYheBy7AIfT7KYioAiYByYV7E%2B67UW7Zmsgi7ACSfOITCIxH%2BVluYrV7M53N5/L5rwAVIl9ut0bsNCiRerRRFUJ5xQhFrR0AB9Smco0mqhiJTm1WW3ag3aoRLRBUkPU/ZUWv1igk/H03aNqgD0Sd2ACVMAQlgxdsRMFRoowVrsiECeXyBa9UBFtDrdghC1HE3ms8Qc5IzPHo2iGQm/dbbUGQ4qwGADuiILLhyRdmgGEN9mYAGwEBAK35zhfC32J3OZ7NA3aj8cltcEU6JLt%2BntNtXYqvB%2BWK6zWCPbvu7w0ns3rFUf9U3ju6oAH5Tk%2BJBvgCt5%2BngVC7BACj2l4jouvinJElBQG7qKrpfHGv7QZagH/mqxGkRi8b3NccRgaGSpRMAwSQYCLYHsSq54AopAliymA3r%2B9JURoNFynRuyuMxe6tjmwoUQJxFUXJf73AGLBMExGF/mqHJfFqFa6m8lFAaSgjkmYZhRPwebUoi5nIgRQFWfBTBeKWqi7CAbyaYRZZkhC5nWugACei6wugNmQtxqj2VpYpkWKJlgv5ZgsC86D0BFdlXqKTkQC5bkeV5kZYQlIKmclgUheYYWZWYUUxT5KbifUtAhaxbY%2Be1OZcNlgnGWVSWQkwVByrVDVAV1pqURRtwcPMtCcLCvB%2BBwWikKgnBuC%2BljiosyyYIu6w8KQBCaHN8yfCAZiwqcAAcsK3ZI6ySH8ZhHRofzxOs%2BicJIy1netnC8AoIAaCdZ3zHAsAwIgICLAQiSueQlBoCwiR0NEoSsKsqhLpIuwsAoDzILsDxcHEpwGHyQy8IKhAkC8ej8IIIhiOwUgyIIigqOoq06HoADu8qJJwPDzYt/184DHAAPKuYjUqoHBqi3UuAC0eO7MAyAk1wN1cPBHhoxjSobFwsy8KdfOzPMDZMFgMQ/KQl3rGYpzk2YXC3Rot1/HEcS3WYGgaNIC0cH9pAsCArvu2Yt3%2Bxoz3x29sLSCta0bRwwOg%2BD1ukFDsPwwryMQHTiqM60zPCKI4gc1X3NqADuh1ULTAi9wc0/RwS2kOnvCZ3LCOuYGcF4lgNAhK8EBG%2Bj9Cm29ZgW7nWg26QdsO5Q4vh7wUfPacsKfX7D0dkuXCu60ffS9nYNWyv%2BcwxASBF0jFAQKjs%2BY9jnC4/jhPE6T5NThPV2AANSEFwP47t1i03wOXdATNZCs1rtIeuShG5S10N9Vu7cxZdx7pfAe8th5K12D/AmRMSZkwpsAsBECoGG1QMbOeh11hL1vudNemB7b9CdmHCOu91inDMHEJcnt1je0ekuV2sJe4A0ztfZeHDLpnzum9YO6wlxxFhJor2dUw7QNkVLeRijIYPzMfAR%2BKBGEfzIK/d%2BJsQDAD1pXOgcoQYQAiADCIwR6hBVFrwbxzBiBBRljWHU/jSCozYIIGW%2BI/FSywBELwwA3BiFoCDDupAsBqSMOIBJeA8wODwA8TAGS1qYFUPyVyqw1qgkqADWgeAIjymCR4LAAMCAgijpkkpxBrRKHRJgHJwBGlGAhnwAwwAFAgLwJgAWMtgwrWOlXJB7MUGyAbrzNauhWgGDGaYSw1h9BNJBpAeYQZqgZLVjLdYuw1bHHHAcqwlgpB3IAOppPeRUzpTBdj916SCLApynZ2B1OkFwDB3CeGaP4SFUw%2BgxFaLkNIAhRgtCSCkFFDB4UlH6OMSoYKBCdBGNCsYbRCU1GGN0YIvRcWItsFStFegJgNBxTMc2CwlgrAkFvfBcjOCkNVhrfG2tdb63grgem88jpsIhhdEAsIwZ8J3tHN26xYTOIPs9SQ908aGIzkDWwOd2GmIsUgexc8S4Wv6NsQwwAg5cDBjQWgbjKCeKloE3xETPXBNCbWBwESomMAILE1qANEnJNSbQdJETsl2ryWtfAhTHAlLKbTSpyBqkAzqWHNajTmm%2BLaTUy2XSIm9P6ZgQZwzRmgDzlQSZ0zZnzMWRElZNc1mc3kGgrZ/M9F2pQNtGw%2BbgXnMSJczg1zbn3IIOFY0TzrCvLVh86NXzVA/P%2BdEQFpT4DzFBUU5wpdIVMtaIEGl0w8UYryOkY9l6sVsovXu6oxLGikvRY%2BjoVL730pZS%2BrIb7P1noRTyzl%2B1gNhz5UYgVKt1aa1tUYXYQd3anA0BK2BM4zayutrbLhG9eG/RVXvA%2B8RfZLg0KIv4XAlz6v7oakGN9xkF0sda2xKNrEOOJokRIToqFOiek6WhfwnRnydD/PgrjojuPdWtH18Tjoyb9eEzJQaYlxPDZgJJKS0kZOOnG3JxaskFLBamgGFSql8mzYIepUt80tKCkWjppaenRArVW%2BNwRa133rUwKZMy5kLMYK2xB7aJDrK5t2pu0d9D9vnZYY5EQR3rTHekDJSYZZmF2EmY4A7DkvPxkmN5EdUAApeNus55L91%2BEPVCv9sL0BfpyJi6oN7kXVHq/i9oRLGWvuZQSirz62sMq6Den9bXd17W5Ry8DksDUcFIZrP%2BlDAE0PAZAs%2BqGpWHXNpbOVnDuGOy3vw6OgiPrPREUuWEcQzAdj%2BHqghtHjU7Zdm7VWt11XmTeuTYS319HTZo1nEx98LHmNgE/IhBAS5lwZvAyuQW2Yhc7ZsiL0hsH%2BN5b96Wg8FYj0FTBkVOtdh63dgwph0RDqSEwyvbDe3N7OwVUq/DkdVXIYgRo265NJBfVe2nfl/26MA8Y6Doe4PX6Q8BQglmwW64bPCxghVpAUcdzR9RjHYPsfQeFbOPZwAEMaCQyh6ebHmEbFhBTjh68eEHYI5IWOIjrpcDev7SjCQ7u84e3nAXcMwcQ7Q2LmHEu4dS7CzzCLVGFe4Km8rwhQu1fzYoQA6htyBP0INyT6VS5Ter3N/truh31iCPuusYOS4lxs41Rzv4kf7v0aw7TxVXcDEu%2B2%2B74HsNmNWsN/0JxGqxMuok26rxPjgnesHyEsJAalOMOiSG1TCT1ORq07GoZ8b9NJqM9uqWpnM3maljmhpTTbP2alp0vA3TjrlpSJWpfuT3PjK8z5pt/mlm8DbQH0LXbg%2By9DtFwdcWEsXOSxOg8nOoOousurQKuj8n8utMVkCjuuVtUBCtVjCienCoBnSg1leqit1kio1ukANu%2Bp1kNtgfAR%2BpMGgeyoNiSjVuMABkUEBhygoONuzEri7jjhrqKgTuKhAJKoqJthnlTrhvKnXsqoznnpXq7tXnfB7m3nYh3jEHBvahoI6j3q6h4gPkErJgEiPgpuPsdMptPmGrPhplGjGpkrpiMivoZkUsZhvhmlmjvpZrmrwDZoWhgPpsfqfrwOfgMlfiMjfnWg2r5s2gFpki/sggjjLtsnLlrtls8kOicnAf/gIFckAeiDFhYKAZ8kut8vKBusQFuglgQZVq4DeqenQegTgZgRkMQS1ngeQQ%2Br1k%2Bl1tQSQYQayvUd%2Bs0cgZQdSuUeymNlyswXgujpnGwbBlrjrnrutrwRhk3pTrtoITngRjdHEDqoqsRnnrdL7BfDzgoiaoDiDlYqnu3qniABxlxjxnxgJkJusCJnqs6qoVJloRocPhoToVbHoZPsGqGvEomnPpptGtprwOYQmsCVYSmuvuUnYdvrUo4XvgWq0m4Q5ifmWs5hfq5tfmMgEd5o2n5i2qEbDuEagh/lEVRjEekb/okUlskZwJljOrEQunlgVnkQUXAUUYgaUagX0RerUVgS0bydih0T1h1pSkQS0UUf1kKTQWKd0SNlKQMaBpNhLOIXNr/HHlQkAonitsnjwehh9vwQsRbksaIVwMhmXlokocXlwPblouIXsY9nTvXiMYaiavKkuKtkdHjH8N6SXsJGRl3GYCqo6mDI3iYm6brkdLqnrGzkHGRjdgGSqv7KcP7CmamWmXaWGQmRma6aQL0qkM4JIEAA%3D%3D
template <class T, ALock Lock>
struct LockProtected {
	TL_MAKE_FIXED(LockProtected);

	using Value = T;

	LockProtected() = default;
	LockProtected(T value) : unprotected(value) {}

	template <class Fn>
	decltype(auto) use(Fn fn) {
		scoped(_lock);
		return fn(unprotected);
	}

	template <class Fn>
	decltype(auto) operator->*(Fn fn) { return use(fn); }

	struct Iterator {
		LockProtected *p = 0;
		bool should_unlock = false;
		T &operator*() const {
			lock(_lock);
			return p->unprotected;
		}
		bool operator!=(Iterator const &that) const {
			return p != that.p;
		}
		void operator++() {
			p = 0;
		}
		~Iterator() {
			if (should_unlock) {
				unlock(_lock);
			}
		}
	};

	Iterator begin() { return {this}; }
	Iterator end() { return {}; }
	

	// private:
	Lock _lock;
	T unprotected;
};

/*
// Example usage:
locked_use_expr(queue, context->queue) {
	queue.push(42);
};
*/
#define locked_use_ret_expr(name, expr) (expr) ->* [&](typename std::remove_cvref_t<decltype(expr)>::Value &name)
#define locked_use_expr(name, expr) locked_use_ret_expr(name, expr) -> decltype(auto)

/*
// `name` must be a single identifier
// Example usage:
locked_use(queue) {
	queue.push(42);
};
*/
#define locked_use_ret(name) locked_use_ret_expr(name, name)
#define locked_use(name) locked_use_expr(name, name)

/*
// Example usage:
use_locked(auto &queue : context->queue) {
	queue.push(42);
};
*/
#define use_locked for

template <class T, ALock Lock, class Allocator = Allocator>
struct LockQueue : LockProtected<Queue<T, Allocator>, Lock> {
	void push(T const &value) {
		this->use([&](auto &queue) {
			queue.push(value);
		});
	}
	void push(Span<T> values) {
		this->use([&](auto &queue) {
			// FIXME: use reserve
			for (auto &value : values) {
				queue.push(value);
			}
		});
	}
	Optional<T> pop() {
		return this->use([&](auto &queue) {
			return queue.pop();
		});
	}
	umm count() {
		return this->unprotected.count;
	}
};

struct RecursiveSpinLock {
	TL_MAKE_FIXED(RecursiveSpinLock);

	u32 volatile thread_id = 0;
	u32 counter = 0;

	RecursiveSpinLock() = default;
};

inline bool try_lock(RecursiveSpinLock &m, u32 *locked_by = 0) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
		return true;
	} else {
		auto prev_id = atomic_compare_exchange(&m.thread_id, thread_id, (u32)0);
		if (prev_id == 0)
			return true;

		if (locked_by)
			*locked_by = prev_id;
		return false;
	}
}
template <ASpinner Spinner = SleepySpinner>
inline void lock(RecursiveSpinLock &m, Spinner spinner = {}) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
	} else {
		loop_while([&] {
			return atomic_compare_exchange(&m.thread_id, thread_id, (u32)0);
		}, spinner);
	}
}
inline void unlock(RecursiveSpinLock &m) {
	if (m.counter == 0) {
		m.thread_id = 0;
	} else {
		--m.counter;
	}
}

template <>
struct Scoped<RecursiveSpinLock> {
	RecursiveSpinLock *mutex;
	void enter(RecursiveSpinLock &mutex) {
		this->mutex = &mutex;
		lock(mutex);
	}
	void exit() {
		unlock(*mutex);
	}
};

struct TL_API ConditionVariable {
	TL_MAKE_FIXED(ConditionVariable);

	struct Sleeper {
		Sleeper(ConditionVariable *cv) : cv(cv) {}

		void sleep(u32 timeout_milliseconds = -1);

	private:
		ConditionVariable *cv;
	};

	ConditionVariable();

	void wake();
	void wake_all();
	void section(auto &&block) {
		lock();
		if constexpr (requires { block(); }) {
			block();
		} else {
			block(Sleeper(this));
		}
		unlock();
	}

private:
	void *handle;

	inline struct ConditionVariableImpl &impl() {
		return *(struct ConditionVariableImpl *)handle;
	}
	void lock();
	void unlock();
};

#ifdef TL_IMPL
#if OS_WINDOWS
struct ConditionVariableImpl {
	CONDITION_VARIABLE c = {};
	CRITICAL_SECTION s = {};
};

ConditionVariable::ConditionVariable() {
	handle = DefaultAllocator{}.allocate<ConditionVariableImpl>();
	InitializeConditionVariable(&impl().c);
	InitializeCriticalSection(&impl().s);
}

void ConditionVariable::Sleeper::sleep(u32 timeout_milliseconds) {
	SleepConditionVariableCS(&cv->impl().c, &cv->impl().s, timeout_milliseconds == -1 ? INFINITE : timeout_milliseconds);
}
void ConditionVariable::wake() {
	WakeConditionVariable(&impl().c);
}
void ConditionVariable::wake_all() {
	WakeAllConditionVariable(&impl().c);
}
void ConditionVariable::lock() {
	EnterCriticalSection(&impl().s);
}
void ConditionVariable::unlock() {
	LeaveCriticalSection(&impl().s);
}
#endif
#endif

template <class T, class Allocator = Allocator>
struct SignalQueue : private Queue<T, Allocator> {
	using Base = Queue<T, Allocator>;

	void push(T value) {
		cv.section([&]{
			Base::push(value);
		});
		cv.wake();
	}

	void push(Span<T> value) {
		cv.section([&]{
			Base::push(value);
		});
		cv.wake();
	}

	Optional<T> pop(auto cancelled, u32 timeout = -1) {
		Optional<T> result;

		cv.section([&] (ConditionVariable::Sleeper sleeper) {
			while (!cancelled()) {
				if (result = Base::pop()) {
					break;
				} else {
					sleeper.sleep(timeout);
				}
			}
		});

		return result;
	}
	T pop() {
		return pop([] { return false; }).value();
	}
private:
	ConditionVariable cv;
};

enum WaitForCompletionOption {
	just_wait,
	do_my_task,
	do_any_task,
};

// Simple thread pool.
// Intended usage cycle:
//     1. Push some amount of tasks, heavy first.
//     2. Wait for them to complete.
//
// Only one thread may push tasks.

// :NestedOptionsStruct:
// This could have been a nested struct of TaskQueueThreadPool, but gcc has a bug which does not allow that.
// Because of that I have to pull out default_worker_proc as well.
struct TaskQueueThreadPool;
inline void TaskQueueThreadPool_default_worker_proc(TaskQueueThreadPool *pool);
struct TaskQueueThreadPoolInitParams {
	void (*worker_initter)() = [](){};
	void (*worker_proc)(TaskQueueThreadPool *pool) = TaskQueueThreadPool_default_worker_proc;
};

struct TaskQueueThreadPool {
	struct Task {
		TaskQueueThreadPool *pool = 0;
		void (*fn)(void *param) = 0;
		void *param = 0;
		void (*free)(void *param) = 0;
	};

	inline bool init(u32 thread_count, TaskQueueThreadPoolInitParams params = {}) {
		start_sync_point = create_sync_point(thread_count + 1); // sync workers and main thread
		end_sync_point   = create_sync_point(thread_count + 1); // sync workers and main thread
		stopped_thread_count = 0;
		stopping = false;
		allocator =
			threads.allocator =
			tasks.allocator =
			TL_GET_CURRENT(allocator);
		if (thread_count) {
			threads.reserve(thread_count);

			struct StartInfo {
				TaskQueueThreadPool *pool;
				void (*init)();
				void (*proc)(TaskQueueThreadPool *pool);
			};
			StartInfo info;
			info.pool = this;
			info.init = params.worker_initter;
			info.proc = params.worker_proc;

			auto start_proc = [](Thread *thread) {
				StartInfo *info = (StartInfo *)thread->param;
				(*info->init)();
				(*info->proc)(info->pool);
			};

			for (u32 i = 0; i < thread_count; ++i) {
				auto thread = create_thread(+start_proc, (void *)&info);
				if (!valid(thread)) {
					stopping = true;
					for (auto t : threads) {
						free(t);
					}
					threads.clear();
					return false;
				}
				threads.add(thread);
			}

			sync(start_sync_point);

		} else {
			threads.clear();
		}

		return true;
	}
	inline void deinit() {
		assert(tasks.count == 0, "Wait for all the tasks to complete before calling TaskQueueThreadPool::deinit");
		stopping = true;
		sync(end_sync_point);
		threads.clear();
	}

	inline void add_task(void (*fn)(void *param), void *param) {
		atomic_increment(&started_task_count);

		add_task({this, fn, param, [](void *){}});
	}
	template <class Fn, class ...Args>
	void add_task(Fn &&fn, Args &&...args) {
		using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;

		// TODO: Figure out a way to use custom allocator without race conditions.
		//       Using DefaultAllocator for now.

		auto fnParams = DefaultAllocator{}.allocate_uninitialized<Tuple>();
		new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);

		constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
		constexpr auto freer = [](void *param) {
			auto tuple = (Tuple *)param;
			tuple->~Tuple(); 
			DefaultAllocator{}.free_t(tuple, 1);
		};

		atomic_increment(&started_task_count);

		add_task({this, invokerProc, fnParams, freer});
	}
	template <class Fn>
	auto &operator+=(Fn &&fn) {
		add_task(std::forward<Fn>(fn));
		return *this;
	}

	inline void wait_for_completion(WaitForCompletionOption option = WaitForCompletionOption::just_wait) {
		switch (option) {
			case WaitForCompletionOption::just_wait: {
				while (started_task_count > finished_task_count) {
					completion_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
						sleeper.sleep(1);
					});
				}
				break;
			}
			case WaitForCompletionOption::do_my_task:
			case WaitForCompletionOption::do_any_task: {
				while (started_task_count > finished_task_count) {
					Optional<Task> task;
					new_work_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
						task = tasks.pop();
						if (!task) {
							sleeper.sleep(1);
						}
					});
					if (task) {
						run(task.value_unchecked());
					}
				}
				break;
			}
		}
	}
	#if TL_DEBUG
	~TaskQueueThreadPool() {
		assert(started_task_count == finished_task_count, "TaskQueueThreadPool::wait_for_completion must be called before TaskQueueThreadPool's destructor is called");
		assert(stopping, "TaskQueueThreadPool was not properly deinitted. Call TaskQueueThreadPool::deinit.");
	}
	#endif

	Allocator allocator = TL_GET_CURRENT(allocator);
	List<Thread *> threads;

	SyncPoint start_sync_point;
	SyncPoint end_sync_point;
	u32 volatile stopped_thread_count = 0;
	bool volatile stopping = true;
	Queue<Task> tasks;
	
	u32 volatile started_task_count = 0;
	u32 volatile finished_task_count = 0;

	ConditionVariable new_work_notifier;
	ConditionVariable completion_notifier;
	
	#if TL_DEBUG
	u32 producer_id;
	#endif

	inline void add_task(Task task) {
		new_work_notifier.section([&]{
			#if TL_DEBUG
			if (tasks.count == 0) {
				producer_id = get_current_thread_id();
			} else {
				assert_equal(producer_id, get_current_thread_id());
			}
			#endif
			tasks.push(task);
		});

		new_work_notifier.wake();
	}
	inline void run(Task task) {
		assert(task.fn);
		task.fn(task.param);
		task.free(task.param);
		atomic_increment(&finished_task_count);
	}

};

inline void TaskQueueThreadPool_default_worker_proc(TaskQueueThreadPool *pool) {

	sync(pool->start_sync_point);

	while (1) {
		Optional<TaskQueueThreadPool::Task> task;
		pool->new_work_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
			while (!pool->stopping) {
				if (task = pool->tasks.pop()) {
					assert(task.value_unchecked().fn);
					break;
				} else {
					// NOTE: Arbitrary timeout
					sleeper.sleep(1);
				}
			}
		});

		if (task) {
			pool->run(task.value_unchecked());
			pool->completion_notifier.wake();
		} else {
			break;
		}
	}
	atomic_add(&pool->stopped_thread_count, 1);
	sync(pool->end_sync_point);
}
	
struct TaskListsThreadPool;
inline void TaskListsThreadPool_default_worker_proc(TaskListsThreadPool *pool);
	
// Not a nested struct cuz gcc is stupid
struct TaskListsThreadPoolInitParams {
	void (*worker_initter)() = [](){};
	void (*worker_proc)(TaskListsThreadPool *pool) = TaskListsThreadPool_default_worker_proc;
};

// Runs the latest pushed tasks 
struct TaskListsThreadPool {
	struct TaskList;

	struct Task {
		TaskList *list = 0;
		void (*fn)(void *param) = 0;
		void *param = 0;
		void (*free)(void *param) = 0;
	};

	struct TaskList {
	private:
		friend struct TaskListsThreadPool;

		TaskListsThreadPool *pool = 0;

		u32 volatile started_task_count = 0;
		u32 volatile finished_task_count = 0;

	public:
		TaskList() = default;
		TaskList(TaskListsThreadPool *pool) : pool(pool) {}
		TaskList(const TaskList &that) = delete;
		TaskList(TaskList &&that) = delete;
		TaskList &operator=(const TaskList &that) = delete;
		TaskList &operator=(TaskList &&that) {
			assert(!pool, "Assigning to already initialized task list is not allowed");
			memcpy(this, &that, sizeof(*this));
			return *this;
		}

		~TaskList() {
			assert(started_task_count == finished_task_count, "TaskList::wait_for_completion must be called before TaskList's destructor is called");
		}

		inline void add_task(void (*fn)(void *param), void *param) {
			atomic_increment(&started_task_count);

			pool->add_task({this, fn, param, [](void *){}});
		}
		template <class Fn, class ...Args>
		void add_task(Fn &&fn, Args &&...args) {
			using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;

			// TODO: Figure out a way to use custom allocator without race conditions.
			//       Using DefaultAllocator for now.

			auto fnParams = DefaultAllocator{}.allocate_uninitialized<Tuple>();
			new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);

			constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
			constexpr auto freer = [](void *param) {
				auto tuple = (Tuple *)param;
				tuple->~Tuple(); 
				DefaultAllocator{}.free_t(tuple, 1);
			};

			atomic_increment(&started_task_count);

			pool->add_task({this, invokerProc, fnParams, freer});
		}
		template <class Fn>
		auto &operator+=(Fn &&fn) {
			add_task(std::forward<Fn>(fn));
			return *this;
		}

		inline void wait_for_completion(WaitForCompletionOption option = WaitForCompletionOption::just_wait) {
			switch (option) {
				case WaitForCompletionOption::just_wait: {
					while (started_task_count > finished_task_count) {
						pool->completion_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
							sleeper.sleep(1);
						});
					}
					break;
				}
				case WaitForCompletionOption::do_my_task: {
					while (started_task_count > finished_task_count) {
						Optional<Task> maybe_task;
						pool->new_work_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
							maybe_task = pool->tasks.pop();
							if (maybe_task) {
								auto task = maybe_task.value_unchecked();
								if (task.list != this) {
									pool->add_task(task);
									maybe_task = {};
									sleeper.sleep(1);
								}
							} else {
								sleeper.sleep(1);
							}
						});
						if (maybe_task) {
							pool->run(maybe_task.value_unchecked());
						}
					}
					break;
				}
				case WaitForCompletionOption::do_any_task: {
					while (started_task_count > finished_task_count) {
						Optional<Task> task;
						pool->new_work_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
							task = pool->tasks.pop();
							if (!task) {
								sleeper.sleep(1);
							}
						});
						if (task) {
							pool->run(task.value_unchecked());
						}
					}
					break;
				}
			}
		}
		friend void TaskListsThreadPool_default_worker_proc(TaskListsThreadPool *pool);
	};

	inline bool init(u32 thread_count, TaskListsThreadPoolInitParams params = {}) {
		start_sync_point = create_sync_point(thread_count + 1); // sync workers and main thread
		end_sync_point   = create_sync_point(thread_count + 1); // sync workers and main thread
		stopped_thread_count = 0;
		stopping = false;
		allocator =
			threads.allocator =
			tasks.allocator =
			TL_GET_CURRENT(allocator);
		if (thread_count) {
			threads.reserve(thread_count);

			struct StartInfo {
				TaskListsThreadPool *pool;
				void (*init)();
				void (*proc)(TaskListsThreadPool *pool);
			};
			StartInfo info;
			info.pool = this;
			info.init = params.worker_initter;
			info.proc = params.worker_proc;

			auto start_proc = [](Thread *thread) {
				StartInfo *info = (StartInfo *)thread->param;
				(*info->init)();
				(*info->proc)(info->pool);
			};

			for (u32 i = 0; i < thread_count; ++i) {
				auto thread = create_thread(+start_proc, (void *)&info);
				if (!valid(thread)) {
					stopping = true;
					for (auto t : threads) {
						free(t);
					}
					threads.clear();
					return false;
				}
				threads.add(thread);
			}

			sync(start_sync_point);

		} else {
			threads.clear();
		}

		return true;
	}
	inline void deinit() {
		assert(tasks.count == 0, "Wait for all the tasks to complete before calling TaskListsThreadPool::deinit");
		stopping = true;
		sync(end_sync_point);
		threads.clear();
	}

	TaskList create_task_list() {
		return {this};
	}

	#if TL_DEBUG
	~TaskListsThreadPool() {
		assert(stopping, "TaskListsThreadPool was not properly deinitted. Call TaskListsThreadPool::deinit.");
	}
	#endif

private:
	Allocator allocator = TL_GET_CURRENT(allocator);
	List<Thread *> threads;

	SyncPoint start_sync_point;
	SyncPoint end_sync_point;
	u32 volatile stopped_thread_count = 0;
	bool volatile stopping = true;
	List<Task> tasks;

	ConditionVariable new_work_notifier;
	ConditionVariable completion_notifier;
	
	inline void add_task(Task task) {
		new_work_notifier.section([&]{
			tasks.add(task);
		});

		new_work_notifier.wake();
	}
	inline void run(Task task) {
		assert(task.fn);
		task.fn(task.param);
		task.free(task.param);
		atomic_increment(&task.list->finished_task_count);
	}

	friend void TaskListsThreadPool_default_worker_proc(TaskListsThreadPool *pool);
};

void TaskListsThreadPool_default_worker_proc(TaskListsThreadPool *pool) {

	sync(pool->start_sync_point);

	while (1) {
		Optional<TaskListsThreadPool::Task> task;
		pool->new_work_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
			while (!pool->stopping) {
				if (task = pool->tasks.pop()) {
					assert(task.value_unchecked().fn);
					break;
				} else {
					// NOTE: Arbitrary timeout
					sleeper.sleep(1);
				}
			}
		});

		if (task) {
			pool->run(task.value_unchecked());
			pool->completion_notifier.wake();
		} else {
			break;
		}
	}
	atomic_add(&pool->stopped_thread_count, 1);
	sync(pool->end_sync_point);
}

// Not nested because gcc is shit
struct TaskQueuesThreadPool;
inline void TaskQueuesThreadPool_default_worker_proc(TaskQueuesThreadPool *pool);
struct TaskQueuesThreadPoolInitParams {
	void (*worker_initter)() = [](){};
	void (*worker_proc)(TaskQueuesThreadPool *pool) = TaskQueuesThreadPool_default_worker_proc;
};

// Runs tasks in order of addition.
struct TaskQueuesThreadPool {
	struct TaskQueue;

	struct Task {
		TaskQueue *queue = 0;
		void (*fn)(void *param) = 0;
		void *param = 0;
		void (*free)(void *param) = 0;
	};

	struct TaskQueue {
	private:
		friend struct TaskQueuesThreadPool;

		TaskQueuesThreadPool *pool = 0;
		Queue<Task> tasks;

		u32 volatile started_task_count = 0;
		u32 volatile finished_task_count = 0;
		
		ConditionVariable completion_notifier;

		bool initted = false;
		bool freed = false;
		
		#if TL_DEBUG
		u32 producer_id = -1;
		#endif

	public:
		TaskQueue() = default;
		TaskQueue(TaskQueuesThreadPool &pool) {
			init(pool);
		}
		TaskQueue(const TaskQueue &that) = delete;
		TaskQueue(TaskQueue &&that) = delete;
		TaskQueue &operator=(const TaskQueue &that) = delete;
		TaskQueue &operator=(TaskQueue &&that) = delete;

		void init(TaskQueuesThreadPool &pool) {
			assert(!initted, "you must `wait_for_completion` before calling `init` a second time on a TaskQueue");
			initted = true;
			freed = false;
			this->pool = &pool;
			auto &shared = pool.shared;
			locked_use(shared) {
				//tasks = shared.unused_task_queues.pop().value_or({.allocator = pool.allocator});
				tasks = {};
				tasks.allocator = pool.allocator;
				shared.task_queues.add(this);
			};
			#if TL_DEBUG
			assert(producer_id == -1);
			producer_id = get_current_thread_id();
			#endif
		}

		~TaskQueue() {
			assert(!initted, "`wait_for_completion` must be called on a TaskQueue before destruction");
		}

		inline void push_task(void (*fn)(void *param), void *param) {
			push_task_and_notify({this, fn, param, [](void *){}});
		}
		template <class Fn, class ...Args>
		void push_task(Fn &&fn, Args &&...args) {
			using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;

			// TODO: Figure out a way to use custom allocator without race conditions.
			//       Using DefaultAllocator for now.

			auto fnParams = DefaultAllocator{}.allocate_uninitialized<Tuple>();
			new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);
			//print("allocating {}\n", fnParams);

			constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
			constexpr auto freer = [](void *param) {
				auto tuple = (Tuple *)param;
				tuple->~Tuple();
				//print("freeing {}\n", param);
				DefaultAllocator{}.free_t(tuple, 1);
			};

			push_task_and_notify({this, invokerProc, fnParams, freer});
		}
		template <class Fn>
		auto &operator+=(Fn &&fn) {
			push_task(std::forward<Fn>(fn));
			return *this;
		}

		inline void wait_for_completion(WaitForCompletionOption option = WaitForCompletionOption::just_wait) {
			switch (option) {
				case WaitForCompletionOption::do_my_task: // not implemented, fall through
				case WaitForCompletionOption::do_any_task: // not implemented, fall through
				case WaitForCompletionOption::just_wait: {
					while (started_task_count > finished_task_count) {
						completion_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
							sleeper.sleep(1);
						});
					}
					break;
				}
			}
			assert(initted);
			initted = false;
			assert(started_task_count == finished_task_count, "TaskQueue::wait_for_completion must be called before `~TaskQueue` or `free` is called");
			auto &shared = pool->shared;
			locked_use(shared) {
				bool erased = find_and_erase(shared.task_queues, this);
				assert(erased);
				assert_equal(tasks.count, 0);
				if (tasks.capacity) {
					//shared.unused_task_queues.add(tasks);
					tasks = {};
				}
			};
			#if TL_DEBUG
			producer_id = -1;
			#endif
		}
	private:
		inline void push_task_and_notify(Task task) {
			#if TL_DEBUG
			assert(producer_id != -1);
			assert_equal(producer_id, get_current_thread_id(), "Only one thread may operate on a TaskQueue");
			#endif
			atomic_increment(&started_task_count);
			auto &shared = pool->shared;
			locked_use(shared) {
				tasks.push(task);
			};
			pool->new_work_notifier.wake();
		}
		friend void TaskQueuesThreadPool_default_worker_proc(TaskQueuesThreadPool *pool);
	};

	inline bool init(u32 thread_count, TaskQueuesThreadPoolInitParams params = {}) {
		start_sync_point = create_sync_point(thread_count + 1); // sync workers and main thread
		end_sync_point   = create_sync_point(thread_count + 1); // sync workers and main thread
		stopped_thread_count = 0;
		stopping = false;
		allocator =
			threads.allocator =
			shared.unprotected.task_queues.allocator =
			//shared.unprotected.unused_task_queues.allocator =
			TL_GET_CURRENT(allocator);
		if (thread_count) {
			threads.reserve(thread_count);

			struct StartInfo {
				TaskQueuesThreadPool *pool;
				void (*init)();
				void (*proc)(TaskQueuesThreadPool *pool);
			};
			StartInfo info;
			info.pool = this;
			info.init = params.worker_initter;
			info.proc = params.worker_proc;

			auto start_proc = [](Thread *thread) {
				StartInfo *info = (StartInfo *)thread->param;
				(*info->init)();
				(*info->proc)(info->pool);
			};

			for (u32 i = 0; i < thread_count; ++i) {
				auto thread = create_thread(+start_proc, (void *)&info);
				if (!valid(thread)) {
					stopping = true;
					for (auto t : threads) {
						free(t);
					}
					threads.clear();
					return false;
				}
				threads.add(thread);
			}

			sync(start_sync_point);

		} else {
			threads.clear();
		}

		return true;
	}
	inline void deinit() {
		locked_use(shared) {
			assert(shared.task_queues.count == 0, "Wait for all the tasks to complete before calling TaskQueuesThreadPool::deinit");
		};
		stopping = true;
		sync(end_sync_point);
		threads.clear();
	}

	#if TL_DEBUG
	~TaskQueuesThreadPool() {
		assert(stopping, "TaskQueuesThreadPool was not properly deinitted. Call TaskQueuesThreadPool::deinit.");
	}
	#endif

private:
	Allocator allocator = TL_GET_CURRENT(allocator);
	List<Thread *> threads;

	SyncPoint start_sync_point;
	SyncPoint end_sync_point;
	u32 volatile stopped_thread_count = 0;
	bool volatile stopping = true;

	struct Shared {
		List<TaskQueue *> task_queues;
		//List<Queue<Task>> unused_task_queues;

		Shared() = default;
		Shared(Shared const &) = delete;
		Shared(Shared &&) = delete;
		Shared &operator=(Shared const &) = delete;
		Shared &operator=(Shared &&) = delete;
	};

	LockProtected<Shared, OsLock> shared;

	ConditionVariable new_work_notifier;
	
	inline void run(Task task) {
		//print("running {}\n", task.param);
		assert(task.fn);
		task.fn(task.param);
		task.free(task.param);
		atomic_increment(&task.queue->finished_task_count);
	}
	friend void TaskQueuesThreadPool_default_worker_proc(TaskQueuesThreadPool *pool);
};

void TaskQueuesThreadPool_default_worker_proc(TaskQueuesThreadPool *pool) {

	sync(pool->start_sync_point);
		
	auto &shared = pool->shared;

	while (1) {
		Optional<TaskQueuesThreadPool::Task> task;
		pool->new_work_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
			while (!pool->stopping) {
				locked_use(shared) {
					for (umm i = shared.task_queues.count - 1; i != -1; --i) {
						task = shared.task_queues[i]->tasks.pop();
						if (task) {
							assert(task.value_unchecked().fn != (void *)0xdddddddddddddddd);
							break;
						}
					}
				};
				if (task) {
					assert(task.value_unchecked().fn);
					break;
				} else {
					// NOTE: Arbitrary timeout
					sleeper.sleep(1);
				}
			}
		});

		if (task) {
			REDECLARE_VAL(task, task.value_unchecked());
			pool->run(task);
			task.queue->completion_notifier.wake();
		} else {
			break;
		}
	}
	atomic_add(&pool->stopped_thread_count, 1);
	sync(pool->end_sync_point);
}
} // namespace tl
#pragma warning(pop)

#ifdef TL_IMPL

namespace tl {

}

#endif