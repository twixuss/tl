#pragma once

#ifndef TL_ENABLE_PROFILER
#define TL_ENABLE_PROFILER 1
#endif

#if TL_ENABLE_PROFILER
#define timed_begin(profiler, name) (profiler).begin(name, as_span(__FILE__), __LINE__)
#define timed_end(profiler) (profiler).end()
#define timed_block(profiler, name) timed_begin(profiler, name); defer{ timed_end(profiler); }
//#define timed_function() timed_block(([](utf8 const *name){scoped(temporary_allocator); return demangle(name);})(__FUNCDNAME__))
#define timed_function(profiler) timed_block(profiler, as_span(__FUNCTION__))
#define timed_x(profiler, expression) ([&]() -> decltype(auto) { timed_block(profiler, #expression); return expression; }())
#else
#define timed_begin(profiler, name)
#define timed_end(profiler)
#define timed_block(profiler, name)
#define timed_function(profiler)
#define timed_x(profiler, expression) expression
#endif

#include "common.h"
// #include "compiler.h"
#include "list.h"
#include "hash_map.h"
#include "multi_array.h"
#include "thread.h"
#include "math.h"

#ifndef TL_PROFILER_SUBTRACT_SELF_TIME
#define TL_PROFILER_SUBTRACT_SELF_TIME 0
#endif

namespace tl {

struct TL_API Profiler {
	struct Allocator : AllocatorBase<Allocator> {
		ArenaAllocator *arena;
		
		inline AllocationResult allocate_impl(umm size, umm alignment TL_LP) {
			return arena->allocate_impl(size, alignment TL_LA);
		}
		inline AllocationResult reallocate_impl(void *data, umm old_size, umm new_size, umm alignment TL_LP) {
			return arena->reallocate_impl(data, old_size, new_size, alignment TL_LA);
		}
		inline void deallocate_impl(void *data, umm size, umm alignment TL_LP) {
			return arena->deallocate_impl(data, size, alignment TL_LA);
		}

		static Allocator current() {
			return {};
		}
	};

	struct TimeSpan {
		u64 begin = 0;
		u64 end = 0;
		Span<utf8> name = {};
		Span<utf8> file = {};
		u32 line = 0;
		u64 self = 0;
		u16 depth = 0;
	};
	struct Mark {
		u64 time = 0;
		Span<utf8> name = {};
		Span<utf8> file = {};
		u32 line = 0;
	};
	struct alignas(64) ThreadInfo {
		List<TimeSpan, Allocator> time_span_stack = {};
		List<TimeSpan, Allocator> recorded_time_spans = {};
		List<Mark, Allocator> marks = {};
#if TL_PROFILER_SUBTRACT_SELF_TIME
		u64 self_time = 0;
#endif
		u32 depth = 0;
	};

	ArenaAllocator allocator;
	umm init_allocation_size = 0;

	MultiArray<Allocator, true, u32, LockProtected<ThreadInfo, SpinLock>> ids_and_infos = {};
	umm ids_and_infos_allocated_count = 0;

	s64 start_time = 0;

	bool enabled = true;
	bool waiting_for_everyone = false;
	bool everyone_is_done = false;

	void init(ArenaAllocator allocator, umm unique_threads_max_count);
	void reset();
	void free();

	void begin(Span<utf8> name, Span<utf8> file, u32 line);
	template <class T>
	inline void begin(T name, Span<char> file, u32 line) { return begin(as_utf8(as_span(name)), as_utf8(file), line); }

	void end();

	void mark(Span<utf8> name, Span<utf8> file, u32 line);
	inline void mark(Span<char> name, Span<utf8> file, u32 line) { return mark(as_utf8(name), file, line); }
	inline void mark(char const *name, Span<utf8> file, u32 line) { return mark(as_utf8(as_span(name)), file, line); }

	// Waits for all threads to finish their timed blocks.
	// Executes `fn` while holding all locks.
	void when_everyone_is_done(auto &&fn) {
		if (everyone_is_done) {
			fn();
			return;
		}

		scoped_replace(waiting_for_everyone, true);

		auto lock_everyone = [&] {
			umm initial_ids_and_infos_allocated_count = ids_and_infos_allocated_count;

			for (umm i = 0; i < initial_ids_and_infos_allocated_count; ++i) {
				auto &thread_info = ids_and_infos.base_of<1>()[i];

				if (thread_info.unprotected.depth != 0) {
					for (umm j = 0; j < i; ++j) {
						auto &thread_info = ids_and_infos.base_of<1>()[j];
						unlock(thread_info._lock);
					}
					return false;
				}

				lock(thread_info._lock);
			}

			if (initial_ids_and_infos_allocated_count != ids_and_infos_allocated_count) {
				for (umm i = 0; i < initial_ids_and_infos_allocated_count; ++i) {
					auto &thread_info = ids_and_infos.base_of<1>()[i];
					unlock(thread_info._lock);
				}
				return false;
			}

			return true;
		};

		while (!lock_everyone()) {
			sleep_milliseconds(1);
		}

		{
			scoped_replace(everyone_is_done, true);
			fn();
		}
	
		for (umm i = 0; i < ids_and_infos_allocated_count; ++i) {
			auto &thread_info = ids_and_infos.base_of<1>()[i];
			unlock(thread_info._lock);
		}
	}

	struct Report {
		struct Thread {
			u32 id;
			Span<TimeSpan> time_spans;
			Span<Mark> marks;
		};

		List<TimeSpan> all_time_spans;
		List<Mark> all_marks;
		List<Thread> per_thread;
		u64 start_time;
	};

	void collect_report(Report &report) {
		assert(waiting_for_everyone);

		report.start_time = start_time;

		report.all_time_spans.clear();
		report.all_marks.clear();
		report.per_thread.clear();

		for (umm i = 0; i < ids_and_infos_allocated_count; ++i) {
			auto &thread_id   = ids_and_infos.base_of<0>()[i];
			auto &protected_thread_info = ids_and_infos.base_of<1>()[i];

			// No need to lock. It is already taken since waiting_for_everyone is true.
			auto &thread_info = protected_thread_info.unprotected;

			report.per_thread.add({
				.id = thread_id,
				.time_spans = {
					(TimeSpan *)report.all_time_spans.count,
					thread_info.recorded_time_spans.count,
				},
				.marks = {
					(Mark *)report.all_marks.count,
					thread_info.marks.count,
				},
			});
			report.all_time_spans.add(thread_info.recorded_time_spans);
			report.all_marks.add(thread_info.marks);
		}

		for (auto &thread : report.per_thread) {
			thread.time_spans.data = report.all_time_spans.data + (umm)thread.time_spans.data;
			thread.marks.data = report.all_marks.data + (umm)thread.marks.data;
		}
	}
	
	struct ReportForChrome : Report {};
	struct ReportForTimed : Report {};
};


void append(StringBuilder &b, Profiler::ReportForChrome const &r);
void append(StringBuilder &b, Profiler::ReportForTimed const &r);


struct ProfileRenderer {
	using Nanoseconds = s64;
	struct Event {
		aabb<v2s64> rect;
		Span<utf8> name;
		Nanoseconds begin;
		Nanoseconds end;
		u32 depth;
		Nanoseconds self_duration;
		forceinline Nanoseconds duration() const {
			return end - begin;
		}
	};

	struct Mark {
		aabb<v2s64> rect;
		Span<utf8> name;
		Nanoseconds time;
	};

	struct ThreadDrawList {
		u32 id = 0;
		u32 height = 0;
		List<Event> events;
		List<Mark> marks;
	};

	struct EventGroup {
		Span<utf8> name;
		Nanoseconds total_duration;
	};

	List<ThreadDrawList> events_to_draw_by_thread;
	Nanoseconds events_begin = 0;
	Nanoseconds events_end = 0;
	Nanoseconds events_duration = 0;

	void setup(Profiler::Report const &r);

	void render(f64 view_scale, f64 scroll_amount_in_nanoseconds, s32 button_height);

	void free();
};

}

#ifdef TL_IMPL
#if OS_WINDOWS
#include "win32.h"
#include "string.h"
#include "int128.h"
#include "file.h"

#pragma optimize("g", on)
#pragma optimize("t", on)
#pragma optimize("y", on)

namespace tl {

#ifdef __AVX2__
inline static constexpr umm unique_threads_max_count_alignment = 8;
#else
inline static constexpr umm unique_threads_max_count_alignment = 4;
#endif

auto &get_thread_info(Profiler &profiler, u32 thread_id) {
	auto ids_base = profiler.ids_and_infos.base_of<0>();
	auto infos_base = profiler.ids_and_infos.base_of<1>();

	#ifdef __AVX2__
	__m256i wthread_id = _mm256_set1_epi32(thread_id);
	for (umm i = 0; i < profiler.ids_and_infos_allocated_count; i += 8) {
		__m256i stored_ids = _mm256_loadu_si256((__m256i *)(ids_base + i));

		__m256i wmask = _mm256_cmpeq_epi32(stored_ids, wthread_id);

		u8 mask = _mm256_movemask_ps(_mm256_castsi256_ps(wmask));

		if (mask != 0) {
			u8 tz = count_trailing_zeros(mask);
			return infos_base[i + tz];
		}
	}
	#else
	__m128i wthread_id = _mm_set1_epi32(thread_id);
	for (umm i = 0; i < profiler.ids_and_infos_allocated_count; i += 4) {
		__m128i stored_ids = _mm_loadu_si128((__m128i *)(ids_base + i));

		__m128i wmask = _mm_cmpeq_epi32(stored_ids, wthread_id);

		u8 mask = _mm_movemask_ps(_mm_castsi128_ps(wmask));

		if (mask != 0) {
			u8 tz = count_trailing_zeros(mask);
			return infos_base[i + tz];
		}
	}
	#endif

	#if 0
	// Reference
	for (umm i = 0; i < profiler.ids_and_infos_allocated_count; ++i) {
		if (ids_base[i] == thread_id) {
			return infos_base[i];
		}
	}
	#endif

	umm dst_index = atomic_increment(&profiler.ids_and_infos_allocated_count);

	assert(dst_index < profiler.ids_and_infos.count);

	ids_base[dst_index] = thread_id;
	auto &info = infos_base[dst_index];
	info.unprotected.time_span_stack.allocator = Profiler::Allocator{.arena = &profiler.allocator};
	info.unprotected.recorded_time_spans.allocator = Profiler::Allocator{.arena = &profiler.allocator};
	info.unprotected.marks.allocator = Profiler::Allocator{.arena = &profiler.allocator};
	return info;
}

void Profiler::init(ArenaAllocator _allocator, umm unique_threads_max_count) {
	unique_threads_max_count = ceil(unique_threads_max_count, unique_threads_max_count_alignment);

	allocator = _allocator;
	ids_and_infos.allocator = Profiler::Allocator{.arena = &allocator};
	ids_and_infos.init(unique_threads_max_count);
	start_time = get_performance_counter();

	init_allocation_size = allocator.cursor - allocator.base;
}
void Profiler::free() {
	allocator.free();
}

void Profiler::begin(Span<utf8> name, Span<utf8> file, u32 line) {
	if (!enabled)
		return;
	
#if TL_PROFILER_SUBTRACT_SELF_TIME
	auto self_begin = get_performance_counter();
#endif

	u32 thread_id = get_current_thread_id();
	
	bool should_sleep = false;

retry:
	while (should_sleep && waiting_for_everyone) {
		sleep_milliseconds(1);
	}

	auto &protected_thread_info = get_thread_info(*this, thread_id);
	scoped_locked_use_expr(thread_info, protected_thread_info);

	// TODO: profiler.reset() requires waiting for everyone to finish their timed blocks. 
	//       If reset() is waiting, ideally we should delay starting a new timed block
	//       until reset() does it's thing. If we don't do that - reset() might not catch the moment
	//       thread_info.depth goes to zero and will be waiting for way too long, causing lag spikes.
	//       The code below implements that, but deadlocks sometimes.
	if (thread_info.depth == 0 && waiting_for_everyone) {
		should_sleep = true;
		goto retry;
	}

	thread_info.depth += 1;

	auto &span = thread_info.time_span_stack.add();
	span.name = name;
	span.file = file;
	span.line = line;
	span.depth = thread_info.time_span_stack.count - 1;

#if TL_PROFILER_SUBTRACT_SELF_TIME
	auto begin_counter = get_performance_counter();
	thread_info.self_time += begin_counter - self_begin;
	span.begin = begin_counter - thread_info.self_time;
#else
	span.begin = get_performance_counter();
#endif
}
void Profiler::end() {
	if (!enabled)
		return;
	
	auto end_counter = get_performance_counter();

	u32 thread_id = get_current_thread_id();
	
	auto &protected_thread_info = get_thread_info(*this, thread_id);
	scoped_locked_use_expr(thread_info, protected_thread_info);

	TimeSpan span = thread_info.time_span_stack.back();
	thread_info.time_span_stack.pop();

#if TL_PROFILER_SUBTRACT_SELF_TIME
	span.end = end_counter - thread_info.self_time;
#else
	span.end = end_counter;
#endif
	
	if (thread_info.time_span_stack.count) {
		thread_info.time_span_stack.back().self -= span.end - span.begin;
	}
	span.self += span.end - span.begin;
	thread_info.recorded_time_spans.add(span);
	thread_info.depth -= 1;

#if TL_PROFILER_SUBTRACT_SELF_TIME
	thread_info.self_time += get_performance_counter() - end_counter;
#endif
}
void Profiler::mark(Span<utf8> name, Span<utf8> file, u32 line) {
	if (!enabled)
		return;
	
	auto time = get_performance_counter();
	
	u32 thread_id = get_current_thread_id();
	
	auto &protected_thread_info = get_thread_info(*this, thread_id);
	scoped_locked_use_expr(thread_info, protected_thread_info);

	Profiler::Mark mark = {
		.time = time,
		.name = name,
		.file = file,
		.line = line,
	};

#if TL_PROFILER_SUBTRACT_SELF_TIME
	mark.time -= thread_info.self_time;
#endif

	thread_info.marks.add(mark);
}
void Profiler::reset() {
	when_everyone_is_done([&] {
		for (umm i = 0; i < ids_and_infos_allocated_count; ++i) {
			auto &thread_info = ids_and_infos.base_of<1>()[i];
			tl::free(thread_info.unprotected.time_span_stack);
			tl::free(thread_info.unprotected.recorded_time_spans);
			tl::free(thread_info.unprotected.marks);
		}

		allocator.cursor = allocator.base + init_allocation_size;

		start_time = get_performance_counter();
	});
}

void append(StringBuilder &builder, Profiler::ReportForChrome const &r) {
	append(builder, R"({"otherData":{},"traceEvents":[
)"s);

	bool needComma = false;
	for (auto thread : r.per_thread) {
		for (auto span : thread.time_spans) {
			if (needComma) {
				append(builder, ",\n"s);
			}
			append_format(builder, R"({{"cat":"function","dur":{},"name":"{}","ph":"X","pid":0,"tid":{},"ts":{}}})",
				FormatFloat{.value = (span.end - span.begin) * 1000000. / performance_frequency, .precision = 6},
				span.name,
				thread.id,
				FormatFloat{.value = span.begin * 1000000. / performance_frequency, .precision = 6}
			);
			needComma = true;
		}
	}
	append(builder, "\n]}"s);
}

void append(StringBuilder &builder, Profiler::ReportForTimed const &r) {
	/*
	struct Event {
		s64 start; // nanoseconds
		s64 end;
		u32 thread_id;
		u16 name_size;
		utf8 name[];
	};
	*/

	s64 const nanoseconds_in_second = 1'000'000'000;

	append_bytes(builder, (u32)r.all_time_spans.count);
	for (auto thread : r.per_thread) {
		for (auto span : thread.time_spans) {
			append_bytes(builder, (s64)(divide(multiply(span.begin - r.start_time, nanoseconds_in_second), performance_frequency)));
			append_bytes(builder, (s64)(divide(multiply(span.end   - r.start_time, nanoseconds_in_second), performance_frequency)));
			append_bytes(builder, (u32)thread.id);
			append_bytes(builder, (u16)span.name.count);
			append_bytes(builder, span.name);
		}
	}
	append_bytes(builder, (u32)r.all_marks.count);
	for (auto thread : r.per_thread) {
		for (auto mark : thread.marks) {
			append_bytes(builder, (s64)(divide(multiply(mark.time - r.start_time, nanoseconds_in_second), performance_frequency)));
			append_bytes(builder, (u32)thread.id);
			append_bytes(builder, (u32)-1); // color
		}
	}
}

void ProfileRenderer::setup(Profiler::Report const &r) {
	events_begin = max_value<Nanoseconds>;
	events_end = min_value<Nanoseconds>;

	ContiguousHashMap<u32, ThreadDrawList, DefaultHashTraits<u32>, TemporaryAllocator> thread_id_to_events_to_draw;

	for (auto thread : r.per_thread) {
		for (auto span : thread.time_spans) {
			auto begin = (Nanoseconds)divide(multiply(span.begin, 1'000'000'000), performance_frequency);
			auto end = (Nanoseconds)divide(multiply(span.end, 1'000'000'000), performance_frequency);
			Event e = {
				.name = span.name,
				.begin = begin,
				.end = end,
				.depth = span.depth,
				.self_duration = (Nanoseconds)divide(multiply(span.self, 1'000'000'000), performance_frequency),
			};
			auto &t = thread_id_to_events_to_draw.get_or_insert(thread.id);
			t.id = thread.id;
			t.events.add(e);
			events_begin = min(events_begin, e.begin);
			events_end = max(events_end, e.end);
		}
	}
	for (auto thread : r.per_thread) {
		for (auto mark : thread.marks) {
			Mark m = {
				.name = mark.name,
				.time = (Nanoseconds)divide(multiply(mark.time, 1'000'000'000), performance_frequency),
			};
			auto &t = thread_id_to_events_to_draw.get_or_insert(thread.id);
			t.id = thread.id;
			t.marks.add(m);
			events_begin = min(events_begin, m.time);
			events_end = max(events_end, m.time);
		}
	}

	events_duration = events_end - events_begin;

	foreach(it, thread_id_to_events_to_draw) {
		auto &thread_id = it.key();
		auto &events_to_draw = it.value();

		for (auto &event : events_to_draw.events) {
			event.begin -= events_begin;
			event.end -= events_begin;
		}

		for (auto &mark : events_to_draw.marks) {
			mark.time -= events_begin;
		}
	}

	events_to_draw_by_thread.clear();
	foreach(it, thread_id_to_events_to_draw) {
		events_to_draw_by_thread.add(it.value());
	}
	quick_sort(events_to_draw_by_thread, [](ThreadDrawList t) { return t.id; });
}

void ProfileRenderer::render(f64 view_scale, f64 scroll_amount_in_nanoseconds, s32 button_height) {
	for (auto &events_to_draw : events_to_draw_by_thread) {
		u32 max_depth = 0;

		for (auto &mark : events_to_draw.marks) {
			mark.rect.min.x = (mark.time - scroll_amount_in_nanoseconds) * view_scale;
			mark.rect.max.x = mark.rect.min.x + 1;
			mark.rect.min.y = 0;
			mark.rect.max.y = button_height * 1;
		}
		for (auto &event : events_to_draw.events) {
			event.rect.min.x = (event.begin - scroll_amount_in_nanoseconds) * view_scale;
			event.rect.max.x = max<f64>((event.end - scroll_amount_in_nanoseconds) * view_scale, event.rect.min.x + 1);
			event.rect.min.y = button_height * (event.depth + 0);
			event.rect.max.y = button_height * (event.depth + 1);

			max_depth = max(max_depth, event.depth);
		}

		events_to_draw.height = (max_depth + 1) * button_height;
	}
}

void ProfileRenderer::free() {
	for (auto &events_to_draw : events_to_draw_by_thread) {
		tl::free(events_to_draw.events);
		tl::free(events_to_draw.marks);
	}
	tl::free(events_to_draw_by_thread);
}

}

#if TL_DEBUG
#pragma optimize("", on)
#endif

#endif
#endif
