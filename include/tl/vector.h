#pragma once

#include "common.h"

namespace tl {

#define DEFN_2 \
	struct { Scalar x, y; }; \
	Scalar s[2];\
	template <class U>\
	forceinline constexpr explicit operator v2<U>() const { return {(U)x, (U)y}; } \
	forceinline constexpr v2 operator+() const { return *this; } \
	forceinline constexpr v2 yx() const { return {y, x}; } \


#define DEFN_3 \
	using v2 = v2<Scalar>; \
	struct { Scalar x, y, z; }; \
	v2 xy; \
	struct { Scalar _pad; v2 yz; }; \
	Scalar s[3];\
	template <class U>\
	forceinline constexpr explicit operator v3<U>() const { return {(U)x, (U)y, (U)z}; } \
	forceinline constexpr v3 operator+() const { return *this; } \
	forceinline constexpr v2 yx() const { return {y, x}; } \
	forceinline constexpr v2 xz() const { return {x, z}; } \
	forceinline constexpr v2 zx() const { return {z, x}; } \
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

#define CMPOP_2(o) \
	forceinline constexpr Mask operator o(v2 b) const { return {x o b.x, y o b.y}; } \
	forceinline constexpr Mask operator o(Scalar b) const { return {x o b, y o b}; } \
	forceinline constexpr friend Mask operator o(Scalar a, v2 b) { return {a o b.x, a o b.y};} \

#define CMPOP_3(o) \
	forceinline constexpr Mask operator o(v3 b) const { return {x o b.x, y o b.y, z o b.z}; } \
	forceinline constexpr Mask operator o(Scalar b) const { return {x o b, y o b, z o b}; } \
	forceinline constexpr friend Mask operator o(Scalar a, v3 b) { return {a o b.x, a o b.y, a o b.z};} \

#define CMPOP_4(o) \
	forceinline constexpr Mask operator o(v4 b) const { return {x o b.x, y o b.y, z o b.z, w o b.w}; } \
	forceinline constexpr Mask operator o(Scalar b) const { return {x o b, y o b, z o b, w o b}; } \
	forceinline constexpr friend Mask operator o(Scalar a, v4 b) { return {a o b.x, a o b.y, a o b.z, a o b.w};} \

template <class _Scalar>
union v2 {
	using Scalar = _Scalar;
	using Mask = v2<bool>;
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
	CMPOP_2(&&)
	CMPOP_2(||)
	CMPOP_2(==)
	CMPOP_2(!=)
	CMPOP_2(<)
	CMPOP_2(>)
	CMPOP_2(<=)
	CMPOP_2(>=)
};

template <class _Scalar>
union v3 {
	using Scalar = _Scalar;
	using Mask = v3<bool>;
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
	CMPOP_3(&&)
	CMPOP_3(||)
	CMPOP_3(==)
	CMPOP_3(!=)
	CMPOP_3(<)
	CMPOP_3(>)
	CMPOP_3(<=)
	CMPOP_3(>=)
};

template <class _Scalar>
union v4 {
	using Scalar = _Scalar;
	using Mask = v4<bool>;
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
	CMPOP_4(&&)
	CMPOP_4(||)
	CMPOP_4(==)
	CMPOP_4(!=)
	CMPOP_4(<)
	CMPOP_4(>)
	CMPOP_4(<=)
	CMPOP_4(>=)
};

/*
#ifdef TL_ENABLE_VEC4_SIMD

#undef BINOP_4
#define BINOP_4(o, func, set1) \
	forceinline v4 operator o(v4 b) const { return {.m = func(m, b.m)}; } \
	forceinline v4 operator o(Scalar b) const { return {.m = func(m, set1(b))}; } \
	forceinline friend v4 operator o(Scalar a, v4 b) { return {.m = func(set1(a), b.m)};} \
	forceinline v4 &operator o=(v4 b) { return m = func(m, b.m), *this;} \
	forceinline v4 &operator o=(Scalar b) { return m = func(m, set1(b)), *this;}

template <>
union v4<f32> {
	using Scalar = f32;
	using Mask = v4<bool>;
	DEFN_4
	f32x4 m;

	forceinline v4 operator-() const { return {.m = _mm_xor_ps(m, _mm_set1_ps(-1))}; }

	BINOP_4(+, f32x4_add, f32x4_set1)
	BINOP_4(-, f32x4_sub, f32x4_set1)
	BINOP_4(*, f32x4_mul, f32x4_set1)
	BINOP_4(/, f32x4_div, f32x4_set1)

	EQ_4
};

#endif
*/

#undef DEFN_2
#undef DEFN_3
#undef DEFN_4
#undef UNOP_2
#undef UNOP_3
#undef UNOP_4
#undef BINOP_2
#undef BINOP_3
#undef BINOP_4
#undef CMPOP_2
#undef CMPOP_3
#undef CMPOP_4
#undef EQ_2
#undef EQ_3
#undef EQ_4

using v2b = v2<bool>; using v3b = v3<bool>; using v4b = v4<bool>;

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

#define V2(f32, v2f, V2f)                                     \
	forceinline constexpr v2f V2f(f32 x, f32 y) { return {x, y}; } \
	forceinline constexpr v2f V2f(f32 v = 0) { return {v, v}; }
V2(f32, v2f, V2f);
V2(s32, v2s, V2s);
V2(u32, v2u, V2u);
#undef V2

#define V3(f32, v2f, v3f, V3f)                                          \
	forceinline constexpr v3f V3f(f32 x, f32 y, f32 z) { return {x, y, z}; } \
	forceinline constexpr v3f V3f(f32 v = 0) { return {v, v, v}; }               \
	forceinline constexpr v3f V3f(v2f xy, f32 z) { return {xy.x, xy.y, z}; } \
	forceinline constexpr v3f V3f(f32 x, v2f yz) { return {x, yz.x, yz.y}; }
V3(f32, v2f, v3f, V3f);
V3(s32, v2s, v3s, V3s);
V3(u32, v2u, v3u, V3u);
#undef V3

#define V4(f32, v2f, v3f, v4f, V4f)                                        \
	forceinline constexpr v4f V4f(f32 x, f32 y, f32 z, f32 w) { return {x, y, z, w}; } \
	forceinline constexpr v4f V4f(f32 v = 0) { return {v, v, v, v}; }                      \
	forceinline constexpr v4f V4f(v2f xy, v2f zw) { return {xy.x, xy.y, zw.x, zw.y}; } \
	forceinline constexpr v4f V4f(v2f xy, f32 z, f32 w) { return {xy.x, xy.y, z, w}; } \
	forceinline constexpr v4f V4f(f32 x, f32 y, v2f zw) { return {x, y, zw.x, zw.y}; } \
	forceinline constexpr v4f V4f(v3f xyz, f32 w) { return {xyz.x, xyz.y, xyz.z, w}; } \
	forceinline constexpr v4f V4f(f32 x, v3f yzw) { return {x, yzw.x, yzw.y, yzw.z}; }
V4(f32, v2f, v3f, v4f, V4f);
V4(s32, v2s, v3s, v4s, V4s);
V4(u32, v2u, v3u, v4u, V4u);
#undef V4

#define C(vec, Vec) forceinline constexpr vec Vec(vec v) { return v; }
C(v2f, V2f) C(v2s, V2s) C(v2u, V2u);
C(v3f, V3f) C(v3s, V3s) C(v3u, V3u);
C(v4f, V4f) C(v4s, V4s) C(v4u, V4u);
#undef C

forceinline constexpr bool all_true(v2b v) { return v.x && v.y; }
forceinline constexpr bool all_true(v3b v) { return v.x && v.y && v.z; }
forceinline constexpr bool all_true(v4b v) { return v.x && v.y && v.z && v.w; }

forceinline constexpr bool any_true(v2b v) { return v.x || v.y; }
forceinline constexpr bool any_true(v3b v) { return v.x || v.y || v.z; }
forceinline constexpr bool any_true(v4b v) { return v.x || v.y || v.z || v.w; }

forceinline constexpr bool all_false(v2b v) { return !any_true(v); }
forceinline constexpr bool all_false(v3b v) { return !any_true(v); }
forceinline constexpr bool all_false(v4b v) { return !any_true(v); }

forceinline constexpr bool any_false(v2b v) { return !all_true(v); }
forceinline constexpr bool any_false(v3b v) { return !all_true(v); }
forceinline constexpr bool any_false(v4b v) { return !all_true(v); }

}
