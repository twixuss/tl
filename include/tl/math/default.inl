#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con)
union v4x2 {
	struct {
		v4 i, j;
	};
	v4x2 operator-() const { return V4x2(-i, -j); }
	v4x2 operator+(v4x2 b) const { return V4x2(i + b.i, j + b.j); }
	v4x2 operator-(v4x2 b) const { return V4x2(i - b.i, j - b.j); }
	v4x2 operator*(v4x2 b) const { return V4x2(i * b.i, j * b.j); }
	v4x2 operator/(v4x2 b) const { return V4x2(i / b.i, j / b.j); }
	bool operator==(v4x2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4x2, f32, V4x2);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v4sx2 {
	struct {
		v4s i, j;
	};
	v4sx2 operator-() const { return V4sx2(-i, -j); }
#define OP(op) \
	v4sx2 operator op(v4sx2 b) const { return V4sx2(i op b.i, j op b.j); }
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	v4sx2 operator<<(s32 b) const { return V4sx2(i << b, j << b); }
	v4sx2 operator>>(s32 b) const { return V4sx2(i << b, j << b); }
#undef OP
	bool operator==(v4sx2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4sx2, s32, V4sx2);
};

#undef MEMFUNS
#define MEMFUNS(vec, scl, con) MEMFUNS_BASIC(vec, scl, con) MEMFUNS_INT(vec, scl, con)
union v4ux2 {
	struct {
		v4u i, j;
	};
#define OP(op) \
	v4ux2 operator op(v4ux2 b) const { return V4ux2(i op b.i, j op b.j); }
	OP(+) OP(-) OP(*) OP(/) OP(%) OP(^) OP(|) OP(&) OP(<<) OP(>>);
	v4ux2 operator<<(u32 b) const { return V4ux2(i << b, j << b); }
	v4ux2 operator>>(u32 b) const { return V4ux2(i << b, j << b); }
#undef OP
	bool operator==(v4ux2 b) const { return i == b.i && j == b.j; }
	MEMFUNS(v4ux2, u32, V4ux2);
};
#undef MEMFUNS