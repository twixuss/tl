#pragma once
#include "common.h"

#ifndef TL_INITIAL_RING_BUFFER_CAPACITY
#define TL_INITIAL_RING_BUFFER_CAPACITY 8
#endif

namespace tl {

template <class T, class Allocator_ = Allocator>
struct RingBuffer {
	using Allocator = Allocator_;
	using Element = T;
	using Spans = StaticList<Span<T>, 2>;

	T *storage = 0;
	umm capacity = 0; // power of 2

	umm start = 0; // [0; capacity)
	umm count = 0; // [0; capacity]

	[[no_unique_address]] Allocator allocator = Allocator::current();

	T &push_back(T value = {}) {
		reserve(count + 1);
	
		return *new (get(start + count++)) T(value);
	}
	
	Spans push_back(Span<T> span) {
		Spans result;

		reserve(count + span.count);

		umm back_space = capacity - start - count;
		T *past_last = storage + start + count;

		if (span.count <= back_space) {
			memcpy(past_last, span.data, span.count * sizeof(T));

			result.add({past_last, span.count});
		} else {
			umm second_count = span.count - back_space;

			memcpy(past_last, span.data, back_space * sizeof(T));
			memcpy(storage, span.data + back_space, second_count * sizeof(T));

			result.add({past_last, back_space});
			result.add({storage, second_count});
		}
	
		count += span.count;
	
		return result;
	}
	
	T &push_front(T value = {}) {
		reserve(count + 1);
	
		count += 1;
		start = (start - 1) & (capacity - 1);
		return *new (storage + start) T(value);
	}
	
	Spans push_front(Span<T> span) {
		Spans result;

		reserve(count + span.count);
	
		umm front_space = start;
		if (span.count <= front_space) {
			memcpy(storage + start - span.count, span.data, span.count * sizeof(T));

			result.add({storage + start - span.count, span.count});
		} else {
			umm back_count = span.count - front_space;

			memcpy(storage + capacity - back_count, span.data, back_count * sizeof(T));
			memcpy(storage, span.data + back_count, front_space * sizeof(T));

			result.add({storage + capacity - back_count, back_count});
			result.add({storage, front_space});
		}

		count += span.count;
	
		return result;
	}
	
	Optional<T> pop_back() {
		if (!count)
			return {};
		return storage[--count & (capacity - 1)];
	}
	
	Optional<T> pop_front() {
		if (!count)
			return {};
		--count;
		auto result = storage[start];
		++start &= (capacity - 1);
		return result;
	}
	
	void pop_back(umm count_to_pop) {
		count_to_pop = min(count_to_pop, count);
		count -= count_to_pop;
	}
	
	void pop_front(umm count_to_pop) {
		count_to_pop = min(count_to_pop, count);

		count -= count_to_pop;
		start += count_to_pop;
		start &= (capacity - 1);
	}
	
	void clear() {
		start = 0;
		count = 0;
	}
	
	bool is_empty() const { return count == 0; }
	bool is_full() const { return count == capacity; }
	
	auto &back (this auto &&self) { bounds_check(assert(self.count)); return self.storage[(self.start + self.count - 1) & (self.capacity - 1)]; }
	auto &front(this auto &&self) { bounds_check(assert(self.count)); return self.storage[(self.start + 0) & (self.capacity - 1)]; }
	
	auto &operator[](this auto &&self, umm i) {
		bounds_check(assert_less(i, self.count));
		return self.storage[(self.start + i) & (self.capacity - 1)];
	}
	
	auto &at(this auto &&self, umm i) {
		bounds_check(assert_less(i, self.count));
		return self.storage[(self.start + i) & (self.capacity - 1)];
	}


	void reserve(umm desired_capacity) {
		if (desired_capacity <= capacity)
			return;
		
		umm new_capacity = ceil_to_power_of_2(max((umm)TL_INITIAL_RING_BUFFER_CAPACITY, desired_capacity));

		T *new_storage = allocator.template allocate_uninitialized<T>(new_capacity);

		if (start + count <= capacity) {
			memcpy(new_storage, storage + start, count * sizeof(T));
		} else {
			umm first_count = capacity - start;
			umm second_count = count - first_count;
			memcpy(new_storage, storage + start, first_count * sizeof(T));
			memcpy(new_storage + first_count, storage, second_count * sizeof(T));
		}

		allocator.free_t(storage, capacity);

		storage = new_storage;
		start = 0;
		capacity = new_capacity;
	}
	
	Spans spans() const {
		if (start + count <= capacity) {
			return {{ storage + start, count }};
		} else {
			return {
				{ storage + start, capacity - start},
				{ storage, count - capacity + start},
			};
		}
	}
	
	// Packs elements to be contiguous
	// 
	// defg_abc
	//  =>
	// abcdefg_
	Span<T> pack() {

		for (umm i = 0; i < count; ++i) {
			T *dst = &storage[i];
			T *src = &storage[(start + i) & (capacity - 1)];
			Swap(*dst, *src);
		}

		start = 0;

		return {storage, count};
	}

	void erase_all(auto &&predicate)
		requires requires { { predicate(storage[0]) } -> std::convertible_to<bool>; }
	{
		umm idst = 0;

		for (umm isrc = 0; isrc < count; ++isrc) {
			auto &src = storage[(start + isrc) & (capacity - 1)];
			if (predicate(src)) {
				// remove
				src.~T();
			} else {
				// keep
				auto &dst = storage[(start + idst) & (capacity - 1)];
				dst = (T &&)src;
				idst += 1;
			}
		}

		count = idst;
	}

	void free() {
		if (storage == 0)
			return;

		allocator.free(storage);
		start = 0;
		count = 0;
		storage = 0;
		capacity = 0;
	}

	template <bool is_const_>
	struct Iter {
		inline static constexpr bool is_const = is_const_;

		MakeConst<RingBuffer, is_const> *buffer = 0;
		umm index = 0;
		umm end = 0;
		s8 step = 0;

		auto &operator*() {
			return buffer->at(index);
		}
		void next() {
			index += step;
		}
		explicit operator bool() {
			return index != end;
		}
		auto &value() {
			return buffer->at(index);
		}
		auto key() {
			return index;
		}
	};
	
	using IterOptions = ReverseIterOption;
	
	auto iter(this auto &&self, IterOptions options = {}) {
		return Iter<tl_self_const>{
			.buffer = &self, 
			.index =       options.reverse ? self.count - 1 : 0,
			.end   = (umm)(options.reverse ? -1 : self.count),
			.step  = (s8) (options.reverse ? -1 : 1),
		};
	}
	
	
	auto begin(this auto &&self) { return IterAdapter{self.iter()}; }
	auto end(this auto &&self) { return Empty{}; }
	auto rbegin(this auto &&self) { return IterAdapter{self.iter({.reverse = true})}; }
	auto rend(this auto &&self) { return Empty{}; }
	
private:
	T *get(umm i) {
		return storage + (i & (capacity - 1));
	}
};

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	RingBuffer<int> b;

	b.reserve(8);
	assert(b.capacity == 8);

	auto check = [&](Span<int> expected, umm first_span_count) {
		assert(b.count == expected.count);
		for (umm i = 0; i < expected.count; ++i)
			assert(b[i] == expected[i]);
		
		if (expected.count == first_span_count) {
			assert(b.spans().count == 1);
		} else {
			assert(b.spans().count == 2);
		}

		assert(b.spans()[0].data == &b[0]);
		assert(b.spans()[0].count == first_span_count);
		for (umm i = 0; i < first_span_count; ++i)
			assert(b.spans()[0][i] == expected[i]);

		if (expected.count != first_span_count) {
			assert(b.spans()[1].data == &b[first_span_count]);
			assert(b.spans()[1].count == expected.count - first_span_count);
			for (umm i = 0; i < expected.count - first_span_count; ++i)
				assert(b.spans()[1][i] == expected[first_span_count + i]);
		}
		
		umm index = 0;
		foreach(it, b) {
			assert(it.key() == index);
			assert(it.value() == expected[index]);
			++index;
		}
		assert(index == expected.count);
		
		index = expected.count - 1;
		foreach(it, b.iter({.reverse = true})) {
			assert(it.key() == index);
			assert(it.value() == expected[index]);
			--index;
		}
		assert(index == -1);
		
		index = 0;
		for (auto &it : b) {
			assert(it == expected[index]);
			++index;
		}
		assert(index == expected.count);
		
		index = expected.count - 1;
		for (auto &it : reversed(b)) {
			assert(it == expected[index]);
			--index;
		}
		assert(index == -1);
	};

	b.push_back({0, 1, 2, 3, 4, 5});
	check({0, 1, 2, 3, 4, 5}, 6);
	// 0 1 2 3 4 5 . .
	// ^

	for (int i = 0; i < 3; ++i)
		b.pop_front();
	check({3, 4, 5}, 3);
	// . . . 3 4 5 . .
	//       ^

	b.push_back({6, 7, 8});
	check({3, 4, 5, 6, 7, 8}, 5);
	// 8 . . 3 4 5 6 7
	//       ^
	
	for (int i = 0; i < 3; ++i)
		b.pop_back();
	check({3, 4, 5}, 3);
	// . . . 3 4 5 . .
	//       ^

	b.push_front({9, 10, 11, 12, 13});
	check({3, 4, 5, 9, 10, 11, 12, 13}, 5);
	// 11 12 13 3 4 5 9 10
	//          ^
};

#endif