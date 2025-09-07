#pragma once
#include "common.h"
#include "array.h"

namespace tl {

// Tightly stores multiple arrays of different types in a single allocation.
template <class Allocator, bool store_base_pointers, class ...Ts>
struct MultiArray {
	[[no_unique_address]] std::conditional_t<store_base_pointers, Empty, u8 *> buffer = {};
	[[no_unique_address]] std::conditional_t<store_base_pointers, std::tuple<Ts *...>, Empty> base_pointers = {};
	umm capacity = 0;
	[[no_unique_address]] Allocator allocator = Allocator::current();

	void init(umm new_capacity) {
		base()   = (u8 *)allocator.allocate(get_buffer_size(new_capacity), max(alignof(Ts)...));
		capacity = new_capacity;

		if constexpr (store_base_pointers) {
			for_each_type(I, T, Ts) {
				if constexpr (I) {
					std::get<I>(base_pointers) = compute_base_of<I>();
				}
			};
		}
	}
	void free() {
		allocator.free(base(), get_buffer_size(capacity), max(alignof(Ts)...));
		buffer = {};
		base_pointers = {};
		capacity = 0;
	}
	
	u8 *&base() {
		if constexpr (store_base_pointers) {
			return (u8 *&)std::get<0>(base_pointers);
		} else {
			return buffer;
		}
	}
	u8 *const &base() const {
		if constexpr (store_base_pointers) {
			return (u8 *&)std::get<0>(base_pointers);
		} else {
			return buffer;
		}
	}

	template <umm type_index>
	auto compute_base_of() const {
		static_assert(type_index < sizeof...(Ts));
		
		umm offset = 0;

		for_each_type(I, T, Ts) {
			if constexpr (I < type_index) {
				offset += capacity * Array{sizeof(Ts)...}[I];
				offset = ceil(offset, Array{alignof(Ts)...}[I + 1]);
			}
		};

		return (TypeAt<type_index, Ts...> *)(base() + offset);
	}

	template <umm type_index>
	auto base_of() const {
		static_assert(type_index < sizeof...(Ts));
		
		if constexpr (store_base_pointers) {
			return std::get<type_index>(base_pointers);
		} else {
			return compute_base_of<type_index>();
		}
	}
	
	template <class T>
	auto base_of() const requires AllDifferent<Ts...> {
		return base_of<type_index_of<T, Ts...>>();
	}

	template <umm type_index>
	auto &at(umm index) const {
		bounds_check(assert_less(index, capacity));
		return base_of<type_index>()[index];
	}

	template <class T>
	auto &at(umm index) const requires AllDifferent<Ts...> {
		return at<type_index_of<T, Ts...>>(index);
	}

	umm get_buffer_size(umm capacity) const {
		umm size = 0;

		for (umm i = 0; i < sizeof...(Ts); ++i) {
			size = ceil(size, Array{alignof(Ts)...}[i]);
			size += capacity * Array{sizeof(Ts)...}[i];
		}

		return size;
	}
};

}
