#pragma once
#include "common.h"

namespace tl {

inline static constexpr struct null_opt_t {} null_opt;

template <class T>
struct Optional {
	Optional() {
		_has_value = false;
	}
	Optional(T that) {
		_value = that;
		_has_value = true;
	}
	Optional(null_opt_t) {
		_has_value = false;
	}
	Optional &operator=(T that) {
		return *new(this) Optional(that);
	}
	Optional &operator=(null_opt_t) {
		return *new(this) Optional();
	}

	void reset() {
		_has_value = false;
	}

	explicit operator bool() const { return _has_value; }

	bool has_value() const { return _has_value; }
	T const &value() const { assert_always(_has_value); return _value; }
	T       &value()       { assert_always(_has_value); return _value; }
	T const &value_unchecked() const { return _value; }
	T       &value_unchecked()       { return _value; }

	template <class Fallback>
	T value_or(Fallback &&fallback) {
		if (_has_value)
			return _value;
		if constexpr (std::is_convertible_v<Fallback, T>)
			return fallback;
		else
			return fallback();
	}

private:
	union {
		T _value;
	};
	bool _has_value;
#pragma warning(suppress: 4820)
};

template <class T> Optional<T> operator+(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() + b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator-(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() - b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator*(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() * b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator/(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() / b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator%(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() % b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator^(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() ^ b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator&(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() & b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator|(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() | b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator<<(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() << b.value_unchecked()} : Optional<T>{}; }
template <class T> Optional<T> operator>>(Optional<T> a, Optional<T> b) { return (a.has_value() && b.has_value()) ? Optional<T>{a.value_unchecked() >> b.value_unchecked()} : Optional<T>{}; }

}
