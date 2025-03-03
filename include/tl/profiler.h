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
#include "thread.h"
#include "math.h"

#ifndef TL_PROFILER_SUBTRACT_SELF_TIME
#define TL_PROFILER_SUBTRACT_SELF_TIME 0
#endif

namespace tl {
struct TL_API Profiler {
	struct TimeSpan {
		u64 begin;
		u64 end;
		Span<utf8> name;
		Span<utf8> file;
		u32 line;
		u32 thread_id;
	};
	struct Mark {
		u64 counter;
		u32 color;
		u32 thread_id;
	};
	struct ThreadInfo {
		List<TimeSpan> time_spans;
#if TL_PROFILER_SUBTRACT_SELF_TIME
		u64 self_time;
#endif
	};

	ArenaAllocator allocator;

	List<Mark> marks;
	SpinLock marks_lock;

	List<TimeSpan> recorded_time_spans;
	SpinLock recorded_time_spans_lock;

	HashMap<u32, ThreadInfo> thread_infos;
	SpinLock thread_infos_lock;

	s64 start_time;

	bool enabled = true;

	void init(ArenaAllocator allocator);
	void reset();
	void free();

	// Returns old arena
	ArenaAllocator switch_arena(ArenaAllocator new_arena);

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

	struct ScopedTimer {
		Profiler *profiler = 0;
		ScopedTimer() = default;
		ScopedTimer(ScopedTimer const &) = delete;
		ScopedTimer(ScopedTimer &&that) : profiler(that.profiler) {
			that.profiler = 0;
		}
		~ScopedTimer() {
			if (profiler) {
				profiler->end();
			}
		}
	};

	ScopedTimer scoped_timer(char const *name, std::source_location location = std::source_location::current()) {
		ScopedTimer timer;
		timer.profiler = this;
		begin(name, as_span(location.file_name()), location.line());
		return timer;
	}


	struct ScopeMeasurer {
		Profiler *profiler = 0;
		Span<utf8> name;
		Span<utf8> file;
		u32 line = 0;
	};

	ScopeMeasurer measure(Span<utf8> name, std::source_location location = std::source_location::current()) {
		return {this, name, as_utf8(as_span(location.file_name())), location.line()};
	}
	ScopeMeasurer measure(char const *name, std::source_location location = std::source_location::current()) {
		return measure(as_utf8(as_span(name)), location);
	}
	ScopeMeasurer measure(std::source_location location = std::source_location::current()) {
		return measure(as_utf8(as_span(location.function_name())), location);
	}
};

template <>
struct Scoped<Profiler::ScopeMeasurer> {
	Profiler *profiler;
	void enter(Profiler::ScopeMeasurer measurer) {
		this->profiler = measurer.profiler;
		measurer.profiler->begin(measurer.name, measurer.file, measurer.line);
	}
	void exit() {
		profiler->end();
	}
};

struct ProfileRenderer {
	using Nanoseconds = s64;
	struct Event {
		aabb<v2s64> rect;
		//Event *parent;
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

private:

	List<ThreadDrawList> all_events_to_draw;
	Nanoseconds events_begin = 0;
	Nanoseconds events_end = 0;
	Nanoseconds events_duration = 0;

public:
	void setup(Span<Profiler::TimeSpan> spans, Span<Profiler::Mark> marks);
	void free();

	template <class Drawer>
	void render(f64 view_scale, f64 scroll_amount, s32 button_height, Drawer drawer) {
		for (auto &events_to_draw : all_events_to_draw) {
			u32 max_depth = 0;

			for (auto &mark : events_to_draw.marks) {
				mark.rect.min.x = mark.time * view_scale + scroll_amount;
				mark.rect.max.x = mark.rect.min.x + 1;
				mark.rect.min.y = 0;
				mark.rect.max.y = button_height * 1;
			}
			for (auto &event : events_to_draw.events) {
				event.rect.min.x = event.begin * view_scale + scroll_amount;
				event.rect.max.x = max<f64>(event.end * view_scale + scroll_amount, event.rect.min.x + 1);
				event.rect.min.y = button_height * (event.depth + 0);
				event.rect.max.y = button_height * (event.depth + 1);

				max_depth = max(max_depth, event.depth);
			}

			events_to_draw.height = max_depth * button_height;

			drawer(events_to_draw);
		}
	}
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

void Profiler::init(ArenaAllocator _allocator) {
	allocator = _allocator;
	marks.allocator = allocator;
	recorded_time_spans.allocator = allocator;
	thread_infos.allocator = allocator;
	start_time = get_performance_counter();
}
ArenaAllocator Profiler::switch_arena(ArenaAllocator new_arena) {
	tl::free(marks);
	tl::free(recorded_time_spans);
	thread_infos.free();
	defer {
		allocator = new_arena;
	};
	return allocator;
}
void Profiler::free() {
	tl::free(marks);
	tl::free(recorded_time_spans);
	thread_infos.free();
	allocator.free();
}
void Profiler::begin(Span<utf8> name, Span<utf8> file, u32 line) {
	if (!enabled)
		return;

#if TL_PROFILER_SUBTRACT_SELF_TIME
	auto self_begin = get_performance_counter();
#endif

	u32 thread_id = get_current_thread_id();

	auto &info = with(thread_infos_lock, thread_infos.get_or_insert(thread_id));

	auto &span = info.time_spans.add();
	span.name = name;
	span.file = file;
	span.line = line;
	span.thread_id = thread_id;

#if TL_PROFILER_SUBTRACT_SELF_TIME
	auto begin_counter = get_performance_counter();
	info.self_time += begin_counter - self_begin;
	span.begin = begin_counter - info.self_time;
#else
	span.begin = get_performance_counter();
#endif
}
void Profiler::end() {
	if (!enabled)
		return;

	auto end_counter = get_performance_counter();
	u32 thread_id = get_current_thread_id();

	auto found = with(thread_infos_lock, thread_infos.find(thread_id));
	assert(found);

	auto info = found.value;
	auto &list = info->time_spans;

	TimeSpan span = list.back();
	list.pop();

#if TL_PROFILER_SUBTRACT_SELF_TIME
	span.end = end_counter - info->self_time;
#else
	span.end = end_counter;
#endif

	with(recorded_time_spans_lock, recorded_time_spans.add(span));

#if TL_PROFILER_SUBTRACT_SELF_TIME
	info->self_time += get_performance_counter() - end_counter;
#endif
}
void Profiler::mark(Span<utf8> name, u32 color) {
	if (!enabled)
		return;

	scoped(thread_infos_lock);
	scoped(marks_lock);
	auto found = thread_infos.find(get_current_thread_id());
	assert(found);
	auto info = found.value;
#if TL_PROFILER_SUBTRACT_SELF_TIME
	marks.add({get_performance_counter() - info->self_time, color, get_current_thread_id()});
#else
	marks.add({get_performance_counter(), color, get_current_thread_id()});
#endif
}
void Profiler::reset() {
	marks.clear();
	recorded_time_spans.clear();
	thread_infos.clear();
	start_time = get_performance_counter();
}

List<ascii> Profiler::output_for_chrome() {
	StringBuilder builder;
	defer { tl::free(builder); };

	append(builder, R"({"otherData":{},"traceEvents":[
)"s);

	if (!recorded_time_spans.is_empty()) {
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
	defer { tl::free(builder); };
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

void ProfileRenderer::free() {
	for (auto &events_to_draw : all_events_to_draw) {
		tl::free(events_to_draw.events);
		tl::free(events_to_draw.marks);
	}
	tl::free(all_events_to_draw);
}

void ProfileRenderer::setup(Span<Profiler::TimeSpan> spans, Span<Profiler::Mark> marks) {
	events_begin = max_value<Nanoseconds>;
	events_end = min_value<Nanoseconds>;

	ContiguousHashMap<u32, ThreadDrawList, DefaultHashTraits<u32>, TemporaryAllocator> thread_id_to_events_to_draw;

	for (auto span : spans) {
		auto begin = (Nanoseconds)divide(multiply(span.begin, 1'000'000'000), performance_frequency);
		auto end = (Nanoseconds)divide(multiply(span.end, 1'000'000'000), performance_frequency);
		Event e = {
			.name = span.name,
			.begin = begin,
			.end = end,
			.self_duration = end - begin,
		};
		auto &t = thread_id_to_events_to_draw.get_or_insert(span.thread_id);
		t.id = span.thread_id;
		t.events.add(e);
		events_begin = min(events_begin, e.begin);
		events_end = max(events_end, e.end);
	}
	for (auto mark : marks) {
		Mark m = {
			.time = (Nanoseconds)divide(multiply(mark.counter, 1'000'000'000), performance_frequency),
		};
		auto &t = thread_id_to_events_to_draw.get_or_insert(mark.thread_id);
		t.id = mark.thread_id;
		t.marks.add(m);
		events_begin = min(events_begin, m.time);
		events_end = max(events_end, m.time);
	}

	events_duration = events_end - events_begin;

	thread_id_to_events_to_draw.for_each([&](auto kv) {
		auto &thread_id = *kv.key;
		auto &events_to_draw = *kv.value;
		quick_sort(events_to_draw.events, [](Event const &a, Event const &b) {
			if (a.begin != b.begin) {
				return a.begin < b.begin;
			}
			return a.duration() > b.duration();
		});

		List<Event *, TemporaryAllocator> event_stack;

		for (auto &event : events_to_draw.events) {
			//
			// Calculate depth
			//
			event.begin -= events_begin;
			event.end -= events_begin;


			while (1) {
				if (event_stack.count) {
					auto previous = event_stack.back();
					assert(event.begin >= previous->begin);
					if (event.begin >= previous->end) {
						event_stack.pop();
						continue;
					}
					previous->self_duration -= event.duration();
					event.depth = previous->depth + 1;
				} else {
					event.depth = 0;
				}
				event_stack.add(&event);
				break;
			}









		//	Event *parent = 0;

		//check_next_parent:
		//	if (event_stack.count) {
		//		parent = event_stack.back();
		//	}
		//	if (parent) {
		//		if (intersects(aabb_min_max(parent->begin, parent->end), aabb_min_max(event.begin, event.end))) {
		//			if (event.begin == parent->begin && event.duration() > parent->duration()) {
		//				// 'parent' is actually a child (deeper in the stack), and 'event' is parent,
		//				// so swap em
		//				if (parent->parent) {
		//					parent->parent->self_duration =
		//						parent->parent->self_duration
		//						+ parent->duration()
		//						- event.duration();
		//				}
		//				event.self_duration -= parent->duration();

		//				event.parent = parent->parent;
		//				event.depth  = parent->depth;
		//				parent->parent = &event;
		//				parent->depth += 1;
		//				event_stack.back() = &event;
		//				event_stack.add(parent);
		//			} else {
		//				event.parent = parent;
		//				event.depth = parent->depth + 1;
		//				event_stack.add(&event);
		//				parent->self_duration -= event.duration();
		//			}
		//			continue;
		//		} else {
		//			event_stack.pop();
		//			parent = 0;
		//			goto check_next_parent;
		//		}
		//	} else {
		//		event_stack.add(&event);
		//	}
		}

		for (auto &mark : events_to_draw.marks) {
			mark.time -= events_begin;
		}
	});

	thread_id_to_events_to_draw.set_to(all_events_to_draw);
	quick_sort(all_events_to_draw, [](ThreadDrawList t) { return t.id; });
}

}

#if TL_DEBUG
#pragma optimize("", on)
#endif

#endif
#endif
