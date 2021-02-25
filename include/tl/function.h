#pragma once
#include "common.h"

namespace TL {


struct Function {
	Allocator allocator;

	void (*function)(void *param);
	void *param;

	void operator()() {
		return function(param);
	}
};

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

inline Function create_function(void (*func)(void *param), void *param) {
	Function result = {};
	result.function = func;
	result.param = param;
	return result;
}
inline Function create_function(void (*func)()) {
	Function result = {};
	result.function = (void (*)(void *))func;
	return result;
}
template <class Fn, class ...Args>
Function create_function(Fn &&fn, Args &&...args) {
	if constexpr (std::is_convertible_v<Fn, void(*)()>) {
		return create_function((void(*)())fn);
	} else {
		auto allocator = current_allocator;

		using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
		auto params = ALLOCATE(Tuple, allocator);
		new(params) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);

		Function result = {};
		result.allocator = allocator;
		result.function = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
		result.param = params;
		return result;
	}
}
inline void free_function(Function &function) {
	if (function.allocator)
		FREE(function.allocator, function.param);
	function = {};
}


}