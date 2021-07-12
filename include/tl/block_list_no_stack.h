#pragma once

#include "common.h"

namespace tl {

template <class T, umm block_capacity_>
struct BlockListNoStack {
	static constexpr umm block_capacity = block_capacity_;
	using Storage = Storage<T>;
	struct Block : StaticList<T, block_capacity> {
		Block *next = 0;
		Block *previous = 0;
		umm available_space() const { return block_capacity - this->size; }
	};
	struct Index {
		Block *block;
		umm value_index;
	};

	Allocator allocator = current_allocator;
	Block *first = 0;
	Block *last = 0;
	Block *alloc_last = 0;

	BlockListNoStack() {}
	BlockListNoStack(BlockListNoStack const &that) = default;
	BlockListNoStack(BlockListNoStack &&that) = delete;
	BlockListNoStack &operator=(BlockListNoStack const &that) = default;
	BlockListNoStack &operator=(BlockListNoStack &&that) = delete;

	bool empty() const { return first == 0 || ((last == first) && (first->size == 0)); }

	T &front() {
		bounds_check(first && first->size);

		return *first->data;
	}

	T &back() {
		bounds_check(first && first->size);

		return last->back();
	}

	T &add(T value = {}) {
		Block *block;
		if (first) {
			block = first;
			while (block && (block->available_space() == 0)) {
				block = block->next;
			}

			if (!block) {
				auto new_block = allocate_block(alloc_last);
				alloc_last->next = new_block;
				block = last = alloc_last = new_block;
			}
		} else {
			first = last = alloc_last = block = allocate_block(alloc_last);
		}

		return block->add(value);
	}
	Block *allocate_block(Block *previous) {
		Block *result = allocator.allocate<Block>();
		result->previous = previous;
		return result;
	}
	/*

	void clear() {
		if (!first)
			return;

		for (auto block = first; block != 0; block = block->next) {
			block->end = block->buffer;
		}
		last = &first;
	}
	umm available_space() {
		umm space = 0;
		auto block = first;
		while (block) {
			space += block->available_space();
			block = block->next;
		}
		return space;
	}
	void ensure_space(umm amount) {
		umm space = available_space();
		while (space < amount) {
			alloc_last = alloc_last->next = allocate_block(alloc_last);
			space += block_capacity;
		}
	}
	void ensure_consecutive_space(umm amount) {
		assert(amount <= block_capacity, "reserving this amount of consecutive space is impossible");
		if (last->available_space() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocate_block(last);
				last = alloc_last = last->next;
			}
		}
	}

	T &add(T const &value) { return add_in_place(value); }
	T &add(T &&value) { return add_in_place(std::move(value)); }
	T &add() { return add_in_place(); }

	umm size() const {
		umm total_size = 0;
		for (auto block = first; block != 0; block = block->next) {
			total_size += block->size();
		}
		return total_size;
	}

	T &back() { return last->buffer.back(); }
	T const &back() const { return last->buffer.back(); }

	void pop_back() {
		if (!first)
			return;

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
	T &operator[](BlockListNoStackIndex index) {
		auto block = &first;
		while (index.block_index--) {
			block = block->next;
			bounds_check(block);
		}
		bounds_check(index.value_index < block->size());
		return block->buffer[index.value_index].value;
	}
	T &operator[](umm index) {
		auto block = &first;
		while (index >= block->size()) {
			index -= block->size();
			block = block->next;
			bounds_check(block);
		}
		return block->buffer[index].value;
	}

	BlockListNoStack &operator+=(T const &v) { this->add(v); return *this; }
	BlockListNoStack &operator+=(T &&v) { this->add(std::move(v)); return *this; }
	BlockListNoStack &operator+=(Span<T const> v) { this->insert(v, this->end()); return *this; }
	BlockListNoStack &operator+=(BlockListNoStack const &v) { this->insert(as_span(v), this->end()); return *this; }
	BlockListNoStack &operator+=(std::initializer_list<T> v) { this->insert(Span(v.begin(), v.end()), this->end()); return *this; }

private:
	void init_block(Block *block) {
		block->end = block->buffer;
		block->next = 0;
		block->previous = 0;
	}
	*/
};

template <class T, umm block_size>
struct BlockListNoStackIterator {
	T *pointer;
	typename BlockListNoStack<T, block_size>::Block *block;
	bool operator!=(BlockListNoStackIterator that) { return pointer != that.pointer; }
	BlockListNoStackIterator &operator++() {
		++pointer;
		if (pointer == block->end()) {
			block = block->next;
			if (block) {
				pointer = block->data;
			} else {
				pointer = 0;
			}
		}
		return *this;
	}
	T &operator*() {
		return *pointer;
	}
};

template <class T, umm block_size>
BlockListNoStackIterator<T, block_size> begin(BlockListNoStack<T, block_size> &list) {
	BlockListNoStackIterator<T, block_size> result = {};
	if (list.first && list.first->size) {
		result.pointer = list.first->data;
		result.block = list.first;
	}
	return result;
}

template <class T, umm block_size>
BlockListNoStackIterator<T, block_size> end(BlockListNoStack<T, block_size> &list) {
	return {};
}

template <class T, umm block_size>
T *next(BlockListNoStack<T, block_size> &list, T *source) {
	auto block = list.first;
	while (block) {
		if (block->data <= source && source < block->end()) {
			++source;
			if (source == block->end()) {
				if (block->next) {
					return block->next->data;
				} else {
					return 0;
				}
			} else {
				return source;
			}
		}
		block = block->next;
	}
	return 0;
}

template <class T, umm block_size>
T *previous(BlockListNoStack<T, block_size> &list, T *source) {
	auto block = list.first;
	while (block) {
		if (block->data <= source && source < block->end()) {
			if (source-- == block->data) {
				if (block->previous) {
					return &block->previous->back();
				} else {
					return 0;
				}
			} else {
				return source;
			}
		}
		block = block->next;
	}
	return 0;
}

template <class T, umm block_size, class Predicate>
T *find_if(BlockListNoStack<T, block_size> &list, Predicate &&predicate) {
	auto block = list.first;
	while (block) {
		for (auto &it : *block) {
			if (predicate(it)) {
				return &it;
			}
		}
		block = block->next;
	}

	return 0;
}

/*

template <class T, umm block_size, class Fn>
void for_each(BlockListNoStack<T, block_size> &list, Fn &&fn) {
	constexpr bool using_index = std::is_invocable_v<Fn, T&, BlockListNoStackIndex>;

	if constexpr (using_index) {
		using ReturnType = decltype(fn(*(T*)0, BlockListNoStackIndex{}));
		constexpr bool breakable = is_same<bool, ReturnType>;
		BlockListNoStackIndex index = {};
		auto block = &list.first;
		do {
			for (auto it = block->buffer; it != block->end; ++it) {
				if constexpr (breakable) {
					if (fn(it->value, index))
						return;
				} else {
					fn(it->value, index);
				}
				++index.value_index;
			}
			block = block->next;
			++index.block_index;
			index.value_index = 0;
		} while (block);
	} else {
		using ReturnType = decltype(fn(*(T*)0));
		constexpr bool breakable = is_same<bool, ReturnType>;
		auto block = &list.first;
		do {
			for (auto it = block->buffer; it != block->end; ++it) {
				if constexpr (breakable) {
					if (fn(it->value))
						return;
				} else {
					fn(it->value);
				}
			}
			block = block->next;
		} while (block);
	}
}

template <class T, umm block_size>
T *find(BlockListNoStack<T, block_size> &list, T const &to_find, BlockListNoStackIndex *result_index = 0) {
	BlockListNoStackIndex index = {};
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
T *find(BlockListNoStack<T, block_size> &list, T const &to_find, typename BlockListNoStack<T, block_size>::Index *result_index = 0) {
	typename BlockListNoStack<T, block_size, Allocator>::Index index = {};
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

*/

}
