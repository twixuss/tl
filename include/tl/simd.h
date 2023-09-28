#include "common.h"
#include <zmmintrin.h>

namespace tl {

template <class Scalar_, umm count_>
struct Vector {
	using Scalar = Scalar_;
	inline static constexpr umm count = count_;

	Scalar s[count];
};

template <class Scalar, umm count>
using MaskVector = Vector<TypeAt<log2(sizeof Scalar), u8, u16, u32, u64>, count>;

#define S(Scalar_, count_, bits) \
	template <> \
	struct Vector<Scalar_, count_> { \
		using Scalar = Scalar_; \
		inline static constexpr umm count = count_; \
		union { \
			Scalar s[count]; \
			__m##bits mf; \
			__m##bits##d md; \
			__m##bits##i mi; \
		}; \
	}

S(u8, 16, 128);
S(u16, 8, 128);
S(u32, 4, 128);
S(u64, 2, 128);
S(s8, 16, 128);
S(s16, 8, 128);
S(s32, 4, 128);
S(s64, 2, 128);
S(f32, 4, 128);
S(f64, 2, 128);

S(u8, 32, 256);
S(u16, 16, 256);
S(u32, 8, 256);
S(u64, 4, 256);
S(s8, 32, 256);
S(s16, 16, 256);
S(s32, 8, 256);
S(s64, 4, 256);
S(f32, 8, 256);
S(f64, 4, 256);

S(u8, 64, 512);
S(u16, 32, 512);
S(u32, 16, 512);
S(u64, 8, 512);
S(s8, 64, 512);
S(s16, 32, 512);
S(s32, 16, 512);
S(s64, 8, 512);
S(f32, 16, 512);
S(f64, 8, 512);

#undef S

#define O(op) \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) { for (umm i = 0; i < count; ++i) a.s[i] = a.s[i] op b.s[i]; return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<Scalar, count> operator op(Vector<Scalar, count> a, Scalar b) { for (umm i = 0; i < count; ++i) a.s[i] = a.s[i] op b; return a; } \
	template <class Scalar, umm count> requires requires(Scalar s) { s op s; } forceinline constexpr Vector<Scalar, count> operator op(Scalar a, Vector<Scalar, count> b) { for (umm i = 0; i < count; ++i) b.s[i] = a op b.s[i]; return b; } \

O(+);
O(-);
O(*);
O(/);
O(%);
O(^);
O(&);
O(|);
O(<<);
O(>>);
O(&&);
O(||);

#undef O

#define O(op) \
	template <class Scalar, umm count> forceinline constexpr MaskVector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) { MaskVector<Scalar, count> r; for (umm i = 0; i < count; ++i) r.s[i] = -1 * (a.s[i] op b.s[i]); return r; } \
	template <class Scalar, umm count> forceinline constexpr MaskVector<Scalar, count> operator op(Vector<Scalar, count> a, Scalar b) { MaskVector<Scalar, count> r; for (umm i = 0; i < count; ++i) r.s[i] = -1 * (a.s[i] op b); return r; } \
	template <class Scalar, umm count> forceinline constexpr MaskVector<Scalar, count> operator op(Scalar a, Vector<Scalar, count> b) { MaskVector<Scalar, count> r; for (umm i = 0; i < count; ++i) r.s[i] = -1 * (a op b.s[i]); return r; } \

O(==);
O(!=);
O(>);
O(<);
O(>=);
O(<=);

#undef O

template <std::integral Scalar, umm count> forceinline constexpr bool all(Vector<Scalar, count> a) { bool r = true; for (umm i = 0; i < count; ++i) r &= a.s[i] != 0; return r; }
template <std::integral Scalar, umm count> forceinline constexpr bool any(Vector<Scalar, count> a) { bool r = false; for (umm i = 0; i < count; ++i) r |= a.s[i] != 0; return r; }

#define SA(Scalar, count, op, intrinsic) \
	template <>  \
	forceinline Vector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) {  \
		Vector<Scalar, count> r; \
		intrinsic; \
		return r; \
	}

#define SC(Scalar, count, op, intrinsic) \
	template <>  \
	forceinline MaskVector<Scalar, count> operator op(Vector<Scalar, count> a, Vector<Scalar, count> b) {  \
		MaskVector<Scalar, count> r; \
		intrinsic; \
		return r; \
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
#define ugt(bits) C(u,bits,>, r.mi = _mm_cmpgt_epi##bits(fsb##bits##i(a.mi), fsb##bits##i(b.mi)))
#define ult(bits) C(u,bits,<, r.mi = _mm_cmpgt_epi##bits(fsb##bits##i(b.mi), fsb##bits##i(a.mi)))
#define uge(bits) C(u,bits,>=,r.mi = noti(_mm_cmpgt_epi##bits(fsb##bits##i(b.mi), fsb##bits##i(a.mi))))
#define ule(bits) C(u,bits,<=,r.mi = noti(_mm_cmpgt_epi##bits(fsb##bits##i(a.mi), fsb##bits##i(b.mi))))
#define sgt(bits) C(s,bits,>, r.mi = _mm_cmpgt_epi##bits(a.mi, b.mi))
#define slt(bits) C(s,bits,<, r.mi = _mm_cmpgt_epi##bits(b.mi, a.mi))
#define sge(bits) C(s,bits,>=,r.mi = noti(_mm_cmpgt_epi##bits(b.mi, a.mi)))
#define sle(bits) C(s,bits,<=,r.mi = noti(_mm_cmpgt_epi##bits(a.mi, b.mi)))

#define A(type, bits, op, intrinsic) SA(type##bits, 128/bits, op, intrinsic)
#define C(type, bits, op, intrinsic) SC(type##bits, 128/bits, op, intrinsic)

#define iadd(type,bits) A(type,bits,+,r.mi=_mm_add_epi##bits(a.mi,b.mi))
#define isub(type,bits) A(type,bits,-,r.mi=_mm_sub_epi##bits(a.mi,b.mi))
#define imul(type,bits) A(type,bits,*,r.mi=_mm_mullo_epi##bits(a.mi,b.mi))
#define udiv(bits) A(u,bits,/,r.mi=_mm_div_epu##bits(a.mi,b.mi))
#define sdiv(bits) A(s,bits,/,r.mi=_mm_div_epi##bits(a.mi,b.mi))
#define umod(bits) A(u,bits,%,r.mi=_mm_sub_epi##bits(a.mi,(Vector<u##bits,128/bits>{.mi=_mm_div_epu##bits(a.mi,b.mi)}*b).mi))
#define smod(bits) A(s,bits,%,r.mi=_mm_sub_epi##bits(a.mi,(Vector<s##bits,128/bits>{.mi=_mm_div_epi##bits(a.mi,b.mi)}*b).mi))
#define ieq(type,bits) C(type,bits,==,r.mi=_mm_cmpeq_epi##bits(a.mi,b.mi));
#define ine(type,bits) C(type,bits,!=,r.mi=noti(_mm_cmpeq_epi##bits(a.mi,b.mi)));

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

A(f,32, +, r.mf = _mm_add_ps(a.mf, b.mf));
A(f,32, -, r.mf = _mm_sub_ps(a.mf, b.mf));
A(f,32, *, r.mf = _mm_mul_ps(a.mf, b.mf));
A(f,32, /, r.mf = _mm_div_ps(a.mf, b.mf));
C(f,32, ==, r.mf = _mm_cmpeq_ps(a.mf, b.mf));
C(f,32, !=, r.mf = _mm_cmpneq_ps(a.mf, b.mf));
C(f,32, <=, r.mf = _mm_cmple_ps(a.mf, b.mf));
C(f,32, >=, r.mf = _mm_cmpge_ps(a.mf, b.mf));
C(f,32, <, r.mf = _mm_cmplt_ps(a.mf, b.mf));
C(f,32, >, r.mf = _mm_cmpgt_ps(a.mf, b.mf));

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

}