#pragma once
#include "common.h"

namespace TL {

struct OptionalBase_Trivial {};

template <class Optional>
struct OptionalBase_NonTrivial {
	~OptionalBase_NonTrivial() {
		Optional *opt = (Optional *)this;
		if (opt->_hasValue) {
			using T = decltype(opt->_value);
			opt->_value.~T();
		}
		opt->_hasValue = false;
	}
};

template <class T>
struct Optional : Conditional<std::is_trivially_destructible_v<T>, 
							  OptionalBase_Trivial, 
							  OptionalBase_NonTrivial<Optional<T>>> {
	Optional() : _hasValue(false) {}
	Optional(Optional const &that) {
		_hasValue = that._hasValue;
		if (that.has_value())
			new (&_value) T(that._value);
	}
	Optional(Optional &&that) {
		if (this == std::addressof(that))
			return;
		_hasValue = that._hasValue;
		if (that.has_value()) {
			new (&_value) T(std::move(that._value));
			that.reset();
		}
	}
	Optional(T const &that) : _value(that), _hasValue(true) {}
	Optional(T &&that) : _value(std::move(that)), _hasValue(true) {}
	~Optional() { reset(); }
	Optional &operator=(Optional const &that) { 
		if (that.has_value())
			emplace(*that); 
		else
			reset();
		return *this;
	}
	Optional &operator=(Optional &&that) { 
		if (this == std::addressof(that))
			return *this;
		if (that.has_value())
			emplace(*std::move(that)); 
		else
			reset();
		return *this;
	}
	Optional &operator=(T const &that) { emplace(that); return *this; }
	Optional &operator=(T &&that) { emplace(std::move(that)); return *this; }
	void reset() {
		if (_hasValue) {
			_value.~T();
			_hasValue = false;
		}
	}
	template <class ...Args>
	void emplace(Args &&...args) {
		if (_hasValue)
			reset();
		new (std::addressof(_value)) T(std::forward<Args>(args)...);
		_hasValue = true;
	}
	T &value() & { assert(_hasValue); return _value; }
	T &&value() && { assert(_hasValue); return _value; }
	T const &value() const & { assert(_hasValue); return std::move(_value); }
	T const &&value() const && { assert(_hasValue); return std::move(_value); }
	T *operator->() { return std::addressof(_value); }
	T const *operator->() const { return std::addressof(_value); }

	operator bool() const { return _hasValue; }
	bool has_value() const { return _hasValue; }

	T &operator*() & { assert(_hasValue); return _value; }
	T &&operator*() && { assert(_hasValue); return std::move(_value); }
	T const &operator*() const & { assert(_hasValue); return _value; }
	T const &&operator*() const && { assert(_hasValue); return std::move(_value); }

	union {
		T _value;
	};
	bool _hasValue;
#pragma warning(suppress: 4820)
};

}
