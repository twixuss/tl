#include <tl/thread.h>
#include <tl/default_logger.h>
#include <tl/hash_map.h>

using namespace tl;

static LockProtected<HashMap<u32, u32, DefaultHashTraits<u32>, DefaultAllocator>, OsLock> thread_id_to_index;
static u32 thread_index_counter;
void add_thread_index() {
	locked_use(thread_id_to_index) {
		thread_id_to_index.get_or_insert(get_current_thread_id()) = thread_index_counter++;
	};
}
static u32 get_thread_index() {
	return locked_use(thread_id_to_index) {
		return thread_id_to_index.find(get_current_thread_id())->value;
	};
}

void thread_pool_test() {
	ThreadPool pool;
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

		inner_tasks.wait_for_completion({.do_tasks = false});
		current_logger.info("[{}] inner tasks completed", get_thread_index());
	};

	outer_tasks.wait_for_completion({.do_tasks = true});
	current_logger.info("[{}] outer tasks completed", get_thread_index());
}