#pragma once
#include "common.h"
#include "optional.h"
#include "list.h"

#pragma warning(push)
#pragma warning(disable : 4582)
#pragma warning(disable : 4583)
#pragma warning(disable : 4625)
#pragma warning(disable : 4820)
#pragma warning(disable : 5220)

namespace TL {

TL_API void sleep_milliseconds(u32 milliseconds);
TL_API void sleep_seconds(u32 seconds);
#if OS_WINDOWS
TL_API void switch_thread();
forceinline void spin_iteration() { _mm_pause(); }

forceinline s8  atomic_add(s8  volatile *a, s8  b) { return _InterlockedExchangeAdd8((char *)a, (char)b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return _InterlockedExchangeAdd16(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return (s32)_InterlockedExchangeAdd((long *)a, (long)b); }
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return _InterlockedExchangeAdd64(a, b); }

template <class T>
forceinline T atomic_set(T volatile *dst, T src) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedExchange64((long long*)dst, *(long long*)&src); 
	else if constexpr (sizeof(T) == 4) result = _InterlockedExchange  ((long     *)dst, *(long     *)&src);
	else if constexpr (sizeof(T) == 2) result = _InterlockedExchange16((short    *)dst, *(short    *)&src);
	else if constexpr (sizeof(T) == 1) result = _InterlockedExchange8 ((char     *)dst, *(char     *)&src);
	else static_assert(false, "lockSet is not available for this size");
	return *(T *)&result;
}
template <class T>
forceinline T atomic_set(T volatile &dst, T src) { return atomic_set(&dst, src); }

template <class T>
forceinline T atomic_set_if_equals(T volatile *dst, T newValue, T comparand) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedCompareExchange64((long long*)dst, *(long long*)&newValue, *(long long*)&comparand);
	else if constexpr (sizeof(T) == 4) result = _InterlockedCompareExchange  ((long     *)dst, *(long     *)&newValue, *(long     *)&comparand);
	else if constexpr (sizeof(T) == 2) result = _InterlockedCompareExchange16((short    *)dst, *(short    *)&newValue, *(short    *)&comparand);
	else if constexpr (sizeof(T) == 1) result = _InterlockedCompareExchange8 ((char     *)dst, *(char     *)&newValue, *(char     *)&comparand);
	else static_assert(false, "atomic_set_if_equals is not available for this size");
	return *(T *)&result;
}
template <class T>
forceinline T atomic_set_if_equals(T volatile &dst, T newValue, T comparand) {
	return atomic_set_if_equals(std::addressof(dst), newValue, comparand);
}

TL_DECLARE_HANDLE(Thread);
TL_API Thread create_thread(void (*fn)(void *), void *param);
TL_API void destroy_thread(Thread thread);
TL_API u32 get_current_thread_id();
TL_API u32 get_thread_id(Thread thread);
TL_API void join(Thread thread);

inline Thread create_thread(Function function) {
	return create_thread(function.function, function.param);
}

template <class Fn, class ...Args>
Thread create_thread(Fn &&fn, Args &&...args) {
	return create_thread(create_function(std::forward<Fn>(fn), std::forward<Args>(args)...));
}

#else
forceinline s8  atomic_add(s8  volatile *a, s8  b) { return __sync_fetch_and_add(a, b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return __sync_fetch_and_add(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return __sync_fetch_and_add(a, b); }
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return __sync_fetch_and_add(a, b); }

template <class T>
forceinline T atomic_set_if_equals(T volatile *dst, T newValue, T comparand) {
	return __sync_val_compare_and_swap(dst, comparand, newValue);
}
#endif

forceinline u8  atomic_add(u8  volatile *a, u8  b) { return (u8 )atomic_add((s8  *)a, (s8 )b); }
forceinline u16 atomic_add(u16 volatile *a, u16 b) { return (u16)atomic_add((s16 *)a, (s16)b); }
forceinline u32 atomic_add(u32 volatile *a, u32 b) { return (u32)atomic_add((s32 *)a, (s32)b); }
forceinline u64 atomic_add(u64 volatile *a, u64 b) { return (u64)atomic_add((s64 *)a, (s64)b); }

forceinline s16 atomic_add(s16 volatile &a, s16 b) { return atomic_add(&a, b); }
forceinline s32 atomic_add(s32 volatile &a, s32 b) { return atomic_add(&a, b); }
forceinline s64 atomic_add(s64 volatile &a, s64 b) { return atomic_add(&a, b); }
forceinline u16 atomic_add(u16 volatile &a, u16 b) { return atomic_add(&a, b); }
forceinline u32 atomic_add(u32 volatile &a, u32 b) { return atomic_add(&a, b); }
forceinline u64 atomic_add(u64 volatile &a, u64 b) { return atomic_add(&a, b); }

#ifdef TL_IMPL

#if OS_WINDOWS

void sleep_milliseconds(u32 milliseconds) { Sleep(milliseconds); }
void sleep_seconds(u32 seconds) { Sleep(seconds * 1000); }
void switch_thread() { SwitchToThread(); }

Thread create_thread(void (*fn)(void *), void *param) {
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
	return (Thread)result;
}
void destroy_thread(Thread thread) {
	CloseHandle((HANDLE)thread);
}
u32 get_current_thread_id() {
	return GetCurrentThreadId();
}
u32 get_thread_id(Thread thread) {
	return GetThreadId((HANDLE)thread);
}
void join(Thread thread) {
	WaitForSingleObjectEx((HANDLE)thread, INFINITE, false);
}

#else // ^^^ OS_WINDOWS ^^^ vvvvvv
#endif
#endif // TL_IMPL

template <class Predicate>
void loop_until(Predicate &&predicate) {
	u32 try_count = 0;
	while (!predicate()) {
		spin_iteration();
		if (try_count >= 64)
			switch_thread();
		if (try_count >= 4096)
			sleep_milliseconds(1);
		++try_count;
	}
}

struct Mutex {
	bool volatile in_use = false;
};

inline bool try_lock(Mutex &m) {
	return !atomic_set_if_equals(&m.in_use, true, false);
}
inline void lock(Mutex &m) {
	loop_until([&] {
		return try_lock(m);
	});
}
inline void unlock(Mutex &m) {
	m.in_use = false;
}
inline void wait_for_unlock(Mutex &m) {
	loop_until([&] {
		return !m.in_use;
	});
}

struct RecursiveMutex {
	u32 volatile thread_id = 0;
	u32 counter = 0;
};

inline bool try_lock(RecursiveMutex &m) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
		return true;
	} else {
		return !atomic_set_if_equals(m.thread_id, thread_id, (u32)0);
	}
}
inline void lock(RecursiveMutex &m) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
	} else {
		loop_until([&] {
			return !atomic_set_if_equals(m.thread_id, thread_id, (u32)0);
		});
	}
}
inline void unlock(RecursiveMutex &m) {
	if (m.counter == 0) {
		m.thread_id = 0;
	} else {
		--m.counter;
	}
}

#define scoped_lock(mutex) lock(mutex); defer { unlock(mutex); }
#define scoped_unlock(mutex) unlock(mutex); defer { lock(mutex); }

template <class T, class Mutex = Mutex, class Allocator = TL_DEFAULT_ALLOCATOR>
struct MutexQueue {
	Queue<T, Allocator> base;
	Mutex mutex;
	
	template <class ...Args> void push_front_unordered_no_lock(Args &&...args) { base.push_front_unordered(std::forward<Args>(args)...); }
	template <class ...Args> void push_front_unordered(Args &&...args) { scoped_lock(mutex); base.push_front_unordered(std::forward<Args>(args)...); }
	template <class ...Args> void push_no_lock(Args &&...args) { base.push(std::forward<Args>(args)...); }
	template <class ...Args> void push(Args &&...args) { scoped_lock(mutex); base.push(std::forward<Args>(args)...); }
	Optional<T> try_pop() {
		Optional<T> result;
		scoped_lock(mutex);
		if (base.size()) {
			result.emplace(std::move(base.front()));
			base.pop();
		}
		return result;
	}
	T pop() {
		Optional<T> opt;
		loop_until([&] {
			opt = try_pop();
			return opt.has_value();
		});
		return opt.value();
	}
	void clear() {
		scoped_lock(mutex);
		base.clear();
	}
	void erase_no_lock(T *ptr) {
		base.erase(ptr);
	}
	template <class Fn>
	void pop_all_nolock(Fn &&fn) {
		for (auto &value : base) {
			fn(value);
		}
		base.clear();
	}
	template <class Fn>
	void pop_all(Fn &&fn) {
		scoped_lock(mutex);
		pop_all_nolock(std::forward<Fn>(fn));
	}
	umm size() {
		scoped_lock(mutex);
		return base.size();
	}

	MutexQueue &operator+=(T const &v)                 { base += v; return *this; }
	MutexQueue &operator+=(T &&v)                      { base += v; return *this; }
	MutexQueue &operator+=(Span<T const> v)            { base += v; return *this; }
	MutexQueue &operator+=(std::initializer_list<T> v) { base += v; return *this; }
};

template <class T, class Mutex, class Allocator>
Span<T> as_span(MutexQueue<T, Mutex, Allocator> &queue) { return { queue.base.begin(), queue.base.end() }; }

template <class T, umm capacity, class Mutex = Mutex>
struct StaticMutexCircularQueue {
	void push(T &&value) {
		scoped_lock(mutex);
		base.push(std::move(value));
	}
	void push(T const &value) {
		scoped_lock(mutex);
		base.push(value);
	}
	Optional<T> try_pop() {
		Optional<T> result;
		scoped_lock(mutex);
		if (base.size()) {
			result.emplace(std::move(base.front()));
			base.pop();
		}
		return result;
	}
	T pop() {
		Optional<T> opt;
		loop_until([&] {
			opt = try_pop();
			return opt.has_value();
		});
		return opt.value();
	}
	void clear() {
		scoped_lock(mutex);
		base.clear();
	}

private:
	StaticCircularQueue<T, capacity> base;
	Mutex mutex;
};

template <class Allocator>
struct WorkQueue;

template <class Allocator>
struct ThreadWork {
	WorkQueue<Allocator> *queue;
	void (*fn)(void *param);
	void *param;
};

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Queue = MutexQueue<ThreadWork<Allocator>, Mutex, Allocator>>
struct ThreadPool {
	List<Thread, Allocator> threads;
	u32 thread_count = 0;
	u32 volatile initialized_thread_count = 0;
	u32 volatile dead_thread_count = 0;
	bool volatile running = false;
	bool volatile stopping = false;
	Queue all_work;
};
template <class Allocator>
bool try_do_work(ThreadPool<Allocator> *pool);

template <class Allocator>
struct WorkQueue {
	ThreadPool<Allocator> *pool = 0;
	u32 volatile work_to_do = 0;
	bool important = false;
	inline void push(void (*fn)(void *param), void *param) {
		if (pool->thread_count) {
			ThreadWork<Allocator> work;
			work.fn = fn;
			work.param = param;
			work.queue = this;
			if (important)
				pool->all_work.push_front_unordered(work);
			else
				pool->all_work.push(work);
		} else {
			fn(param);
		}
	}
	template <class Fn, class ...Args>
	void push(Fn &&fn, Args &&...args) {
		if (pool->thread_count) {
			using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
			auto fnParams = ALLOCATE_T(Allocator, Tuple, 1, 0);
			new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);
			constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
			atomic_add(work_to_do, 1);
			push((void (*)(void *))invokerProc, (void *)fnParams);
		} else {
			std::invoke(fn, std::forward<Args>(args)...);
		}
	}
	template <class Fn>
	WorkQueue &operator+=(Fn &&fn) {
		push(std::forward<Fn>(fn));
		return *this;
	}
	inline void wait_for_completion() {
		if (pool->thread_count) {
			loop_until([&] {
				try_do_work(pool);
				return work_to_do == 0;
			});
		}
	}

};

template <class Allocator>
inline WorkQueue<Allocator> make_work_queue(ThreadPool<Allocator> &pool, bool important = false) {
	WorkQueue<Allocator> result = {};
	result.pool = &pool;
	result.important = important;
	return result;
}

template <class Allocator>
inline void do_work(ThreadWork<Allocator> work) {
	work.fn(work.param);
	DEALLOCATE(Allocator, work.param);
	atomic_add(work.queue->work_to_do, (u32)-1);
}
template <class Allocator>
inline bool try_do_work(ThreadPool<Allocator> *pool) {
	if (auto popped = pool->all_work.try_pop()) {
		do_work(*popped);
		return true;
	}
	return false;
}
template <class Allocator>
inline bool do_work(ThreadPool<Allocator> *pool) {
	ThreadWork<Allocator> work = {};
	loop_until([&] {
		if (pool->stopping) {
			return true;
		}
		if (auto popped = pool->all_work.try_pop()) {
			work = *popped;
			return true;
		}
		return false;
	});
	if (work.fn) {
		do_work(work);
		return true;
	} else {
		return false;
	}
}

template <class Allocator>
inline void default_thread_pool_proc(ThreadPool<Allocator> *pool) {
	atomic_add(pool->initialized_thread_count, 1);
	loop_until([&] { return pool->running || pool->stopping; });
	while (1) {
		if (!do_work(pool))
			break;
	}
	atomic_add(pool->dead_thread_count, 1);
}
template <class Allocator, class ThreadProc = decltype(default_thread_pool_proc<Allocator>)>
bool init_thread_pool(ThreadPool<Allocator> &pool, u32 thread_count, ThreadProc &&threadProc = default_thread_pool_proc<Allocator>) {
	pool.initialized_thread_count = 0;
	pool.dead_thread_count = 0;
	pool.running = false;
	pool.stopping = false;
	pool.thread_count = thread_count;
	if (thread_count) {
		pool.threads.reserve(thread_count);
		
		struct StartParams {
			ThreadPool<Allocator> *pool;
			ThreadProc *proc;
		};
		StartParams params;
		params.pool = &pool;
		params.proc = std::addressof(threadProc);
		
		auto startProc = [](void *param) {
			StartParams *info = (StartParams *)param;
			(*info->proc)(info->pool);
		};

		for (u32 i = 0; i < thread_count; ++i) {
			auto thread = create_thread(startProc, &params);
			if (!thread) {
				pool.stopping = true;
				for (auto t : pool.threads) {
					destroy_thread(t);
				}
				pool.threads.clear();
				return false;
			}
			pool.threads.push_back(thread);
		}

		loop_until([&] {
			return pool.initialized_thread_count == pool.thread_count;
		});
		pool.running = true;
	} else {
		pool.threads.clear();
	}

	return true;
}
template <class Allocator>
inline void deinitThreadPool(ThreadPool<Allocator> *pool, bool waitForThreads = true) {
	pool->stopping = true;
	if (waitForThreads) {
		loop_until([&] { return pool->dead_thread_count == pool->thread_count; });
	}
	pool->threads.clear();
}

} // namespace TL
#pragma warning(pop)
