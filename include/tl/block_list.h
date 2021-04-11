#pragma once

#include "common.h"

namespace TL {

template <class T, umm blockCapacity_>
struct BlockList {
	static constexpr umm blockCapacity = blockCapacity_;
	using Storage = Storage<T>;
	struct Block {
		Storage buffer[blockCapacity];
		Storage *end;
		Block *next;
		Block *previous;

		umm size() const { return (umm)(end - buffer); }
		umm availableSpace() const { return blockCapacity - size(); }
	};
	struct Index {
		Block *block;
		umm value_index;
	};

	Allocator allocator = current_allocator;
	Block first;
	Block *last = &first;
	Block *allocLast = &first;

	BlockList() {
		init_block(&first);
	}
	BlockList(BlockList const &that) = default;
	BlockList(BlockList &&that) = delete;
	BlockList &operator=(BlockList const &that) = default;
	BlockList &operator=(BlockList &&that) = delete;
#if 0
	BlockList(BlockList const &that) {
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(that.first.buffer[i].value);
		}

		auto previous = &first;
		for (auto srcBlock = that.first.next; srcBlock != 0;) {
			if (srcBlock->size()) {
				last = allocLast = allocateBlock(previous);
				for (u32 i = 0; i < srcBlock->size(); ++i) {
					new (&last->buffer[i].value) T(srcBlock->buffer[i].value);
				}
				previous = last;
			}
		}
	}
	BlockList(BlockList &&that) {
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(std::move(that.first.buffer[i].value));
		}

		first.next = that.first.next;
		last = that.last;
		allocLast = that.allocLast;

		that.first.end = that.first.buffer;
		that.last = that.allocLast = &that.first;
	}
	BlockList &operator=(BlockList const &that) {
		clear();

		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(that.first.buffer[i].value);
		}

		last = &first;
		for (auto srcBlock = that.first.next; srcBlock != 0;) {
			if (srcBlock->size()) {
				if (!last->next)
					last->next = allocLast = allocateBlock(last);
				last = last->next;
				for (u32 i = 0; i < srcBlock->size(); ++i) {
					new (&last->buffer[i].value) T(srcBlock->buffer[i].value);
				}
			}
		}

		return *this;
	}
	BlockList &operator=(BlockList &&that) {
		clear();
		return *new (this) BlockList(std::move(that));
	}
	~BlockList() {
		for (u32 i = 0; i < first.size(); ++i) {
			first.buffer[i].value.~T();
		}
		first.end = first.buffer;

		for (auto block = first.next; block != 0;) {
			for (u32 i = 0; i < block->size(); ++i) {
				block->buffer[i].value.~T();
			}
			auto next = block->next;
			FREE(allocator, block);
			block = next;
		}

		last = &first;
		allocLast = &first;
	}
#endif

	void clear() {
		for (u32 i = 0; i < first.size(); ++i) {
			first.buffer[i].value.~T();
		}
		first.end = first.buffer;

		for (auto block = first.next; block != 0; block = block->next) {
			for (u32 i = 0; i < block->size(); ++i) {
				block->buffer[i].value.~T();
			}
			block->end = block->buffer;
		}
		last = &first;
	}
	umm availableSpace() {
		umm space = 0;
		auto block = last;
		while (block) {
			space += block->availableSpace();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount) {
		umm space = availableSpace();
		while (space < amount) {
			allocLast = allocLast->next = allocateBlock(allocLast);
			space += blockCapacity;
		}
	}
	void ensureConsecutiveSpace(umm amount) {
		assert(amount <= blockCapacity, "reserving this amount of consecutive space is impossible");
		if (last->availableSpace() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocateBlock(last);
				last = allocLast = last->next;
			}
		}
	}
	template <class ...Args>
	T &add_in_place(Args &&...args) {
		auto dstBlock = last;
		while (dstBlock && (dstBlock->availableSpace() == 0)) {
			dstBlock = dstBlock->next;
		}

		if (!dstBlock) {
			auto new_block = allocateBlock(allocLast);
			allocLast->next = new_block;
			dstBlock = last = allocLast = new_block;
		}
		return *new (dstBlock->end++) T(std::forward<Args>(args)...);
	}

	T &add(T const &value) { return add_in_place(value); }
	T &add(T &&value) { return add_in_place(std::move(value)); }
	T &add() { return add_in_place(); }

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
			auto blockCapacity = block->size();
			if (blockCapacity) {
				fn(block->buffer, blockCapacity);
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

	BlockList &operator+=(T const &v) { this->add(v); return *this; }
	BlockList &operator+=(T &&v) { this->add(std::move(v)); return *this; }
	BlockList &operator+=(Span<T const> v) { this->insert(v, this->end()); return *this; }
	BlockList &operator+=(BlockList const &v) { this->insert(as_span(v), this->end()); return *this; }
	BlockList &operator+=(std::initializer_list<T> v) { this->insert(Span(v.begin(), v.end()), this->end()); return *this; }

private:
	void init_block(Block *block) {
		block->end = block->buffer;
		block->next = 0;
		block->previous = 0;
	}
	Block *allocateBlock(Block *previous) {
		Block *result = ALLOCATE(Block, allocator);
		init_block(result);
		result->previous = previous;
		return result;
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
		constexpr bool breakable = is_same<bool, ReturnType>;
		BlockListIndex index = {};
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
		auto blockCapacity = block->size();
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
