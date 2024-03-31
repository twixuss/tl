#pragma once
#include "common.h"

#include <variant>

namespace tl {

template <class ...T>
struct Variant {
	std::variant<T...> base;

	template <class U>
	static constexpr umm index_of = type_index_of<U, T...>;

	static constexpr umm count = sizeof...(T);
	
	static constexpr umm sizes[] = {sizeof(T) ...};

	Variant() = default;

	template <OneOf<T...> U>
	Variant(U value) {
		defer{debug_check();};
		base = value;
	}

	auto visit(this auto &&self, auto &&fn) requires requires { std::visit(fn, self.base); } {
		defer{self.debug_check();};
		return std::visit(fn, self.base);
	}

	template <OneOf<T...> U>
	bool is() const {
		defer{debug_check();};
		return std::holds_alternative<U>(base);
	}

	template <OneOf<T...> U>
	auto as_ptr(this auto &&self) {
		defer{self.debug_check();};
		return std::get_if<U>(&self.base);
	}

	template <OneOf<T...> U>
	Optional<U> as() const {
		defer{debug_check();};
		if (auto p = as_ptr<U>()) {
			return *p;
		}
		return {};
	}

	umm index() const {
		return base.index();
	}

	umm current_value_size() const {
		return sizes[index()];
	}

	bool operator==(Variant const &that) const {
		defer{debug_check();};
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

	void debug_check() const {
		assert((umm)this % alignof(decltype(*this)) == 0);
		assert_less(index(), count);
	}
};

template <class ...T>
inline umm append(StringBuilder &builder, Variant<T...> const &v) {
	return v.visit([&](auto &inner) {
		return append(builder, inner);
	});
}

}