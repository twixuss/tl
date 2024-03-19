#pragma once

#include "common.h"

namespace tl {

struct BlockListIndex {
	umm block_index;
	umm value_index;
};

template <class T, umm block_capacity_>
struct StaticBlockList {
	static constexpr umm block_capacity = block_capacity_;
	struct Block : StaticList<T, block_capacity> {
		Block *next = 0;
		Block *previous = 0;

		umm available_space() const { return block_capacity - this->count; }
	};
	struct Index {
		Block *block;
		umm value_index;
	};

	struct Iterator {
		Block *block;
		umm value_index;

		Iterator &operator++() {
			++value_index;
			if (value_index >= block->count) {
				if (block->next) {
					block = block->next;
					value_index = 0;
				}
			}
			return *this;
		}
		Iterator operator++(int) {
			auto copy = *this;
			++*this;
			return copy;
		}

		Iterator &operator--() {
			if (value_index == 0) {
				block = block->previous;
				value_index = block->count - 1;
			} else {
				--value_index;
			}
			return *this;
		}
		Iterator operator--(int) {
			auto copy = *this;
			--*this;
			return copy;
		}

		bool operator==(Iterator that) {
			return block == that.block && value_index == that.value_index;
		}
		bool operator!=(Iterator that) {
			return !(*this == that);
		}

		T *operator->() { return &(*block)[value_index]; }
		T &operator*() { return (*block)[value_index]; }
	};

	Allocator allocator = current_allocator;
	Block first;
	Block *last = &first;
	Block *alloc_last = &first;

	StaticBlockList() = default;
	StaticBlockList(StaticBlockList const &that) = default;
	StaticBlockList(StaticBlockList &&that) = delete;
	StaticBlockList &operator=(StaticBlockList const &that) = default;
	StaticBlockList &operator=(StaticBlockList &&that) = delete;

	void clear() {
		for (auto block = &first; block != 0; block = block->next) {
			block->clear();
		}
		last = &first;
	}

	T &add(T value TL_LP) {
		auto dest_block = last;
		while (dest_block && (dest_block->available_space() == 0)) {
			assert(dest_block != dest_block->next);
			dest_block = dest_block->next;
		}
		if (!dest_block) {
			dest_block = allocator.allocate<Block>(TL_LAC);
			dest_block->next = 0;
			dest_block->previous = alloc_last;
			alloc_last->next = dest_block;
			last = alloc_last = dest_block;
		}
		return dest_block->add(value);
	}
	T &add(TL_LPC) { return add({} TL_LA); }

	bool empty() const { return (last == &first) && (first.count == 0); }

	T &back() { return last->back(); }
	T const &back() const { return last->back(); }

	T pop_back() {
		if (last == &first)
			bounds_check(first.count);

		T result = last->data[--last->count];
		if (last->count == 0 && last != &first) {
			last = last->previous;
		}
		return result;
	}

	template <class Fn>
	void for_each_block(Fn &&fn) const {
		auto block = &first;
		do {
			auto block_capacity = block->count;
			if (block_capacity) {
				fn(block->buffer, block_capacity);
			}
			block = block->next;
		} while (block);
	}

	T &operator[](Index index) {
#ifndef TL_NO_BOUNDS_CHECK
		{
			auto block = &first;
			do {
				if (block == index.block) {
					goto check_value_index;
				}
				block = block->next;
			} while (block);
			bounds_check(false);
		check_value_index:

			bounds_check(index.value_index < block->count);
		}
#endif
		return index.block->buffer[index.value_index].value;
	}
	T &operator[](BlockListIndex index) {
		auto block = &first;
		while (index.block_index--) {
			block = block->next;
			bounds_check(block);
		}
		bounds_check(index.value_index < block->count);
		return block->data[index.value_index];
	}
	T &operator[](umm index) {
		auto block = &first;
		while (index >= block->count) {
			index -= block->count;
			block = block->next;
			bounds_check(block);
		}
		return block->data[index];
	}

	Iterator begin() {
		return {&first, 0};
	}
	Iterator end() {
		return {last, last->count};
	}
};

template <class T, umm block_size>
umm count_of(StaticBlockList<T, block_size> const &list) {
	umm total_count = 0;
	for (auto block = &list.first; block != 0; block = block->next) {
		total_count += block->count;
	}
	return total_count;
}

template <class T, umm block_size>
T *next(StaticBlockList<T, block_size> &list, T *source) {
	auto block = &list.first;
	while (block) {
		if (&block->buffer->value <= source && source < &block->buffer->end()->value) {
			if (source + 1 < block->buffer->end()) {
				return source + 1;
			} else {
				if (block->next) {
					return &block->next->buffer->value;
				} else {
					return 0;
				}
			}
		}
		block = block->next;
	}
	return 0;
}

template <class T, umm block_size>
struct StaticBlockListIterator {
	T *pointer;
	typename StaticBlockList<T, block_size>::Block *block;
	bool operator!=(StaticBlockListIterator that) { return pointer != that.pointer; }
	StaticBlockListIterator &operator++() {
		++pointer;
		if (pointer >= block->end()) {
			block = block->next;
			pointer = &block->buffer->value;
		}
		return *this;
	}
	T &operator*() {
		return *pointer;
	}
};

template <class T, umm block_size>
StaticBlockListIterator<T, block_size> begin(StaticBlockList<T, block_size> &list) {
	return {&list.first.buffer->value, &list.first};
}

template <class T, umm block_size>
StaticBlockListIterator<T, block_size> end(StaticBlockList<T, block_size> &list) {
	return {};
}

template <class T, umm block_size, class Fn>
bool for_each(StaticBlockList<T, block_size> &list, Fn &&fn) {
	constexpr bool using_index = std::is_invocable_v<Fn, T&, BlockListIndex>;

	if constexpr (using_index) {
		using ReturnType = decltype(fn(*(T*)0, BlockListIndex{}));
		constexpr bool returns_directive = std::is_same_v<ForEachDirective, ReturnType>;

		BlockListIndex index = {};
		auto block = &list.first;
		do {
			for (auto &it : *block) {
				if constexpr (returns_directive) {
					auto d = fn(it, index);
					if (d & ForEach_erase) not_implemented();
					if (d & ForEach_erase_unordered) not_implemented();
					if (d & ForEach_break)
						return true;
				} else {
					fn(it, index);
				}
				++index.value_index;
			}
			block = block->next;
			++index.block_index;
			index.value_index = 0;
		} while (block);
	} else {
		using ReturnType = decltype(fn(*(T*)0));
		constexpr bool returns_directive = std::is_same_v<ForEachDirective, ReturnType>;

		auto block = &list.first;
		do {
			for (auto it : *block) {
				if constexpr (returns_directive) {
					auto d = fn(it);
					if (d & ForEach_erase) not_implemented();
					if (d & ForEach_erase_unordered) not_implemented();
					if (d & ForEach_break)
						return true;
				} else {
					fn(it);
				}
			}
			block = block->next;
		} while (block);
	}
	return false;
}

template <class T, umm block_size>
T *find(StaticBlockList<T, block_size> &list, T const &to_find, BlockListIndex *result_index = 0) {
	BlockListIndex index = {};
	auto block = &list.first;
	do {
		index.value_index = 0;
		for (auto it = block->buffer; it != block->end; ++it) {
			if (it->value == to_find) {
				if (result_index)
					*result_index = index;
				return &it->value;
			}
			++index.value_index;
		}
		block = block->next;
		++index.block_index;
	} while (block);

	return 0;
}

template <class T, umm block_size>
T *find(StaticBlockList<T, block_size> &list, T const &to_find, typename StaticBlockList<T, block_size>::Index *result_index = 0) {
	typename StaticBlockList<T, block_size, Allocator>::Index index = {};
	auto block = &list.first;
	do {
		index.block = block;
		index.value_index = 0;
		auto block_capacity = block->size();
		for (auto it = block->buffer; it != block->end; ++it) {
			if (it == to_find) {
				if (result_index)
					*result_index = index;
				return &it;
			}
			++index.value_index;
		}
		block = block->next;
	} while (block);

	return 0;
}

template <class T, umm block_size, class Predicate>
T *find_if(StaticBlockList<T, block_size> &list, Predicate &&predicate) {
	auto block = &list.first;
	do {
		for (auto it = block->buffer; it != block->end; ++it) {
			if (predicate(it->value)) {
				return &it->value;
			}
		}
		block = block->next;
	} while (block);

	return 0;
}

template <class T, umm destination_block_size, umm source_block_size>
void add(StaticBlockList<T, destination_block_size> *destination, StaticBlockList<T, source_block_size> source TL_LP) {
	for_each(source, [&](T const &value) {
		destination->add(value TL_LA);
	});
}


template <class T, umm block_size>
List<T> to_list(StaticBlockList<T, block_size> list TL_LP) {
	List<T> result;
	result.reserve(count_of(list) TL_LA);
	for_each(list, [&](T const &value) {
		result.add(value);
	});
	return result;
}




template <class T>
struct BlockList {
	using Value = T;
	struct Block {
		umm count = 0;
		umm capacity = 0;
		Block *next = 0;
		Block *previous = 0;

		T *data() { return (T*)(this + 1); }

		T *begin() { return data(); }
		T *end() { return data() + count; }

		umm available_space() const { return capacity - count; }
		T &add(T value) {
			defer { assert(count <= capacity); };
			return data()[count++] = value;
		}
		T &front() { return *data(); }
		T &back() { return data()[count - 1]; }

		Span<T> span() { return {data(), count}; }

		T &operator[](umm i) { bounds_check(i < count); return data()[i]; }
	};
	struct Index {
		Block *block;
		umm value_index;
	};

	struct Iterator {
		Block *block;
		umm value_index;

		Iterator &operator++() {
			++value_index;
			if (value_index >= block->count) {
				if (block->next) {
					block = block->next;
					value_index = 0;
				}
			}
			return *this;
		}
		Iterator operator++(int) {
			auto copy = *this;
			++*this;
			return copy;
		}

		Iterator &operator--() {
			if (value_index == 0) {
				block = block->previous;
				value_index = block->count - 1;
			} else {
				--value_index;
			}
			return *this;
		}
		Iterator operator--(int) {
			auto copy = *this;
			--*this;
			return copy;
		}

		Iterator &operator+=(smm i) {
			if (i >= 0) {
				value_index += i;
				while (value_index >= block->count) {
					if (block->next) {
						value_index -= block->count;
						block = block->next;
					}
				}
			} else {
				smm new_index = (smm)value_index + i;
				while (new_index < 0) {
					new_index += value_index;
					block = block->previous;
					value_index = block->count;
				}
				value_index = new_index;
				assert(value_index < block->count);
			}
			return *this;
		}

		bool operator==(Iterator that) {
			return block == that.block && value_index == that.value_index;
		}
		bool operator!=(Iterator that) {
			return !(*this == that);
		}

		T *operator->() { return &(*block)[value_index]; }
		T &operator*() { return (*block)[value_index]; }

		T &operator[](smm i) {
			Iterator it = *this;
			it += i;
			return *it;
		}
	};

	Allocator allocator = current_allocator;
	Block *first = 0;
	Block *last = 0;
	Block *alloc_last = 0;
	umm count = 0;

	void clear() {
		for (auto block = first; block; block = block->next) {
			block->count = 0;
		}
		last = first;
	}

	Block *allocate_block(umm capacity TL_LP) {
		auto result = (Block *)allocator.allocate(
			sizeof(Block) + sizeof(T) * capacity,
			max(alignof(T), alignof(Block))
			TL_LA
		);
		result->capacity = capacity;
		return result;
	}

	void ensure_capacity(umm desired_capacity) {
		while (last && last->available_space() < desired_capacity) {
			last = last->next;
		}
		if (!last) {
			if (alloc_last)
				last = alloc_last = alloc_last->next = allocate_block(desired_capacity);
			else
				first = last = alloc_last = allocate_block(desired_capacity);
		}
	}

	T &add(T value TL_LP) {
		defer { count += 1; };

		for (;;) {
			if (!last)
				break;

			if (last->available_space())
				return last->add(value);

			last = last->next;
		}


		auto next_capacity = max(alloc_last ? alloc_last->count * 2 : 0, 16);
		auto new_block = allocate_block(next_capacity TL_LA);

		if (first) {
			new_block->previous = alloc_last;
			alloc_last->next = new_block;
			last = alloc_last = new_block;
		} else {
			first = last = alloc_last = new_block;
		}

		return new_block->add(value);
	}
	T &add(TL_LPC) { return add({} TL_LA); }

	bool empty() const { return (last == first) && (first->count == 0); }

	T &back() { return last->back(); }
	T const &back() const { return last->back(); }

	T pop_back() {
		if (last == first)
			bounds_check(first->count);

		defer { debug_count_check(); };

		count -= 1;
		auto result = last->back();
		last->count--;
		if (last->count == 0 && last != first) {
			last = last->previous;
		}
		return result;
	}

	template <class Fn>
	void for_each_block(Fn &&fn) const {
		auto block = first;
		do {
			if (block->count) {
				fn(block->data(), block->count);
			}
			block = block->next;
		} while (block);
	}

	T &operator[](Index index) {
#ifndef TL_NO_BOUNDS_CHECK
		{
			auto block = &first;
			do {
				if (block == index.block) {
					goto check_value_index;
				}
				block = block->next;
			} while (block);
			bounds_check(false);
		check_value_index:

			bounds_check(index.value_index < block->count);
		}
#endif
		return index.block->buffer[index.value_index].value;
	}
	T &operator[](BlockListIndex index) {
		auto block = first;
		while (index.block_index--) {
			block = block->next;
			bounds_check(block);
		}
		bounds_check(index.value_index < block->count);
		return block->data()[index.value_index];
	}
	T &operator[](umm index) {
		auto block = first;
		while (index >= block->count) {
			index -= block->count;
			block = block->next;
			bounds_check(block);
		}
		return block->data()[index];
	}

	Iterator begin() {
		return {first, 0};
	}
	Iterator end() {
		return {last, last ? last->count : 0};
	}

	void debug_count_check() {
		umm sum = 0;
		auto block = first;
		while (block) {
			sum += block->count;
			if (block == last)
				break;
			block = block->next;
		}
		assert_equal(count, sum);
	}

	void copy_into(T *d) {
		for_each_block([&] (T *s, umm count) {
			memcpy(d, s, count);
			d += count;
		});
	}

	Optional<Span<T>> subspan(umm offset, umm count) {
		auto block = first;
		while (offset >= block->count) {
			offset -= block->count;
			block = block->next;
		}

		if (offset + count > block->count)
			return {};

		return Span{block->begin() + offset, count};
	}
};

template <class T>
umm count_of(BlockList<T> const &list) {
	return list.count;
}

template <class T>
umm index_of(BlockList<T> const &list, T *value) {
	umm index = 0;
	auto block = list.first;
	while (block) {
		if (block->begin() <= value && value < block->end()) {
			return index + (value - block->begin());
		}
		index += block->count;
		block = block->next;
	}
	bounds_check(false);
	return (umm)~0;
}

template <class T>
T *next(BlockList<T> &list, T *source) {
	auto block = &list.first;
	while (block) {
		if (&block->buffer->value <= source && source < &block->buffer->end()->value) {
			if (source + 1 < block->buffer->end()) {
				return source + 1;
			} else {
				if (block->next) {
					return &block->next->buffer->value;
				} else {
					return 0;
				}
			}
		}
		block = block->next;
	}
	return 0;
}

template <class T>
struct BlockListIterator {
	T *pointer;
	typename BlockList<T>::Block *block;
	bool operator!=(BlockListIterator that) { return pointer != that.pointer; }
	BlockListIterator &operator++() {
		++pointer;
		if (pointer >= block->end()) {
			block = block->next;
			pointer = &block->buffer->value;
		}
		return *this;
	}
	T &operator*() {
		return *pointer;
	}
};

template <class T>
BlockListIterator<T> begin(BlockList<T> &list) {
	return {&list.first.buffer->value, &list.first};
}

template <class T>
BlockListIterator<T> end(BlockList<T> &list) {
	return {};
}

template <class T, class Fn>
void for_each(BlockList<T> &list, Fn &&fn) {
	constexpr bool using_index = std::is_invocable_v<Fn, T&, BlockListIndex>;

	if constexpr (using_index) {
		using ReturnType = decltype(fn(*(T*)0, BlockListIndex{}));
		constexpr bool returns_directive = std::is_same_v<ForEachDirective, ReturnType>;

		BlockListIndex index = {};
		auto block = list.first;
		while (block) {
			for (auto &it : *block) {
				if constexpr (returns_directive) {
					if (fn(it, index) == ForEach_break)
						return;
				} else {
					fn(it, index);
				}
				++index.value_index;
			}
			block = block->next;
			++index.block_index;
			index.value_index = 0;
		}
	} else {
		using ReturnType = decltype(fn(*(T*)0));
		constexpr bool returns_directive = std::is_same_v<ForEachDirective, ReturnType>;

		auto block = list.first;
		while (block) {
			for (auto it : *block) {
				if constexpr (returns_directive) {
					if (fn(it) == ForEach_break)
						return;
				} else {
					fn(it);
				}
			}
			block = block->next;
		}
	}
}

template <class T>
T *find(BlockList<T> &list, T const &to_find, BlockListIndex *result_index = 0) {
	BlockListIndex index = {};
	auto block = list.first;
	do {
		index.value_index = 0;
		for (auto it = block->buffer; it != block->end; ++it) {
			if (it->value == to_find) {
				if (result_index)
					*result_index = index;
				return &it->value;
			}
			++index.value_index;
		}
		block = block->next;
		++index.block_index;
	} while (block);

	return 0;
}

template <class T>
T *find(BlockList<T> &list, T const &to_find, typename BlockList<T>::Index *result_index = 0) {
	typename BlockList<T, Allocator>::Index index = {};
	auto block = list.first;
	do {
		index.block = block;
		index.value_index = 0;
		auto block_capacity = block->size();
		for (auto it = block->buffer; it != block->end; ++it) {
			if (it == to_find) {
				if (result_index)
					*result_index = index;
				return &it;
			}
			++index.value_index;
		}
		block = block->next;
	} while (block);

	return 0;
}

template <class T, class Predicate>
T *find_if(BlockList<T> &list, Predicate &&predicate) {
	auto block = list.first;
	do {
		for (auto it = block->buffer; it != block->end; ++it) {
			if (predicate(it->value)) {
				return &it->value;
			}
		}
		block = block->next;
	} while (block);

	return 0;
}

template <class T>
void free(BlockList<T> &list) {
	auto block = list.first;
	while (block) {
		auto next = block->next;
		list.allocator.free(block);
		block = next;
	}
	list.first = list.last = list.alloc_last = 0;
	list.count = 0;
}

template <class T>
BlockList<T> copy(BlockList<T> list) {
	// TODO: optimize
	BlockList<T> result;
	for_each(list, [&](T value) {
		result.add(value);
	});
	return result;
}

template <class Block>
void double_join(Block *a, Block *b) {
	a->next = b;
	b->previous = a;
}

template <class T>
void add_steal(BlockList<T> *destination, BlockList<T> *source) {
	if (source->last) {
		if (source->last->next) {
			assert_equal(source->last->next->count, 0);
		}
	}

	assert(destination->allocator == source->allocator, "different allocator are not implemented");

	if (!source->last) {
		assert_equal(source->count, 0);
		return;
	}

	defer {
		source->first = source->last = source->alloc_last = 0;
		source->count = 0;
	};

	if (!destination->last) {
		*destination = *source;
		return;
	}

	auto destination_excess_blocks = destination->last->next;
	auto source_excess_blocks = source->last->next;

	if (destination_excess_blocks) {
		if (!source_excess_blocks) {
			assert_equal(source->last, source->alloc_last);
		}

		if (source_excess_blocks) {
			// have: dst <-> dstex     src <-> srcex
			// need: dst <-> src <-> srcex <-> dstex

			destination->last->next = source->first;
			source->first->previous = destination->last;

			source->alloc_last->next = destination_excess_blocks;
			destination_excess_blocks->previous = source->alloc_last;
		} else {
			// have: dst <-> dstex     src
			// need: dst <-> src <-> dstex

			destination->last->next = source->first;
			source->first->previous = destination->last;

			source->last->next = destination_excess_blocks;
			destination_excess_blocks->previous = source->last;
		}

		// double_join(destination->last, source->first);
		// double_join(source->alloc_last, destination_excess_blocks);
	} else {
		// have: dst     src <-> srcex
		// need: dst <-> src <-> srcex
		double_join(destination->last, source->first);

		destination->alloc_last = source->alloc_last;
	}

	destination->last = source->last;

	destination->count += source->count;
}

template <class T>
void add(BlockList<T> *destination, BlockList<T> source TL_LP) {
	for_each(source, [&](T &x) {
		destination->add(x TL_LA);
	});
	return;
	// TODO: de-bug this

	auto src_block = source.first;
	auto src_remaining = src_block->count;
	auto src_data = src_block->data();

	auto dst_block = destination->last;

	while (dst_block) {
		auto copy_count = min(src_remaining, dst_block->available_space());

		memcpy(dst_block->end(), src_data, copy_count * sizeof(T));

		dst_block->count += copy_count;
		src_remaining -= copy_count;
		src_data += copy_count;

		if (src_remaining == 0) {
			// Source block has no more data left. Go to the next one.
			src_block = src_block->next;
			if (!src_block) {
				break;
			}
		}

		if (dst_block->available_space() == 0) {
			// Destination block has no more space. Go to the next one.
			dst_block = dst_block->next;
		}
	}

	if (src_block) {
		// We still have source data an destination ran out of space.
		// Allocate a big enough block to store everything that is left.
		umm remaining_count = src_remaining;
		auto stopped_on_src_block = src_block;
		src_block = src_block->next;
		while (src_block) {
			remaining_count += src_remaining;
			src_block = src_block->next;
		}

		if (!remaining_count)
			return;

		dst_block = destination->allocate_block(max(remaining_count, destination->next_capacity) TL_LA);
		destination->next_capacity *= 2;

		memcpy(dst_block->end(), src_data, src_remaining * sizeof(T));

		src_block = stopped_on_src_block->next;
		while (src_block) {
			memcpy(dst_block->end(), src_block->data(), src_block->count * sizeof(T));
			src_block = src_block->next;
		}

		dst_block->count = remaining_count;

		assert_equal(destination->last, destination->alloc_last);
		if (destination->first) {
			destination->last =
			destination->alloc_last =
			destination->alloc_last->next = dst_block;
		} else {
			destination->first =
			destination->last =
			destination->alloc_last = dst_block;
		}
	}
}


template <class T>
List<T> to_list(BlockList<T> list TL_LP) {
	List<T> result;
	result.reserve(count_of(list) TL_LA);
	for_each(list, [&](T const &value) {
		result.add(value);
	});
	return result;
}


}
