#pragma once
#include "hash.h"

namespace tl {

//
// This is a mess.
// for_each has always been calling fn with a reference type,
// but now as I refactor hash maps to store keys and values
// separately, this is no longer possible. I tried some stuff
// with reference wrapping with conversions, but... it's a mess.
// For now I'm making key-value for_each'es pass a struct with pointers
// instead of references. Maybe I should do pointers everywhere.

template <class Key_, class Value_>
struct KeyValue {
	using Key = Key_;
	using Value = Value_;
	Key key;
	Value value;

	constexpr bool operator==(KeyValue const &that) const requires(std::equality_comparable<Key> && std::equality_comparable<Value>) {
		return key == that.key && value == that.value;
	}
};

template <class Key, class Value>
struct KeyValueRef {
	Key const *key = 0;
	Value *value = 0;

	constexpr explicit operator bool() const { return key; }

	KeyValue<Key, Value> load() { return {*key, *value}; }
};

template <class T, class Key, class Value>
concept HashMapIterFn = std::invocable<T, KeyValueRef<Key, Value>>;

}
