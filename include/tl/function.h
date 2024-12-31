#pragma once
#include "common.h"

#include <type_traits>
#include <functional>

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

template <class State, class Param, umm... indices>
static void invoke_separated(void *state, void *param) noexcept {
	std::invoke(*(State *)state, std::move(std::get<indices>(*(Param *)param))...);
}
template <class State, class Param, umm... indices>
static constexpr auto get_invoke_separated(std::index_sequence<indices...>) noexcept {
	return &invoke_separated<State, Param, indices...>;
}

} // namespace Detail

struct FatFunctionPointer {
	void (*function)(void *);
	void *parameter;
	
	void operator()() {
		return function(parameter);
	}
};

template <class Allocator = Allocator, class Fn>
FatFunctionPointer create_fat_function_pointer(Fn &&fn, Allocator allocator = Allocator::current()) {
	FatFunctionPointer result = {};
	allocator = Allocator::current();

	using Tuple = std::tuple<std::decay_t<Fn>>;
	result.parameter = allocator.template allocate_uninitialized<Tuple>();
	new(result.parameter) Tuple(std::forward<Fn>(fn));

	result.function = Detail::get_invoke<Tuple>(std::make_index_sequence<1>{});
	return result;
}
template <class Allocator = Allocator>
void free(FatFunctionPointer &fn, Allocator allocator) {
	if (fn.parameter)
		allocator.free(fn.parameter);
	fn = {};
}

template <class ParameterlessFunction, class Allocator = Allocator>
struct Function {
	Allocator allocator = {};

	void (*function)(void *state) = 0;
	void *state = 0;

	Function() = default;

	template <class Fn>
	Function(Fn fn) requires requires { fn(); } {
		allocator = Allocator::current();

		using Tuple = std::tuple<std::decay_t<Fn>>;
		state = allocator.template allocate_uninitialized<Tuple>();
		new(state) Tuple(std::forward<Fn>(fn));

		function = Detail::get_invoke<Tuple>(std::make_index_sequence<1>{});
	}

	void operator()() {
		return function(state);
	}

	void free() {
		if (state)
			allocator.free(state);
		*this = {};
	}

	explicit operator bool() { return function; }
};

template <class ReturnType, class ...Args, class Allocator>
struct Function<ReturnType(Args...), Allocator> {
	Allocator allocator = {};

	void (*function)(void *state, void *param) = 0;
	void *state = 0;

	Function() = default;

	template <class Fn>
		requires requires (Fn fn, Args ...args) { fn(args...); }
	Function(Fn fn) {
		allocator = Allocator::current();

		using Func = std::decay_t<Fn>;
		state = allocator.template allocate_uninitialized<Func>();
		new(state) Func(std::forward<Fn>(fn));

		function = Detail::get_invoke_separated<std::decay_t<Fn>, std::tuple<std::decay_t<Args>...>>(std::make_index_sequence<sizeof...(Args)>{});
	}

	void operator()(Args ...args) {
		using Param = std::tuple<std::decay_t<Args>...>;
		auto param = allocator.template allocate_uninitialized<Param>();
		new(param) Param(std::forward<Args>(args)...);
		defer { allocator.free(param); };

		return function(state, param);
	}

	void free() {
		if (state)
			allocator.free(state);
		*this = {};
	}
	
	explicit operator bool() { return function; }
};

}
