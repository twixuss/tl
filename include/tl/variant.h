#pragma once
#include "common.h"

#include <variant>

namespace tl {

template <class ...T>
struct Variant {
	std::variant<T...> base;

	template <class U>
	static constexpr umm index_of = type_index_of<U, T...>;

	Variant() = default;

	template <OneOf<T...> U>
	Variant(U value) {
		base = value;
	}

	auto visit(auto &&fn) {
		return std::visit(fn, base);
	}

	template <class U>
	Optional<U> get() {
		if (auto p = std::get_if<U>(&base)) {
			return *p;
		}
		return {};
	}

	template <class U>
	bool is() {
		return std::holds_alternative<U>(base);
	}

	template <class U>
	U *as() {
		return std::get_if<U>(&base);
	}

	umm index() {
		return base.index();
	}
};

}