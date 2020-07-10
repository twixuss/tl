#pragma once
#include "common.h"

namespace TL {

template <class T>
struct Optional {
	Optional() : _hasValue(false) {}
	Optional(T const &that) : _value(that), _hasValue(true) {}
	Optional(T &&that) : _value(std::move(that)), _hasValue(true) {}
	~Optional() {
		clear();
	}
	void clear() {
		if (_hasValue) {
			_value.~T();
		}
	}
	template <class ...Args>
	void emplace(Args &&...args) {
		if (_hasValue)
			clear();
		new (std::addressof(_value)) T(std::forward<Args>(args)...);
		_hasValue = true;
	}
	T &value() & { ASSERT(_hasValue); return _value; }
	T &&value() && { ASSERT(_hasValue); return _value; }
	T const &value() const & { ASSERT(_hasValue); return std::move(_value); }
	T const &&value() const && { ASSERT(_hasValue); return std::move(_value); }
	T *operator->() { return std::addressof(_value); }
	T const *operator->() const { return std::addressof(_value); }

	operator bool() const { return _hasValue; }
	bool has_value() const { return _hasValue; }

	T &operator*() & { ASSERT(_hasValue); return _value; }
	T &&operator*() && { ASSERT(_hasValue); return _value; }
	T const &operator*() const & { ASSERT(_hasValue); return std::move(_value); }
	T const &&operator*() const && { ASSERT(_hasValue); return std::move(_value); }

	union {
		T _value;
	};
	bool _hasValue;
};

}
