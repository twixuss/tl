#pragma once
#include "common.h"

namespace tl {

template <class T, umm _capacity>
struct StaticList {
	using Element = T;
	using Iterator = T *;
	using Size = UintForCount<_capacity>;

	inline static constexpr umm capacity = _capacity;
	
	Size count = 0;
	union {
		T data[capacity];
	};

	forceinline constexpr StaticList() {}
	forceinline constexpr ~StaticList() {
		for (umm i = 0; i < count; ++i)
			data[i].~T();
	}

	template <umm that_capacity>
	constexpr StaticList(StaticList<T, that_capacity> const &that) {
		assert(that.count <= capacity);
		count = (Size)that.count;
		memcpy(data, that.data, count * sizeof(T));
	}

	template <umm that_capacity>
	constexpr StaticList(StaticList<T, that_capacity> &&that) {
		memcpy(data, that.data, that.count * sizeof(T));
		count = that.count;
		that.count = 0;
	}

	constexpr StaticList(std::initializer_list<T> that) {
		assert(that.size() <= capacity);
		count = (Size)that.size();
		memcpy(data, that.begin(), count * sizeof(T));
	}

	template <umm that_capacity>
	constexpr StaticList &operator=(StaticList<T, that_capacity> const &that) {
		return *new (this) StaticList(that);
	}

	template <umm that_capacity>
	constexpr StaticList &operator=(StaticList<T, that_capacity> &&that) {
		this->~StaticList();
		memcpy(data, that.data, that.count * sizeof(T));
		count = that.count;
		that.count = 0;
		return *this;
	}

	constexpr StaticList &operator=(std::initializer_list<T> that) {
		return *new (this) StaticList(that);
	}

	forceinline constexpr auto begin(this auto &&self) { return self.data; }
	forceinline constexpr auto end(this auto &&self) { return self.data + self.count; }

	forceinline constexpr ReverseIterator<T *> rbegin() { return data + count - 1; }
	forceinline constexpr ReverseIterator<T *> rend() { return data - 1; }

	forceinline constexpr bool empty() const { return count == 0; }
	forceinline constexpr bool full() const { return count == capacity; }

	forceinline constexpr T &front(this auto &&self) { bounds_check(assert(self.count)); return self.data[0]; }
	forceinline constexpr T &back(this auto &&self) { bounds_check(assert(self.count)); return self.data[self.count - 1]; }
	forceinline constexpr T &operator[](this auto &&self, umm i) { bounds_check(assert(self.count)); return self.data[i]; }

	constexpr void resize(umm new_count) {
		bounds_check(assert(new_count <= capacity));
		if (new_count > count) {
			for (umm i = count; i < new_count; ++i) {
				new (data + i) T();
			}
		}
		count = new_count;
	}
	constexpr T &insert_at(T value, umm where) {
		bounds_check(assert(where <= count));
		bounds_check(assert(count < capacity));

		memmove(data + where + 1, data + where, (count - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++count;
		return data[where];
	}
	constexpr Span<T> insert_at(Span<T> span, umm where) {
		bounds_check(assert(where <= count));
		bounds_check(assert(count + span.count <= capacity));

		memmove(data + where + span.count, data + where, (count - where) * sizeof(T));
		memcpy(data + where, span.data, span.count * sizeof(T));

		count = (Size)(count + span.count);
		return {data + where, span.count};
	}
	constexpr Span<T> insert(Span<T> span, T *where) {
		return insert_at(span, where - data);
	}

	forceinline constexpr StaticList &operator+=(T const &that) { add(that); return *this; }
	forceinline constexpr StaticList &operator+=(T &&that) { add(std::move(that)); return *this; }
	forceinline constexpr StaticList &operator+=(Span<T> that) { add(that); return *this; }
	template <umm capacity>
	forceinline constexpr StaticList &operator+=(StaticList<T, capacity> const &that) { add(that.span()); return *this; }
	forceinline constexpr StaticList &operator+=(std::initializer_list<T> that) { add(Span((T *)that.begin(), (T *)that.end())); return *this; }

	template <class Size = umm>
	forceinline constexpr Span<T, Size> span() const { return {data, count}; }

	template <class U, class ThatSize>
	constexpr explicit operator Span<U, ThatSize>() const {
		static_assert(sizeof(U) == sizeof(T));
		assert_equal((ThatSize)count, count);
		return {(U *)data, (ThatSize)count};
	}

	template <class ThatSize>
	constexpr operator Span<T, ThatSize>() const {
		assert_equal((ThatSize)count, count);
		return {data, (ThatSize)count};
	}

	forceinline constexpr T &add() {
		bounds_check(assert(!full()));
		return *new(data + count++) T();
	}
	
	forceinline constexpr T &add(T const &value) {
		bounds_check(assert(!full()));
		return data[count++] = value;
	}

	forceinline constexpr void add(Optional<T> value) {
		if (value) {
			bounds_check(assert(!full()));
			data[count++] = value.value();
		}
	}

	template <class Size>
	forceinline constexpr Span<T> add(Span<T, Size> span) {
		bounds_check(assert(count + span.count <= capacity));
		memcpy(data + count, span.data, span.count * sizeof(T));
		defer { count = (StaticList::Size)(count + span.count); };
		return {data + count, span.count};
	}
	forceinline constexpr Span<T> add(std::initializer_list<T> list) {
		return add(as_span(list));
	}

	constexpr Optional<T> pop() {
		if (count == 0)
			return {};
		return data[--count];
	}

	constexpr T pop_back() {
		bounds_check(assert(count));
		return data[--count];
	}
	constexpr T pop_front() {
		bounds_check(assert(count));
		T popped = *data;
		memmove(data, data + 1, --count * sizeof(T));
		return popped;
	}
	forceinline constexpr void pop_back_unordered() { erase_unordered(&back()); }
	forceinline constexpr void pop_front_unordered() { erase_unordered(begin()); }

	T erase_at(umm where) {
		bounds_check(assert(where < count));
		T erased = data[where];
		memmove(data + where, data + where + 1, (--count - where) * sizeof(T));
		return erased;
	}
	forceinline T erase(T *where) { return erase_at(where - data); }

	T erase_at_unordered(umm where) {
		bounds_check(assert(where < count));
		T erased = data[where];
		--count;
		if (count != where) {
			data[where] = data[count];
		}
		return erased;
	}
	forceinline T erase_unordered(T *where) { return erase_at_unordered(where - data); }

	void erase_at(umm where, umm amount) {
		if (amount == 0)
			return;
		bounds_check(assert(where + amount <= count));
		T *dst = data + where;
		T *src = dst + amount;
		umm cnt = end() - src;
		memmove(dst, src, cnt * sizeof(T));
		count -= amount;
	}

	constexpr void clear() {
		count = 0;
	}
	umm index_of(T const *value) const {
		return value - data;
	}
	
	template <bool is_const>
	struct Iter {
		using Value = MakeConst<T, is_const>;

		StaticList *list;

		T *end = 0;
		T *it = 0;
		int step = 0;

		explicit operator bool() { return it != end; }

		void next() {
			it += step;
		}

		umm key() { return it - list->data; }
		Value &value() { return *it; }

		Value &operator*() { return *it; }

		
		void erase() requires(!is_const) {
			list->erase(it);
			--it;
		}
		void erase_unordered() requires(!is_const) {
			list->erase_unordered(it);
			--it;
		}


		std::pair<umm, Value &> key_value() { return {key(), value()}; }
	};
	
	auto iter(this auto &&self, ReverseIterOption options = {}) {
		auto start = self.data;
		auto end = self.data + self.count;
		return Iter<tl_self_const>{
			.list = (StaticList *)&self,
			.end  = (T *)(options.reverse ? start - 1 : end),
			.it   = (T *)(options.reverse ? end - 1   : start),
			.step = options.reverse ? -1        : 1,
		};
	}
};

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	StaticList<int, 8> list;

	list.add(1);
	list.add({2, 3});

	assert(list.count == 3);
	assert(list[0] == 1);
	assert(list[1] == 2);
	assert(list[2] == 3);

	auto found = find(list, 2);
	assert(found == &list[1]);
};

#endif
