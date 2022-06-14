#pragma once
#include "function.h"
#include "optional.h"
#include "list.h"

#pragma warning(push)
#pragma warning(disable: TL_DISABLED_WARNINGS)
#pragma warning(disable: 4582)
#pragma warning(disable: 4583)
#pragma warning(disable: 4625)
#pragma warning(disable: 4820)
#pragma warning(disable: 5220)

namespace tl {

TL_API void sleep_milliseconds(u32 milliseconds);
TL_API void sleep_seconds(u32 seconds);
#if OS_WINDOWS
TL_API void switch_thread();
forceinline void spin_iteration() { _mm_pause(); }

forceinline s8  atomic_add(s8  volatile *a, s8  b) { return _InterlockedExchangeAdd8((char *)a, (char)b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return _InterlockedExchangeAdd16(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return (s32)_InterlockedExchangeAdd((long *)a, (long)b); }
#if ARCH_X64
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return _InterlockedExchangeAdd64(a, b); }
#endif

forceinline u16 atomic_increment(u16 volatile *a) { return (u16)_InterlockedIncrement16((short *)a); }
forceinline u32 atomic_increment(u32 volatile *a) { return (u32)_InterlockedIncrement((long *)a); }
#if ARCH_X64
forceinline u64 atomic_increment(u64 volatile *a) { return (u64)_InterlockedIncrement64((long long *)a); }
#endif

forceinline s16 atomic_increment(s16 volatile *a) { return (s16)atomic_increment((u16 *)a); }
forceinline s32 atomic_increment(s32 volatile *a) { return (s32)atomic_increment((u32 *)a); }
#if ARCH_X64
forceinline s64 atomic_increment(s64 volatile *a) { return (s64)atomic_increment((u64 *)a); }
#endif

forceinline u16 atomic_decrement(u16 volatile *a) { return (u16)_InterlockedDecrement16((short *)a); }
forceinline u32 atomic_decrement(u32 volatile *a) { return (u32)_InterlockedDecrement((long *)a); }
#if ARCH_X64
forceinline u64 atomic_decrement(u64 volatile *a) { return (u64)_InterlockedDecrement64((long long *)a); }
#endif

forceinline s16 atomic_decrement(s16 volatile *a) { return (s16)atomic_decrement((u16 *)a); }
forceinline s32 atomic_decrement(s32 volatile *a) { return (s32)atomic_decrement((u32 *)a); }
#if ARCH_X64
forceinline s64 atomic_decrement(s64 volatile *a) { return (s64)atomic_decrement((u64 *)a); }
#endif

template <class T>
forceinline T atomic_set(T volatile *dst, T src) {
	s64 result;
	     if constexpr (sizeof(T) == 1) result = _InterlockedExchange8 ((char     *)dst, *(char     *)&src);
	else if constexpr (sizeof(T) == 2) result = _InterlockedExchange16((short    *)dst, *(short    *)&src);
	else if constexpr (sizeof(T) == 4) result = _InterlockedExchange  ((long     *)dst, *(long     *)&src);
	else if constexpr (sizeof(T) == 8) result = _InterlockedExchange64((long long*)dst, *(long long*)&src);
	else static_assert(false, "lockSet is not available for this size");
	return *(T *)&result;
}
template <class T>
forceinline T atomic_set(T volatile &dst, T src) { return atomic_set(&dst, src); }

template <class T>
forceinline T atomic_set_if_equals(T volatile &dst, T newValue, T comparand) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedCompareExchange64((long long*)&dst, *(long long*)&newValue, *(long long*)&comparand);
	else if constexpr (sizeof(T) == 4) result = _InterlockedCompareExchange  ((long     *)&dst, *(long     *)&newValue, *(long     *)&comparand);
	else if constexpr (sizeof(T) == 2) result = _InterlockedCompareExchange16((short    *)&dst, *(short    *)&newValue, *(short    *)&comparand);
	else if constexpr (sizeof(T) == 1) result = _InterlockedCompareExchange8 ((char     *)&dst, *(char     *)&newValue, *(char     *)&comparand);
	else static_assert(false, "atomic_set_if_equals is not available for this size");
	return *(T *)&result;
}

struct Thread {
	Allocator allocator;
	void (*function)(Thread *self) = 0;
	void *param = 0;
	void *handle = 0;
};

template <class Ret>
struct ThreadRet : Thread {
	void *returning_function = 0;

	ThreadRet() {}
	~ThreadRet() {}
	union {
		Ret return_value;
	};
};

template <>
struct ThreadRet<void> : Thread {
	void *returning_function = 0;

	ThreadRet() {}
	~ThreadRet() {}
};


TL_API void run_thread(Thread *thread);
inline Thread *create_thread(void (*function)(Thread *self), void *param) {
	auto allocator = current_allocator;
	auto thread = allocator.allocate<Thread>();
	thread->allocator = allocator;
	thread->function = function;
	thread->param = param;
	run_thread(thread);
	return thread;
}

template <class ...Types>
struct Tuple;

template <>
struct Tuple<> {
};

template <class First, class ...Rest>
struct Tuple<First, Rest...> {
	First first;
	Tuple<Rest...> rest;
};

template <umm index, class ...Args>
auto get(Tuple<Args...> tuple) {
	if constexpr (index == 0) {
		return tuple.first;
	} else {
		return get<index - 1>(tuple.rest);
	}
}
template <class Ret>
inline ThreadRet<Ret> *create_thread(Ret (*function)()) {
	using Fn = Ret (*)();

	auto allocator = current_allocator;
	auto thread = allocator.allocate<ThreadRet<Ret>>();

	thread->allocator = allocator;
	thread->returning_function = function;
	thread->param = 0;
	thread->function = [](Thread *thread) {
		if constexpr (is_same<Ret, void>) {
			((Fn)((ThreadRet<Ret> *)thread)->returning_function)();
		} else {
			thread->return_value = ((Fn)thread->returning_function)();
		}
	};
	run_thread(thread);
	return thread;
}

namespace td {
template <class Fn, class Tuple, umm... indices>
inline static decltype(auto) invoke(Fn &&fn, Tuple &tuple, std::index_sequence<indices...>) noexcept {
	return fn(get<indices>(tuple)...);
}
}

template <class Ret, class First, class ...Rest>
inline ThreadRet<Ret> *create_thread(Ret (*function)(First first, Rest ...rest), First first, Rest ...rest) {
	using Fn = Ret (*)(First, Rest...);
	using ArgTuple = Tuple<First, Rest...>;

	auto allocator = current_allocator;
	auto thread = allocator.allocate<ThreadRet<Ret>>();

	auto arg_tuple = allocator.allocate<ArgTuple>();
	*arg_tuple = {first, rest...};

	thread->allocator = allocator;
	thread->returning_function = function;
	thread->param = arg_tuple;
	thread->function = [](Thread *_thread) {
		auto thread = (ThreadRet<Ret> *)_thread;
		if constexpr (is_same<Ret, void>) {
			td::invoke((Fn)thread->returning_function, *(ArgTuple *)thread->param, std::make_index_sequence<sizeof...(rest) + 1>{});
		} else {
			thread->return_value = td::invoke((Fn)thread->returning_function, *(ArgTuple *)thread->param, std::make_index_sequence<sizeof...(rest) + 1>{});
		}
	};
	run_thread(thread);
	return thread;
}

//template <class Fn, class ...Args>
//Thread create_thread(Fn &&fn, Args &&...args) {
//	return create_thread(create_function(fn, args...));
//}

TL_API void join(Thread *thread);
template <class Ret>
inline Ret join(ThreadRet<Ret> *thread) {
	join((Thread *)thread);
	return thread->return_value;
}

TL_API bool finished(Thread *thread);
TL_API void free(Thread *thread);

inline bool valid(Thread *thread) { return thread->handle != 0; }

TL_API u32 get_current_thread_id();
TL_API u32 get_thread_id(Thread *thread);

inline void join_and_free(Thread *thread) {
	join(thread);
	free(thread);
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
#if ARCH_X64
forceinline u64 atomic_add(u64 volatile *a, u64 b) { return (u64)atomic_add((s64 *)a, (s64)b); }
#endif

#ifdef TL_IMPL

#if OS_WINDOWS

void sleep_milliseconds(u32 milliseconds) { Sleep(milliseconds); }
void sleep_seconds(u32 seconds) { Sleep(seconds * 1000); }
void switch_thread() { SwitchToThread(); }

void run_thread(Thread *thread) {
	thread->handle = CreateThread(0, 0, [](void *param) noexcept -> DWORD {
		auto thread = (Thread *)param;
		init_allocator();
		defer {deinit_allocator();};
		current_printer = standard_output_printer;
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

struct Spinner {
	u32 try_count = 0;
};
inline void iteration(Spinner &spinner) {
	spin_iteration();
	if (spinner.try_count >= 64)
		switch_thread();
	if (spinner.try_count >= 4096)
		sleep_milliseconds(1);
	++spinner.try_count;
}

template <class Predicate>
void loop_while(Predicate &&predicate) {
	Spinner spinner;
	while (predicate()) {
		iteration(spinner);
	}
}
template <class Predicate>
void loop_until(Predicate &&predicate) {
	return loop_while([&]{return!predicate();});
}

struct SyncPoint {
	u32 target_thread_count;
	u32 volatile synced_thread_count;
};

inline SyncPoint create_sync_point(u32 thread_count) {
	return {
		.target_thread_count = thread_count,
		.synced_thread_count = 0,
	};
}

inline void sync(SyncPoint &point) {
	atomic_add(&point.synced_thread_count, 1);
	loop_until([&] { return point.synced_thread_count == point.target_thread_count; });
}

struct Mutex {
	bool volatile in_use = false;
};

inline bool try_lock(Mutex &m) {
	return !atomic_set_if_equals(m.in_use, true, false);
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
		loop_while([&] {
			return atomic_set_if_equals(m.thread_id, thread_id, (u32)0);
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

template <class T, class Mutex = Mutex>
struct MutexQueue {
	Queue<T> base;
	Mutex mutex;

	void push_front_unordered_no_lock(T const &value) { base.push_front_unordered(value); }
	void push_front_unordered(T const &value) { scoped_lock(mutex); base.push_front_unordered(value); }
	void push_no_lock(T const &value) { base.push(value); }
	void push(T const &value) { scoped_lock(mutex); base.push(value); }

	Optional<T> try_pop() {
		Optional<T> result;
		scoped_lock(mutex);
		if (base.count) {
			result = base.front();
			base.pop();
		}
		return result;
	}
	template <class Fn>
	Optional<T> try_pop_and(Fn &&fn) {
		Optional<T> result;
		scoped_lock(mutex);
		if (base.count) {
			result = base.front();
			base.pop();
			fn(result.get());
		}
		return result;
	}
	T pop() {
		Optional<T> opt;
		loop_until([&] {
			opt = try_pop();
			return opt.has_value();
		});
		return opt.get();
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
	umm count() { return base.count; }

	void add_no_lock(T const &v)                 { base.add(v); }
	void add_no_lock(T &&v)                      { base.add(v); }
	void add_no_lock(Span<T const> v)            { base.add(v); }
	void add_no_lock(std::initializer_list<T> v) { base.add(v); }
	void add(T const &v)                 { scoped_lock(mutex); base.add(v); }
	void add(T &&v)                      { scoped_lock(mutex); base.add(v); }
	void add(Span<T const> v)            { scoped_lock(mutex); base.add(v); }
	void add(std::initializer_list<T> v) { scoped_lock(mutex); base.add(v); }
};

template <class T, class Mutex>
Span<T> as_span(MutexQueue<T, Mutex> &queue) { return as_span(queue.base); }

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
		return opt.get();
	}
	void clear() {
		scoped_lock(mutex);
		base.clear();
	}

private:
	StaticCircularQueue<T, capacity> base;
	Mutex mutex;
};

struct WorkQueue;

struct ThreadWork {
	WorkQueue *queue;
	void (*fn)(void *param);
	void *param;
};

struct ThreadPool {
	Allocator allocator = current_allocator;
	List<Thread *> threads;
	u32 thread_count = 0;
	u32 volatile initialized_thread_count = 0;
	u32 volatile dead_thread_count = 0;
	bool volatile running = false;
	bool volatile stopping = false;
	MutexQueue<ThreadWork> all_work;

	u32 started_work_count = 0;
	u32 finished_work_count = 0;
};
bool try_do_work(ThreadPool *pool);

struct WorkQueue {
	ThreadPool *pool = 0;
	u32 volatile work_to_do = 0;

#if TL_DEBUG
	bool debug_finished = true;
	~WorkQueue() {
		assert(debug_finished, "WorkQueue can not be destroyed until all it's work is finished.");
	}
#endif

	inline void push(void (*fn)(void *param), void *param) {
#if TL_DEBUG
		debug_finished = false;
#endif
		atomic_increment(&pool->started_work_count);
		if (pool->thread_count) {
			ThreadWork work;
			work.fn = fn;
			work.param = param;
			work.queue = this;
			pool->all_work.push(work);
		} else {
			fn(param);
		}
	}
	template <class Fn, class ...Args>
	void push(Fn &&fn, Args &&...args) {
		if (pool->thread_count) {
			using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
			auto fnParams = pool->allocator.allocate_uninitialized<Tuple>();
			new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);
			constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
			atomic_add(&work_to_do, 1);
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
#if TL_DEBUG
			debug_finished = true;
#endif
		}
	}

};

inline WorkQueue make_work_queue(ThreadPool &pool) {
	WorkQueue result = {};
	result.pool = &pool;
	return result;
}

inline void do_work(ThreadWork work) {
	assert(work.fn);
	assert(work.queue);
	assert(work.queue->pool);

	work.fn(work.param);
	work.queue->pool->allocator.free(work.param);
	atomic_increment(&work.queue->pool->finished_work_count);

	// NOTE: Because the queue may be immediately destroyed after work_to_do reaches 0,
	// this must happen after incrementing finished_work_count
	atomic_add(&work.queue->work_to_do, (u32)-1);
}
inline bool try_do_work(ThreadPool *pool) {
	if (auto popped = pool->all_work.try_pop()) {
		do_work(popped.value_unchecked());
		return true;
	}
	return false;
}
inline bool do_work(ThreadPool *pool) {
	ThreadWork work = {};
	loop_until([&] {
		if (pool->stopping) {
			return true;
		}
		if (auto popped = pool->all_work.try_pop()) {
			work = popped.value_unchecked();
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

inline void default_thread_pool_proc(ThreadPool *pool) {
	atomic_add(&pool->initialized_thread_count, 1);
	loop_until([&] { return pool->running || pool->stopping; });
	while (1) {
		if (!do_work(pool))
			break;
	}
	atomic_add(&pool->dead_thread_count, 1);
}
template <class ThreadProc = decltype(default_thread_pool_proc)>
bool init_thread_pool(ThreadPool &pool, u32 thread_count, ThreadProc &&thread_proc = default_thread_pool_proc) {
	pool.initialized_thread_count = 0;
	pool.dead_thread_count = 0;
	pool.running = false;
	pool.stopping = false;
	pool.thread_count = thread_count;
	pool.allocator =
	pool.threads.allocator =
	pool.all_work.base.allocator =
	current_allocator;
	if (thread_count) {
		pool.threads.reserve(thread_count);

		struct StartParams {
			ThreadPool *pool;
			ThreadProc *proc;
		};
		StartParams params;
		params.pool = &pool;
		params.proc = std::addressof(thread_proc);

		auto start_proc = [](void *param) {
			StartParams *info = (StartParams *)param;
			(*info->proc)(info->pool);
		};

		for (u32 i = 0; i < thread_count; ++i) {
			auto thread = create_thread(+start_proc, (void *)&params);
			if (!valid(thread)) {
				pool.stopping = true;
				for (auto t : pool.threads) {
					free(t);
				}
				pool.threads.clear();
				return false;
			}
			pool.threads.add(thread);
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
inline void deinit_thread_pool(ThreadPool *pool, bool waitForThreads = true) {
	pool->stopping = true;
	if (waitForThreads) {
		loop_until([&] { return pool->dead_thread_count == pool->thread_count; });
	}
	pool->threads.clear();
}

inline void wait_for_completion(ThreadPool &pool) {
	loop_while([&]{ return pool.started_work_count != pool.finished_work_count; });
}

} // namespace tl
#pragma warning(pop)
