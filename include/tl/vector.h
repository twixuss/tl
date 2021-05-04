#pragma once

#include "common.h"

namespace TL {

#define DEFN_2 \
	struct { Scalar x, y; }; \
	Scalar s[2];\
	template <class U>\
	forceinline constexpr explicit operator v2<U>() const { return {(U)x, (U)y}; } \
	forceinline constexpr v2 operator+() const { return *this; } \
	forceinline constexpr v2 yx() const { return {y, x}; }

#define DEFN_3 \
	using v2 = v2<Scalar>; \
	struct { Scalar x, y, z; }; \
	v2 xy; \
	struct { Scalar _pad; v2 yz; }; \
	Scalar s[3];\
	template <class U>\
	forceinline constexpr explicit operator v3<U>() const { return {(U)x, (U)y, (U)z}; } \
	forceinline constexpr v3 operator+() const { return *this; } \
	forceinline constexpr v3 yzx() const { return {y, z, x}; } \
	forceinline constexpr v3 zxy() const { return {z, x, y}; }


#define DEFN_4 \
	using v2 = v2<Scalar>; \
	using v3 = v3<Scalar>; \
	struct { Scalar x, y, z, w; }; \
	struct { v2 xy; v2 zw; }; \
	v3 xyz; \
	struct { Scalar _; v2 yz; }; \
	struct { Scalar _; v3 yzw; }; \
	Scalar s[4];\
	template <class U>\
	forceinline constexpr explicit operator v4<U>() const { return {(U)x, (U)y, (U)z, (U)w}; } \
	forceinline constexpr v4 operator+() const { return *this; }

#define UNOP_2(o) \
	forceinline constexpr v2 operator o() const { return {o x, o y}; }

#define UNOP_3(o) \
	forceinline constexpr v3 operator o() const { return {o x, o y, o z}; }

#define UNOP_4(o) \
	forceinline constexpr v4 operator o() const { return {o x, o y, o z, o w}; }

#define BINOP_2(o) \
	forceinline constexpr v2 operator o(v2 b) const { return {x o b.x, y o b.y}; } \
	forceinline constexpr v2 operator o(Scalar b) const { return {x o b, y o b}; } \
	forceinline constexpr friend v2 operator o(Scalar a, v2 b) { return {a o b.x, a o b.y};} \
	forceinline constexpr v2 &operator o=(v2 b) { return x o= b.x, y o= b.y, *this;} \
	forceinline constexpr v2 &operator o=(Scalar b) { return x o= b, y o= b, *this;}

#define BINOP_3(o) \
	forceinline constexpr v3 operator o(v3 b) const { return {x o b.x, y o b.y, z o b.z}; } \
	forceinline constexpr v3 operator o(Scalar b) const { return {x o b, y o b, z o b}; } \
	forceinline constexpr friend v3 operator o(Scalar a, v3 b) { return {a o b.x, a o b.y, a o b.z};} \
	forceinline constexpr v3 &operator o=(v3 b) { return x o= b.x, y o= b.y, z o= b.z, *this;} \
	forceinline constexpr v3 &operator o=(Scalar b) { return x o= b, y o= b, z o= b, *this;}

#define BINOP_4(o) \
	forceinline constexpr v4 operator o(v4 b) const { return {x o b.x, y o b.y, z o b.z, w o b.w}; } \
	forceinline constexpr v4 operator o(Scalar b) const { return {x o b, y o b, z o b, w o b}; } \
	forceinline constexpr friend v4 operator o(Scalar a, v4 b) { return {a o b.x, a o b.y, a o b.z, a o b.w};} \
	forceinline constexpr v4 &operator o=(v4 b) { return x o= b.x, y o= b.y, z o= b.z, w o= b.w, *this;} \
	forceinline constexpr v4 &operator o=(Scalar b) { return x o= b, y o= b, z o= b, w o= b, *this;}

#define EQ_2 \
	forceinline constexpr bool operator==(v2 b) const { return x == b.x && y == b.y; } \
	forceinline constexpr bool operator!=(v2 b) const { return x != b.x || y != b.y; }

#define EQ_3 \
	forceinline constexpr bool operator==(v3 b) const { return x == b.x && y == b.y && z == b.z; } \
	forceinline constexpr bool operator!=(v3 b) const { return x != b.x || y != b.y || z != b.z; }

#define EQ_4 \
	forceinline constexpr bool operator==(v4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; } \
	forceinline constexpr bool operator!=(v4 b) const { return x != b.x || y != b.y || z != b.z || w != b.w; }


template <class _Scalar>
union v2 {
	using Scalar = _Scalar;
	DEFN_2
	UNOP_2(-)
	BINOP_2(+)
	BINOP_2(-)
	BINOP_2(*)
	BINOP_2(/)
	BINOP_2(%)
	BINOP_2(^)
	BINOP_2(&)
	BINOP_2(|)
	BINOP_2(<<)
	BINOP_2(>>)
	EQ_2
};

template <class _Scalar>
union v3 {
	using Scalar = _Scalar;
	DEFN_3
	UNOP_3(-)
	BINOP_3(+)
	BINOP_3(-)
	BINOP_3(*)
	BINOP_3(/)
	BINOP_3(%)
	BINOP_3(^)
	BINOP_3(&)
	BINOP_3(|)
	BINOP_3(<<)
	BINOP_3(>>)
	EQ_3
};

template <class _Scalar>
union v4 {
	using Scalar = _Scalar;
	DEFN_4
	UNOP_4(-)
	BINOP_4(+)
	BINOP_4(-)
	BINOP_4(*)
	BINOP_4(/)
	BINOP_4(%)
	BINOP_4(^)
	BINOP_4(&)
	BINOP_4(|)
	BINOP_4(<<)
	BINOP_4(>>)
	EQ_4
};

using v2s8 = v2<s8>; using v3s8 = v3<s8>; using v4s8 = v4<s8>;
using v2u8 = v2<u8>; using v3u8 = v3<u8>; using v4u8 = v4<u8>;

using v2s16 = v2<s16>; using v3s16 = v3<s16>; using v4s16 = v4<s16>;
using v2u16 = v2<u16>; using v3u16 = v3<u16>; using v4u16 = v4<u16>;

using v2f32 = v2<f32>; using v3f32 = v3<f32>; using v4f32 = v4<f32>;
using v2s32 = v2<s32>; using v3s32 = v3<s32>; using v4s32 = v4<s32>;
using v2u32 = v2<u32>; using v3u32 = v3<u32>; using v4u32 = v4<u32>;

using v2f64 = v2<f64>; using v3f64 = v3<f64>; using v4f64 = v4<f64>;
using v2s64 = v2<s64>; using v3s64 = v3<s64>; using v4s64 = v4<s64>;
using v2u64 = v2<u64>; using v3u64 = v3<u64>; using v4u64 = v4<u64>;

using v2f = v2f32; using v3f = v3f32; using v4f = v4f32;
using v2s = v2s32; using v3s = v3s32; using v4s = v4s32;
using v2u = v2u32; using v3u = v3u32; using v4u = v4u32;

template<class T> inline static constexpr bool is_vector = false;
template<class T> inline static constexpr bool is_vector<v2<T>> = true;
template<class T> inline static constexpr bool is_vector<v3<T>> = true;
template<class T> inline static constexpr bool is_vector<v4<T>> = true;

template<class T> inline static constexpr bool is_integer_like<v2<T>> = is_integer_like<T>;
template<class T> inline static constexpr bool is_integer_like<v3<T>> = is_integer_like<T>;
template<class T> inline static constexpr bool is_integer_like<v4<T>> = is_integer_like<T>;

template <class T> inline static constexpr u32 dimension_of = 0;
template <class T> inline static constexpr u32 dimension_of<v2<T>> = 2;
template <class T> inline static constexpr u32 dimension_of<v3<T>> = 3;
template <class T> inline static constexpr u32 dimension_of<v4<T>> = 4;

forceinline constexpr v2f V2f(f32 = 0);
forceinline constexpr v2s V2s(s32 = 0);
forceinline constexpr v2u V2u(u32 = 0);

forceinline constexpr v3f V3f(f32 = 0);
forceinline constexpr v3s V3s(s32 = 0);
forceinline constexpr v3u V3u(u32 = 0);

forceinline constexpr v4f V4f(f32 = 0);
forceinline constexpr v4s V4s(s32 = 0);
forceinline constexpr v4u V4u(u32 = 0);

forceinline constexpr v2f V2f(f32 x, f32 y);
forceinline constexpr v2s V2s(s32 x, s32 y);
forceinline constexpr v2u V2u(u32 x, u32 y);

forceinline constexpr v3f V3f(f32 x, f32 y, f32 z);
forceinline constexpr v3s V3s(s32 x, s32 y, s32 z);
forceinline constexpr v3u V3u(u32 x, u32 y, u32 z);

#undef DEFN_2
#undef DEFN_3
#undef DEFN_4
#undef UNOP_2
#undef UNOP_3
#undef UNOP_4
#undef BINOP_2
#undef BINOP_3
#undef BINOP_4
#undef EQ_2
#undef EQ_3
#undef EQ_4

}
