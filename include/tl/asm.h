#include "common.h"
#include <immintrin.h>
#pragma warning(push)
#pragma warning(disable : 4201) // unnamed struct
namespace TL {
namespace ASM {

struct XMMWORD_PTR;
struct YMMWORD_PTR;
struct ZMMWORD_PTR;

#define ELEMENTS(bytes) \
	s8  s8 [(bytes/16)*16]; \
	s16 s16[(bytes/16)* 8]; \
	s32 s32[(bytes/16)* 4]; \
	s64 s64[(bytes/16)* 2]; \
	u8  u8 [(bytes/16)*16]; \
	u16 u16[(bytes/16)* 8]; \
	u32 u32[(bytes/16)* 4]; \
	u64 u64[(bytes/16)* 2]; \
	f32 f32[(bytes/16)* 4]; \
	f64 f64[(bytes/16)* 2]

union XMM {
	ELEMENTS(sizeof(__m128));
	__m128 ps;
	__m128i pi;
	__m128d pd;
	FORCEINLINE constexpr XMM() = default;
	FORCEINLINE constexpr XMM(__m128 ps) : ps(ps) {}
	FORCEINLINE constexpr XMM(__m128i pi) : pi(pi) {}
	FORCEINLINE constexpr XMM(__m128d pd) : pd(pd) {}
	FORCEINLINE constexpr operator __m128() const { return ps; }
	FORCEINLINE constexpr operator __m128i() const { return pi; }
	FORCEINLINE constexpr operator __m128d() const { return pd; }

	FORCEINLINE XMM operator!() const { return _mm_xor_ps(ps, _mm_castsi128_ps(_mm_set1_epi32(~0))); }
	FORCEINLINE XMM operator~() const { return _mm_xor_ps(ps, _mm_castsi128_ps(_mm_set1_epi32(~0))); }
	FORCEINLINE XMM operator^ (XMM b) const { return _mm_xor_ps(ps, b); }
	FORCEINLINE XMM operator| (XMM b) const { return _mm_or_ps(ps, b); }
	FORCEINLINE XMM operator& (XMM b) const { return _mm_and_ps(ps, b); }
	FORCEINLINE XMM operator||(XMM b) const { return _mm_or_ps(ps, b); }
	FORCEINLINE XMM operator&&(XMM b) const { return _mm_and_ps(ps, b); }
	FORCEINLINE XMM operator==(XMM b) const { return _mm_cmpeq_epi32(pi, b); }
	FORCEINLINE XMM operator!=(XMM b) const { return _mm_xor_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(pi, b)), _mm_castsi128_ps(_mm_set1_epi32(~0))); }
	FORCEINLINE XMM &operator^=(XMM b) { return *this = _mm_xor_ps(ps, b); }
	FORCEINLINE XMM &operator|=(XMM b) { return *this = _mm_or_ps (ps, b); }
	FORCEINLINE XMM &operator&=(XMM b) { return *this = _mm_and_ps (ps, b); }
};
struct XMMWORD_PTR {
	void *ptr;

	explicit FORCEINLINE constexpr XMMWORD_PTR(void *a) : ptr(a) {}
	template <class T>
	explicit FORCEINLINE constexpr XMMWORD_PTR(T &a) : ptr(&a) {}

	FORCEINLINE constexpr operator __m128 &() { return *(__m128 *)ptr; }
	FORCEINLINE constexpr operator __m128i &() { return *(__m128i *)ptr; }
	FORCEINLINE constexpr operator __m128d &() { return *(__m128d *)ptr; }
	FORCEINLINE constexpr operator XMM &() { return *(XMM *)ptr; }

	FORCEINLINE constexpr XMMWORD_PTR operator+(smm val) { return XMMWORD_PTR((u8 *)ptr + val); }
	FORCEINLINE constexpr XMMWORD_PTR operator-(smm val) { return XMMWORD_PTR((u8 *)ptr - val); }

	FORCEINLINE constexpr XMM operator*() const { return *(XMM *)ptr; }
	FORCEINLINE constexpr XMM &operator*() { return *(XMM *)ptr; }
};

union YMM {
	ELEMENTS(sizeof(__m256));
#if ARCH_AVX
	__m256 ps;
	__m256i pi;
	__m256d pd;
	FORCEINLINE constexpr YMM(__m256 ps) : ps(ps) {}
	FORCEINLINE constexpr YMM(__m256i pi) : pi(pi) {}
	FORCEINLINE constexpr YMM(__m256d pd) : pd(pd) {}
	FORCEINLINE constexpr operator __m256() const { return ps; }
	FORCEINLINE constexpr operator __m256i() const { return pi; }
	FORCEINLINE constexpr operator __m256d() const { return pd; }
	
	FORCEINLINE YMM operator!() const { return _mm256_xor_ps(ps, _mm256_castsi256_ps(_mm256_set1_epi32(~0))); }
	FORCEINLINE YMM operator~() const { return _mm256_xor_ps(ps, _mm256_castsi256_ps(_mm256_set1_epi32(~0))); }
	FORCEINLINE YMM operator^ (YMM b) const { return _mm256_xor_ps(ps, b); }
	FORCEINLINE YMM operator| (YMM b) const { return _mm256_or_ps(ps, b); }
	FORCEINLINE YMM operator& (YMM b) const { return _mm256_and_ps(ps, b); }
	FORCEINLINE YMM operator||(YMM b) const { return _mm256_or_ps(ps, b); }
	FORCEINLINE YMM operator&&(YMM b) const { return _mm256_and_ps(ps, b); }
	FORCEINLINE YMM operator==(YMM b) const { return _mm256_cmpeq_epi32(pi, b); }
	FORCEINLINE YMM operator!=(YMM b) const { return _mm256_xor_ps(_mm256_castsi256_ps(_mm256_cmpeq_epi32(pi, b)), _mm256_castsi256_ps(_mm256_set1_epi32(~0))); }
	FORCEINLINE YMM &operator^=(YMM b) { return *this = _mm256_xor_ps(ps, b); }
	FORCEINLINE YMM &operator|=(YMM b) { return *this = _mm256_or_ps (ps, b); }
	FORCEINLINE YMM &operator&=(YMM b) { return *this = _mm256_and_ps (ps, b); }
#else
	struct { XMM l, h; };
	FORCEINLINE constexpr YMM(XMM l, XMM h) : l(l), h(h) {}
	FORCEINLINE YMM operator!() const { return {!l, !h}; }
	FORCEINLINE YMM operator~() const { return {~l, ~h}; }
	FORCEINLINE YMM operator^ (YMM b) const { return {l ^  b.l, h ^  b.h}; }
	FORCEINLINE YMM operator| (YMM b) const { return {l |  b.l, h |  b.h}; }
	FORCEINLINE YMM operator& (YMM b) const { return {l &  b.l, h &  b.h}; }
	FORCEINLINE YMM operator||(YMM b) const { return {l || b.l, h || b.h}; }
	FORCEINLINE YMM operator&&(YMM b) const { return {l && b.l, h && b.h}; }
	FORCEINLINE YMM operator==(YMM b) const { return {l == b.l, h == b.h}; }
	FORCEINLINE YMM operator!=(YMM b) const { return {l != b.l, h != b.h}; }
	FORCEINLINE YMM &operator^=(YMM b) { return l ^= b.l, h ^= b.h, *this; }
	FORCEINLINE YMM &operator|=(YMM b) { return l |= b.l, h |= b.h, *this; }
	FORCEINLINE YMM &operator&=(YMM b) { return l &= b.l, h &= b.h, *this; }
#endif
	FORCEINLINE constexpr YMM() = default;
	FORCEINLINE constexpr operator YMMWORD_PTR();
};

struct YMMWORD_PTR {
	void *ptr;

	explicit FORCEINLINE constexpr YMMWORD_PTR(void *a) : ptr(a) {}
	template <class T>
	explicit FORCEINLINE constexpr YMMWORD_PTR(T &a) : ptr(&a) {}

	FORCEINLINE constexpr operator __m256 &() { return *(__m256 *)ptr; }
	FORCEINLINE constexpr operator __m256i &() { return *(__m256i *)ptr; }
	FORCEINLINE constexpr operator __m256d &() { return *(__m256d *)ptr; }
	FORCEINLINE constexpr operator YMM &() { return *(YMM *)ptr; }

	FORCEINLINE constexpr YMMWORD_PTR operator+(smm val) { return YMMWORD_PTR((u8 *)ptr + val); }
	FORCEINLINE constexpr YMMWORD_PTR operator-(smm val) { return YMMWORD_PTR((u8 *)ptr - val); }

	FORCEINLINE constexpr YMM operator*() const { return *(YMM *)ptr; }
	FORCEINLINE constexpr YMM &operator*() { return *(YMM *)ptr; }
};

union ZMM {
	ELEMENTS(sizeof(__m512));
#if ARCH_AVX512F
	__m512 ps;
	__m512i pi;
	__m512d pd;
	FORCEINLINE constexpr ZMM(__m512 ps) : ps(ps) {}
	FORCEINLINE constexpr ZMM(__m512i pi) : pi(pi) {}
	FORCEINLINE constexpr ZMM(__m512d pd) : pd(pd) {}
	FORCEINLINE constexpr operator __m512() const { return ps; }
	FORCEINLINE constexpr operator __m512i() const { return pi; }
	FORCEINLINE constexpr operator __m512d() const { return pd; }
	
	FORCEINLINE ZMM operator!() const { return _mm512_xor_ps(ps, _mm512_castsi512_ps(_mm512_set1_epi32(~0))); }
	FORCEINLINE ZMM operator~() const { return _mm512_xor_ps(ps, _mm512_castsi512_ps(_mm512_set1_epi32(~0))); }
	FORCEINLINE ZMM operator^ (ZMM b) const { return _mm512_xor_ps(ps, b); }
	FORCEINLINE ZMM operator| (ZMM b) const { return _mm512_or_ps(ps, b); }
	FORCEINLINE ZMM operator& (ZMM b) const { return _mm512_or_ps(ps, b); }
	FORCEINLINE ZMM operator||(ZMM b) const { return _mm512_or_ps(ps, b); }
	FORCEINLINE ZMM operator&&(ZMM b) const { return _mm512_or_ps(ps, b); }
	FORCEINLINE ZMM operator==(ZMM b) const { return _mm512_cmpeq_epi32(pi, b); }
	FORCEINLINE ZMM operator!=(ZMM b) const { return _mm512_xor_ps(_mm512_castsi512_ps(_mm512_cmpeq_epi32(pi, b)), _mm512_castsi512_ps(_mm512_set1_epi32(~0))); }
	FORCEINLINE ZMM &operator^=(ZMM b) { return *this = _mm512_xor_ps(ps, b); }
	FORCEINLINE ZMM &operator|=(ZMM b) { return *this = _mm512_or_ps (ps, b); }
	FORCEINLINE ZMM &operator&=(ZMM b) { return *this = _mm512_or_ps (ps, b); }
#else
	struct { YMM l, h; };
	FORCEINLINE constexpr ZMM(YMM l, YMM h) : l(l), h(h) {}
	FORCEINLINE ZMM operator!() const { return {!l, !h}; }
	FORCEINLINE ZMM operator~() const { return {~l, ~h}; }
	FORCEINLINE ZMM operator^ (ZMM b) const { return {l ^  b.l, h ^  b.h}; }
	FORCEINLINE ZMM operator| (ZMM b) const { return {l |  b.l, h |  b.h}; }
	FORCEINLINE ZMM operator& (ZMM b) const { return {l &  b.l, h &  b.h}; }
	FORCEINLINE ZMM operator||(ZMM b) const { return {l || b.l, h || b.h}; }
	FORCEINLINE ZMM operator&&(ZMM b) const { return {l && b.l, h && b.h}; }
	FORCEINLINE ZMM operator==(ZMM b) const { return {l == b.l, h == b.h}; }
	FORCEINLINE ZMM operator!=(ZMM b) const { return {l != b.l, h != b.h}; }
	FORCEINLINE ZMM &operator^=(ZMM b) { return l ^= b.l, h ^= b.h, *this; }
	FORCEINLINE ZMM &operator|=(ZMM b) { return l |= b.l, h |= b.h, *this; }
	FORCEINLINE ZMM &operator&=(ZMM b) { return l &= b.l, h &= b.h, *this; }
#endif

	FORCEINLINE constexpr ZMM() = default;
	FORCEINLINE constexpr operator ZMMWORD_PTR();
};

struct ZMMWORD_PTR {
	void *ptr;

	explicit FORCEINLINE constexpr ZMMWORD_PTR(void *a) : ptr(a) {}
	template <class T>
	explicit FORCEINLINE constexpr ZMMWORD_PTR(T &a) : ptr(&a) {}

	FORCEINLINE constexpr operator __m512 &() { return *(__m512 *)ptr; }
	FORCEINLINE constexpr operator __m512i &() { return *(__m512i *)ptr; }
	FORCEINLINE constexpr operator __m512d &() { return *(__m512d *)ptr; }
	FORCEINLINE constexpr operator ZMM &() { return *(ZMM *)ptr; }

	FORCEINLINE constexpr ZMMWORD_PTR operator+(smm val) { return ZMMWORD_PTR((u8 *)ptr + val); }
	FORCEINLINE constexpr ZMMWORD_PTR operator-(smm val) { return ZMMWORD_PTR((u8 *)ptr - val); }

	FORCEINLINE constexpr ZMM operator*() const { return *(ZMM *)ptr; }
	FORCEINLINE constexpr ZMM &operator*() { return *(ZMM *)ptr; }
};

FORCEINLINE constexpr YMM::operator YMMWORD_PTR() { return YMMWORD_PTR(this); }

FORCEINLINE void movaps(XMMWORD_PTR dst, XMMWORD_PTR src) { _mm_store_ps((float *)dst.ptr, *src); }

#if ARCH_AVX
FORCEINLINE void vmovaps(XMMWORD_PTR dst, XMMWORD_PTR src) { movaps(dst, src); }
FORCEINLINE void vmovaps(YMMWORD_PTR dst, YMMWORD_PTR src) { _mm256_store_ps((float *)dst.ptr, *src); }
FORCEINLINE void vmovdqa(YMMWORD_PTR dst, YMMWORD_PTR src) { vmovaps(dst, src); }
FORCEINLINE void vunpcklps(YMM &dst, YMM a, YMM b) { dst = _mm256_unpacklo_ps(a, b); }
FORCEINLINE void vunpckhps(YMM &dst, YMM a, YMM b) { dst = _mm256_unpackhi_ps(a, b); }
#endif

#if ARCH_AVX2
FORCEINLINE void vpermd(YMM &dst, YMM indices, YMM src) { dst = _mm256_permutevar8x32_epi32(src, indices); }
#endif

FORCEINLINE XMM andNot(XMM a, XMM b) { return _mm_andnot_ps(b, a); }
FORCEINLINE XMM select(XMM mask, XMM a, XMM b) { return _mm_blendv_ps(b, a, mask); }
FORCEINLINE u8 compressMask32(XMM mask) { return (u8)_mm_movemask_ps(mask); }
FORCEINLINE u8 compressMask64(XMM mask) { return (u8)_mm_movemask_pd(mask); }

#if ARCH_AVX
FORCEINLINE YMM select(YMM mask, YMM a, YMM b) { return _mm256_blendv_ps(b, a, mask); }
FORCEINLINE u8 compressMask32(YMM mask) { return (u8)_mm256_movemask_ps(mask); }
FORCEINLINE u8 compressMask64(YMM mask) { return (u8)_mm256_movemask_pd(mask); }
#else
FORCEINLINE YMM select(YMM mask, YMM a, YMM b) { return {select(mask.l, a.l, b.l), select(mask.h, a.h, b.h)}; }
FORCEINLINE u8 compressMask32(YMM mask) { return (u8)((compressMask32(mask.h) << 4) | compressMask32(mask.l)); }
FORCEINLINE u8 compressMask64(YMM mask) { return (u8)((compressMask64(mask.h) << 2) | compressMask64(mask.l)); }
#endif

#if ARCH_AVX512DQ
FORCEINLINE u16 compressMask32(ZMM mask) { return _mm512_movepi32_mask(m); }
FORCEINLINE u8 compressMask64(ZMM mask) { return _mm512_movepi64_mask(m); }
#else
FORCEINLINE u16 compressMask32(ZMM mask) { return (u16)((compressMask32(mask.h) << 8) | compressMask32(mask.l)); }
FORCEINLINE u8 compressMask64(ZMM mask) { return (u8)((compressMask64(mask.h) << 4) | compressMask64(mask.l)); }
#endif

} // namespace ASM
} // namespace TL
#pragma warning(pop)
