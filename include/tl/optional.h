#pragma once
#include "common.h"

namespace tl {

template <class T>
struct Optional {
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
	explicit operator bool() const { return has_value; }
	union {
		T value;
	};
	bool has_value;

	T get() const {

		return value;
	}
#pragma warning(suppress: 4820)
};

}
