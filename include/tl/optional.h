#pragma once
#include "common.h"

namespace TL {

template <class T>
struct Optional {
	static_assert(std::is_trivial_v<T>, "don't do that shieet");
	Optional() {
		has_value = false;
	}
	Optional(T that) { 
		value = that;
		has_value = true;
	}
	Optional &operator=(T that) { 
		return *new(this) Optional(that);
	}
	operator bool() const { return has_value; }
	union {
		T value;
	};
	bool has_value;
#pragma warning(suppress: 4820)
};

}
