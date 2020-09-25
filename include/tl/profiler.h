#pragma once
#include "common.h"

namespace TL { namespace Profiler {

struct TimeSpan {
	s64 begin;
	s64 end;
	char const *name;
	char const *file;
	u32 line;
	u32 threadId;
};

TL_API void begin(char const *name, char const *file, u32 line);
TL_API void end();
TL_API void reset();

TL_API void waitForCompletion();
TL_API List<TimeSpan> const &getRecordedTimeSpans();

TL_API void outputForChrome(char const *path);

#ifndef TL_ENABLE_PROFILER
#define TL_ENABLE_PROFILER 0
#endif

#if TL_ENABLE_PROFILER
#define TL_TIMED_BLOCK(name) ::TL::Profiler::begin(name, __FILE__, __LINE__); DEFER{ ::TL::Profiler::end(); }
#define TL_TIMED_FUNCTION TL_TIMED_BLOCK(__FUNCTION__)
#else
#define TL_TIMED_BLOCK(name)
#define TL_TIMED_FUNCTION
#endif

}}

#ifdef TL_IMPL
#if OS_WINDOWS
#include "win32.h"
#include "list.h"
#include "string.h"
#include "thread.h"
#include <unordered_map>
namespace TL { namespace Profiler {

s64 performanceFrequency = getPerformanceFrequency();
List<TimeSpan> recordedTimeSpans;
Mutex recordedTimeSpansMutex;
std::unordered_map<u32, List<TimeSpan>> currentTimeSpans;
Mutex currentTimeSpansMutex;
//f64 startTime;
u32 undoneSpanCount;

void begin(char const *name, char const *file, u32 line) {
	u32 threadId = getCurrentThreadId();
	TimeSpan span;
	span.begin = getPerformanceCounter();
	span.name = name;
	span.file = file;
	span.line = line;
	span.threadId = threadId;

	//if (currentTimeSpans.empty()) {
	//	startTime = (f64)span.begin * 1000.0 / performanceFrequency;
	//}

	lockAdd(undoneSpanCount, 1);
	SCOPED_LOCK(currentTimeSpansMutex);
	currentTimeSpans[threadId].push_back(span);
}
void end() {
	u32 threadId = getCurrentThreadId();

	lock(currentTimeSpansMutex);
	auto &list = currentTimeSpans[threadId];
	unlock(currentTimeSpansMutex);

	TimeSpan span = list.back();
	list.pop_back();

	span.end = getPerformanceCounter();

	lockAdd(undoneSpanCount, (u32)-1);
	SCOPED_LOCK(recordedTimeSpansMutex);
	recordedTimeSpans.push_back(span);
}
void reset() {
	recordedTimeSpans.clear();
	currentTimeSpans.clear();
}

void waitForCompletion() {
	loopUntil([]{ return undoneSpanCount == 0; });
}
List<TimeSpan> const &getRecordedTimeSpans() {
	return recordedTimeSpans;
}

void outputForChrome(char const *path) {
	StringBuilder<OsAllocator, 8> builder;
	builder.append("{\"otherData\":{},\"traceEvents\":[\n");
	if (!recordedTimeSpans.empty()) {
		bool needComma = false;
		for (auto span : recordedTimeSpans) {
			if (needComma) {
				builder.append(",\n");
			}
			builder.append("{\"cat\":\"function\",\"dur\":");
			builder.append((f64)(span.end - span.begin) / (f64)performanceFrequency * 1000000.0);
			builder.append(",\"name\":\"");
			builder.append(span.name);
			builder.append("\",\"ph\":\"X\",\"pid\":0,\"tid\":");
			builder.append(span.threadId);
			builder.append(",\"ts\":");
			builder.append((f64)span.begin / (f64)performanceFrequency * 1000000.0);
			builder.append("}");
			needComma = true;
		}
	}
	builder.append("\n]}");

	auto str = builder.get();
	writeEntireFile(path, str.data(), str.size());
}

}}
#endif
#endif
