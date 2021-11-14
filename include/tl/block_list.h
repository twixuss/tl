#pragma once

#include "common.h"

namespace tl {

struct BlockListIndex {
	umm block_index;
	umm value_index;
};

template <class T, umm block_capacity_>
struct BlockList {
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
				block = block->next;
				value_index = 0;
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

	BlockList() = default;
	BlockList(BlockList const &that) = default;
	BlockList(BlockList &&that) = delete;
	BlockList &operator=(BlockList const &that) = default;
	BlockList &operator=(BlockList &&that) = delete;

	void clear() {
		for (auto block = &first; block != 0; block = block->next) {
			block->clear();
		}
		last = &first;
	}

	T &add(T value) {
		auto dest_block = last;
		while (dest_block && (dest_block->available_space() == 0)) {
			assert(dest_block != dest_block->next);
			dest_block = dest_block->next;
		}
		if (!dest_block) {
			dest_block = allocator.allocate<Block>();
			dest_block->next = 0;
			dest_block->previous = alloc_last;
			alloc_last->next = dest_block;
			last = alloc_last = dest_block;
		}
		return dest_block->add(value);
	}
	T &add() { return add({}); }

	umm size() const {
		umm totalSize = 0;
		for (auto block = &first; block != 0; block = block->next) {
			totalSize += block->size();
		}
		return totalSize;
	}
	bool empty() const { return (last == &first) && (first.size() == 0); }

	T &back() { return last->end[-1].value; }
	T const &back() const { return last->end[-1].value; }

	void pop_back() {
		if (last == &first)
			bounds_check(first.size());

		(last->end--)[-1].value.~T();
		if (last->end == last->buffer && last != &first) {
			last = last->previous;
		}
	}

	template <class Fn>
	void for_each_block(Fn &&fn) const {
		auto block = &first;
		do {
			auto block_capacity = block->size();
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

			bounds_check(index.value_index < block->size());
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
		bounds_check(index.value_index < block->size());
		return block->data[index.value_index];
	}
	T &operator[](umm index) {
		auto block = &first;
		while (index >= block->size) {
			index -= block->size;
			block = block->next;
			bounds_check(block);
		}
		return block->data[index];
	}

	BlockList &operator+=(T const &v) { this->add(v); return *this; }
	BlockList &operator+=(T &&v) { this->add(std::move(v)); return *this; }
	BlockList &operator+=(Span<T const> v) { this->insert(v, this->end()); return *this; }
	BlockList &operator+=(BlockList const &v) { this->insert(as_span(v), this->end()); return *this; }
	BlockList &operator+=(std::initializer_list<T> v) { this->insert(Span(v.begin(), v.end()), this->end()); return *this; }


	Iterator begin() {
		return {&first, 0};
	}
	Iterator end() {
		return {last, last->count};
	}
};

template <class T, umm block_size>
T *next(BlockList<T, block_size> &list, T *source) {
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
struct BlockListIterator {
	T *pointer;
	typename BlockList<T, block_size>::Block *block;
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

template <class T, umm block_size>
BlockListIterator<T, block_size> begin(BlockList<T, block_size> &list) {
	return {&list.first.buffer->value, &list.first};
}

template <class T, umm block_size>
BlockListIterator<T, block_size> end(BlockList<T, block_size> &list) {
	return {};
}

template <class T, umm block_size, class Fn>
void for_each(BlockList<T, block_size> &list, Fn &&fn) {
	constexpr bool using_index = std::is_invocable_v<Fn, T&, BlockListIndex>;

	if constexpr (using_index) {
		using ReturnType = decltype(fn(*(T*)0, BlockListIndex{}));
		constexpr bool returns_directive = is_same<ForEachDirective, ReturnType>;

		BlockListIndex index = {};
		auto block = &list.first;
		do {
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
		} while (block);
	} else {
		using ReturnType = decltype(fn(*(T*)0));
		constexpr bool returns_directive = is_same<ForEachDirective, ReturnType>;

		auto block = &list.first;
		do {
			for (auto it : *block) {
				if constexpr (returns_directive) {
					if (fn(it) == ForEach_break)
						return;
				} else {
					fn(it);
				}
			}
			block = block->next;
		} while (block);
	}
}

template <class T, umm block_size>
T *find(BlockList<T, block_size> &list, T const &to_find, BlockListIndex *result_index = 0) {
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
T *find(BlockList<T, block_size> &list, T const &to_find, typename BlockList<T, block_size>::Index *result_index = 0) {
	typename BlockList<T, block_size, Allocator>::Index index = {};
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
T *find_if(BlockList<T, block_size> &list, Predicate &&predicate) {
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

}
