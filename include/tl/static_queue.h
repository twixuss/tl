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
	T &add(T value) { return this->add_back(value); }
	void add(Span<T> span) { return this->add_back(span); }
	Optional<T> pop() { return this->pop_front(); }
};

}
