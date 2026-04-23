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

	T &add_back(T value = {}) {
		reserve(count + 1);
		T *d = storage + ((start + count) & (capacity - 1));
		memcpy(d, &value, sizeof(T));
		count += 1;
		return *d;
	}
	
	Spans add_back(Span<T> span) {
		Spans result;

		reserve(count + span.count);

		auto dst = unused_spans();

		if (span.count <= dst[0].count) {
			memcpy(dst[0].data, span.data, span.count * sizeof(T));
			
			result.add({dst[0].data, span.count});
		} else {
			memcpy(dst[0].data, span.data, dst[0].count * sizeof(T));
			memcpy(dst[1].data, span.data + dst[0].count, (span.count - dst[0].count) * sizeof(T));

			result.add(dst[0]);
			result.add({dst[1].data, span.count - dst[0].count});
		}

		count += span.count;

		return result;
	}
	
	T &add_front(T value = {}) {
		reserve(count + 1);
		start = (start - 1) & (capacity - 1);
		T *d = storage + start;
		memcpy(d, &value, sizeof(T));
		count += 1;
		return *d;
	}
	
	Spans add_front(Span<T> span) {
		Spans result;

		reserve(count + span.count);

		auto dst = unused_spans();

		if (span.count <= dst.back().count) {
			memcpy(dst.back().end() - span.count, span.data, span.count * sizeof(T));

			result.add({dst.back().end() - span.count, span.count});
		} else {
			memcpy(dst[0].end() - (span.count - dst[1].count), span.data, (span.count - dst[1].count) * sizeof(T));
			memcpy(dst[1].data, span.end() - dst[1].count, dst[1].count * sizeof(T));

			result.add({dst[0].end() - (span.count - dst[1].count), span.count - dst[1].count});
			result.add(dst[1]);
		}

		count += span.count;
		start = (start - span.count) & (capacity - 1);

		return result;
	}
	
	Optional<T> pop_back() {
		if (!count)
			return {};
		count -= 1;
		auto p = storage + ((start + count) & (capacity - 1));
		auto value = *p;
		mark_dead(p);
		return value;
	}
	
	Optional<T> pop_front() {
		if (!count)
			return {};
		count -= 1;
		auto p = storage + start;
		auto value = *p;
		mark_dead(p);
		start = (start + 1) & (capacity - 1);
		return value;
	}
	
	void pop_back(umm n) {
		n = min(n, count);

		// TODO: mark_dead		
		count -= n;
	}
	
	void pop_front(umm n) {
		n = min(n, count);

		// TODO: mark_dead		
		count -= n;
		start += n;
		start &= (capacity - 1);
	}
	
	void clear() {
		start = 0;
		count = 0;
		mark_dead(storage, capacity);
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

		auto spans = this->spans();
		memcpy(new_storage, spans[0].data, spans[0].count * sizeof(T));
		if (spans.count == 2) {
			memcpy(new_storage + spans[0].count, spans[1].data, spans[1].count * sizeof(T));
		}
		mark_dead(new_storage + count, capacity - count);

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

	// For internal use.
	// Returns spans to unused elements in order after last element until start.
	Spans unused_spans() const {
		if (start + count <= capacity) {
			return {
				{storage + start + count, capacity - start - count},
				{storage, start},
			};
		} else {
			return {{storage + start + count - capacity, capacity - count}};
		}
	}
	
	// Packs elements to be contiguous, so all of them are accesible through a single Span.
	// 
	// defg_abc
	//  =>
	// abcdefg_
	Span<T> pack() {
		if (start + count <= capacity) {
			return {storage + start, count};
		} else {
			#if 0
			// Swap last with its destination

			// f . . a b c d e
			//         |     |
			// f . . a e c d b
			//   |           |
			// f b . a e c d .
			//             | |
			// f b . a e c . d
			//       |       |
			// f b . d e c . a
			// |             |
			// a b . d e c . f
			//           |   |
			// a b . d e f . c
			//     |         |
			// a b c d e f . .
			//               |
			// a b c d e f . .

			umm dst = capacity - start - 1;

			for (umm i = 0; i < capacity; ++i) {
				Swap(storage[capacity-1], storage[dst & (capacity - 1)]);
				dst -= start;
			}
			#else
			// Swap contiguous ranges
			
			// d e f . . a b c
			// |         |
			// a e f . . d b c
			//   |         |
			// a b f . . d e c
			//     |         |
			// a b c . . d e f
			//       |   |
			// a b c d . . e f
			//         |   |
			// a b c d e . . f
			//           |   |
			// a b c d e f . .
			//             |  
			// a b c d e f . .

			umm s = start;

			for (umm d = 0; d < count; ++d) {
				while (s < d) {
					// s = (s + start) & (capacity - 1); // Add one by one until s is bigger
					s = (s + (d - s + start - 1) / start * start) & (capacity - 1); // Add some multiple of `start` at once. Less iterations, but with a division. Needs benchmarking.
				}

				Swap(storage[d], storage[s]);
				s = (s + 1) & (capacity - 1);
			}

			#endif
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
	auto end(this auto &&self) { (void)self; return Empty{}; }
	auto rbegin(this auto &&self) { return IterAdapter{self.iter({.reverse = true})}; }
	auto rend(this auto &&self) { (void)self; return Empty{}; }
	
private:
	T *get(umm i) {
		return storage + (i & (capacity - 1));
	}
};

}

#ifdef TL_ENABLE_TESTS

#include "list.h"

TL_TEST(RingBuffer) {
	using namespace tl;

	RingBuffer<int> b;

	// TL_INITIAL_RING_BUFFER_CAPACITY might be bigger than 8, but these tests are written for capacity of exactly 8
	//b.reserve(8);
	b.capacity = 8;
	b.storage = b.allocator.allocate<int>(b.capacity);

	auto check = [&](Span<int> expected, umm expected_start, umm first_span_count) {
		assert(b.start == expected_start);
		assert(b.count == expected.count);
		for (umm i = 0; i < expected.count; ++i)
			assert(b[i] == expected[i]);
		
		if (expected.count == first_span_count) {
			assert(b.spans().count == 1);
		} else {
			assert(b.spans().count == 2);
		}

		assert(b.spans()[0].data == (b.storage + b.start));
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

	b.add_front({0, 1, 2});
	check({0, 1, 2}, 5, 3);
	// . . . . . 0 1 2
	//           ^

	b.pop_front(3);
	check({}, 0, 0);
	// . . . . . . . .
	// ^

	b.add_back({0, 1, 2, 3, 4, 5});
	check({0, 1, 2, 3, 4, 5}, 0, 6);
	// 0 1 2 3 4 5 . .
	// ^

	for (int i = 0; i < 3; ++i)
		b.pop_front();
	check({3, 4, 5}, 3, 3);
	// . . . 3 4 5 . .
	//       ^

	b.add_back({6, 7, 8});
	check({3, 4, 5, 6, 7, 8}, 3, 5);
	// 8 . . 3 4 5 6 7
	//       ^
	
	b.add_back({1, 2});
	check({3, 4, 5, 6, 7, 8, 1, 2}, 3, 5);
	// 8 1 2 3 4 5 6 7
	//       ^

	b.pop_back(2);
	check({3, 4, 5, 6, 7, 8}, 3, 5);
	// 8 . . 3 4 5 6 7
	//       ^

	b.add_front({1, 2});
	check({1, 2, 3, 4, 5, 6, 7, 8}, 1, 7);
	// 8 1 2 3 4 5 6 7
	//   ^

	b.pop_front(2);
	check({3, 4, 5, 6, 7, 8}, 3, 5);
	// 8 . . 3 4 5 6 7
	//       ^

	for (int i = 0; i < 3; ++i)
		b.pop_back();
	check({3, 4, 5}, 3, 3);
	// . . . 3 4 5 . .
	//       ^

	b.add_front({9, 10, 11, 12, 13});
	check({9, 10, 11, 12, 13, 3, 4, 5}, 6, 2);
	// 11 12 13 3 4 5 9 10
	//                ^






	/* pack */ {
		const int capacity = 32;
		int arr[capacity];
		RingBuffer<int> b;
		b.storage = arr;
		b.capacity = capacity;

		// h a b c d e f g
		// a h b c d e f g
		// a b h c d e f g
		// a b c h d e f g
		// a b c d h e f g
		// a b c d e h f g
		// a b c d e f h g
		// a b c d e f g h

		for (int start = 0; start < capacity; ++start) {
			for (int count = 0; count <= capacity; ++count) {
				b.start = start;
				b.count = count;
				memset(arr, -1, sizeof(arr));
				for (int i = 0; i < count; ++i) {
					arr[(start + i) & (capacity - 1)] = i;
				}

				auto span = b.pack();
				//assert(span.data == arr); // no longer in contract
				assert(span.count == count);
				for (int i = 0; i < count; ++i) {
					assert(b[i] == i);
				}
			}
		}
	}

	/* stress */ if (0) {
		List<List<int>> a;
		RingBuffer<List<int>> b;

		int to_push = 0;
		int expected_pop = 0;
		for (int i = 0; i < 1024*1024; ++i) {
			int x = i;
			int k = 2654435789;
			x = x*k^k;
			x = x*k^k;
			x = x*k^k;
			x = x*k^k;

			if ((x & 0x8000'0000) || (a.count == 0)) {
				List<int> al, bl;
				al.add(to_push);
				bl.add(to_push);
				a.add(al);
				b.add_back(bl);
				++to_push;
			} else {
				auto av = a.pop_front().value();
				auto bv = b.pop_front().value();
				assert(av == bv);
				assert(av == Span(&expected_pop, 1));
				++expected_pop;
			}

			assert(a.count == b.count);
			for (int j = 0; j < a.count; ++j) {
				assert(a[j] == b[j]);
			}
		}

		while (a.count) {
			auto av = a.pop_front().value();
			auto bv = b.pop_front().value();
			assert(av == bv);
			assert(av == Span(&expected_pop, 1));
			++expected_pop;
		}
	}
};

#endif