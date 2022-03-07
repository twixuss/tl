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


struct FunctionStorage {
	Allocator allocator;

	void (*function)(void *param);
	void *param;

	void operator()() {
		return function(param);
	}

	struct Maker {
		inline FunctionStorage operator>(void (*func)()) const {
			FunctionStorage result = {};
			result.function = (void (*)(void *))func;
			return result;
		}
		template <class Fn>
		inline FunctionStorage operator>(Fn &&fn) const {
			if constexpr (std::is_convertible_v<Fn, void(*)()>) {
				return operator>((void(*)())fn);
			} else {
				auto allocator = current_allocator;

				auto params = allocator.allocate_uninitialized<Fn>();
				new(params) Fn(std::forward<Fn>(fn));

				FunctionStorage result = {};
				result.allocator = allocator;
				result.function = [](void *param) {
					(*(Fn *)param)();
				};
				result.param = params;
				return result;
			}
		}
	};

	inline static constexpr Maker make = {};
};

inline FunctionStorage create_function_storage(void (*func)(void *param), void *param) {
	FunctionStorage result = {};
	result.function = func;
	result.param = param;
	return result;
}
inline FunctionStorage create_function_storage(void (*func)()) {
	FunctionStorage result = {};
	result.function = (void (*)(void *))func;
	return result;
}
template <class Fn, class ...Args>
FunctionStorage create_function_storage(Fn &&fn, Args &&...args) {
	if constexpr (std::is_convertible_v<Fn, void(*)()>) {
		return create_function_storage((void(*)())fn);
	} else {
		auto allocator = current_allocator;

		using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
		auto params = allocator.allocate_uninitialized<Tuple>();
		new(params) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);

		FunctionStorage result = {};
		result.allocator = allocator;
		result.function = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
		result.param = params;
		return result;
	}
}
inline void free(FunctionStorage &function) {
	if (function.allocator)
		function.allocator.free(function.param);
	function = {};
}


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
