#pragma once
#include "common.h"
#include "list.h"
#include "file.h"

namespace TL { namespace Profiler {

struct TimeSpan {
	s64 begin;
	s64 end;
	Span<char> name;
	char const *file;
	u32 line;
	u32 threadId;
};

TL_API void init();
TL_API void deinit();
TL_API void begin(Span<char> name, char const *file, u32 line);
TL_API void end();
TL_API void reset();

TL_API void wait_for_completion();
TL_API Span<TimeSpan> get_recorded_time_spans();

TL_API void output_for_chrome(char const *path);

#ifndef TL_ENABLE_PROFILER
#define TL_ENABLE_PROFILER 1
#endif

#if TL_ENABLE_PROFILER
#define timed_begin(name) ::TL::Profiler::begin(name, __FILE__, __LINE__)
#define timed_end() ::TL::Profiler::end()
#define timed_block(name) timed_begin(name); defer{ timed_end(); }
#define timed_function() timed_block(as_span(__FUNCTION__))
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
u32 remaining_span_count;
s64 start_time;

void init() {
	recorded_time_spans.allocator = default_allocator;
	start_time = get_performance_counter();
}
void deinit() {
	free(recorded_time_spans);
}
void begin(Span<char> name, char const *file, u32 line) {
	u32 threadId = get_current_thread_id();
	TimeSpan span;
	span.begin = get_performance_counter();
	span.name = name;
	span.file = file;
	span.line = line;
	span.threadId = threadId;

	//if (current_time_spans.empty()) {
	//	startTime = (f64)span.begin * 1000.0 / performanceFrequency;
	//}

	atomic_add(remaining_span_count, 1);
	scoped_lock(current_time_spans_mutex);
	current_time_spans[threadId].add(span);
}
void end() {
	u32 threadId = get_current_thread_id();

	lock(current_time_spans_mutex);
	auto &list = current_time_spans[threadId];
	unlock(current_time_spans_mutex);

	TimeSpan span = list.back();
	list.pop();

	span.end = get_performance_counter();

	atomic_add(remaining_span_count, (u32)-1);
	scoped_lock(recorded_time_spans_mutex);
	recorded_time_spans.add(span);
}
void reset() {
	recorded_time_spans.clear();
	current_time_spans.clear();
}

void wait_for_completion() {
	loop_until([]{ return remaining_span_count == 0; });
}
Span<TimeSpan> get_recorded_time_spans() {
	return recorded_time_spans;
}

void output_for_chrome(char const *path) {
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
				(span.end - span.begin) * 1000000. / performance_frequency,
				span.name,
				span.threadId,
				(span.begin - start_time) * 1000000. / performance_frequency
			);
			needComma = true;
		}
	}
	append(builder, "\n]}"s);
	auto s = to_string(builder);
	write_entire_file(path, s.data, s.size);
}

}}
#endif
#endif
#endif
