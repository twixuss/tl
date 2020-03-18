#pragma once
#include <xhash>
#include "math.h"
namespace std {
template<>
struct hash<V3> {
	size_t operator()(V3 val) const {
		return
			hash<f32>()(val.x) ^
			hash<f32>()(val.y) ^
			hash<f32>()(val.z);
	}
};
template<>
struct hash<V3i> {
	size_t operator()(V3i val) const {
		return
			hash<i32>()(val.x) ^
			hash<i32>()(val.y) ^
			hash<i32>()(val.z);
	}
};
}