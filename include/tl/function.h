#pragma once
#include "function_detail.h"

namespace tl {

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

template <class Fn, umm storage_capacity, umm alignment = 8>
struct InlineFunction {
	static_error_t(Fn, "InlineFunction can't work with specified template arguments.");
};

template <class Ret, umm storage_capacity, umm storage_alignment, class ...Args>
struct InlineFunction<Ret(Args...), storage_capacity, storage_alignment> {
	alignas(storage_alignment) u8 storage[storage_capacity] = {};
	Ret (*function)(void *self, void *param) = 0;
	
	InlineFunction() = default;

	template <class Fn>
	InlineFunction(Fn &&fn) requires requires {
		{ fn(std::declval<Args>()...) } -> std::same_as<Ret>;
		requires sizeof(Fn) <= storage_capacity;
		requires alignof(Fn) <= storage_alignment;
	} {
		using StoredFn = std::decay_t<Fn>;
		new(storage) StoredFn(std::forward<Fn>(fn));

		function = Detail::get_invoke_separated<StoredFn, std::tuple<Args...>>(std::make_index_sequence<sizeof...(Args)>{});
	}

	template <class Fn>
	InlineFunction &operator=(Fn &&fn) requires requires {
		{ fn(std::declval<Args>()...) } -> std::same_as<Ret>;
		requires sizeof(Fn) <= storage_capacity;
		requires alignof(Fn) <= storage_alignment;
	} {
		this->~InlineFunction();
		new(this) InlineFunction(std::forward<Fn>(fn));
	}

	Ret operator()(Args ...args) {
		using Param = std::tuple<std::decay_t<Args>...>;
		Param param(std::forward<Args>(args)...);
		return function(storage, &param);
	}
};

}
