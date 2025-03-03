#include <tl/console.h>
#include <tl/thread.h>
#include <tl/default_logger.h>
#include <tl/hash_map.h>

using namespace tl;

#pragma warning(push)
#pragma warning(disable: 4459) // global shadowing

static LockProtected<HashMap<u32, u32, DefaultHashTraits<u32>, DefaultAllocator>, OsLock> thread_id_to_index;
static u32 thread_index_counter;
void add_thread_index() {
	locked_use(thread_id_to_index) {
		thread_id_to_index.get_or_insert(get_current_thread_id()) = thread_index_counter++;
	};
}
static u32 get_thread_index() {
	return locked_use(thread_id_to_index) {
		return *thread_id_to_index.find(get_current_thread_id()).value;
	};
}

void test_TaskListsThreadPool() {
	println("==== test_TaskListsThreadPool ====");
	TaskListsThreadPool pool;
	add_thread_index();
	pool.init(4, {.worker_initter = [] {
		add_thread_index();
	}});
	defer { pool.deinit(); };

	current_logger.info("[{}] starting thread pool test", get_thread_index());

	auto outer_tasks = pool.create_task_list();

	for (int i = 0; i < 10; ++i) {
		outer_tasks += [i] { sleep_milliseconds(i); current_logger.info("[{}] slept {}", get_thread_index(), i); };
	}

	sleep_milliseconds(1);

	outer_tasks += [&pool] {
		auto inner_tasks = pool.create_task_list();
		
		for (int i = 0; i < 10; ++i) {
			inner_tasks += [i] { current_logger.info("[{}] inner {}", get_thread_index(), i); };
		}

		inner_tasks.wait_for_completion(WaitForCompletionOption::just_wait);
		current_logger.info("[{}] inner tasks completed", get_thread_index());
	};

	outer_tasks.wait_for_completion(WaitForCompletionOption::do_any_task);
	current_logger.info("[{}] outer tasks completed", get_thread_index());
}

void test_TaskQueuesThreadPool() {
	println("==== test_TaskQueuesThreadPool ====");
	TaskQueuesThreadPool pool;
	add_thread_index();
	pool.init(4, {.worker_initter = [] {
		add_thread_index();
	}});
	defer { pool.deinit(); };

	TaskQueuesThreadPool::TaskQueue tasks = {pool};

	for (int i = 0; i < 10; ++i) {
		tasks += [i] { current_logger.info("[{}] {}", get_thread_index(), i); };
	}

	current_logger.info("[{}] waiting for completion", get_thread_index());
	tasks.wait_for_completion();
	current_logger.info("[{}] tasks completed", get_thread_index());
}

void thread_pool_test() {
	test_TaskListsThreadPool();
	test_TaskQueuesThreadPool();
}