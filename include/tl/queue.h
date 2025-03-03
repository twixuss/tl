#pragma once
#include "common.h"
#include "static_list.h"

#ifndef TL_INITIAL_QUEUE_CAPACITY
#define TL_INITIAL_QUEUE_CAPACITY TL_INITIAL_LIST_CAPACITY
#endif

namespace tl {

// Uses ring buffer for storage
// Capacity is always power of 2 to replace mod with and
template <class T, class Allocator = Allocator>
struct Queue {
	T *buffer = 0;
	umm capacity = 0;
	umm start = 0;
	umm count = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	struct Iterator {
		Queue *queue;
		umm index; // relative to buffer

		Iterator &operator++() {
			++index;
			return *this;
		}
		Iterator operator++(int) {
			defer { ++*this; };
			return *this;
		}

		bool operator==(Iterator const &that) const { return index == that.index; }
		bool operator!=(Iterator const &that) const { return index != that.index; }

		T &operator*() { return queue->get(index); }
		T *operator->() { return &*this; }

		T &operator[](smm i) {
			umm x = index + i;
			bounds_check(assert_less_equal(queue->start, x));
			bounds_check(assert_less(x, queue->start + queue->count));
			return queue->get(x);
		}
	};

	Iterator begin() { return {this, start}; }
	Iterator end() { return {this, start + count}; }

	bool is_empty() const { return count == 0; }


	T &push(T const &value TL_LP) {
		reserve(count + 1 TL_LA);
		return *new(&get(start + count++)) T(value);
	}

	void push(Span<T> span TL_LP) {
		reserve(count + span.count TL_LA);
		for (auto &value : span) {
			new(&get(start + count++)) T(value);
		}
	}

	Optional<T> pop() {
		if (!count)
			return {};
		T &result = get(start);
		defer {
			result.~T();
			++start;
			--count;
		};
		return result;
	}
	
	T &operator[](umm i) {
		bounds_check(assert_less(i, count));
		return get(start + i);
	}
	T &operator[](umm i) const {
		bounds_check(assert_less(i, count));
		return get(start + i);
	}

	void clear() {
		for (T &it : *this) {
			it.~T();
		}
		start = 0;
		count = 0;
	}

	void reserve(umm required_count TL_LP) {
		if (required_count <= capacity)
			return;
		umm new_capacity = ceil_to_power_of_2(max((umm)TL_INITIAL_QUEUE_CAPACITY, required_count));
		reallocate(new_capacity TL_LA);
	}

	StaticList<Span<T>, 2> spans() const {
		if (start + count <= capacity) {
			return {{ buffer + start, buffer + count }};
		} else {
			return {
				{ buffer + start, buffer + capacity},
				{ buffer, buffer + start + count - capacity},
			};
		}
	}
	
	void free() {
		if (buffer == 0)
			return;

		allocator.free(buffer);
		start = 0;
		count = 0;
		buffer = 0;
		capacity = 0;
	}

private:
	T &get(umm index) { return buffer[index & (capacity - 1)]; }

	void reallocate(umm new_capacity TL_LP) {
		T *new_data = allocator.template allocate<T>(new_capacity TL_LA);

		for (umm i = 0; i < count; ++i) {
			auto &source = get(start + i);
			auto &destination = new_data[i];

			new(&destination) T(std::move(source));
			source.~T();
		}

		if (buffer)
			allocator.free_t(buffer, capacity);

		buffer = new_data;
		capacity = new_capacity;
		start = 0;
	}
};

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	Queue<int> q;

	assert(q.count == 0);
	q.push(42);
	assert(q.count == 1);
	q.push(69);
	assert(q.count == 2);
	q.push(23);
	assert(q.count == 3);

	assert(q.pop().value() == 42);

	assert(q.count == 2);

	assert(q[0] == 69);
	assert(q[1] == 23);

	q.free();
};

#endif
