#pragma once
#include "system.h"

#if COMPILER_MSVC
#pragma warning(push,0)
#include <intrin.h>
#else
#include <immintrin.h>
#endif

namespace tl {

 // simd register width in bytes
#if ARCH_AVX512F
static constexpr u32 simd_width = 64;
#elif ARCH_AVX
static constexpr u32 simd_width = 32;
#else
static constexpr u32 simd_width = 16;
#endif

template <class T>
static constexpr u32 simd_count = simd_width / sizeof(T);

#define f2i16 _mm_castps_si128
#define f2d16 _mm_castps_pd
#define i2f16 _mm_castsi128_ps
#define d2f16 _mm_castpd_ps

#define f2i32 _mm256_castps_si256
#define f2d32 _mm256_castps_pd
#define i2f32 _mm256_castsi256_ps
#define d2f32 _mm256_castpd_ps

#define f2i64 _mm512_castps_si512
#define f2d64 _mm512_castps_pd
#define i2f64 _mm512_castsi512_ps
#define d2f64 _mm512_castpd_ps

//
// Type definitions
//
// It's not great because no type safety
// The reason for this is ultra slowness of wrappers
//

// f - float
// s - signed int
// u - unsigned int
// b - boolean
// a - any

namespace simd {
using vec16 = __m128;
using f32x4 = __m128; using f64x2 = __m128;
using s8x16 = __m128; using s16x8 = __m128; using s32x4 = __m128; using s64x2 = __m128;
using u8x16 = __m128; using u16x8 = __m128; using u32x4 = __m128; using u64x2 = __m128;
using b8x16 = __m128; using b16x8 = __m128; using b32x4 = __m128; using b64x2 = __m128;
using a8x16 = __m128; using a16x8 = __m128; using a32x4 = __m128; using a64x2 = __m128;

#if ARCH_AVX
using vec32 = __m256;
using f32x8 = __m256; using f64x4 = __m256;
using s8x32 = __m256; using s16x16 = __m256; using s32x8 = __m256; using s64x4 = __m256;
using u8x32 = __m256; using u16x16 = __m256; using u32x8 = __m256; using u64x4 = __m256;
using b8x32 = __m256; using b16x16 = __m256; using b32x8 = __m256; using b64x4 = __m256;
using a8x32 = __m256; using a16x16 = __m256; using a32x8 = __m256; using a64x4 = __m256;
#endif

#if ARCH_AVX512F
using vec64  = __m512;
using f32x16 = __m512; using f64x8  = __m512;
using s8x64 = __m512; using s16x32  = __m512; using s32x16 = __m512; using s64x8 = __m512;
using u8x64 = __m512; using u16x32  = __m512; using u32x16 = __m512; using u64x8 = __m512;
using b8x64 = __m512; using b16x32  = __m512; using b32x16 = __m512; using b64x8 = __m512;
using a8x64 = __m512; using a16x32  = __m512; using a32x16 = __m512; using a64x8 = __m512;
#endif
}

//
// Vector constants
//

#define vec16_zero _mm_setzero_ps()
#define vec16_one i2f16(_mm_set1_epi8(0xFF)

#if ARCH_AVX
#define vec32_zero _mm256_setzero_ps()
#define vec32_one i2f32(_mm256_set1_epi8(0xFF)
#endif

#if ARCH_AVX512F
#define vec64_zero _mm512_setzero_ps()
#define vec64_one i2f64(_mm512_set1_epi8(0xFF)
#endif


//
// Logical operations
//

#define vec16_xor(a,b) _mm_xor_ps(a,b)
#define vec16_and(a,b) _mm_and_ps(a,b)
#define vec16_or(a,b)  _mm_or_ps(a,b)
#define vec16_not(a) _mm_xor_ps(a,vec16_one))
#define vec16_and_not(a,b) _mm_andnot_ps(b,a)

#if ARCH_AVX
#define vec32_xor(a,b) _mm256_xor_ps(a,b)
#define vec32_and(a,b) _mm256_and_ps(a,b)
#define vec32_or(a,b)  _mm256_or_ps(a,b)
#define vec32_not(a) _mm256_xor_ps(a,vec32_one))
#define vec32_and_not(a,b) _mm256_andnot_ps(b,a)
#endif

#if ARCH_AVX512F
#define vec64_xor(a,b) _mm512_xor_ps(a,b)
#define vec64_and(a,b) _mm512_and_ps(a,b)
#define vec64_or(a,b)  _mm512_or_ps(a,b)
#define vec64_not(a) _mm512_xor_ps(a,vec64_one))
#define vec64_and_not(a,b) _mm512_andnot_ps(b,a)
#endif


//
// Swizzle/Shuffle/ operations
//

#define select32x4(mask,a,b) _mm_blendv_ps(b,a,mask)
#if ARCH_AVX
#define shuffle32x4(a,n0,n1,n2,n3) _mm_permute_ps(a,_MM_SHUFFLE(n3,n2,n1,n0))
#else
#define shuffle32x4(a,n0,n1,n2,n3) ([](a32x8 _a){return _mm_shuffle_ps(_a,_a,_MM_SHUFFLE(n3,n2,n1,n0));}(a))
#endif

#if ARCH_AVX
#define select32x8(mask,a,b) _mm256_blendv_ps(b,a,mask)
#if ARCH_AVX2
#define permute32x8(a,n0,n1,n2,n3,n4,n5,n6,n7) _mm256_permutevar8x32_ps(a,_mm256_setr_epi32(n0,n1,n2,n3,n4,n5,n6,n7))
#else
#endif
#endif


//
// Operations with 16-byte vectors
//

#define f32x4_set(a,b,c,d) _mm_setr_ps(a,b,c,d)
#define f32x4_set1(a) _mm_set1_ps(a)
#define f32x4_load(address) _mm_loadu_ps((f32*)(address))
#define f32x4_store(address, v) _mm_storeu_ps((f32*)(address), v)
#define f32x4_get(a,b) ([&]{int _wtf_=_mm_extract_ps(a,b);return*(f32*)&_wtf_;}())
#if ARCH_AVX
#define f32x4_lt(a,b) _mm_cmp_ps(a,b,_CMP_LT_OQ)
#define f32x4_gt(a,b) _mm_cmp_ps(a,b,_CMP_GT_OQ)
#define f32x4_le(a,b) _mm_cmp_ps(a,b,_CMP_LE_OQ)
#define f32x4_ge(a,b) _mm_cmp_ps(a,b,_CMP_GE_OQ)
#define f32x4_eq(a,b) _mm_cmp_ps(a,b,_CMP_EQ_OQ)
#define f32x4_ne(a,b) _mm_cmp_ps(a,b,_CMP_NEQ_OQ)
#else
#define f32x4_lt(a,b) _mm_cmplt_ps(a,b)
#define f32x4_gt(a,b) _mm_cmpgt_ps(a,b)
#define f32x4_le(a,b) _mm_cmple_ps(a,b)
#define f32x4_ge(a,b) _mm_cmpge_ps(a,b)
#define f32x4_eq(a,b) _mm_cmpeq_ps(a,b)
#define f32x4_ne(a,b) _mm_cmpneq_ps(a,b)
#endif
#define f32x4_add(a,b) _mm_add_ps(a,b)
#define f32x4_sub(a,b) _mm_sub_ps(a,b)
#define f32x4_mul(a,b) _mm_mul_ps(a,b)
#define f32x4_div(a,b) _mm_div_ps(a,b)
#define f32x4_neg(a) _mm_xor_ps(a,_mm_set1_ps(-0.0f))
#define f32x4_rcp(a) _mm_rcp_ps(a)
#define f32x4_dot(a, b) _mm_cvtss_f32(_mm_dp_ps(a,b,0xf1))
#define f32x4_trunc(a) _mm_round_ps(a,_MM_FROUND_TRUNC|_MM_FROUND_NO_EXC)
#define f32x4_round(a) _mm_round_ps(a,_MM_FROUND_TO_NEAREST_INT|_MM_FROUND_NO_EXC)
#define f32x4_floor(a) _mm_round_ps(a,_MM_FROUND_TO_NEG_INF|_MM_FROUND_NO_EXC)
#define f32x4_ceil(a)  _mm_round_ps(a,_MM_FROUND_TO_POS_INF|_MM_FROUND_NO_EXC)
#define f32x4_min(a,b) _mm_min_ps(a,b)
#define f32x4_max(a,b) _mm_max_ps(a,b)
#define f32x4_clamp(a,min,max) f32x4_min(f32x4_max(a,min),max)
#if ARCH_FMA
#define f32x4_muladd(a,b,c) _mm_fmadd_ps(a,b,c)
#else
#define f32x4_muladd(a,b,c) f32x4_add(f32x4_mul(a,b),c)
#endif
#define f32x4_lerp(a,b,t) f32x4_muladd(f32x4_sub(b,a),t,a)
#define f32x4_to_s32x4(a) i2f16(_mm_cvttps_epi32(a))
#define f32x4_to_u32x4(a) i2f16(_mm_cvttps_epi32(a))


#define s32x4_set(a,b,c,d) i2f16(_mm_setr_epi32(a,b,c,d))
#define s32x4_set1(a) i2f16(_mm_set1_epi32(a))
#define s32x4_get(a,b) _mm_extract_epi32(f2i16(a),b)
#define s32x4_eq(a,b) i2f16(_mm_cmpeq_epi32(f2i16(a),f2i16(b)))
#define s32x4_ne(a,b) vec16_not(s32x4_eq(b,a))
#define s32x4_gt(a,b) i2f16(_mm_cmpgt_epi32(f2i16(a),f2i16(b)))
#define s32x4_lt(a,b) s32x4_gt(b,a)
#define s32x4_le(a,b) vec16_not(s32x4_gt(a,b))
#define s32x4_ge(a,b) vec16_not(s32x4_lt(a,b))
#define s32x4_add(a,b) i2f16(_mm_add_epi32(f2i16(a),f2i16(b)))
#define s32x4_sub(a,b) i2f16(_mm_sub_epi32(f2i16(a),f2i16(b)))
#define s32x4_mul(a,b) i2f16(_mm_mullo_epi32(f2i16(a),f2i16(b)))
#define s32x4_div(a,b) i2f16(_mm_div_epi32(f2i16(a),f2i16(b)))
#define s32x4_div_f32(a,b) i2f16(_mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(f2i16(a)),_mm_cvtepi32_ps(f2i16(b)))))
#define s32x4_neg(a)   i2f16(_mm_sub_epi32(_mm_setzero_si128(),f2i16(a)))
#define s32x4_to_f32x4(a) _mm_cvtepi32_ps(f2i16(a))
#define s32x4_to_u32x4(a) (a)


#define u32x4_set(a,b,c,d) i2f16(_mm_setr_epi32(a,b,c,d))
#define u32x4_set1(a) i2f16(_mm_set1_epi32(a))
#define u32x4_get(a,b) ((u32)_mm_extract_epi32(f2i16(a),b))
#define u32x4_eq(a,b) s32x4_eq(a,b)
#define u32x4_ne(a,b) s32x4_ne(a,b)
#define u32x4_gt(a,b) s32x4_gt(_mm_xor_si128(f2i16(a),_mm_set1_epi32(0x80000000)),_mm_xor_si128(f2i16(b),_mm_set1_epi32(0x80000000)))
#define u32x4_lt(a,b) u32x4_gt(b,a)
#define u32x4_le(a,b) vec16_not(u32x4_gt(a,b))
#define u32x4_ge(a,b) vec16_not(u32x4_lt(a,b))
#define u32x4_add(a,b) i2f16(_mm_add_epi32(f2i16(a),f2i16(b)))
#define u32x4_sub(a,b) i2f16(_mm_sub_epi32(f2i16(a),f2i16(b)))
#define u32x4_mul(a,b) i2f16(_mm_mullo_epi32(f2i16(a),f2i16(b)))
#define u32x4_div(a,b) i2f16(_mm_div_epu32(f2i16(a),f2i16(b)))
#define u32x4_rem(a,b) i2f16(_mm_rem_epu32(f2i16(a),f2i16(b)))
#define u32x4_to_f32x4(a) _mm_cvtepi32_ps(f2i16(a))
#define u32x4_to_s32x4(a) (a)


#define b32x4_get_mask(a) _mm_movemask_ps(a)
#define b32x4_all_true(a) (b32x4_get_mask(a) == 0xF)
#define b32x4_all_false(a) (b32x4_get_mask(a) == 0)
#define b32x4_any_false(a) (b32x4_get_mask(a) != 0xF)
#define b32x4_any_true(a) (b32x4_get_mask(a) != 0)


#define f64x2_set(a,b) d2f16(_mm_setr_pd(a,b))
#define f64x2_set1(a) d2f16(_mm_set1_pd(a))
#define f64x2_get(a,b) ([&]{auto _wtf_=_mm_extract_epi64(f2i16(a),b);return*(f64*)&_wtf_;}()}
#if ARCH_AVX
#define f64x2_lt(a,b) d2f16(_mm_cmp_pd(f2d16(a),f2d16(b),_CMP_LT_OQ))
#define f64x2_gt(a,b) d2f16(_mm_cmp_pd(f2d16(a),f2d16(b),_CMP_GT_OQ))
#define f64x2_le(a,b) d2f16(_mm_cmp_pd(f2d16(a),f2d16(b),_CMP_LE_OQ))
#define f64x2_ge(a,b) d2f16(_mm_cmp_pd(f2d16(a),f2d16(b),_CMP_GE_OQ))
#define f64x2_eq(a,b) d2f16(_mm_cmp_pd(f2d16(a),f2d16(b),_CMP_EQ_OQ))
#define f64x2_ne(a,b) d2f16(_mm_cmp_pd(f2d16(a),f2d16(b),_CMP_NEQ_OQ))
#else
#define f64x2_lt(a,b) d2f16(_mm_cmplt_pd(f2d16(a),f2d16(b)))
#define f64x2_gt(a,b) d2f16(_mm_cmpgt_pd(f2d16(a),f2d16(b)))
#define f64x2_le(a,b) d2f16(_mm_cmple_pd(f2d16(a),f2d16(b)))
#define f64x2_ge(a,b) d2f16(_mm_cmpge_pd(f2d16(a),f2d16(b)))
#define f64x2_eq(a,b) d2f16(_mm_cmpeq_pd(f2d16(a),f2d16(b)))
#define f64x2_ne(a,b) d2f16(_mm_cmpneq_pd(f2d16(a),f2d16(b)))
#endif
#define f64x2_add(a,b) d2f16(_mm_add_pd(f2d16(a),f2d16(b)))
#define f64x2_sub(a,b) d2f16(_mm_sub_pd(f2d16(a),f2d16(b)))
#define f64x2_mul(a,b) d2f16(_mm_mul_pd(f2d16(a),f2d16(b)))
#define f64x2_div(a,b) d2f16(_mm_div_pd(f2d16(a),f2d16(b)))
#define f64x2_neg(a)   d2f16(_mm_xor_pd(f2d16(a),_mm_set1_pd(-0.0)))
#define f64x2_rcp(a)   d2f16(_mm_div_pd(_mm_set1_pd(1.0f),f2d16(a)))
#define f64x2_trunc(a) d2f16(_mm_round_pd(f2d16(a),_MM_FROUND_TRUNC|_MM_FROUND_NO_EXC))
#define f64x2_round(a) d2f16(_mm_round_pd(f2d16(a),_MM_FROUND_TO_NEAREST_INT|_MM_FROUND_NO_EXC))
#define f64x2_floor(a) d2f16(_mm_round_pd(f2d16(a),_MM_FROUND_TO_NEG_INF|_MM_FROUND_NO_EXC))
#define f64x2_ceil(a)  d2f16(_mm_round_pd(f2d16(a),_MM_FROUND_TO_POS_INF|_MM_FROUND_NO_EXC))


#define s64x2_set(a,b) i2f16(_mm_setr_epi64x(a,b))
#define s64x2_set1(a) i2f16(_mm_set1_epi64x(a))
#define s64x2_get(a,b) _mm_extract_epi64(f2i16(a),b)
#define s64x2_eq(a,b) i2f16(_mm_cmpeq_epi64(f2i16(a),f2i16(b)))
#define s64x2_ne(a,b) vec16_not(s64x2_eq(b,a))
#define s64x2_gt(a,b) i2f16(_mm_cmpgt_epi64(f2i16(a),f2i16(b)))
#define s64x2_lt(a,b) s64x2_gt(b,a)
#define s64x2_le(a,b) vec16_not(s64x2_gt(a,b))
#define s64x2_ge(a,b) vec16_not(s64x2_lt(a,b))
#define s64x2_add(a,b) i2f16(_mm_add_epi64(f2i16(a),f2i16(b)))
#define s64x2_sub(a,b) i2f16(_mm_sub_epi64(f2i16(a),f2i16(b)))
#if ARCH_AVX512DQ
#define s64x2_mul(a,b) i2f16(_mm_mullo_epi64(f2i16(a),f2i16(b)))
#endif
#define s64x2_div(a,b) i2f16(_mm_div_epi64(f2i16(a),f2i16(b)))
#define s64x2_neg(a) i2f16(_mm_sub_epi64(_mm_setzero_si128(),f2i16(a)))


#define u64x2_set(a,b) i2f16(_mm_setr_epi64x(a,b))
#define u64x2_set1(a) i2f16(_mm_set1_epi64x(a))
#define u64x2_get(a,b) ((u64)_mm_extract_epi64(f2i16(a),b))
#define u64x2_eq(a,b) s64x2_eq(a,b)
#define u64x2_ne(a,b) s64x2_ne(a,b)
#define u64x2_gt(a,b) s64x2_gt(_mm_xor_si128(f2i16(a),_mm_set1_epi64x(0x8000000000000000)),_mm_xor_si128(f2i16(b),_mm_set1_epi64x(0x8000000000000000)))
#define u64x2_lt(a,b) u64x2_gt(b,a)
#define u64x2_le(a,b) vec16_not(u64x2_gt(a,b))
#define u64x2_ge(a,b) vec16_not(u64x2_lt(a,b))
#define u64x2_add(a,b) i2f16(_mm_add_epi64(f2i16(a),f2i16(b)))
#define u64x2_sub(a,b) i2f16(_mm_sub_epi64(f2i16(a),f2i16(b)))
#if ARCH_AVX512DQ
#define u64x2_mul(a,b) i2f16(_mm_mullo_epi64(f2i16(a),f2i16(b)))
#endif
#define u64x2_div(a,b) i2f16(_mm_div_epu64(f2i16(a),f2i16(b)))
#define u64x2_rem(a,b) i2f16(_mm_rem_epu64(f2i16(a),f2i16(b)))


#define b64x2_get_mask(a) _mm_movemask_pd(f2d16(a))
#define b64x2_all_true(a) (b64x2_get_mask(a) == 0x3)
#define b64x2_all_false(a) (b64x2_get_mask(a) == 0)
#define b64x2_any_false(a) (b64x2_get_mask(a) != 0x3)
#define b64x2_any_true(a) (b64x2_get_mask(a) != 0)



//
// Operations with 32-byte vectors
//

#if ARCH_AVX
#if ARCH_AVX2
#define s32x8_shift_right_i(a, b) i2f32(_mm256_srai_epi32(f2i32(a), b))
#define u32x8_shift_right_i(a, b) i2f32(_mm256_srli_epi32(f2i32(a), b))
#define i32x8_shift_left_i(a, b)  i2f32(_mm256_slli_epi32(f2i32(a), b))

#define s32x8_get(a, b) ((s32)_mm256_extract_epi32(f2i32(a), b))
#define u32x8_get(a, b) ((u32)_mm256_extract_epi32(f2i32(a), b))
#endif

#define f32x8_set(a,b,c,d,e,f,g,h) _mm256_setr_ps(a,b,c,d,e,f,g,h)
#define f32x8_set1(a) _mm256_set1_ps(a)
#define f32x8_load(address) _mm256_loadu_ps((f32*)(address))
#define f32x8_store(address, v) _mm256_storeu_ps((f32*)(address), v)
#define f32x8_lt(a,b) _mm256_cmp_ps(a,b,_CMP_LT_OQ)
#define f32x8_gt(a,b) _mm256_cmp_ps(a,b,_CMP_GT_OQ)
#define f32x8_le(a,b) _mm256_cmp_ps(a,b,_CMP_LE_OQ)
#define f32x8_ge(a,b) _mm256_cmp_ps(a,b,_CMP_GE_OQ)
#define f32x8_eq(a,b) _mm256_cmp_ps(a,b,_CMP_EQ_OQ)
#define f32x8_ne(a,b) _mm256_cmp_ps(a,b,_CMP_NEQ_OQ)
#define f32x8_add(a,b) _mm256_add_ps(a,b)
#define f32x8_sub(a,b) _mm256_sub_ps(a,b)
#define f32x8_mul(a,b) _mm256_mul_ps(a,b)
#define f32x8_div(a,b) _mm256_div_ps(a,b)
#define f32x8_neg(a) _mm256_xor_ps(a,_mm256_set1_ps(-0.0f))
#define f32x8_rcp(a) _mm256_rcp_ps(a)
#define f32x8_trunc(a) _mm256_round_ps(a,_MM_FROUND_TRUNC|_MM_FROUND_NO_EXC)
#define f32x8_round(a) _mm256_round_ps(a,_MM_FROUND_TO_NEAREST_INT|_MM_FROUND_NO_EXC)
#define f32x8_floor(a) _mm256_round_ps(a,_MM_FROUND_TO_NEG_INF|_MM_FROUND_NO_EXC)
#define f32x8_ceil(a)  _mm256_round_ps(a,_MM_FROUND_TO_POS_INF|_MM_FROUND_NO_EXC)
#define f32x8_min(a,b) _mm256_min_ps(a,b)
#define f32x8_max(a,b) _mm256_max_ps(a,b)
#define f32x8_clamp(a,min,max) f32x8_min(f32x8_max(a,min),max)
#if ARCH_AVX2
#define f32x8_sign(a) select32x8(s32x8_sari(a, 31),f32x8_set1(1),f32x8_set1(-1))
#else
#define f32x8_sign(a) select32x8(f32x8_gt(a, f32x8_set1(0)),f32x8_set1(1),f32x8_set1(-1))
#endif
#if ARCH_FMA
// a * b + c
#define f32x8_muladd(a,b,c) _mm256_fmadd_ps(a,b,c)
#else
// a * b + c
#define f32x8_muladd(a,b,c) f32x8_add(f32x8_mul(a,b),c)
#endif
#define f32x8_lerp(a,b,t) f32x8_muladd(f32x8_sub(b,a),t,a)


#define s32x8_set(a,b,c,d,e,f,g,h) i2f32(_mm256_setr_epi32(a,b,c,d,e,f,g,h))
#define s32x8_set1(a) i2f32(_mm256_set1_epi32(a))
#if ARCH_AVX2
#define s32x8_eq(a,b) i2f32(_mm256_cmpeq_epi32(f2i32(a),f2i32(b)))
#define s32x8_ne(a,b) vec32_not(s32x8_eq(b,a))
#define s32x8_gt(a,b) i2f32(_mm256_cmpgt_epi32(f2i32(a),f2i32(b)))
#define s32x8_lt(a,b) s32x8_gt(b,a)
#define s32x8_le(a,b) vec32_not(s32x8_gt(a,b))
#define s32x8_ge(a,b) vec32_not(s32x8_lt(a,b))
#define s32x8_add(a,b) i2f32(_mm256_add_epi32(f2i32(a),f2i32(b)))
#define s32x8_sub(a,b) i2f32(_mm256_sub_epi32(f2i32(a),f2i32(b)))
#define s32x8_mul(a,b) i2f32(_mm256_mullo_epi32(f2i32(a),f2i32(b)))
#define s32x8_div(a,b) i2f32(_mm256_div_epi32(f2i32(a),f2i32(b)))
#define s32x8_floor(a,b) s32x8_mul(select32x8(s32x8_lt(a,s32x8_set1(0)),s32x8_sub(s32x8_div(s32x8_add(a, s32x8_set1(1)), b), s32x8_set1(1)),s32x8_div(a, b)),b)
#define s32x8_neg(a) s32x8_sub(s32x8_set1(0),a)
#endif

#define u32x8_set(a,b,c,d,e,f,g,h) i2f32(_mm256_setr_epi32(a,b,c,d,e,f,g,h))
#define u32x8_set1(a) i2f32(_mm256_set1_epi32(a))
#if ARCH_AVX2
#define u32x8_eq(a,b) s32x8_eq(a,b)
#define u32x8_ne(a,b) s32x8_ne(a,b)
#define u32x8_gt(a,b) i2f32(_mm256_cmpgt_epi32(_mm256_xor_si256(f2i32(a),_mm256_set1_epi32(0x80000000)),_mm256_xor_si256(f2i32(b),_mm256_set1_epi32(0x80000000))))
#define u32x8_lt(a,b) u32x8_gt(b,a)
#define u32x8_le(a,b) vec32_not(u32x8_gt(a,b))
#define u32x8_ge(a,b) vec32_not(u32x8_lt(a,b))
#define u32x8_add(a,b) i2f32(_mm256_add_epi32(f2i32(a),f2i32(b)))
#define u32x8_sub(a,b) i2f32(_mm256_sub_epi32(f2i32(a),f2i32(b)))
#define u32x8_mul(a,b) i2f32(_mm256_mullo_epi32(f2i32(a),f2i32(b)))
#define u32x8_div(a,b) i2f32(_mm256_div_epu32(f2i32(a),f2i32(b)))
#define u32x8_shift_right(a,b) i2f32(_mm256_srli_epi32(f2i32(a),b))
#endif

#define f64x4_lt(a,b) d2f32(_mm256_cmp_pd(f2d32(a),f2d32(b),_CMP_LT_OQ))
#define f64x4_gt(a,b) d2f32(_mm256_cmp_pd(f2d32(a),f2d32(b),_CMP_GT_OQ))
#define f64x4_le(a,b) d2f32(_mm256_cmp_pd(f2d32(a),f2d32(b),_CMP_LE_OQ))
#define f64x4_ge(a,b) d2f32(_mm256_cmp_pd(f2d32(a),f2d32(b),_CMP_GE_OQ))
#define f64x4_eq(a,b) d2f32(_mm256_cmp_pd(f2d32(a),f2d32(b),_CMP_EQ_OQ))
#define f64x4_ne(a,b) d2f32(_mm256_cmp_pd(f2d32(a),f2d32(b),_CMP_NEQ_OQ))

#endif

#if ARCH_AVX2
#define f32x8_to_s32x8(a) i2f32(_mm256_cvttps_epi32(a))
#define f32x8_to_u32x8(a) i2f32(_mm256_cvttps_epi32(a))
#define s32x8_to_f32x8(a) _mm256_cvtepi32_ps(f2i32(a))
#define u32x8_to_f32x8(a) _mm256_cvtepi32_ps(f2i32(a))
#define s32x8_to_u32x8(a) a
#define u32x8_to_s32x8(a) a
#endif

#define b32x8_get_mask(a) _mm256_movemask_ps(a)
#define b32x8_any_true(a) (b32x8_get_mask(a) != 0)
#define b32x8_all_true(a) (b32x8_get_mask(a) == 255)

}
