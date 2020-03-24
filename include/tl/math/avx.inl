#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con)
union v4x2 {
	struct {
		v4 i, j;
	};
#if ARCH_AVX
	__m256 m;
	v4x2 operator-() const { return V4x2(_mm256_sub_ps(_mm256_set1_ps(0), m)); }
	v4x2 operator+(v4x2 b) const { return V4x2(_mm256_add_ps(m, b.m)); }
	v4x2 operator-(v4x2 b) const { return V4x2(_mm256_sub_ps(m, b.m)); }
	v4x2 operator*(v4x2 b) const { return V4x2(_mm256_mul_ps(m, b.m)); }
	v4x2 operator/(v4x2 b) const { return V4x2(_mm256_div_ps(m, b.m)); }
#else
	v4x2 operator-() const { return V4x2(-i, -j); }
	v4x2 operator+(v4x2 b) const { return V4x2(i + b.i, j + b.j); }
	v4x2 operator-(v4x2 b) const { return V4x2(i - b.i, j - b.j); }
	v4x2 operator*(v4x2 b) const { return V4x2(i * b.i, j * b.j); }
	v4x2 operator/(v4x2 b) const { return V4x2(i / b.i, j / b.j); }
#endif
	bool operator==(v4x2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4x2, f32, V4x2);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v4sx2 {
	struct {
		v4s i, j;
	};
#if ARCH_AVX
	__m256i m;
	v4sx2 operator-() const { return V4sx2(_mm256_sub_epi32(_mm256_set1_epi32(0), m)); }
	v4sx2 operator+(v4sx2 b) const { return V4sx2(_mm256_add_epi32(m, b.m)); }
	v4sx2 operator-(v4sx2 b) const { return V4sx2(_mm256_sub_epi32(m, b.m)); }
	v4sx2 operator*(v4sx2 b) const { return V4sx2(_mm256_mullo_epi32(m, b.m)); }
	v4sx2 operator^(v4sx2 b) const { return V4sx2(_mm256_xor_si256(m, b.m)); }
	v4sx2 operator|(v4sx2 b) const { return V4sx2(_mm256_or_si256(m, b.m)); }
	v4sx2 operator&(v4sx2 b) const { return V4sx2(_mm256_and_si256(m, b.m)); }
#if COMPILER_GCC
	v4sx2 operator/(v4sx2 b) const { return V4sx2(i / b.i, j / b.j); }
	v4sx2 operator%(v4sx2 b) const { return V4sx2(i % b.i, j % b.j); }
#else
	v4sx2 operator/(v4sx2 b) const { return V4sx2(_mm256_div_epi32(m, b.m)); }
	v4sx2 operator%(v4sx2 b) const { return V4sx2(_mm256_rem_epi32(m, b.m)); }
#endif
	v4sx2 operator<<(v4sx2 b) const { return V4sx2(i << b.i, j << b.j); }
	v4sx2 operator>>(v4sx2 b) const { return V4sx2(i >> b.i, j >> b.j); }
	v4sx2 operator<<(s32 b) const { return V4sx2(_mm256_slli_epi32(m, b)); }
	v4sx2 operator>>(s32 b) const { return V4sx2(_mm256_srai_epi32(m, b)); }
#else
	v4sx2 operator-() const { return V4sx2(-i, -j); }
#define OP(op) \
	v4sx2 operator op(v4sx2 b) const { return V4sx2(i op b.i, j op b.j); }
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	v4sx2 operator<<(s32 b) const { return V4sx2(i << b, j << b); }
	v4sx2 operator>>(s32 b) const { return V4sx2(i << b, j << b); }
#undef OP
#endif
	bool operator==(v4sx2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4sx2, s32, V4sx2);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v4ux2 {
	struct {
		v4u i, j;
	};
#if ARCH_AVX
	__m256i m;
	v4ux2 operator+(v4ux2 b) const { return V4ux2(_mm256_add_epi32(m, b.m)); }
	v4ux2 operator-(v4ux2 b) const { return V4ux2(_mm256_sub_epi32(m, b.m)); }
	v4ux2 operator*(v4ux2 b) const { return V4ux2(i * b.i, j * b.j); }
	v4ux2 operator^(v4ux2 b) const { return V4ux2(_mm256_xor_si256(m, b.m)); }
	v4ux2 operator|(v4ux2 b) const { return V4ux2(_mm256_or_si256(m, b.m)); }
	v4ux2 operator&(v4ux2 b) const { return V4ux2(_mm256_and_si256(m, b.m)); }
	v4ux2 operator<<(v4ux2 b) const { return V4ux2(i << b.i, j << b.j); }
	v4ux2 operator>>(v4ux2 b) const { return V4ux2(i >> b.i, j >> b.j); }
#if COMPILER_GCC
	v4ux2 operator/(v4ux2 b) const { return V4ux2(i / b.i, j / b.j); }
	v4ux2 operator%(v4ux2 b) const { return V4ux2(i % b.i, j % b.j); }
#else
	v4ux2 operator/(v4ux2 b) const { return V4ux2(_mm256_div_epi32(m, b.m)); }
	v4ux2 operator%(v4ux2 b) const { return V4ux2(_mm256_rem_epi32(m, b.m)); }
#endif
	v4ux2 operator<<(u32 b) const { return V4ux2(_mm256_slli_epi32(m, (s32)b)); }
	v4ux2 operator>>(u32 b) const { return V4ux2(_mm256_srli_epi32(m, (s32)b)); }
#else
#define OP(op) \
	v4ux2 operator op(v4ux2 b) const { return V4ux2(i op b.i, j op b.j); }
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	v4ux2 operator<<(u32 b) const { return V4ux2(i << b, j << b); }
	v4ux2 operator>>(u32 b) const { return V4ux2(i << b, j << b); }
#undef OP
#endif
	bool operator==(v4ux2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4ux2, u32, V4ux2);
};
#undef MEMFUNS