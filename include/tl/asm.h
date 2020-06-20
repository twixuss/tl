#include "common.h"
#include <immintrin.h>
#pragma warning(push)
#pragma warning(disable : 4201) // unnamed struct
namespace TL {
namespace ASM {

#if ARCH_AVX512F
#define _simdWidth 64
#elif ARCH_AVX
#define _simdWidth 32
#else
#define _simdWidth 16
#endif

// simd register width in bytes
static constexpr u32 simdWidth = _simdWidth;

#undef _simdWidth

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
	FORCEINLINE XMM &operator|=(XMM b) { return *this = _mm_or_ps(ps, b); }
	FORCEINLINE XMM &operator&=(XMM b) { return *this = _mm_and_ps(ps, b); }
};
struct XMMWORD_PTR {
	void *ptr;
	explicit FORCEINLINE constexpr XMMWORD_PTR(void *a) : ptr(a) {}
	FORCEINLINE constexpr operator __m128 &() { return *(__m128 *)ptr; }
	FORCEINLINE constexpr operator __m128i &() { return *(__m128i *)ptr; }
	FORCEINLINE constexpr operator __m128d &() { return *(__m128d *)ptr; }
	FORCEINLINE constexpr operator XMM &() { return *(XMM *)ptr; }
	FORCEINLINE constexpr XMMWORD_PTR operator+(smm val) { return XMMWORD_PTR((u8 *)ptr + val); }
	FORCEINLINE constexpr XMMWORD_PTR operator-(smm val) { return XMMWORD_PTR((u8 *)ptr - val); }
};

union YMM {
	ELEMENTS(sizeof(__m256));
	struct { XMM l, h; };
	FORCEINLINE constexpr YMM(XMM l, XMM h) : l(l), h(h) {}
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
};

struct YMMWORD_PTR {
	void *ptr;
	explicit FORCEINLINE constexpr YMMWORD_PTR(void *a) : ptr(a) {}
	FORCEINLINE constexpr operator __m256 &() { return *(__m256 *)ptr; }
	FORCEINLINE constexpr operator __m256i &() { return *(__m256i *)ptr; }
	FORCEINLINE constexpr operator __m256d &() { return *(__m256d *)ptr; }
	FORCEINLINE constexpr operator YMM &() { return *(YMM *)ptr; }
	FORCEINLINE constexpr YMMWORD_PTR operator+(smm val) { return YMMWORD_PTR((u8 *)ptr + val); }
	FORCEINLINE constexpr YMMWORD_PTR operator-(smm val) { return YMMWORD_PTR((u8 *)ptr - val); }
};

union ZMM {
	ELEMENTS(sizeof(__m512));
	struct { YMM l, h; };
	FORCEINLINE constexpr ZMM(YMM l, YMM h) : l(l), h(h) {}
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
};

struct ZMMWORD_PTR {
	void *ptr;
	explicit FORCEINLINE constexpr ZMMWORD_PTR(void *a) : ptr(a) {}
	FORCEINLINE constexpr operator __m512 &() { return *(__m512 *)ptr; }
	FORCEINLINE constexpr operator __m512i &() { return *(__m512i *)ptr; }
	FORCEINLINE constexpr operator __m512d &() { return *(__m512d *)ptr; }
	FORCEINLINE constexpr operator ZMM &() { return *(ZMM *)ptr; }
	FORCEINLINE constexpr ZMMWORD_PTR operator+(smm val) { return ZMMWORD_PTR((u8 *)ptr + val); }
	FORCEINLINE constexpr ZMMWORD_PTR operator-(smm val) { return ZMMWORD_PTR((u8 *)ptr - val); }
};

template<u32 w, class T, class UInt>
void shuffleFallback(T *dst, UInt const *idx) {
	static_assert(isPowerOf2(w));
	T src[w];
	memcpy(src, dst, sizeof(src));
	for(u32 i=0;i<w;++i) {
		((T *)dst)[i] = src[idx[i] & (w - 1)];
	}
}

template <u32 _cmp>
struct Cmp {
	inline static constexpr u32 cmp = _cmp;
};
Cmp<_CMP_LT_OQ>  cmpLT;
Cmp<_CMP_GT_OQ>  cmpGT;
Cmp<_CMP_LE_OQ>  cmpLE;
Cmp<_CMP_GE_OQ>  cmpGE;
Cmp<_CMP_EQ_OQ>  cmpEQ;
Cmp<_CMP_NEQ_OQ> cmpNE;

FORCEINLINE void movaps(XMM &dst, XMM src) { _mm_store_ps(dst.f32, src); }
FORCEINLINE void movups(XMM &dst, XMM src) { _mm_storeu_ps(dst.f32, src); }
FORCEINLINE void unpcklps(XMM &dst, XMM a, XMM b) { dst = _mm_unpacklo_ps(a, b); }
FORCEINLINE void unpckhps(XMM &dst, XMM a, XMM b) { dst = _mm_unpackhi_ps(a, b); }
FORCEINLINE void addps(XMM &dst, XMM src) { dst = _mm_add_ps(dst, src); }
FORCEINLINE void subps(XMM &dst, XMM src) { dst = _mm_sub_ps(dst, src); }
FORCEINLINE void mulps(XMM &dst, XMM src) { dst = _mm_mul_ps(dst, src); }
FORCEINLINE void divps(XMM &dst, XMM src) { dst = _mm_div_ps(dst, src); }

#if ARCH_AVX
FORCEINLINE void movaps(YMM &dst, YMM src) { _mm256_store_ps(dst.f32, src); }
FORCEINLINE void unpcklps(YMM &dst, YMM a, YMM b) { dst = _mm256_unpacklo_ps(a, b); }
FORCEINLINE void unpckhps(YMM &dst, YMM a, YMM b) { dst = _mm256_unpackhi_ps(a, b); }
FORCEINLINE void addps(YMM &dst, YMM src) { dst = _mm256_add_ps(dst, src); }
FORCEINLINE void subps(YMM &dst, YMM src) { dst = _mm256_sub_ps(dst, src); }
FORCEINLINE void mulps(YMM &dst, YMM src) { dst = _mm256_mul_ps(dst, src); }
FORCEINLINE void divps(YMM &dst, YMM src) { dst = _mm256_div_ps(dst, src); }
template <u32 cmp>
FORCEINLINE void cmpps(XMM &dst, XMM src, Cmp<cmp>) { dst = _mm_cmp_ps(dst, src, cmp); }
template <u32 cmp>
FORCEINLINE void cmpps(YMM &dst, YMM src, Cmp<cmp>) { dst = _mm256_cmp_ps(dst, src, cmp); }
#else
FORCEINLINE void movaps(YMM &dst, YMM src) { _mm_store_ps(dst.l.f32, src.l); _mm_store_ps(dst.h.f32, src.h); }
FORCEINLINE void unpcklps(YMM &dst, YMM a, YMM b) { unpcklps(dst.l, a.l, b.l); unpcklps(dst.l, a.h, b.h); }
FORCEINLINE void unpckhps(YMM &dst, YMM a, YMM b) { unpckhps(dst.l, a.l, b.l); unpckhps(dst.l, a.h, b.h); }
FORCEINLINE void addps(YMM &dst, YMM src) { addps(dst.l, src.l); addps(dst.h, src.h); }
FORCEINLINE void subps(YMM &dst, YMM src) { subps(dst.l, src.l); subps(dst.h, src.h); }
FORCEINLINE void mulps(YMM &dst, YMM src) { mulps(dst.l, src.l); mulps(dst.h, src.h); }
FORCEINLINE void divps(YMM &dst, YMM src) { divps(dst.l, src.l); divps(dst.h, src.h); }
template <u32 cmp>
FORCEINLINE void cmpps(XMM &dst, XMM src, Cmp<cmp>) { 
		 if constexpr(cmp == cmpLT.cmp) dst = _mm_cmplt_ps(dst, src);
	else if constexpr(cmp == cmpGT.cmp) dst = _mm_cmpgt_ps(dst, src);
	else if constexpr(cmp == cmpLE.cmp) dst = _mm_cmple_ps(dst, src);
	else if constexpr(cmp == cmpGE.cmp) dst = _mm_cmpge_ps(dst, src);
	else if constexpr(cmp == cmpEQ.cmp) dst = _mm_cmpeq_ps(dst, src);
	else if constexpr(cmp == cmpNE.cmp) dst = _mm_cmpneq_ps(dst, src);
	else static_assert(false, "bad comparison");
}
template <u32 cmp>
FORCEINLINE void cmpps(YMM &dst, YMM src, Cmp<cmp> c) { cmpps(dst.l, src.l, c); cmpps(dst.h, src.h, c); }
#endif
FORCEINLINE void movdqa(YMM &dst, YMM src) { movaps(dst, src); }

#if ARCH_AVX2
FORCEINLINE void permd(YMM &dst, YMM indices, YMM src) { dst = _mm256_permutevar8x32_epi32(src, indices); }
#endif

FORCEINLINE XMM negate(XMM a) { return _mm_xor_ps(a, _mm_set1_ps(-0.0f)); }

FORCEINLINE XMM andNot(XMM a, XMM b) { return _mm_andnot_ps(b, a); }
FORCEINLINE XMM select(XMM mask, XMM a, XMM b) { return _mm_blendv_ps(b, a, mask); }
FORCEINLINE u8 compressMask32(XMM mask) { return (u8)_mm_movemask_ps(mask); }
FORCEINLINE u8 compressMask64(XMM mask) { return (u8)_mm_movemask_pd(mask); }

#if ARCH_AVX
FORCEINLINE YMM select(YMM mask, YMM a, YMM b) { return _mm256_blendv_ps(b, a, mask); }
FORCEINLINE u8 compressMask32(YMM mask) { return (u8)_mm256_movemask_ps(mask); }
FORCEINLINE u8 compressMask64(YMM mask) { return (u8)_mm256_movemask_pd(mask); }
FORCEINLINE XMM shuffle32(XMM src, XMM idx) { return _mm_permutevar_ps(src.ps, idx.pi); }
FORCEINLINE YMM negate(YMM a) { return _mm256_xor_ps(a, _mm256_set1_ps(-0.0f)); }
#else
FORCEINLINE YMM select(YMM mask, YMM a, YMM b) { return {select(mask.l, a.l, b.l), select(mask.h, a.h, b.h)}; }
FORCEINLINE u8 compressMask32(YMM mask) { return (u8)((compressMask32(mask.h) << 4) | compressMask32(mask.l)); }
FORCEINLINE u8 compressMask64(YMM mask) { return (u8)((compressMask64(mask.h) << 2) | compressMask64(mask.l)); }
FORCEINLINE XMM shuffle32(XMM src, XMM idx) { shuffleFallback<4>(src.f32, idx.u32); return src; }
FORCEINLINE YMM negate(YMM a) { 
	a.l = negate(a.l); 
	a.h = negate(a.h); 
	return a;
}
#endif

#if ARCH_AVX2
FORCEINLINE XMM gather32(void const *src, XMM offsets) { return _mm_i32gather_ps((f32*)src, offsets.pi, 1); }
FORCEINLINE YMM gather32(void const *src, YMM offsets) { return _mm256_i32gather_ps((f32*)src, offsets.pi, 1); }
FORCEINLINE YMM shuffle32(YMM src, YMM idx) { return _mm256_permutevar8x32_ps(src.ps, idx.pi); }
#else
FORCEINLINE XMM gather32(void const *src, XMM offsets) { 
	u8 *memory = (u8* )src;
	return _mm_setr_ps(*(f32*)(memory + offsets.u32[0]), 
					   *(f32*)(memory + offsets.u32[1]), 
					   *(f32*)(memory + offsets.u32[2]), 
					   *(f32*)(memory + offsets.u32[3])); 
}
FORCEINLINE YMM gather32(void const *src, YMM offsets) { 
	YMM result;
	result.l = gather32(src, offsets.l);
	result.h = gather32(src, offsets.h);
	return result;
}
FORCEINLINE YMM shuffle32(YMM src, YMM idx) { 
	YMM result;
	result.l = shuffle32(src.l, idx.l);
	result.h = shuffle32(src.h, idx.h);
	return result;
}
#endif

#if ARCH_AVX512F
FORCEINLINE ZMM gather32(void const *src, ZMM offsets) { return _mm512_i32gather_ps(offsets.pi, (f32*)src, 1); }
#else
FORCEINLINE ZMM gather32(void const *src, ZMM offsets) { 
	ZMM result;
	result.l = gather32(src, offsets.l);
	result.h = gather32(src, offsets.h);
	return result;
}
#endif

#if ARCH_AVX512DQ
FORCEINLINE u16 compressMask32(ZMM mask) { return _mm512_movepi32_mask(m); }
FORCEINLINE u8 compressMask64(ZMM mask) { return _mm512_movepi64_mask(m); }
#else
FORCEINLINE u16 compressMask32(ZMM mask) { return (u16)((compressMask32(mask.h) << 8) | compressMask32(mask.l)); }
FORCEINLINE u8 compressMask64(ZMM mask) { return (u8)((compressMask64(mask.h) << 4) | compressMask64(mask.l)); }
#endif

FORCEINLINE XMM addF32(XMM a, XMM b) { addps(a, b); return a; }
FORCEINLINE XMM subF32(XMM a, XMM b) { subps(a, b); return a; }
FORCEINLINE XMM mulF32(XMM a, XMM b) { mulps(a, b); return a; }
FORCEINLINE XMM divF32(XMM a, XMM b) { divps(a, b); return a; }
FORCEINLINE YMM addF32(YMM a, YMM b) { addps(a, b); return a; }
FORCEINLINE YMM subF32(YMM a, YMM b) { subps(a, b); return a; }
FORCEINLINE YMM mulF32(YMM a, YMM b) { mulps(a, b); return a; }
FORCEINLINE YMM divF32(YMM a, YMM b) { divps(a, b); return a; }

template <u32 cmp> FORCEINLINE XMM cmpF32(XMM a, XMM b, Cmp<cmp> c) { cmpps(a, b, c); return a; }
template <u32 cmp> FORCEINLINE YMM cmpF32(YMM a, YMM b, Cmp<cmp> c) { cmpps(a, b, c); return a; }

} // namespace ASM
} // namespace TL
#pragma warning(pop)
