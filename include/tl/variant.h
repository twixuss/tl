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

	auto visit(this auto &&self, auto &&fn) {
		return std::visit(fn, self.base);
	}

	template <class U>
	bool is() const {
		return std::holds_alternative<U>(base);
	}

	template <class U>
	auto as_ptr(this auto &&self) {
		return std::get_if<U>(&self.base);
	}

	template <class U>
	Optional<U> as() const {
		if (auto p = as_ptr<U>()) {
			return *p;
		}
		return {};
	}

	umm index() const {
		return base.index();
	}

	bool operator==(Variant const &that) const {
		if (index() != that.index())
			return false;

		return visit([&](auto &this_inner) {
			return that.visit([&](auto &that_inner) {
				if constexpr (std::is_same_v<decltype(this_inner), decltype(that_inner)>) {
					return this_inner == that_inner;
				} else {
					return false;
				}
			});
		});
	}
};

}