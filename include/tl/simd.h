#pragma once
#include "common.h"
#include "vector.h"
#include <immintrin.h>

namespace tl {

#define DEFINE_MASK_SCALAR(bits)                                                       \
struct Mask##bits {                                                                    \
	u##bits value;                                                                     \
	operator bool() { return value; }                                                  \
	Mask##bits &operator=(bool new_value) { return value = new_value * -1, *this; }    \
	Mask##bits operator!() { return {(u##bits)~value}; }                               \
	Mask##bits operator&(Mask##bits that) { return {(u##bits)(value & that.value)}; }  \
	Mask##bits operator|(Mask##bits that) { return {(u##bits)(value | that.value)}; }  \
	Mask##bits operator&&(Mask##bits that) { return {(u##bits)(value & that.value)}; } \
	Mask##bits operator||(Mask##bits that) { return {(u##bits)(value | that.value)}; } \
	Mask##bits &operator&=(Mask##bits that) { return value &= that.value, *this; }     \
	Mask##bits &operator|=(Mask##bits that) { return value |= that.value, *this; }     \
}; \
constexpr Mask##bits false##bits = {0}; \
constexpr Mask##bits true##bits = {-1}; \

DEFINE_MASK_SCALAR(8);
DEFINE_MASK_SCALAR(16);
DEFINE_MASK_SCALAR(32);
DEFINE_MASK_SCALAR(64);

#undef DEFINE_MASK_SCALAR

template <class T>
concept CMaskScalar = std::is_same_v<T, Mask8> || std::is_same_v<T, Mask16> || std::is_same_v<T, Mask32> || std::is_same_v<T, Mask64>;

template <class Scalar, umm count>
struct SimdTypeT { using Type = Empty; };

template <class Scalar, umm count>
using SimdType = typename SimdTypeT<Scalar, count>::Type;

#define S(Scalar, count, mtype) \
	template <> \
	struct SimdTypeT<Scalar, count> { \
		using Type = mtype; \
	}

S(Mask8, 16, __m128i);
S(Mask16, 8, __m128i);
S(Mask32, 4, __m128i);
S(Mask64, 2, __m128i);
S(u8, 16, __m128i);
S(u16, 8, __m128i);
S(u32, 4, __m128i);
S(u64, 2, __m128i);
S(s8, 16, __m128i);
S(s16, 8, __m128i);
S(s32, 4, __m128i);
S(s64, 2, __m128i);
S(f32, 4, __m128);
S(f64, 2, __m128d);

S(Mask8, 32, __m256i);
S(Mask16, 16, __m256i);
S(Mask32, 8, __m256i);
S(Mask64, 4, __m256i);
S(u8, 32, __m256i);
S(u16, 16, __m256i);
S(u32, 8, __m256i);
S(u64, 4, __m256i);
S(s8, 32, __m256i);
S(s16, 16, __m256i);
S(s32, 8, __m256i);
S(s64, 4, __m256i);
S(f32, 8, __m256);
S(f64, 4, __m256d);

S(Mask8, 64, __m512i);
S(Mask16, 32, __m512i);
S(Mask32, 16, __m512i);
S(Mask64, 8, __m512i);
S(u8, 64, __m512i);
S(u16, 32, __m512i);
S(u32, 16, __m512i);
S(u64, 8, __m512i);
S(s8, 64, __m512i);
S(s16, 32, __m512i);
S(s32, 16, __m512i);
S(s64, 8, __m512i);
S(f32, 16, __m512);
S(f64, 8, __m512d);

#undef S

template <class Scalar_, umm count_>
union Vector;

template <class ToScalar, class FromScalar, umm count>
struct Converter {
	Vector<ToScalar, count> operator()(Vector<FromScalar, count> from);
};

#define VECTOR_BASE(count_)                               \
	using Scalar = Scalar_;                               \
	inline static constexpr umm count = count_;           \
	                                                      \
	Scalar s[count];                                      \
	SimdType<Scalar, count> m;                            \
	                                                      \
	Scalar &operator[](umm i) { return s[i]; }            \
	Scalar const &operator[](umm i) const { return s[i]; }\
	                                                      \
	template <class U>                                    \
		requires requires(Scalar s) { (U)s; }             \
	forceinline explicit operator Vector<U, count>() const { return Converter<U, Scalar, count>{}(*this); }

template <class Scalar_, umm count_>
union Vector {
	VECTOR_BASE(count_);
	Scalar x;
};

template <class Scalar_>
union Vector<Scalar_, 2> {
	VECTOR_BASE(2);
	struct {
		Scalar x, y;
	};
};

template <class Scalar_>
union Vector<Scalar_, 3> {
	VECTOR_BASE(3);
	struct {
		Scalar x, y, z;
	};
	Vector<Scalar, 2> xy;
	struct {
		Scalar _dummy0;
		Vector<Scalar, 2> yz;
	};
};

template <class Scalar_>
union Vector<Scalar_, 4> {
	VECTOR_BASE(4);
	struct {
		Scalar x, y, z, w;
	};
	Vector<Scalar, 2> xy;
	Vector<Scalar, 3> xyz;
	struct {
		Scalar _dummy0;
		union {
			Vector<Scalar, 2> yz;
			Vector<Scalar, 3> yzw;
		};
	};
	struct {
		Vector<Scalar, 2> _dummy1;
		Vector<Scalar, 2> zw;
	};
};

#undef VECTOR_BASE

template <class ToScalar, class FromScalar, umm count>
Vector<ToScalar, count> Converter<ToScalar, FromScalar, count>::operator()(Vector<FromScalar, count> from) {
	Vector<ToScalar, count> result;
	for (umm i = 0; i < count; ++i) {
		result.s[i] = (ToScalar)from.s[i];
	}
	return result;
}

template <class Scalar_, umm count_>
union Vector<v2<Scalar_>, count_> {
	using Scalar = Scalar_;
	inline static constexpr umm count = count_;

	struct {
		Vector<Scalar, count> x;
		Vector<Scalar, count> y;
	};
	Vector<Scalar, count> s[2];

	struct Accessor {
		Vector &v;
		umm i;

		v2<Scalar> value() {
			return {
				v.x.s[i], 
				v.y.s[i],
			};
		}
		operator v2<Scalar>() { return value(); }
		Accessor &operator=(v2<Scalar> n) {
			return 
				v.x.s[i] = n.x, 
				v.y.s[i] = n.y,
				*this;
		}
		Accessor &operator=(Accessor that) { return *this = that.value(); }
	};

	Accessor operator[](umm i) { return {*this, i}; }
	Accessor const operator[](umm i) const { return {*this, i}; }

	template <class U>
		requires requires(Scalar s) { (U)s; }
	forceinline explicit operator Vector<v2<U>, count>() const {
		return {
			(Vector<U, count>)x,
			(Vector<U, count>)y,
		};
	}
};

template <class Scalar_, umm count_>
union Vector<v3<Scalar_>, count_> {
	using Scalar = Scalar_;
	inline static constexpr umm count = count_;

	struct {
		Vector<Scalar, count> x;
		Vector<Scalar, count> y;
		Vector<Scalar, count> z;
	};
	Vector<Scalar, count> s[3];

	struct Accessor {
		Vector &v;
		umm i;

		v3<Scalar> value() {
			return {
				v.x.s[i], 
				v.y.s[i],
				v.z.s[i],
			};
		}
		operator v3<Scalar>() { return value(); }
		Accessor &operator=(v3<Scalar> n) {
			return 
				v.x.s[i] = n.x,
				v.y.s[i] = n.y,
				v.z.s[i] = n.z,
				*this;
		}
	};

	Accessor operator[](umm i) { return {*this, i}; }
	Accessor const operator[](umm i) const { return {*this, i}; }

	template <class U>
		requires requires(Scalar s) { (U)s; }
	forceinline explicit operator Vector<v3<U>, count>() const {
		return {
			(Vector<U, count>)x,
			(Vector<U, count>)y,
			(Vector<U, count>)z,
		};
	}
};

template <class Scalar_, umm count_>
union Vector<v4<Scalar_>, count_> {
	using Scalar = Scalar_;
	inline static constexpr umm count = count_;

	struct {
		Vector<Scalar, count> x;
		Vector<Scalar, count> y;
		Vector<Scalar, count> z;
		Vector<Scalar, count> w;
	};
	Vector<Scalar, count> s[4];

	struct Accessor {
		Vector &v;
		umm i;

		v4<Scalar> value() {
			return {
				v.x.s[i], 
				v.y.s[i],
				v.z.s[i],
				v.w.s[i],
			};
		}
		operator v4<Scalar>() { return value(); }
		Accessor &operator=(v4<Scalar> n) {
			return 
				v.x.s[i] = n.x,
				v.y.s[i] = n.y,
				v.z.s[i] = n.z,
				v.w.s[i] = n.w,
				*this;
		}
	};

	Accessor operator[](umm i) { return {*this, i}; }
	Accessor const operator[](umm i) const { return {*this, i}; }

	template <class U>
		requires requires(Scalar s) { (U)s; }
	forceinline explicit operator Vector<v4<U>, count>() const {
		return {
			(Vector<U, count>)x,
			(Vector<U, count>)y,
			(Vector<U, count>)z,
			(Vector<U, count>)w,
		};
	}
};

TL_DECLARE_CONCEPT(Vector);

template <class Scalar>
using MaskForScalar = TypeAt<log2(sizeof Scalar), Mask8, Mask16, Mask32, Mask64>;

template <class Scalar, umm count>
struct MaskVectorT {
	using Type = Vector<MaskForScalar<Scalar>, count>;
};
template <class Scalar, umm count>
struct MaskVectorT<v2<Scalar>, count> {
	using Type = Vector<v2<MaskForScalar<Scalar>>, count>;
};
template <class Scalar, umm count>
struct MaskVectorT<v3<Scalar>, count> {
	using Type = Vector<v3<MaskForScalar<Scalar>>, count>;
};
template <class Scalar, umm count>
struct MaskVectorT<v4<Scalar>, count> {
	using Type = Vector<v4<MaskForScalar<Scalar>>, count>;
};

template <class Scalar, umm count>
using MaskVector = typename MaskVectorT<Scalar, count>::Type; // FIXME: compiler error when two last u64's are not provided. wtf

template <umm count, class Scalar>
forceinline Vector<Scalar, count> broadcast(Scalar s) {
	Vector<Scalar, count> r;
	for (umm i = 0; i < count; ++i) {
		r.s[i] = s;
	}
	return r;
}
template <umm count, class Scalar>
forceinline Vector<v2<Scalar>, count> V2(Scalar s) {
	Vector<v2<Scalar>, count> r;
	r.x = broadcast<count>(s);
	r.y = broadcast<count>(s);
	return r;
}
template <umm count, class Scalar>
forceinline Vector<v2<Scalar>, count> V2(v2<Scalar> s) {
	Vector<v2<Scalar>, count> r;
	r.x = broadcast<count>(s.x);
	r.y = broadcast<count>(s.y);
	return r;
}
template <class Scalar, umm count>
forceinline Vector<v2<Scalar>, count> V2(Vector<Scalar, count> x, Vector<Scalar, count> y) {
	Vector<v2<Scalar>, count> r;
	r.x = x;
	r.y = y;
	return r;
}
template <umm count, class Scalar>
forceinline Vector<v4<Scalar>, count> V4(v4<Scalar> s) {
	Vector<v4<Scalar>, count> r;
	r.x = broadcast<count>(s.x);
	r.y = broadcast<count>(s.y);
	r.z = broadcast<count>(s.z);
	r.w = broadcast<count>(s.w);
	return r;
}
template <class Scalar, umm count>
forceinline Vector<Scalar, count> select(MaskVector<Scalar, count> mask, Vector<Scalar, count> a, Vector<Scalar, count> b) {
	Vector<Scalar, count> r;
	for (umm i = 0; i < count; ++i) {
		r.s[i] = mask.s[i] ? a.s[i] : b.s[i];
	}
	return r;
}
template <class Scalar, umm count>
forceinline Vector<Scalar, count> select(MaskVector<Scalar, count> mask, std::type_identity_t<Scalar> a, Vector<Scalar, count> b) {
	return select(mask, broadcast<count>(a), b);
}
template <class Scalar, umm count>
forceinline Vector<Scalar, count> select(MaskVector<Scalar, count> mask, Vector<Scalar, count> a, std::type_identity_t<Scalar> b) {
	return select(mask, a, broadcast<count>(b));
}
template <class Scalar, umm count>
forceinline Vector<Scalar, count> select(MaskVector<Scalar, count> mask, Scalar a, Scalar b) {
	return select(mask, broadcast<count>(a), broadcast<count>(b));
}
template <class Scalar, umm count>
forceinline Vector<v2<Scalar>, count> select(MaskVector<Scalar, count> mask, Vector<v2<Scalar>, count> a, Vector<v2<Scalar>, count> b) {
	Vector<v2<Scalar>, count> r;
	for (umm i = 0; i < 2; ++i) {
		r.s[i] = select(mask, a.s[i], b.s[i]);
	}
	return r;
}
template <class Scalar, umm count>
forceinline Vector<v3<Scalar>, count> select(MaskVector<Scalar, count> mask, Vector<v3<Scalar>, count> a, Vector<v3<Scalar>, count> b) {
	Vector<v3<Scalar>, count> r;
	for (umm i = 0; i < 3; ++i) {
		r.s[i] = select(mask, a.s[i], b.s[i]);
	}
	return r;
}
template <class Scalar, umm count>
forceinline Vector<v4<Scalar>, count> select(MaskVector<Scalar, count> mask, Vector<v4<Scalar>, count> a, Vector<v4<Scalar>, count> b) {
	Vector<v4<Scalar>, count> r;
	for (umm i = 0; i < 4; ++i) {
		r.s[i] = select(mask, a.s[i], b.s[i]);
	}
	return r;
}

#define UNOP(op) \
	template <class Scalar, umm count> requires requires(Scalar s) { op s; } forceinline constexpr Vector<Scalar, count> operator op(Vector<Scalar, count> a) { for (umm i = 0; i < count; ++i) a.s[i] = op a.s[i]; return a; }

UNOP(+)
UNOP(-)
UNOP(!)

#undef UNOP

template <class Scalar, umm count> requires requires(Scalar s) { ++s; } forceinline constexpr Vector<Scalar, count> &operator++(Vector<Scalar, count> &a) { for (umm i = 0; i < count; ++i) ++a.s[i]; return a; }
template <class Scalar, umm count> requires requires(Scalar s) { --s; } forceinline constexpr Vector<Scalar, count> &operator--(Vector<Scalar, count> &a) { for (umm i = 0; i < count; ++i) --a.s[i]; return a; }

template <class Scalar, umm count> requires requires(Scalar s) { ++s; } forceinline constexpr Vector<Scalar, count> operator++(Vector<Scalar, count> a, int) { auto b = a; for (umm i = 0; i < count; ++i) a.s[i]++; return b; }
template <class Scalar, umm count> requires requires(Scalar s) { --s; } forceinline constexpr Vector<Scalar, count> operator--(Vector<Scalar, count> a, int) { auto b = a; for (umm i = 0; i < count; ++i) a.s[i]--; return b; }

#define O(op) \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) { for (umm i = 0; i < count; ++i) a.s[i] = a.s[i] op b.s[i]; return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<Scalar, count> operator op(Vector<Scalar, count> a, std::type_identity_t<Scalar> b) { return a op broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<Scalar, count> operator op(std::type_identity_t<Scalar> a, Vector<Scalar, count> b) { return broadcast<count>(a) op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v2<Scalar>, count> operator op(Vector<v2<Scalar>, count> a, Vector<v2<Scalar>, count> b)    { for (umm j = 0; j < 2; ++j) { a.s[j] = a.s[j] op b.s[j]; } return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v3<Scalar>, count> operator op(Vector<v3<Scalar>, count> a, Vector<v3<Scalar>, count> b)    { for (umm j = 0; j < 3; ++j) { a.s[j] = a.s[j] op b.s[j]; } return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v4<Scalar>, count> operator op(Vector<v4<Scalar>, count> a, Vector<v4<Scalar>, count> b)    { for (umm j = 0; j < 4; ++j) { a.s[j] = a.s[j] op b.s[j]; } return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v2<Scalar>, count> operator op(Vector<v2<Scalar>, count> a, Vector<Scalar, count> b)        { for (umm j = 0; j < 2; ++j) { a.s[j] = a.s[j] op b; } return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v3<Scalar>, count> operator op(Vector<v3<Scalar>, count> a, Vector<Scalar, count> b)        { for (umm j = 0; j < 3; ++j) { a.s[j] = a.s[j] op b; } return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v4<Scalar>, count> operator op(Vector<v4<Scalar>, count> a, Vector<Scalar, count> b)        { for (umm j = 0; j < 4; ++j) { a.s[j] = a.s[j] op b; } return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v2<Scalar>, count> operator op(Vector<v2<Scalar>, count> a, std::type_identity_t<Scalar> b) { return a op broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v3<Scalar>, count> operator op(Vector<v3<Scalar>, count> a, std::type_identity_t<Scalar> b) { return a op broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v4<Scalar>, count> operator op(Vector<v4<Scalar>, count> a, std::type_identity_t<Scalar> b) { return a op broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v2<Scalar>, count> operator op(std::type_identity_t<Scalar> a, Vector<v2<Scalar>, count> b) { return broadcast<count>(a) op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v3<Scalar>, count> operator op(std::type_identity_t<Scalar> a, Vector<v3<Scalar>, count> b) { return broadcast<count>(a) op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<v4<Scalar>, count> operator op(std::type_identity_t<Scalar> a, Vector<v4<Scalar>, count> b) { return broadcast<count>(a) op b; } \

#define A(op) \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<Scalar, count> &operator op=(Vector<Scalar, count> &a, Vector<Scalar, count> b) { return a = a op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<Scalar, count> &operator op=(Vector<Scalar, count> &a, std::type_identity_t<Scalar> b) { return a op= broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v2<Scalar>, count> &operator op=(Vector<v2<Scalar>, count> &a, Vector<v2<Scalar>, count> b)    { return a = a op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v3<Scalar>, count> &operator op=(Vector<v3<Scalar>, count> &a, Vector<v3<Scalar>, count> b)    { return a = a op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v4<Scalar>, count> &operator op=(Vector<v4<Scalar>, count> &a, Vector<v4<Scalar>, count> b)    { return a = a op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v2<Scalar>, count> &operator op=(Vector<v2<Scalar>, count> &a, Vector<Scalar, count> b)        { return a = a op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v3<Scalar>, count> &operator op=(Vector<v3<Scalar>, count> &a, Vector<Scalar, count> b)        { return a = a op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v4<Scalar>, count> &operator op=(Vector<v4<Scalar>, count> &a, Vector<Scalar, count> b)        { return a = a op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v2<Scalar>, count> &operator op=(Vector<v2<Scalar>, count> &a, std::type_identity_t<Scalar> b) { return a op= broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v3<Scalar>, count> &operator op=(Vector<v3<Scalar>, count> &a, std::type_identity_t<Scalar> b) { return a op= broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op= s; } forceinline constexpr Vector<v4<Scalar>, count> &operator op=(Vector<v4<Scalar>, count> &a, std::type_identity_t<Scalar> b) { return a op= broadcast<count>(b); } \

#define OPERATION(op) O(op) A(op)

OPERATION(+);
OPERATION(-);
OPERATION(*);
OPERATION(/);
OPERATION(%);
OPERATION(^);
OPERATION(&);
OPERATION(|);
OPERATION(<<);
OPERATION(>>);

#undef X

#define X(op) O(op)

O(&&);
O(||);

#undef X

#undef O
#undef A

#define O(op) \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) { MaskVector<Scalar, count> r; for (umm i = 0; i < count; ++i) r.s[i] = -1 * (a.s[i] op b.s[i]); return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<Scalar, count> operator op(Vector<Scalar, count> a, std::type_identity_t<Scalar> b) { return a op broadcast<count>(b); } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<Scalar, count> operator op(std::type_identity_t<Scalar> a, Vector<Scalar, count> b) { return broadcast<count>(a) op b; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v2<Scalar>, count> operator op(Vector<v2<Scalar>, count> a, Vector<v2<Scalar>, count> b)    { MaskVector<v2<Scalar>, count> r; for (umm i = 0; i < 2; ++i) { r.s[i] = -1 * (a.s[i] op b.s[i]); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v3<Scalar>, count> operator op(Vector<v3<Scalar>, count> a, Vector<v3<Scalar>, count> b)    { MaskVector<v3<Scalar>, count> r; for (umm i = 0; i < 3; ++i) { r.s[i] = -1 * (a.s[i] op b.s[i]); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v4<Scalar>, count> operator op(Vector<v4<Scalar>, count> a, Vector<v4<Scalar>, count> b)    { MaskVector<v4<Scalar>, count> r; for (umm i = 0; i < 4; ++i) { r.s[i] = -1 * (a.s[i] op b.s[i]); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v2<Scalar>, count> operator op(Vector<v2<Scalar>, count> a, std::type_identity_t<Scalar> b) { MaskVector<v2<Scalar>, count> r; for (umm i = 0; i < 2; ++i) { r.s[i] = -1 * (a.s[i] op b); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v3<Scalar>, count> operator op(Vector<v3<Scalar>, count> a, std::type_identity_t<Scalar> b) { MaskVector<v3<Scalar>, count> r; for (umm i = 0; i < 3; ++i) { r.s[i] = -1 * (a.s[i] op b); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v4<Scalar>, count> operator op(Vector<v4<Scalar>, count> a, std::type_identity_t<Scalar> b) { MaskVector<v4<Scalar>, count> r; for (umm i = 0; i < 4; ++i) { r.s[i] = -1 * (a.s[i] op b); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v2<Scalar>, count> operator op(std::type_identity_t<Scalar> a, Vector<v2<Scalar>, count> b) { MaskVector<v2<Scalar>, count> r; for (umm i = 0; i < 2; ++i) { r.s[i] = -1 * (a op b.s[i]); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v3<Scalar>, count> operator op(std::type_identity_t<Scalar> a, Vector<v3<Scalar>, count> b) { MaskVector<v3<Scalar>, count> r; for (umm i = 0; i < 3; ++i) { r.s[i] = -1 * (a op b.s[i]); } return r; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr MaskVector<v4<Scalar>, count> operator op(std::type_identity_t<Scalar> a, Vector<v4<Scalar>, count> b) { MaskVector<v4<Scalar>, count> r; for (umm i = 0; i < 4; ++i) { r.s[i] = -1 * (a op b.s[i]); } return r; } \

O(==);
O(!=);
O(>);
O(<);
O(>=);
O(<=);

#undef O

template <class Scalar, umm count>
forceinline Vector<Scalar, count> min(Vector<Scalar, count> a, Vector<Scalar, count> b) {
	Vector<Scalar, count> r;
	for (umm i = 0; i < count; ++i) {
		r.s[i] = min(a.s[i], b.s[i]);
	}
	return r;
}

template <class Scalar, umm count>
forceinline Vector<Scalar, count> max(Vector<Scalar, count> a, Vector<Scalar, count> b) {
	Vector<Scalar, count> r;
	for (umm i = 0; i < count; ++i) {
		r.s[i] = max(a.s[i], b.s[i]);
	}
	return r;
}

template <class Scalar, umm count>
forceinline Scalar dot(Vector<Scalar, count> a, Vector<Scalar, count> b) {
	Scalar r = {};
	for (umm i = 0; i < count; ++i) {
		r += a.s[i] * b.s[i];
	}
	return r;
}

template <class Scalar, std::integral Index, umm count>
forceinline Vector<Scalar, count> gather(Scalar *s, Vector<Index, count> b) {
	Vector<Scalar, count> r;
	for (umm i = 0; i < count; ++i) {
		r.s[i] = s[b.s[i]];
	}
	return r;
}

template <class Scalar, std::integral Index, umm count>
forceinline void scatter(Vector<Scalar, count> s, Scalar *d, Vector<Index, count> b) {
	for (umm i = 0; i < count; ++i) {
		d[b.s[i]] = s.s[i];
	}
}

template <CMaskScalar Scalar, umm count> forceinline constexpr bool all(Vector<Scalar, count> a) { bool r = true; for (umm i = 0; i < count; ++i) r &= a.s[i] != 0; return r; }
template <CMaskScalar Scalar, umm count> forceinline constexpr bool any(Vector<Scalar, count> a) { bool r = false; for (umm i = 0; i < count; ++i) r |= a.s[i] != 0; return r; }
template <CMaskScalar Scalar, umm count> forceinline constexpr bool all(Vector<v2<Scalar>, count> a) { bool r = true; for (umm i = 0; i < count; ++i) for (umm j = 0; j < 2; ++j) r &= a.s[j].s[i] != 0; return r; }
template <CMaskScalar Scalar, umm count> forceinline constexpr bool any(Vector<v2<Scalar>, count> a) { bool r = false; for (umm i = 0; i < count; ++i) for (umm j = 0; j < 2; ++j) r |= a.s[j].s[i] != 0; return r; }
template <CMaskScalar Scalar, umm count> forceinline constexpr bool all(Vector<v3<Scalar>, count> a) { bool r = true; for (umm i = 0; i < count; ++i) for (umm j = 0; j < 3; ++j) r &= a.s[j].s[i] != 0; return r; }
template <CMaskScalar Scalar, umm count> forceinline constexpr bool any(Vector<v3<Scalar>, count> a) { bool r = false; for (umm i = 0; i < count; ++i) for (umm j = 0; j < 3; ++j) r |= a.s[j].s[i] != 0; return r; }
template <CMaskScalar Scalar, umm count> forceinline constexpr bool all(Vector<v4<Scalar>, count> a) { bool r = true; for (umm i = 0; i < count; ++i) for (umm j = 0; j < 4; ++j) r &= a.s[j].s[i] != 0; return r; }
template <CMaskScalar Scalar, umm count> forceinline constexpr bool any(Vector<v4<Scalar>, count> a) { bool r = false; for (umm i = 0; i < count; ++i) for (umm j = 0; j < 4; ++j) r |= a.s[j].s[i] != 0; return r; }

template <class Scalar, umm count>
constexpr Vector<Scalar, count> sign(Vector<Scalar, count> v) {
	for (umm i = 0; i < count; ++i) {
		v.s[i] = v.s[i] == 0 ? 0 : v.s[i] < 0 ? -1 : 1;
	}
	return v;
}

template <class Scalar, umm count>
constexpr Vector<Scalar, count> absolute(Vector<Scalar, count> v) {
	for (umm i = 0; i < count; ++i) {
		v.s[i] = v.s[i] < 0 ? -v.s[i] : v.s[i];
	}
	return v;
}

template <std::floating_point Scalar, umm count>
constexpr Vector<Scalar, count> reciprocal(Vector<Scalar, count> v) {
	for (umm i = 0; i < count; ++i) {
		v.s[i] = 1 / v.s[i];
	}
	return v;
}

template <std::floating_point Scalar, umm count>
constexpr Vector<Scalar, count> floor(Vector<Scalar, count> v) {
	for (umm i = 0; i < count; ++i) {
		v.s[i] = floor(v.s[i]);
	}
	return v;
}

template <std::floating_point Scalar, umm count>
constexpr Vector<Scalar, count> ceil(Vector<Scalar, count> v) {
	for (umm i = 0; i < count; ++i) {
		v.s[i] = ceil(v.s[i]);
	}
	return v;
}

template <std::floating_point Scalar, umm count>
constexpr Vector<Scalar, count> frac(Vector<Scalar, count> v) {
	for (umm i = 0; i < count; ++i) {
		v.s[i] = frac(v.s[i]);
	}
	return v;
}

template <std::floating_point Scalar, umm count>
constexpr Vector<Scalar, count> floor(Vector<Scalar, count> v, Vector<Scalar, count> b) {
	return floor(v / b) * b;
}

template <std::floating_point Scalar, umm count>
constexpr Vector<Scalar, count> ceil(Vector<Scalar, count> v, Vector<Scalar, count> b) {
	return ceil(v / b) * b;
}

template <std::floating_point Scalar, umm count>
constexpr Vector<Scalar, count> frac(Vector<Scalar, count> v, Vector<Scalar, count> b) {
	return frac(v / b) * b;
}

#define SA(Scalar, count, op, intrinsic) \
	template <>  \
	forceinline Vector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) {  \
		return {.m = intrinsic}; \
	}

#define SC(Scalar, count, op, intrinsic) \
	template <>  \
	forceinline MaskVector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) {  \
		return {.m = intrinsic}; \
	}

#if ARCH_AVX
#define AVX(x) x
#else
#define AVX(x)
#endif
#if ARCH_AVX2
#define AVX2(x) x
#else
#define AVX2(x)
#endif
#if ARCH_AVX512
#define AVX512(x) x
#else
#define AVX512(x)
#endif

#define sb8  ((s8)0x80)
#define sb16 ((s16)0x8000)
#define sb32 ((s32)0x8000'0000)
#define sb64 ((s64)0x8000'0000'0000'0000)

#define noti(x)  _mm_xor_si128(x, _mm_set1_epi32(~0))
#define notf(x)  _mm_xor_ps(x, _mm_castsi128_ps(_mm_set1_epi32(~0)))
#define notd(x)  _mm_xor_pd(x, _mm_castsi128_ps(_mm_set1_epi32(~0)))
#define fsb8i(x) _mm_xor_si128(x, _mm_set1_epi8(sb8))
#define fsb16i(x) _mm_xor_si128(x, _mm_set1_epi16(sb16))
#define fsb32i(x) _mm_xor_si128(x, _mm_set1_epi32(sb32))
#define fsb64i(x) _mm_xor_si128(x, _mm_set1_epi64x(sb64))
#define fsb32f(x) _mm_xor_ps(x, _mm_castsi128_ps(_mm_set1_epi32(sb32)))
#define fsb64f(x) _mm_xor_ps(x, _mm_castsi128_ps(_mm_set1_epi64x(sb64)))
#define fsb32d(x) _mm_xor_pd(x, _mm_castsi128_ps(_mm_set1_epi32(sb32)))
#define fsb64d(x) _mm_xor_pd(x, _mm_castsi128_ps(_mm_set1_epi64x(sb64)))
#define ugt(bits) C(u,bits,>, _mm_cmpgt_epi##bits(fsb##bits##i(a.m), fsb##bits##i(b.m)))
#define ult(bits) C(u,bits,<, _mm_cmpgt_epi##bits(fsb##bits##i(b.m), fsb##bits##i(a.m)))
#define uge(bits) C(u,bits,>=,noti(_mm_cmpgt_epi##bits(fsb##bits##i(b.m), fsb##bits##i(a.m))))
#define ule(bits) C(u,bits,<=,noti(_mm_cmpgt_epi##bits(fsb##bits##i(a.m), fsb##bits##i(b.m))))
#define sgt(bits) C(s,bits,>, _mm_cmpgt_epi##bits(a.m, b.m))
#define slt(bits) C(s,bits,<, _mm_cmpgt_epi##bits(b.m, a.m))
#define sge(bits) C(s,bits,>=,noti(_mm_cmpgt_epi##bits(b.m, a.m)))
#define sle(bits) C(s,bits,<=,noti(_mm_cmpgt_epi##bits(a.m, b.m)))

#define A(type, bits, op, intrinsic) SA(type##bits, 128/bits, op, intrinsic)
#define C(type, bits, op, intrinsic) SC(type##bits, 128/bits, op, intrinsic)

#define iadd(type,bits) A(type,bits,+,_mm_add_epi##bits(a.m,b.m))
#define isub(type,bits) A(type,bits,-,_mm_sub_epi##bits(a.m,b.m))
#define imul(type,bits) A(type,bits,*,_mm_mullo_epi##bits(a.m,b.m))
#define udiv(bits) A(u,bits,/,_mm_div_epu##bits(a.m,b.m))
#define sdiv(bits) A(s,bits,/,_mm_div_epi##bits(a.m,b.m))
#define umod(bits) A(u,bits,%,_mm_sub_epi##bits(a.m,(Vector<u##bits,128/bits>{.m=_mm_div_epu##bits(a.m,b.m)}*b).m))
#define smod(bits) A(s,bits,%,_mm_sub_epi##bits(a.m,(Vector<s##bits,128/bits>{.m=_mm_div_epi##bits(a.m,b.m)}*b).m))
#define ieq(type,bits) C(type,bits,==,_mm_cmpeq_epi##bits(a.m,b.m));
#define ine(type,bits) C(type,bits,!=,noti(_mm_cmpeq_epi##bits(a.m,b.m)));

iadd(u,8);
isub(u,8);
// imul(u,8);
udiv(8);
umod(8);
ieq(u,8);
ine(u,8);
ule(8);
uge(8);
ult(8);
ugt(8);

iadd(u,16);
isub(u,16);
imul(u,16);
udiv(16);
umod(16);
ieq(u,16);
ine(u,16);
ule(16);
uge(16);
ult(16);
ugt(16);

iadd(u,32);
isub(u,32);
imul(u,32);
udiv(32);
umod(32);
ieq(u,32);
ine(u,32);
ule(32);
uge(32);
ult(32);
ugt(32);

iadd(u,64);
isub(u,64);
AVX512(imul(u,64));
udiv(64);
umod(64);
ieq(u,64);
ine(u,64);
ule(64);
uge(64);
ult(64);
ugt(64);

iadd(s,8);
isub(s,8);
// imul(s,8);
sdiv(8);
smod(8);
ieq(s,8);
ine(s,8);
sle(8);
sge(8);
slt(8);
sgt(8);

iadd(s,16);
isub(s,16);
imul(s,16);
sdiv(16);
smod(16);
ieq(s,16);
ine(s,16);
sle(16);
sge(16);
slt(16);
sgt(16);

iadd(s,32);
isub(s,32);
imul(s,32);
sdiv(32);
smod(32);
ieq(s,32);
ine(s,32);
sle(32);
sge(32);
slt(32);
sgt(32);

iadd(s,64);
isub(s,64);
AVX512(imul(s,64));
sdiv(64);
smod(64);
ieq(s,64);
ine(s,64);
sle(64);
sge(64);
slt(64);
sgt(64);

A(f,32, +, _mm_add_ps(a.m, b.m));
A(f,32, -, _mm_sub_ps(a.m, b.m));
A(f,32, *, _mm_mul_ps(a.m, b.m));
A(f,32, /, _mm_div_ps(a.m, b.m));
C(f,32, ==, _mm_castps_si128(_mm_cmpeq_ps(a.m, b.m)));
C(f,32, !=, _mm_castps_si128(_mm_cmpneq_ps(a.m, b.m)));
C(f,32, <=, _mm_castps_si128(_mm_cmple_ps(a.m, b.m)));
C(f,32, >=, _mm_castps_si128(_mm_cmpge_ps(a.m, b.m)));
C(f,32, <, _mm_castps_si128(_mm_cmplt_ps(a.m, b.m)));
C(f,32, >, _mm_castps_si128(_mm_cmpgt_ps(a.m, b.m)));

template <> inline Vector<f32, 4> select(Vector<Mask32, 4> m, Vector<f32, 4> a, Vector<f32, 4> b) { return {.m = _mm_blendv_ps(b.m, a.m, _mm_castsi128_ps(m.m))}; }
template <> inline Vector<f32, 4> absolute(Vector<f32, 4> v) { return {.m = _mm_and_ps(v.m, _mm_castsi128_ps(_mm_set1_epi32(0x7fff'ffff)))}; }
template <> inline Vector<f32, 4> reciprocal(Vector<f32, 4> v) { return {.m = _mm_rcp_ps(v.m)}; }
template <> inline Vector<f32, 4> floor(Vector<f32, 4> v) { return {.m = _mm_floor_ps(v.m)}; }
template <> inline Vector<f32, 4> ceil(Vector<f32, 4> v) { return {.m = _mm_ceil_ps(v.m)}; }
template <> inline Vector<f32, 4> frac(Vector<f32, 4> v) { return {.m = _mm_sub_ps(v.m, _mm_floor_ps(v.m))}; }
inline Vector<f32, 4> set_sign(Vector<f32, 4> d, Vector<f32, 4> s) { return {.m = vec16_or(vec16_and(d.m, s32x4_set1(0x7fff'ffff)), vec16_and(s.m, s32x4_set1(0x8000'0000)))}; }
inline Vector<f32, 4> sign(Vector<f32, 4> v) { return set_sign(broadcast<4>(1.0f), v); }

template <> inline bool all(Vector<Mask32, 4> v) { return _mm_movemask_ps(_mm_castsi128_ps(v.m)) == 15; }
template <> inline bool any(Vector<Mask32, 4> v) { return _mm_movemask_ps(_mm_castsi128_ps(v.m)) != 0; }

#undef noti
#undef notf
#undef notd
#undef fsb8i
#undef fsb16i
#undef fsb32i
#undef fsb64i
#undef fsb32f
#undef fsb64f
#undef fsb32d
#undef fsb64d

#if ARCH_AVX
#define noti(x)   _mm256_xor_si256(x, _mm256_set1_epi32(~0))
#define notf(x)   _mm256_xor_ps(x, _mm256_castsi256_ps(_mm256_set1_epi32(~0)))
#define notd(x)   _mm256_xor_pd(x, _mm256_castsi256_ps(_mm256_set1_epi32(~0)))
#define fsb8i(x)  _mm256_xor_si256(x, _mm256_set1_epi8(sb8))
#define fsb16i(x) _mm256_xor_si256(x, _mm256_set1_epi16(sb16))
#define fsb32i(x) _mm256_xor_si256(x, _mm256_set1_epi32(sb32))
#define fsb64i(x) _mm256_xor_si256(x, _mm256_set1_epi64x(sb64))
#define fsb32f(x) _mm256_xor_ps(x, _mm256_castsi256_ps(_mm256_set1_epi32(sb32)))
#define fsb64f(x) _mm256_xor_ps(x, _mm256_castsi256_ps(_mm256_set1_epi64x(sb64)))
#define fsb32d(x) _mm256_xor_pd(x, _mm256_castsi256_ps(_mm256_set1_epi32(sb32)))
#define fsb64d(x) _mm256_xor_pd(x, _mm256_castsi256_ps(_mm256_set1_epi64x(sb64)))

#define A(type, bits, op, intrinsic) SA(type##bits, 256/bits, op, intrinsic)
#define C(type, bits, op, intrinsic) SC(type##bits, 256/bits, op, intrinsic)

A(u,32, +, _mm256_add_epi32(a.m, b.m))
A(u,32, -, _mm256_sub_epi32(a.m, b.m))
A(u,32, *, _mm256_mullo_epi32(a.m, b.m))
A(u,32, /, _mm256_div_epu32(a.m, b.m))
A(u,32, %, _mm256_rem_epu32(a.m, b.m))
A(u,32, ^, _mm256_xor_si256(a.m, b.m))
A(u,32, &, _mm256_and_si256(a.m, b.m))
A(u,32, |, _mm256_or_si256(a.m, b.m))

A(s,32, +, _mm256_add_epi32(a.m, b.m))
A(s,32, -, _mm256_sub_epi32(a.m, b.m))
A(s,32, *, _mm256_mullo_epi32(a.m, b.m))
A(s,32, /, _mm256_div_epi32(a.m, b.m))
A(s,32, %, _mm256_rem_epi32(a.m, b.m))
A(s,32, ^, _mm256_xor_si256(a.m, b.m))
A(s,32, &, _mm256_and_si256(a.m, b.m))
A(s,32, |, _mm256_or_si256(a.m, b.m))

A(f,32, +, _mm256_add_ps(a.m, b.m));
A(f,32, -, _mm256_sub_ps(a.m, b.m));
A(f,32, *, _mm256_mul_ps(a.m, b.m));
A(f,32, /, _mm256_div_ps(a.m, b.m));
C(f,32, ==, _mm256_castps_si256(_mm256_cmp_ps(a.m, b.m, _CMP_EQ_OQ)));
C(f,32, !=, _mm256_castps_si256(_mm256_cmp_ps(a.m, b.m, _CMP_NEQ_OQ)));
C(f,32, <=, _mm256_castps_si256(_mm256_cmp_ps(a.m, b.m, _CMP_LE_OQ)));
C(f,32, >=, _mm256_castps_si256(_mm256_cmp_ps(a.m, b.m, _CMP_GE_OQ)));
C(f,32, <, _mm256_castps_si256(_mm256_cmp_ps(a.m, b.m, _CMP_LT_OQ)));
C(f,32, >, _mm256_castps_si256(_mm256_cmp_ps(a.m, b.m, _CMP_GT_OQ)));

#define CVT(From, To, intrinsic) \
template <> \
struct Converter<To, From, 8> { \
	forceinline Vector<To, 8> operator()(Vector<From, 8> from) { \
		return {.m = intrinsic}; \
	} \
}

CVT(u32, s32, from.m);
// CVT(u32, f32, _mm256_cvtepu32_ps(from.m)); // not available

CVT(s32, u32, from.m);
CVT(s32, f32, _mm256_cvtepi32_ps(from.m));

AVX512(CVT(f32, u32, _mm256_cvtps_epu32(from.m)));
CVT(f32, s32, _mm256_cvtps_epi32(from.m));

#undef CVT

#define SELECT_BASE(Scalar, count, intrinsic) \
template <> \
forceinline Vector<Scalar, count> select(MaskVector<Scalar, count> mask, Vector<Scalar, count> a, Vector<Scalar, count> b) { \
	return {.m = intrinsic}; \
}

#define SELECT(Scalar, intrinsic) SELECT_BASE(Scalar, 32 / sizeof(Scalar), intrinsic)

SELECT(Mask32, _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(b.m), _mm256_castsi256_ps(a.m), _mm256_castsi256_ps(mask.m))));
SELECT(u32, _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(b.m), _mm256_castsi256_ps(a.m), _mm256_castsi256_ps(mask.m))));
SELECT(s32, _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(b.m), _mm256_castsi256_ps(a.m), _mm256_castsi256_ps(mask.m))));
SELECT(f32, _mm256_blendv_ps(b.m, a.m, _mm256_castsi256_ps(mask.m)));

SELECT(Mask64, _mm256_castpd_si256(_mm256_blendv_pd(_mm256_castsi256_pd(b.m), _mm256_castsi256_pd(a.m), _mm256_castsi256_pd(mask.m))));
SELECT(u64, _mm256_castpd_si256(_mm256_blendv_pd(_mm256_castsi256_pd(b.m), _mm256_castsi256_pd(a.m), _mm256_castsi256_pd(mask.m))));
SELECT(s64, _mm256_castpd_si256(_mm256_blendv_pd(_mm256_castsi256_pd(b.m), _mm256_castsi256_pd(a.m), _mm256_castsi256_pd(mask.m))));
SELECT(f64, _mm256_blendv_pd(b.m, a.m, _mm256_castsi256_pd(mask.m)));

#undef SELECT_BASE

#endif
#if ARCH_512
#define noti(x)   _mm512_xor_si512(x, _mm512_set1_epi32(~0))
#define notf(x)   _mm512_xor_ps(x, _mm512_castsi512_ps(_mm512_set1_epi32(~0)))
#define notd(x)   _mm512_xor_pd(x, _mm512_castsi512_ps(_mm512_set1_epi32(~0)))
#define fsb8i(x)  _mm512_xor_si512(x, _mm512_set1_epi8(sb8))
#define fsb16i(x) _mm512_xor_si512(x, _mm512_set1_epi16(sb16))
#define fsb32i(x) _mm512_xor_si512(x, _mm512_set1_epi32(sb32))
#define fsb64i(x) _mm512_xor_si512(x, _mm512_set1_epi64(sb64))
#define fsb32f(x) _mm512_xor_ps(x, _mm512_castsi512_ps(_mm512_set1_epi32(sb32)))
#define fsb64f(x) _mm512_xor_ps(x, _mm512_castsi512_ps(_mm512_set1_epi64(sb64)))
#define fsb32d(x) _mm512_xor_pd(x, _mm512_castsi512_ps(_mm512_set1_epi32(sb32)))
#define fsb64d(x) _mm512_xor_pd(x, _mm512_castsi512_ps(_mm512_set1_epi64(sb64)))
#endif

#undef ugt
#undef ult
#undef uge
#undef ule
#undef sgt
#undef slt
#undef sge
#undef sle

#undef noti
#undef notf
#undef notd

#undef fsb8i
#undef fsb16i
#undef fsb32i
#undef fsb64i
#undef fsb32f
#undef fsb64f
#undef fsb32d
#undef fsb64d

#undef sb8 
#undef sb16
#undef sb32
#undef sb64
#undef AVX
#undef AVX2
#undef AVX512
#undef SC
#undef SA
#undef A
#undef C

}