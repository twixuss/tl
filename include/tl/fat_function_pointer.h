#pragma once
#include "function_detail.h"

namespace tl {

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
	template <class Allocator, class Fn>
	static FatFunctionPointer from(Allocator allocator, Fn &&fn) requires (requires { fn(); } && !std::same_as<std::remove_cvref_t<Fn>, FatFunctionPointer>) {
        FatFunctionPointer result;
		result.parameter = allocator.template allocate_uninitialized<Fn>();
		new(result.parameter) Fn(std::move(fn));
		result.function = [](void *parameter) {
			(*(Fn *)parameter)();
		};
        return result;
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

}