#pragma once
#include "compiler.h"
#include "common.h"
#include "list.h"
#include "file.h"

namespace TL { namespace Profiler {

struct TimeSpan {
	s64 begin;
	s64 end;
	List<char> name;
	char const *file;
	u32 line;
	u32 thread_id;
};

TL_API void init();
TL_API void deinit();
TL_API void begin(Span<char> name, char const *file, u32 line);
TL_API void end();
TL_API void reset();

TL_API Span<TimeSpan> get_recorded_time_spans();

TL_API List<ascii> output_for_chrome();

#ifndef TL_ENABLE_PROFILER
#define TL_ENABLE_PROFILER 1
#endif

#if TL_ENABLE_PROFILER
#define timed_begin(name) ::TL::Profiler::begin(name, __FILE__, __LINE__)
#define timed_end() ::TL::Profiler::end()
#define timed_block(name) timed_begin(name); defer{ timed_end(); }
#define timed_function() timed_block(([](char const *name){scoped_allocator(temporary_allocator); return demangle(name);})(__FUNCDNAME__))
#else
#define timed_begin(name)
#define timed_end()
#define timed_block(name)
#define timed_function()
#endif

}}

#ifdef TL_IMPL
#if TL_ENABLE_PROFILER
#if OS_WINDOWS
#include "win32.h"
#include "string.h"
#include "thread.h"
#include <unordered_map>
namespace TL { namespace Profiler {

List<TimeSpan> recorded_time_spans;
Mutex recorded_time_spans_mutex;

std::unordered_map<u32, List<TimeSpan>> current_time_spans;
Mutex current_time_spans_mutex;
//f64 startTime;
s64 start_time;

void init() {
	recorded_time_spans.allocator = default_allocator;
}
void deinit() {
	free(recorded_time_spans);
}
void begin(Span<char> name, char const *file, u32 line) {
	u32 thread_id = get_current_thread_id();
	scoped_lock(current_time_spans_mutex);
	auto &span = current_time_spans[thread_id].add();

	span.name = name;
	span.file = file;
	span.line = line;
	span.thread_id = thread_id;
	span.begin = get_performance_counter();

	if (!start_time)
		start_time = span.begin;
}
void end() {
	auto counter = get_performance_counter();
	u32 thread_id = get_current_thread_id();

	lock(current_time_spans_mutex);
	auto &list = current_time_spans[thread_id];
	unlock(current_time_spans_mutex);

	TimeSpan span = list.back();
	list.pop();

	span.end = counter;

	scoped_lock(recorded_time_spans_mutex);
	recorded_time_spans.add(span);
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
				FormatFloat((span.end - span.begin) * 1000000. / performance_frequency, 6),
				span.name,
				span.thread_id,
				FormatFloat((span.begin - start_time) * 1000000. / performance_frequency, 6)
			);
			needComma = true;
		}
	}
	append(builder, "\n]}"s);
	return to_string(builder);
}

}}
#endif
#endif
#endif
