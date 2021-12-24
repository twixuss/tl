#pragma once

#ifndef TL_ENABLE_PROFILER
#define TL_ENABLE_PROFILER 1
#endif

#if TL_ENABLE_PROFILER
#define timed_begin(name) ::tl::Profiler::begin(name, as_span(__FILE__), __LINE__)
#define timed_end() ::tl::Profiler::end()
#define timed_block(name) timed_begin(name); defer{ timed_end(); }
//#define timed_function() timed_block(([](char const *name){scoped_allocator(temporary_allocator); return demangle(name);})(__FUNCDNAME__))
#define timed_function() timed_block(as_span(__FUNCSIG__))
#else
#define timed_begin(name)
#define timed_end()
#define timed_block(name)
#define timed_function()
#endif

#include "common.h"

namespace tl { namespace Profiler {

TL_API void begin(Span<char> name, Span<char> file, u32 line);
TL_API void end();
TL_API void mark(Span<char> name, u32 color);

inline void begin(char const *name, char const *file, u32 line) {
	return begin(as_span(name), as_span(file), line);
}
inline void mark(char const *name, u32 color) {
	return mark(as_span(name), color);
}

}}

#include "compiler.h"
#include "list.h"
#include "file.h"

namespace tl { namespace Profiler {

struct TimeSpan {
	s64 begin;
	s64 end;
	Span<char> name;
	Span<char> file;
	u32 line;
	u32 thread_id;
};

extern TL_API thread_local bool enabled;

TL_API void init();
TL_API void deinit();
TL_API void reset();

TL_API Span<TimeSpan> get_recorded_time_spans();

TL_API List<ascii> output_for_chrome();
TL_API List<u8> output_for_timed();

}}

#ifdef TL_IMPL
#if OS_WINDOWS
#include "win32.h"
#include "string.h"
#include "thread.h"
#include "int128.h"
#include <unordered_map>

#pragma optimize("g", on)
#pragma optimize("t", on)
#pragma optimize("y", on)

namespace tl { namespace Profiler {

struct Mark {
	s64 counter;
	u32 color;
	u32 thread_id;
};

List<Mark> marks;
Mutex marks_mutex;

List<TimeSpan> recorded_time_spans;
Mutex recorded_time_spans_mutex;

std::unordered_map<u32, List<TimeSpan>> current_time_spans;
Mutex current_time_spans_mutex;
thread_local bool enabled = TL_ENABLE_PROFILER;
thread_local s64 self_time;

s64 start_time;

void init() {
	marks.allocator = current_allocator;
	recorded_time_spans.allocator = current_allocator;
	start_time = get_performance_counter();
}
void deinit() {
	free(recorded_time_spans);
}
void begin(Span<char> name, Span<char> file, u32 line) {
	auto self_begin = get_performance_counter();
	if (!enabled)
		return;
	u32 thread_id = get_current_thread_id();
	scoped_lock(current_time_spans_mutex);
	auto &span = current_time_spans[thread_id].add();

	span.name = name;
	span.file = file;
	span.line = line;
	span.thread_id = thread_id;
	auto begin_counter = get_performance_counter();
	self_time += begin_counter - self_begin;
	span.begin = begin_counter - self_time;
}
void end() {
	if (!enabled)
		return;
	auto end_counter = get_performance_counter();
	u32 thread_id = get_current_thread_id();

	lock(current_time_spans_mutex);
	auto &list = current_time_spans[thread_id];
	unlock(current_time_spans_mutex);

	TimeSpan span = list.back();
	list.pop();

	span.end = end_counter - self_time;

	scoped_lock(recorded_time_spans_mutex);
	recorded_time_spans.add(span);

	self_time += get_performance_counter() - end_counter;
}
void mark(Span<char> name, u32 color) {
	scoped_lock(marks_mutex);
	marks.add({get_performance_counter() - self_time, color, get_current_thread_id()});
}
void reset() {
	recorded_time_spans.clear();
	current_time_spans.clear();
}

Span<TimeSpan> get_recorded_time_spans() {
	return recorded_time_spans;
}

List<ascii> output_for_chrome() {
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
			append_format(builder, R"({"cat":"function","dur":%,"name":"%","ph":"X","pid":0,"tid":%,"ts":%})",
				FormatFloat{.value = (span.end - span.begin) * 1000000. / performance_frequency, .precision = 6},
				span.name,
				span.thread_id,
				FormatFloat{.value = span.begin * 1000000. / performance_frequency, .precision = 6}
			);
			needComma = true;
		}
	}
	append(builder, "\n]}"s);
	return to_string(builder);
}

List<u8> output_for_timed() {
	/*
	struct Event {
		s64 start; // nanoseconds
		s64 end;
		u32 thread_id;
		u16 name_size;
		char name[];
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

}}

#if TL_DEBUG
#pragma optimize("", on)
#endif

#endif
#endif
