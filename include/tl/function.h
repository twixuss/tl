#pragma once
#include "common.h"

namespace tl {

namespace Detail {
template <class Tuple, umm... indices>
static void invoke(void *raw_vals) noexcept {
	Tuple &tuple = *(Tuple *)raw_vals;
	std::invoke(std::move(std::get<indices>(tuple))...);
}

template <class Tuple, umm... indices>
static constexpr auto get_invoke(std::index_sequence<indices...>) noexcept {
	return &invoke<Tuple, indices...>;
}

} // namespace Detail

template <class Allocator = Allocator>
struct StorableFunction {
	Allocator allocator = {};

	void (*function)(void *param) = 0;
	void *param = 0;

	StorableFunction() = default;

	StorableFunction(void (*function)(void *param), void *param) {
		this->function = function;
		this->param = param;
	}

	StorableFunction(void (*function)()) {
		this->function = autocast function;
	}

	template <class Fn, class ...Args>
	requires requires(Fn fn, Args ...args) { fn(args...); }
	StorableFunction(Fn fn, Args ...args) {
		allocator = Allocator::current();

		using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
		auto params = allocator.allocate_uninitialized<Tuple>();
		new(params) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);

		function = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
		param = params;
	}

	void operator()() {
		return function(param);
	}

	void free() {
		if (param)
			allocator.free(param);
		*this = {};
	}
};



template <class Ret, class ...Args>
struct Function {
	Ret (*fn)(Args ...args);

	Ret operator()(Args ...args) {
		return fn(args...);
	}
};

template <class Ret, class ...Args>
Function<Ret, Args...> create_function(Ret (*fn)(Args ...args)) {
	return {fn};
}

}
