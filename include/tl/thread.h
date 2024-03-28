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
forceinline void yield_smt() { _mm_pause(); }

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

void init_logger_thread();

void run_thread(Thread *thread) {
	thread->handle = CreateThread(0, 0, [](void *param) noexcept -> DWORD {
		auto thread = (Thread *)param;
		init_allocator();
		defer {deinit_allocator();};
		current_printer = standard_output_printer;
		init_logger_thread();
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

	bool volatile in_use = false;

	SpinLock() = default;
};

inline bool try_lock(SpinLock &m) {
	return !atomic_compare_exchange(&m.in_use, true, false);
}
template <ASpinner Spinner = SleepySpinner>
inline void lock(SpinLock &m, Spinner spinner = {}) {
	loop_until([&] {
		return try_lock(m);
	}, spinner);
}
inline void unlock(SpinLock &m) {
	m.in_use = false;
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

struct TL_API OsLock {
	TL_MAKE_FIXED(OsLock);

	void *handle;

	OsLock();
};

TL_API bool try_lock(OsLock &m);
TL_API void lock(OsLock &m);
TL_API void unlock(OsLock &m);

#ifdef TL_IMPL
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
#endif


template <>
struct Scoped<OsLock> {
	OsLock &mutex;
	Scoped(OsLock &mutex) : mutex(mutex) {
		lock(mutex);
	}
	~Scoped() {
		unlock(mutex);
	}
};

template <class T>
concept ALock = requires (T t) {
	lock(t); 
	{ try_lock(t) } -> std::same_as<bool>;
	unlock(t); 
};

template <class T, ALock Lock>
struct LockProtected {
	TL_MAKE_FIXED(LockProtected);

	using Value = T;

	LockProtected() = default;
	LockProtected(T value) : value(value) {}

	template <class Fn>
	decltype(auto) use(Fn fn) {
		scoped(lock);
		return fn(value);
	}

	template <class Fn>
	decltype(auto) operator * (Fn fn) { return use(fn); }

	T &use_unprotected() { return value; }

private:
	Lock lock;
	T value;
};

#define locked_use(name) name * [&](auto &name) -> decltype(auto)

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
		return this->use_unprotected().count;
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
	Scoped(RecursiveSpinLock &mutex) : mutex(&mutex) {
		lock(mutex);
	}
	~Scoped() {
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

struct ThreadPool {
	struct Task {
		void (*fn)(void *param) = 0;
		void *param = 0;
	};

	struct InitParams {
		void (*worker_initter)() = [](){};
		void (*worker_proc)(ThreadPool *pool) = ThreadPool::default_worker_proc;
	};

	inline bool init(u32 thread_count, InitParams params = {}) {
		start_sync_point = create_sync_point(thread_count + 1); // sync workers and main thread
		stopped_thread_count = 0;
		stopping = false;
		allocator =
		threads.allocator =
		tasks.allocator =
		data_to_free_on_main_thread.allocator =
			current_allocator;
		if (thread_count) {
			threads.reserve(thread_count);

			struct StartInfo {
				ThreadPool *pool;
				void (*init)();
				void (*proc)(ThreadPool *pool);
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
	inline void deinit(bool wait_for_threads = true) {
		stopping = true;
		if (wait_for_threads) {
			wait_for_completion();
		}
		threads.clear();
	}

	inline void push(void (*fn)(void *param), void *param) {
		atomic_increment(&started_task_count);

		new_work_notifier.section([&]{
			tasks.add({
				.fn = fn,
				.param = param,
			});
		});

		new_work_notifier.wake();
	}
	template <class Fn, class ...Args>
	void push(Fn &&fn, Args &&...args) {
		using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
		
		auto fnParams = allocator.allocate_uninitialized<Tuple>();
		new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);

		data_to_free_on_main_thread.add(fnParams);

		constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
		push((void (*)(void *))invokerProc, (void *)fnParams);
	}
	template <class Fn>
	auto &operator+=(Fn &&fn) {
		push(std::forward<Fn>(fn));
		return *this;
	}

	inline void wait_for_completion() {
		while (started_task_count > finished_task_count) {
			completion_notifier.section([&] (ConditionVariable::Sleeper sleeper) {
				sleeper.sleep(1);
			});
		}
		for (auto param : data_to_free_on_main_thread) {
			allocator.free(param);
		}
		data_to_free_on_main_thread.clear();
	}

	template <ASpinner Spinner = SleepySpinner>
	inline void wait_for_completion_doing_tasks(Spinner spinner = {}) {
		loop_while([&] {
			Optional<Task> task;
			new_work_notifier.section([&] {
				task = tasks.pop();
			});
			if (task) {
				run(task.value_unchecked());
			}
			return finished_task_count < started_task_count;
		}, spinner);
		for (auto param : data_to_free_on_main_thread) {
			allocator.free(param);
		}
		data_to_free_on_main_thread.clear();
	}

	inline static void default_worker_proc(ThreadPool *pool) {

		sync(pool->start_sync_point);

		while (1) {
			Optional<ThreadPool::Task> task;
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
	}

#if TL_DEBUG
	~ThreadPool() {
		assert(stopping, "ThreadPool was not properly deinitted. Call `deinit` on the thread pool.");
	}
#endif

private:
	Allocator allocator = current_allocator;
	List<Thread *> threads;

	SyncPoint start_sync_point;
	u32 volatile stopped_thread_count = 0;
	bool volatile stopping = true;
	List<Task> tasks;
	List<void *> data_to_free_on_main_thread;

	u32 volatile started_task_count = 0;
	u32 volatile finished_task_count = 0;

	ConditionVariable new_work_notifier;
	ConditionVariable completion_notifier;

	inline void run(Task task) {
		assert(task.fn);
		task.fn(task.param);
		atomic_increment(&finished_task_count);
	}
};

} // namespace tl
#pragma warning(pop)

#ifdef TL_IMPL

namespace tl {

}

#endif