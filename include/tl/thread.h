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
forceinline void yield_smt() { YieldProcessor(); }

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
	else static_error_t(T, "lockSet is not available for this size");
	return *(T *)&result;
}

template <class T>
forceinline T atomic_compare_exchange(T volatile *dst, T new_value, T comparand) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedCompareExchange64((long long*)dst, *(long long*)&new_value, *(long long*)&comparand);
	else if constexpr (sizeof(T) == 4) result = _InterlockedCompareExchange  ((long     *)dst, *(long     *)&new_value, *(long     *)&comparand);
	else if constexpr (sizeof(T) == 2) result = _InterlockedCompareExchange16((short    *)dst, *(short    *)&new_value, *(short    *)&comparand);
	else if constexpr (sizeof(T) == 1) result = _InterlockedCompareExchange8 ((char     *)dst, *(char     *)&new_value, *(char     *)&comparand);
	else static_error_t(T, "atomic_compare_exchange is not available for this size");
	return *(T *)&result;
}

template <class T>
forceinline bool atomic_replace(T volatile *dst, T new_value, T condition) {
	return atomic_compare_exchange(dst, new_value, condition) == condition;
}

struct Thread {
	Allocator allocator;
	void (*function)(Thread *self) = 0;
	void *param = 0;
	void *handle = 0;
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
	auto allocator = current_allocator;
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

#else
forceinline s8  atomic_add(s8  volatile *a, s8  b) { return __sync_fetch_and_add(a, b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return __sync_fetch_and_add(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return __sync_fetch_and_add(a, b); }
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return __sync_fetch_and_add(a, b); }

template <class T>
forceinline T atomic_compare_exchange(T volatile *dst, T new_value, T comparand) {
	return __sync_val_compare_and_swap(dst, comparand, new_value);
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

template <class Predicate>
void loop_while(Predicate &&predicate) {
	u32 try_count = 0;
	while (predicate()) {
		for (u32 i = 0; i < 16; ++i) {
			yield_smt();
			switch_thread();
		}
		++try_count;
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

struct SpinLock {
	bool volatile in_use = false;

	SpinLock() = default;
	SpinLock(const SpinLock &) = delete;
	SpinLock(SpinLock &&) = delete;
	SpinLock &operator=(const SpinLock &) = delete;
	SpinLock &operator=(SpinLock &&) = delete;
};

inline bool try_lock(SpinLock &m) {
	return !atomic_compare_exchange(m.in_use, true, false);
}
inline void lock(SpinLock &m) {
	loop_until([&] {
		return try_lock(m);
	});
}
inline void unlock(SpinLock &m) {
	m.in_use = false;
}
inline void wait_for_unlock(SpinLock &m) {
	loop_until([&] {
		return !m.in_use;
	});
}

template <>
struct Scoped<SpinLock> {
	SpinLock &mutex;
	Scoped(SpinLock &mutex) : mutex(mutex) {
		lock(mutex);
	}
	~Scoped() {
		unlock(mutex);
	}
};

template <class T, class Lock>
struct LockProtected {
	using Value = T;

	LockProtected() = default;
	LockProtected(T value) : _value(value) {
	}

	template <class Fn>
	decltype(auto) use(Fn fn) {
		scoped(_lock);
		return fn(_value);
	}

	template <class Fn>
	decltype(auto) operator * (Fn fn) { return use(fn); }

	T &use_unprotected() { return _value; }

	Lock _lock;
	T _value;
};

// `decltype(name)::Value` instead of `auto` breaks visual studio...
#define locked_use(name) name * [&] (auto &name)

template <class T, class Lock>
struct LockQueue : LockProtected<Queue<T>, Lock> {
	void push(T const &value) {
		this->use([&](Queue<T> &queue) {
			queue.push(value);
		});
	}
	Optional<T> try_pop() {
		return this->use([&](Queue<T> &queue) {
			return queue.pop();
		});
	}
};

struct RecursiveSpinLock {
	u32 volatile thread_id = 0;
	u32 counter = 0;

	RecursiveSpinLock() = default;
	RecursiveSpinLock(const RecursiveSpinLock &) = delete;
	RecursiveSpinLock(RecursiveSpinLock &&) = delete;
	RecursiveSpinLock &operator=(const RecursiveSpinLock &) = delete;
	RecursiveSpinLock &operator=(RecursiveSpinLock &&) = delete;
};

inline bool try_lock(RecursiveSpinLock &m, u32 *locked_by = 0) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
		return true;
	} else {
		auto prev_id = atomic_compare_exchange(m.thread_id, thread_id, (u32)0);
		if (prev_id == 0)
			return true;

		if (locked_by)
			*locked_by = prev_id;
		return false;
	}
}
inline void lock(RecursiveSpinLock &m) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
	} else {
		loop_while([&] {
			return atomic_compare_exchange(m.thread_id, thread_id, (u32)0);
		});
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
	Scoped(RecursiveSpinLock &mutex) : mutex(&mutex) {
		lock(mutex);
	}
	~Scoped() {
		unlock(*mutex);
	}
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
	LockQueue<ThreadWork, SpinLock> all_work;

	u32 volatile started_work_count = 0;
	u32 volatile finished_work_count = 0;
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
			atomic_add(&work_to_do, 1);
			ThreadWork work;
			work.fn = fn;
			work.param = param;
			work.queue = this;
			pool->all_work.push(work);
		} else {
			fn(param);
			atomic_increment(&pool->finished_work_count);
		}
	}
	template <class Fn, class ...Args>
	void push(Fn &&fn, Args &&...args) {
		if (pool->thread_count) {
			using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
			auto fnParams = pool->allocator.allocate_uninitialized<Tuple>();
			new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);
			constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
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
	pool.all_work.use_unprotected().allocator =
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

		auto start_proc = [](Thread *thread) {
			StartParams *info = (StartParams *)thread->param;
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
