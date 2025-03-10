#pragma once
#include "static_ring_buffer.h"

namespace tl {

template <class T, umm _capacity>
struct StaticQueue : private StaticRingBuffer<T, _capacity> {
	using Base = StaticRingBuffer<T, _capacity>;
	using Base::data;
	using Base::count;
	using Base::capacity;
	using Base::begin;
	using Base::end;
	using Base::clear;
	using Base::operator[];
	T &push(T value) { return this->push_back(value); }
	void push(Span<T> span) { return this->push_back(span); }
	Optional<T> pop() { return this->pop_front(); }
};

}
