#pragma once

#ifndef TL_ENABLE_PROFILER
#define TL_ENABLE_PROFILER 1
#endif

#if TL_ENABLE_PROFILER
#define timed_begin(profiler, name) (profiler).begin(name, as_span(__FILE__), __LINE__)
#define timed_end(profiler) (profiler).end()
#define timed_block(profiler, name) timed_begin(profiler, name); defer{ timed_end(profiler); }
//#define timed_function() timed_block(([](utf8 const *name){scoped_allocator(temporary_allocator); return demangle(name);})(__FUNCDNAME__))
#define timed_function(profiler) timed_block(profiler, as_span(__FUNCSIG__))
#else
#define timed_begin(profiler, name)
#define timed_end(profiler)
#define timed_block(profiler, name)
#define timed_function(profiler)
#endif

#include "common.h"
// #include "compiler.h"
#include "list.h"
#include "hash_map.h"
#include "thread.h"

namespace tl {
struct TL_API Profiler {
	struct TimeSpan {
		s64 begin;
		s64 end;
		Span<utf8> name;
		Span<utf8> file;
		u32 line;
		u32 thread_id;
	};
	struct Mark {
		s64 counter;
		u32 color;
		u32 thread_id;
	};
	struct ThreadInfo {
		List<TimeSpan> time_spans;
		s64 self_time;
	};

	List<Mark> marks;
	Mutex marks_mutex;

	List<TimeSpan> recorded_time_spans;
	Mutex recorded_time_spans_mutex;

	HashMap<u32, ThreadInfo> thread_infos;
	Mutex thread_infos_mutex;

	s64 start_time;

	void init();
	void deinit();
	void reset();

	void begin(Span<utf8> name, Span<utf8> file, u32 line);
	void end();
	void mark(Span<utf8> name, u32 color);

	template <class T>
	inline void begin(T name, Span<char> file, u32 line) { return begin(as_utf8(as_span(name)), as_utf8(file), line); }
	inline void mark(Span<char> name, u32 color) { return mark(as_utf8(name), color); }

	inline void mark(char const *name, u32 color) { return mark(as_utf8(as_span(name)), color); }

	Span<TimeSpan> get_recorded_time_spans();
	List<ascii> output_for_chrome();
	List<u8> output_for_timed();
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

void Profiler::init() {
	marks.allocator = current_allocator;
	recorded_time_spans.allocator = current_allocator;
	start_time = get_performance_counter();
}
void Profiler::deinit() {
	free(recorded_time_spans);
}
void Profiler::begin(Span<utf8> name, Span<utf8> file, u32 line) {
	auto self_begin = get_performance_counter();
	u32 thread_id = get_current_thread_id();

	lock(thread_infos_mutex);
	auto &info = thread_infos.get_or_insert(thread_id);
	unlock(thread_infos_mutex);

	auto &span = info.time_spans.add();
	span.name = name;
	span.file = file;
	span.line = line;
	span.thread_id = thread_id;
	auto begin_counter = get_performance_counter();
	info.self_time += begin_counter - self_begin;
	span.begin = begin_counter - info.self_time;
}
void Profiler::end() {
	auto end_counter = get_performance_counter();
	u32 thread_id = get_current_thread_id();

	lock(thread_infos_mutex);
	auto info = thread_infos.find(thread_id);
	assert(info);
	unlock(thread_infos_mutex);

	auto &list = info->time_spans;

	TimeSpan span = list.back();
	list.pop();

	span.end = end_counter - info->self_time;

	scoped_lock(recorded_time_spans_mutex);
	recorded_time_spans.add(span);

	info->self_time += get_performance_counter() - end_counter;
}
void Profiler::mark(Span<utf8> name, u32 color) {
	scoped_lock(thread_infos_mutex);
	scoped_lock(marks_mutex);
	auto info = thread_infos.find(get_current_thread_id());
	assert(info);
	marks.add({get_performance_counter() - info->self_time, color, get_current_thread_id()});
}
void Profiler::reset() {
	recorded_time_spans.clear();
	thread_infos.clear();
}

Span<Profiler::TimeSpan> Profiler::get_recorded_time_spans() {
	return recorded_time_spans;
}

List<ascii> Profiler::output_for_chrome() {
	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append(builder, R"({"otherData":{},"traceEvents":[
)"s);


	if (!recorded_time_spans.empty()) {
		bool needComma = false;
		for (auto span : recorded_time_spans) {
			if (needComma) {
				append(builder, ",\n"s);
			}
			append_format(builder, R"({{"cat":"function","dur":{},"name":"{}","ph":"X","pid":0,"tid":{},"ts":{}}})",
				FormatFloat{.value = (span.end - span.begin) * 1000000. / performance_frequency, .precision = 6},
				span.name,
				span.thread_id,
				FormatFloat{.value = span.begin * 1000000. / performance_frequency, .precision = 6}
			);
			needComma = true;
		}
	}
	append(builder, "\n]}"s);
	return (List<ascii>)to_string(builder);
}

List<u8> Profiler::output_for_timed() {
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

	StringBuilder builder;
	builder.allocator = temporary_allocator;
	append_bytes(builder, (u32)recorded_time_spans.count);
	for (auto span : recorded_time_spans) {
		append_bytes(builder, (s64)(divide(multiply(span.begin - start_time, nanoseconds_in_second), performance_frequency)));
		append_bytes(builder, (s64)(divide(multiply(span.end   - start_time, nanoseconds_in_second), performance_frequency)));
		append_bytes(builder, (u32)span.thread_id);
		append_bytes(builder, (u16)span.name.count);
		append_bytes(builder, span.name);
	}
	append_bytes(builder, (u32)marks.count);
	for (auto mark : marks) {
		append_bytes(builder, (s64)(divide(multiply(mark.counter - start_time, nanoseconds_in_second), performance_frequency)));
		append_bytes(builder, (u32)mark.thread_id);
		append_bytes(builder, (u32)mark.color);
	}
	return (List<u8>)to_string(builder);
}

}

#if TL_DEBUG
#pragma optimize("", on)
#endif

#endif
#endif
