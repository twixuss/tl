#pragma once
#include "common.h"

#include <type_traits>
#include <functional>

namespace tl {

namespace Detail {
template <class Tuple, umm... indices>
static decltype(auto) invoke(void *raw_vals) noexcept {
	Tuple &tuple = *(Tuple *)raw_vals;
	return std::invoke(std::move(std::get<indices>(tuple))...);
}

template <class Tuple, umm... indices>
static constexpr auto get_invoke(std::index_sequence<indices...>) noexcept {
	return &invoke<Tuple, indices...>;
}

template <class State, class Param, umm... indices>
static decltype(auto) invoke_separated(void *state, void *param) noexcept {
	return std::invoke(*(State *)state, std::move(std::get<indices>(*(Param *)param))...);
}
template <class State, class Param, umm... indices>
static constexpr auto get_invoke_separated(std::index_sequence<indices...>) noexcept {
	return &invoke_separated<State, Param, indices...>;
}

} // namespace Detail

struct FatFunctionPointer {
	void (*function)(void *) = 0;
	void *parameter = 0;
	
	void operator()() {
		return function(parameter);
	}
	#ifndef TL_FAT_FUNCTION_POINTER_NO_CONSTRUCTORS
	FatFunctionPointer() = default;
	FatFunctionPointer(FatFunctionPointer const &) = default;
	FatFunctionPointer(FatFunctionPointer &&) = default;
	FatFunctionPointer &operator=(FatFunctionPointer const &) = default;
	FatFunctionPointer &operator=(FatFunctionPointer &&) = default;
	FatFunctionPointer(void (*fn)()) {
		function = (void(*)(void*))fn;
		parameter = 0;
	}
	FatFunctionPointer(void (*fn)(void *param), void *param) {
		function = fn;
		parameter = param;
	}
	template <class Fn>
	FatFunctionPointer(Fn &&fn) requires (requires { fn(); } && !std::same_as<std::remove_cvref_t<Fn>, FatFunctionPointer>) {
		parameter = default_allocator.template allocate_uninitialized<Fn>();
		new(parameter) Fn(std::move(fn));
		function = [](void *parameter) {
			(*(Fn *)parameter)();
		};
	}
	#endif
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

inline FatFunctionPointer chain(FatFunctionPointer a, FatFunctionPointer b) {
	auto fns = DefaultAllocator{}.allocate<FatFunctionPointer>(2);
	fns[0] = a;
	fns[1] = b;

	FatFunctionPointer result;
	result.parameter = fns;
	result.function = [](void *param) {
		auto fns = (FatFunctionPointer *)param;
		fns[0]();
		fns[1]();
	};

	return result;
}
inline void chain(FatFunctionPointer *a, FatFunctionPointer b) {
	*a = chain(*a, b);
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
		sizeof(Fn) <= storage_capacity;
		alignof(Fn) <= storage_alignment;
	} {
		using StoredFn = std::decay_t<Fn>;
		new(storage) StoredFn(std::forward<Fn>(fn));

		function = Detail::get_invoke_separated<StoredFn, std::tuple<Args...>>(std::make_index_sequence<sizeof...(Args)>{});
	}

	template <class Fn>
	InlineFunction &operator=(Fn &&fn) requires requires {
		{ fn(std::declval<Args>()...) } -> std::same_as<Ret>;
		sizeof(Fn) <= storage_capacity;
		alignof(Fn) <= storage_alignment;
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
