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

}
