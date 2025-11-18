#pragma once
#include "common.h"

namespace tl {

template <class T, umm capacity>
struct StaticSet {

	T *begin() { return data; }
	T const *begin() const { return data; }
	T *end() { return data + count; }
	T const *end() const { return data + count; }

	T *find(T const &value) {
		for (auto &it : *this) {
			if (it == value)
				return &it;
		}
		return 0;
	}
	T &add(T const &value) {
		if (auto found = find(value))
			return *found;
		return data[count++] = value;
	}
	void add(Span<T> values) {
		for (auto &value : values) {
			add(value);
		}
	}

	bool erase(T const &value) {
		if (auto found = find(value)) {
			--count;
			memcpy(found, found + 1, sizeof(T) * (end() - found));
			return true;
		}
		return false;
	}

	Optional<T> pop() {
		if (count)
			return data[--count];
		return {};
	}

	Span<T> span() {
		return {data, count};
	}

	union {
		T data[capacity];
	};
	umm count = 0;
};

}
