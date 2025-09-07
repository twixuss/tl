#pragma once
#include "common.h"

#pragma warning(push)
#pragma warning(disable : 4582 4624)

#ifndef TL_INITIAL_LIST_CAPACITY
#define TL_INITIAL_LIST_CAPACITY 4
#endif

#ifndef TL_INITIAL_QUEUE_CAPACITY
#define TL_INITIAL_QUEUE_CAPACITY TL_INITIAL_LIST_CAPACITY
#endif

namespace tl {

#pragma pack(push, 1)
template <class T, class Allocator_ = Allocator, class Size_ = umm>
struct List : Span<T, Size_> {
	using Element = T;
	using Allocator = Allocator_;
	using Size = Size_;
	using Span = Span<T, Size_>;

	using Span::data;
	using Span::count;
	using Span::begin;
	using Span::end;
	using Span::front;
	using Span::back;

	Size capacity = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	void set(T value TL_LP) {
		reserve(1 TL_LA);
		count = 1;
		memcpy(data, &value, sizeof(T));
	}
	void set(Span span TL_LP) {
		reserve(span.count TL_LA);
		count = span.count;
		memcpy(data, span.data, span.count * sizeof(T));
	}
	T &add(TL_LPC) {
		reserve_exponential(count + 1 TL_LA);
		return *new (data + count++) T();
	}
	T &add(T value TL_LP) {
		reserve_exponential(count + 1 TL_LA);
		memcpy(data + count, &value, sizeof(T));
		return data[count++];
	}
	template <class ThatSize>
	Span add(tl::Span<T, ThatSize> span TL_LP) {
		reserve_exponential(count + span.count TL_LA);
		memcpy(data + count, span.data, span.count * sizeof(T));
		count += span.count;
		return {data + count - span.count, (Size)span.count};
	}
	Span add(std::initializer_list<T> list TL_LP) {
		reserve_exponential(count + list.size() TL_LA);
		memcpy(data + count, list.begin(), list.size() * sizeof(T));
		count += list.size();
		return {data + count - list.size(), (Size)list.size()};
	}
	// I don't want to have `add` overload that takes `Repeat<T>` because
	// C++ can't select the wanted overload when using initializer list.
	// Even when it is clearly a different structure from Repeat, it still
	// thinks this overload should participate in resolution.
	Span add_repeat(T value, umm repeat_count TL_LP) {
		reserve_exponential(count + repeat_count TL_LA);
		for (umm i = 0; i < repeat_count; ++i) {
			memcpy(data + count + i, &value, sizeof(T));
		}
		count += repeat_count;
		return {data + count - repeat_count, repeat_count};
	}

	// Moves the data! Slow!
	T &add_front(T value TL_LP) {
		reserve_exponential(count + 1 TL_LA);
		memmove(data + 1, data, count * sizeof(T));
		data[0] = value;
		count += 1;
		return data[0];
	}
	
	void reallocate(Size desired_capacity TL_LP) {
		T *new_data;
		if (data) {
			new_data = allocator.template reallocate_uninitialized<T>(data, capacity, desired_capacity TL_LA);
		} else {
			new_data = allocator.template allocate_uninitialized<T>(desired_capacity TL_LA);
		}
		data = new_data;
		capacity = desired_capacity;
	}

	// Reserves no more than `desired_capacity` elements.
	// Returns true if `data` was relocated.
	bool reserve(Size desired_capacity TL_LP) {
		if (capacity >= desired_capacity) {
			return false;
		}

		reallocate(desired_capacity TL_LA);
		return true;
	}

	// Returns true if `data` was relocated
	bool reserve_exponential(Size desired_capacity TL_LP) {
		if (capacity >= desired_capacity) {
			return false;
		}

		Size new_capacity = ceil_to_power_of_2(max((Size)TL_INITIAL_LIST_CAPACITY, desired_capacity));

		reallocate(new_capacity TL_LA);

		return true;
	}
	void resize(Size new_count, T new_value = {} TL_LP) {
		reserve(new_count TL_LA);

		for (Size i = count; i < new_count; ++i) {
			new (data + i) T(new_value);
		}

		count = new_count;
	}

	void reserve_after_end(Size desired_space) {
		reserve(count + desired_space);
	}

	void clear() {
		count = 0;
	}

	Optional<T> pop() {
		if (count == 0)
			return {};

		--count;
		return data[count];
	}

	template <class U, class ThatSize>
	explicit operator List<U, Allocator, ThatSize>() const {
		assert_equal((ThatSize)count, count);
		assert_equal((ThatSize)capacity, capacity);
		List<U, Allocator, ThatSize> result;
		result.allocator = allocator;
		result.data = (U *)data;
		if constexpr (sizeof(T) == sizeof(U)) {
			result.count      = count;
			result.capacity  = capacity;
		} else {
			if constexpr (sizeof(T) > sizeof(U)) {
				static_assert(sizeof(T) % sizeof(U) == 0);
			} else {
				static_assert(sizeof(U) % sizeof(T) == 0);
			}
			result.count      = count     * sizeof(T) / sizeof(U);
			result.capacity  = capacity * sizeof(T) / sizeof(U);
		}
		return result;
	}

	template <class ThatSize>
	operator List<T, Allocator, ThatSize>() const {
		assert_equal((ThatSize)count, count);
		assert_equal((ThatSize)capacity, capacity);
		List<T, Allocator, ThatSize> result;
		result.data = data;
		result.count = (ThatSize)count;
		result.capacity = (ThatSize)capacity;
		result.allocator = allocator;
		return result;
	}

	T &insert_at(T value, Size where TL_LP) {
		bounds_check(assert(where <= count));

		reserve_exponential(count + 1 TL_LA);

		memmove(data + where + 1, data + where, (count - where) * sizeof(T));
		memcpy(data + where, &value, sizeof(T));

		++count;
		return data[where];
	}
	Span insert_at(Span span, Size where TL_LP) {
		bounds_check(assert(where <= count));

		reserve_exponential(count + span.count TL_LA);

		memmove(data + where + span.count, data + where, (count - where) * sizeof(T));
		memcpy(data + where, span.data, span.count * sizeof(T));

		count += span.count;
		return {data + where, span.count};
	}
	T &insert(T value, T *where TL_LP) { return insert_at(value, where - data TL_LA); }
	Span insert(Span span, T *where TL_LP) { return insert_at(span, where - data TL_LA); }

	Span insert_at(Repeat<T> repeat, Size where) {
		reserve(count + repeat.count);
		auto to_move_count = count - where;
		memmove(data + where + repeat.count, data + where, to_move_count * sizeof(T));
		for (Size i = 0; i != repeat.count; ++i)
			memcpy(data + where + i, &repeat.value, sizeof(T));
		count += repeat.count;
		return {data + where, repeat.count};
	}

	void erase(Span where_) {
		bounds_check(assert(begin() <= where_.begin() && where_.begin() <= end()));
		bounds_check(assert(begin() <= where_.end() && where_.end() <= end()));

		memmove(where_.data, where_.data + where_.count, (count - where_.count + data - where_.data) * sizeof(T));
		count -= where_.count;
	}
	Optional<T> erase_at(Size where) {
		if (where < count) {
			--count;
			auto result = data[where];
			for (Size i = where; i < count; ++i) {
				data[i] = data[i + 1];
			}
			return result;
		}
		return {};
	}
	
	void erase_all(auto &&predicate) 
		requires requires(T v) { predicate(v); }
	{
		count = erase_all_compacting(span(), predicate).count;
	}
	void erase_all(T const &value) {
		erase_all([&](T const &it) { return it == value; });
	}
	
	void here_map(auto &&map) requires requires(T v) { { map(v) } -> std::same_as<T>; } {
		auto end = data + count;
		for (auto it = data; it != end; ++it) {
			*it = map(*it);
		}
	}
	
	void here_map(auto &&map) requires requires(T v) { { map(v) } -> std::same_as<Optional<T>>; } {
		auto end = data + count;
		auto dst = data;
		for (auto it = data; it != end; ++it) {
			auto mapped = map(*it);
			if (mapped) {
				*dst++ = mapped.value();
			} else {
				it->~T();
				--count;
			}
		}
	}

	void erase(T *value) { 
		erase_at(value - data);
	}

	void erase_unordered_at(Size index) {
		bounds_check(assert(index < count));
		memcpy(data + index, &back(), sizeof(T));
		--count;
	}

	void erase_unordered(T *at) {
		*at = back();
		count--;
	}

	
	// Same as:
	// index = list.index_of(where.begin());
	// erase(where);
	// insert_at(with_what, index)
	void replace(Span where, T with_what) {
		bounds_check(assert(where.count <= count));
		bounds_check(assert(begin() <= where.begin() && where.begin() < end()));
		bounds_check(assert(where.end() <= end()));

		memmove(where.data + 1, where.data + where.count, (end() - where.end()) * sizeof(T));
		*where.data = with_what;

		count -= where.count - 1;
	}
	
	// Same as:
	// index = list.index_of(where.begin());
	// erase(where);
	// insert_at(with_what, index)
	void replace(Span where, Span with_what) {
		bounds_check(assert(begin() <= where.begin() && where.begin() <= end()));
		bounds_check(assert(begin() <= where.end()   && where.end()   <= end()));

		umm where_index = where.data - data;
		reserve_exponential(count - where.count + with_what.count);
		where.data = data + where_index;

		memmove(
			where.data + with_what.count,
			where.data + where.count,
			(end() - where.end()) * sizeof(T)
		);
		memcpy(where.data, with_what.data, with_what.count * sizeof(T));

		count -= where.count - with_what.count;
	}

	// Same as:
	// index = list.index_of(to);
	// value = list.erase(from);
	// list.insert_at(value, index)
	void move(T *from, T *to) {
		T temp = *from;
		if (to < from) {
			memmove(to + 1, to, sizeof(T) * (from - to));
		} else {
			memmove(from, from + 1, sizeof(T) * (to - from));
		}
		*to = temp;
	}
	
	Span span() const { return *this; }

	template <bool is_const>
	struct Iter {
		using CList = MakeConst<List, is_const>;
		using CT = MakeConst<T, is_const>;

		CList *list = 0;
		CT *it = 0;
		CT *end = 0;
		s8 step = 0;

		explicit operator bool() { return it != end; }
		void next() { it += step; }
		umm key() { return it - list->data; }
		CT &value() { return *it; }

		// must `continue` right after
		void erase() {
			list->erase(it);
			if (step == 1) {
				it -= 1;
				end = list->data + list->count;
			}
		}
		// must `continue` right after
		void erase_unordered() {
			list->erase_unordered(it);
			if (step == 1) {
				it -= 1;
				end = list->data + list->count;
			}
		}

		CT &operator*() { return value(); }
		std::pair<umm, CT &> key_value() { return {key(), value()}; }
	};

	using IterOptions = ReverseIterOption;
	
	//auto iter(this auto &&self, IterOptions options = {}) {
	//	return Iter<tl_self_const>{
	//		.list = &self,
	//		.it = options.reverse ? self.data + self.count - 1 : self.data,
	//		.end = options.reverse ? self.data - 1 : self.data + self.count,
	//		.step = options.reverse ? -1 : 1,
	//	};
	//}
	auto iter(IterOptions options = {}) {
		return Iter<false>{
			.list = this,
			.it = options.reverse ? data + count - 1 : data,
			.end = options.reverse ? data - 1 : data + count,
			.step = options.reverse ? -1 : 1,
		};
	}
	auto iter(IterOptions options = {}) const {
		return Iter<true>{
			.list = this,
			.it = options.reverse ? data + count - 1 : data,
			.end = options.reverse ? data - 1 : data + count,
			.step = options.reverse ? -1 : 1,
		};
	}
};
#pragma pack(pop)

template <class T, class Allocator, class Size>
void free(List<T, Allocator, Size> &list) {
	if (list.data == 0) return;

	list.allocator.free_t(list.data, list.count);
	list.data = 0;
	list.count = 0;
	list.capacity = 0;
}

template <class T, class Allocator, class Size>
List<T, Allocator, Size> copy(List<T, Allocator, Size> that TL_LP) {
	List<T, Allocator, Size> result;
	result.set(as_span(that));
	return result;
}

template <class Allocator = Allocator, class T>
List<T, Allocator> to_list(std::initializer_list<T> that, Allocator allocator = Allocator::current() TL_LP) {
	List<T, Allocator> result;
	result.allocator = allocator;
	result.set(as_span(that));
	return result;
}

template <class Allocator = Allocator, class Size, class T>
List<T, Allocator, Size> to_list(Span<T, Size> that, Allocator allocator = Allocator::current() TL_LP) {
	List<T, Allocator, Size> result;
	result.allocator = allocator;
	result.set(as_span(that));
	return result;
}

template <class Allocator = Allocator, class Size = umm>
auto to_list(AnIter auto iter TL_LP) {
	List<std::remove_cvref_t<decltype(*iter)>, Allocator, Size> result;

	foreach (it, iter) {
		result.add(*it TL_LA);
	}

	return result;
}

template <class Allocator = Allocator, Iterable InputCollection>
auto to_list(InputCollection &&collection, typename std::remove_cvref_t<InputCollection>::IterOptions options = {}) {
	List<std::remove_cvref_t<decltype(collection.iter().value())>, Allocator> result;
	result.reserve(count_of(collection));
	foreach(it, collection.iter(options)) {
		result.add(it.value());
	}
	return result;
}

template <class Allocator = Allocator, Iterable InputCollection>
auto to_list_with_keys(InputCollection &&collection, typename std::remove_cvref_t<InputCollection>::IterOptions options = {}) {
	List<typename std::remove_cvref_t<InputCollection>::KeyValue, Allocator> result;
	result.reserve(count_of(collection));
	foreach(it, collection.iter(options)) {
		result.add(it.key_value());
	}
	return result;
}

template <class T, class Allocator, class Size>
T *next(List<T, Allocator, Size> list, T *value) {
	return ++value == list.end() ? 0 : value;
}

template <class T, class Allocator, class Size>
T *previous(List<T, Allocator, Size> list, T *value) {
	return value-- == list.data ? 0 : value;
}

template <class T, class Allocator, class Size> constexpr T *find(List<T, Allocator, Size> list, T const &value) { return find(as_span(list), value); }
template <class T, class Allocator, class Size> constexpr T *find(List<T, Allocator, Size> list, Span<T> cmp) { return find(as_span(list), cmp); }
template <class T, class Allocator, class Size> constexpr T *find_last(List<T, Allocator, Size> list, T const &value) { return find_last(as_span(list), value); }

template <class T>
void find_all(Span<T> where, Span<T> what, auto &&on_find) {
	if (what.count > where.count)
		return;

	for (umm where_start = 0; where_start != where.count - what.count + 1; ++where_start) {
		for (umm what_index = 0; what_index < what.count; ++what_index) {
			if (where.data[where_start + what_index] != what.data[what_index]) {
				goto continue_where;
			}
		}

		on_find(Span<T>(where.data + where_start, what.count));

	continue_where:;
	}
}

template <class T, class Allocator = Allocator, class Size = umm>
List<Span<T>, Allocator, Size> find_all(Span<T> where, Span<T> what) {
	List<Span<T>, Allocator, Size> result;
	find_all(where, what, [&](auto v) { result.add(v); });
	return result;
}

template <class T, class Allocator, class Size>
Span<T> as_span(List<T, Allocator, Size> const &list) {
	return (Span<T>)list;
}

template <class T, class Allocator, class Size, class Predicate>
constexpr T *find_if(List<T, Allocator, Size> &list, Predicate &&predicate) {
	return find_if(as_span(list), std::forward<Predicate>(predicate));
}

template <class T, class Allocator, class Size, class Predicate>
constexpr T const *find_if(List<T, Allocator, Size> const &list, Predicate &&predicate) {
	return find_if(as_span(list), std::forward<Predicate>(predicate));
}

template <class T, class Allocator, class Size, class Fn>
constexpr bool for_each(List<T, Allocator, Size> &list, Fn &&fn) {
	using ReturnType = decltype(fn(*(T*)0));
	for (auto it = list.begin(); it != list.end(); ++it) {
		if constexpr (std::is_same_v<ReturnType, void>) {
			fn(*it);
		} else if constexpr (std::is_same_v<ReturnType, ForEachDirective>) {
			auto d = fn(*it);

			assert(!((d & ForEach_erase) && (d & ForEach_erase_unordered)));
			if (d & ForEach_erase) list.erase(it--);
			if (d & ForEach_erase_unordered) list.erase_unordered(it--);
			if (d & ForEach_break) return true;
		} else {
			static_assert("Invalid return type in for_each iterator");
		}
	}
	return false;
}

template <class T, class Allocator, class Size, class Fn>
umm count(List<T, Allocator, Size> list, Fn &&fn) {
	umm result = 0;
	for (auto &v : list) {
		if (fn(v)) {
			result += 1;
		}
	}
	return result;
}

template <class Allocator = Allocator, class T, class Size>
List<T, Allocator, Size> replace(Span<T, Size> where, T what, T with TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(where.count TL_LA);
	for (auto &v : where) {
		result.add(v == what ? with : v);
	}
	return result;
}

template <class Allocator = Allocator, class T, class Size>
List<T, Allocator, Size> erase_all(Span<T, Size> where, T what TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(where.count TL_LA);
	for (auto &v : where) {
		if (v != what)
			result.add(v);
	}
	return result;
}

template <class Allocator = Allocator, class T, class Size, class Fn>
auto map(Span<T, Size> span, Fn &&fn TL_LP) {
	using U = decltype(fn(*(T*)0));
	List<U, Allocator, Size> result;
	result.reserve(span.count TL_LA);

	for (auto &x : span) {
		result.add(fn(x));
	}

	return result;
}

template <class DstAllocator, class DstSize, class Collection>
void set_to(List<typename std::remove_cvref_t<Collection>::Element, DstAllocator, DstSize> &list, Collection &&collection, typename std::remove_cvref_t<Collection>::IterOptions options = {} TL_LP) {
	list.clear();
	list.reserve(count_of(collection) TL_LA);
	foreach(it, collection.iter(options)) {
		list.add(it.value());
	}
}

template <class DstAllocator, class DstSize, class Collection>
void set_to(List<typename std::remove_cvref_t<Collection>::KeyValue, DstAllocator, DstSize> &list, Collection &&collection, typename std::remove_cvref_t<Collection>::IterOptions options = {} TL_LP) {
	list.clear();
	list.reserve(count_of(collection) TL_LA);
	foreach(it, collection.iter(options)) {
		list.add(it.key_value());
	}
}

template <class Allocator = Allocator, class InputCollection>
auto map(InputCollection &&collection, typename std::remove_cvref_t<InputCollection>::IterOptions options, auto mapper) requires requires { mapper(collection.iter().value()); } {
	List<ElementOf<std::remove_cvref_t<InputCollection>>, Allocator> result;
	result.reserve(count_of(collection));
	foreach(it, collection) {
		result.add();
	}
	return result;
}
template <class Allocator = Allocator, class InputCollection>
auto map(InputCollection &&collection, auto mapper) requires requires { mapper(collection.iter().value()); } {
	return map(collection, {}, mapper);
}


template <class Allocator = Allocator, class T, class Size = umm>
List<T, Allocator, Size> make_list(std::initializer_list<T> list TL_LP) {
	List<T, Allocator, Size> result;
	result.reserve(list.size() TL_LA);
	result.count = list.size();
	memcpy(result.data, list.begin(), list.size() * sizeof(T));
	return result;
}

// Returns an index of the value
// If value is not in the list, this function will return index >= list.size
template <class T, class Allocator, class Size>
umm index_of(List<T, Allocator, Size> const &list, T const *value) {
	return value - list.data;
}

template <class T, class Allocator, class Size> umm count_of(List<T, Allocator, Size> const &list) { return list.count; }

template <class T, class Allocator, class Size> T const &front(List<T, Allocator, Size> const &list) { return list.front(); }
template <class T, class Allocator, class Size> T &front(List<T, Allocator, Size> &list) { return list.front(); }

template <class T, class Allocator, class Size> T const &back(List<T, Allocator, Size> const &list) { return list.back(); }
template <class T, class Allocator, class Size> T &back(List<T, Allocator, Size> &list) { return list.back(); }


#pragma warning(pop)

}

#ifdef TL_ENABLE_TESTS

TL_TEST(List) {
	using namespace tl;

	List<int> l;

	assert(l.count == 0);
	assert(l.capacity == 0);
	assert(l.data == 0);
	l.add(42);
	assert(l.count == 1);
	assert(l[0] == 42);
	l.add(69);
	assert(l.count == 2);
	assert(l[0] == 42);
	assert(l[1] == 69);

	assert(l.pop().value() == 69);

	assert(l.count == 1);

	l.add();
	l.add(1);
	l.add(2);

	List<int> other;
	set_to(other, l, {.reverse = true});
	assert(other[0] == 2);
	assert(other[1] == 1);
	assert(other[2] == 0);
	assert(other[3] == 42);


	Span<int> expected = {42, 0, 1, 2};
	umm index = 0;
	for (auto x : l) {
		assert(x == expected[index]);
		++index;
	}
	assert(index == 4);


	index = 0;
	foreach (it, l) {
		assert(it.key() == index);
		assert(it.value() == expected[index]);
		++index;
	}
	assert(index == 4);

	
	index = 3;
	for (auto x : reversed(l)) {
		assert(x == expected[index]);
		--index;
	}
	assert(index == -1);


	index = 3;
	foreach (it, l.iter({.reverse = true})) {
		assert(it.key() == index);
		assert(it.value() == expected[index]);
		--index;
	}
	assert(index == -1);


	free(l);
};

#endif
