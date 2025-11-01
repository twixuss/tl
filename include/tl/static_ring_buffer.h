#pragma once
#include "common.h"

namespace tl {

template <class T, umm _capacity>
struct StaticRingBuffer {
	inline static constexpr umm capacity = _capacity;

	union {
		T storage[capacity];
	};
	T *start = storage;
	umm count = 0;


	// Returns pointer to added value, or null if buffer is full.
	[[nodiscard]]
	T *push_back(T value = {}) {
		if (count == capacity)
			return 0;

		auto dst = start + count;
		count += 1;
		return &(*(dst - capacity * (dst >= storage + capacity)) = value);
	}

	// Returns true if all elements were added, or false if there is not enough space.
	// If you want to add whatever fits, call push_back on each element.
	[[nodiscard]]
	bool push_back(Span<T> span) {
		if (count + span.count > capacity)
			return false;

		if (start + count + span.count > storage + capacity) {
			auto first_count  = storage + capacity - start - count;
			auto second_count = span.count - first_count;
			memcpy(start + count, span.data, first_count * sizeof(T));
			memcpy(storage, span.data + first_count, second_count * sizeof(T));
		} else {
			memcpy(start + count, span.data, span.count * sizeof(T));
		}

		count += span.count;

		return true;
	}

	// Returns pointer to added value, or null if buffer is full.
	[[nodiscard]]
	T *push_front(T value = {}) {
		if (count == capacity)
			return 0;

		count += 1;
		start = sub_wrap(start, 1);
		return &(*start = value);
	}
	
	// Returns true if all elements were added, or false if there is not enough space.
	// If you want to add whatever fits, call push_back on each element.
	[[nodiscard]]
	bool push_front(Span<T> span) {
		if (count + span.count > capacity)
			return false;

		auto old_start = start;
		start -= span.count;
		if (start < storage) {
			start += capacity;
			
			auto first_count = old_start - storage;
			auto second_count = span.count - first_count;

			memcpy(start, span.data, first_count * sizeof(T));
			memcpy(storage, span.data + first_count, second_count * sizeof(T));
		} else {
			memcpy(start, span.data, span.count * sizeof(T));
		}

		count += span.count;

		return true;
	}

	Optional<T> pop_back() {
		if (!count)
			return {};
		defer { --count; };
		return at(count - 1);
	}
	Optional<T> pop_front() {
		if (!count)
			return {};
		auto result_ptr = start;
		start = add_wrap(start, 1);
		count -= 1;
		return *result_ptr;
	}

	auto &back(this auto &&self) { bounds_check(assert(self.count)); return self.at(self.count - 1); }
	auto &front(this auto &&self) { bounds_check(assert(self.count)); return self.at(0); }

	bool is_empty() const { return count == 0; }
	bool is_full() const { return count == capacity; }

	auto &operator[](this auto &&self, umm i) {
		bounds_check(assert_less(i, self.count));
		return self.at(i);
	}

	void clear() {
		start = storage;
		count = 0;
	}

	auto &at(this auto &&self, umm i) {
		bounds_check(assert_less(i, self.count));
		return *self.add_wrap(self.start, i);
	}

	
	template <bool is_const>
	struct Iterator {
		MakeConst<StaticRingBuffer, is_const> *buffer = 0;
		umm index = 0;
		auto &operator*() {
			return buffer->at(index);
		}
		Iterator &operator++() {
			++index;
			return *this;
		}
		bool operator==(Iterator that) const {
			return index == that.index;
		}
	};
	
	auto begin(this auto &&self) { return Iterator<tl_self_const>{&self, 0}; }
	auto end(this auto &&self) { return Iterator<tl_self_const>{&self, self.count}; }

	
	
	template <bool is_const>
	struct Iter {
		MakeConst<StaticRingBuffer, is_const> *buffer = 0;
		umm index = 0;
		auto &operator*() {
			return buffer->at(index);
		}
		void next() {
			++index;
		}
		explicit operator bool() {
			return index != buffer->count;
		}
		auto &value() {
			return buffer->at(index);
		}
		auto key() {
			return index;
		}
	};

	using IterOptions = Empty;

	auto iter(this auto &&self, IterOptions options = {}) {
		return Iter<tl_self_const>{&self, 0};
	}
	

private:
	auto add_wrap(auto *p, umm i) {
		p += i;
		return p - capacity * (p >= storage + capacity);
	}
	auto sub_wrap(auto *p, umm i) {
		p -= i;
		return p + capacity * (p < storage);
	}
};

}

#ifdef TL_ENABLE_TESTS

TL_TEST(StaticRingBuffer) {
	using namespace tl;

	StaticRingBuffer<int, 4> b;

	assert(*b.push_back(0) == 0);
	assert(*b.push_back(1) == 1);
	assert(*b.push_back(2) == 2);
	assert(*b.push_back(3) == 3);
	assert(b.count == 4);
	assert(b.push_back(5) == 0);
	assert(b.count == 4);

	assert(b.pop_back().value() == 3);
	assert(b.count == 3);
	assert(b[0] == 0);
	assert(b[1] == 1);
	assert(b[2] == 2);

	assert(b.pop_front().value() == 0);
	assert(b.count == 2);
	assert(b[0] == 1);
	assert(b[1] == 2);

	assert(b.push_back({6, 7}));
	assert(b.count == 4);
	assert(b[0] == 1);
	assert(b[1] == 2);
	assert(b[2] == 6);
	assert(b[3] == 7);

	assert(b.pop_back().value() == 7);
	assert(b.pop_back().value() == 6);
	assert(b.count == 2);
	assert(b[0] == 1);
	assert(b[1] == 2);

	
	assert(b.push_front({8, 9}));
	assert(b.count == 4);
	assert(b[0] == 8);
	assert(b[1] == 9);
	assert(b[2] == 1);
	assert(b[3] == 2);

	umm index = 0;

	for (auto x : b) {
		switch (index) {
			case 0: assert(x == 8); break;
			case 1: assert(x == 9); break;
			case 2: assert(x == 1); break;
			case 3: assert(x == 2); break;
			default: invalid_code_path();
		}
		++index;
	}
	assert(index == 4);

	index = 0;
	foreach(it, b) {
		switch (index) {
			case 0: assert(it.value() == 8); assert(it.key() == 0); break;
			case 1: assert(it.value() == 9); assert(it.key() == 1); break;
			case 2: assert(it.value() == 1); assert(it.key() == 2); break;
			case 3: assert(it.value() == 2); assert(it.key() == 3); break;
			default: invalid_code_path();
		}
		++index;
	}
	assert(index == 4);
};

#endif