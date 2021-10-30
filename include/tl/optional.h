#pragma once
#include "common.h"

namespace tl {

template <class T>
struct Optional {
	Optional() {
		_has_value = false;
	}
	Optional(T that) {
		_value = that;
		_has_value = true;
	}
	Optional &operator=(T that) {
		return *new(this) Optional(that);
	}
	explicit operator bool() const { return _has_value; }

	bool has_value() const { return _has_value; }
	T const &value() const { assert(_has_value); return _value; }
	T       &value()       { assert(_has_value); return _value; }
	T const &value_unchecked() const { return _value; }
	T       &value_unchecked()       { return _value; }

private:
	union {
		T _value;
	};
	bool _has_value;
#pragma warning(suppress: 4820)
};

}
