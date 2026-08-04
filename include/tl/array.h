#pragma once
#include "common.h"

namespace tl {

template <class T>
inline static constexpr bool is_array = false;

template <class T, umm count_>
struct Array {
	inline static constexpr umm count = count_;
	#include "_array_members_before.inc"
	T data[count];
	#include "_array_members_after.inc"
};

template <class T>
struct Array<T, 1> {
	inline static constexpr umm count = 1;
	#include "_array_members_before.inc"
	union {
		T data[count];
		T s[count]; // Compatibility with vectors
		T x;
	};
	#include "_array_members_after.inc"
};

template <class T>
struct Array<T, 2> {
	inline static constexpr umm count = 2;
	#include "_array_members_before.inc"
	union {
		T data[count];
		T s[count]; // Compatibility with vectors
		struct { T x, y; };
	};
	#include "_array_members_after.inc"
	forceinline constexpr v2 xx() const { return {x,x}; }
	forceinline constexpr v2 yx() const { return {y,x}; }
	forceinline constexpr v2 yy() const { return {y,y}; }
	forceinline constexpr v3 xxx() const { return {x,x,x}; }
	forceinline constexpr v3 xxy() const { return {x,x,y}; }
	forceinline constexpr v3 xyx() const { return {x,y,x}; }
	forceinline constexpr v3 xyy() const { return {x,y,y}; }
	forceinline constexpr v3 yxx() const { return {y,x,x}; }
	forceinline constexpr v3 yxy() const { return {y,x,y}; }
	forceinline constexpr v3 yyx() const { return {y,y,x}; }
	forceinline constexpr v3 yyy() const { return {y,y,y}; }
	forceinline constexpr v4 xxxx() const { return {x,x,x,x}; }
	forceinline constexpr v4 xxxy() const { return {x,x,x,y}; }
	forceinline constexpr v4 xxyx() const { return {x,x,y,x}; }
	forceinline constexpr v4 xxyy() const { return {x,x,y,y}; }
	forceinline constexpr v4 xyxx() const { return {x,y,x,x}; }
	forceinline constexpr v4 xyxy() const { return {x,y,x,y}; }
	forceinline constexpr v4 xyyx() const { return {x,y,y,x}; }
	forceinline constexpr v4 xyyy() const { return {x,y,y,y}; }
	forceinline constexpr v4 yxxx() const { return {y,x,x,x}; }
	forceinline constexpr v4 yxxy() const { return {y,x,x,y}; }
	forceinline constexpr v4 yxyx() const { return {y,x,y,x}; }
	forceinline constexpr v4 yxyy() const { return {y,x,y,y}; }
	forceinline constexpr v4 yyxx() const { return {y,y,x,x}; }
	forceinline constexpr v4 yyxy() const { return {y,y,x,y}; }
	forceinline constexpr v4 yyyx() const { return {y,y,y,x}; }
	forceinline constexpr v4 yyyy() const { return {y,y,y,y}; }
};

template <class T>
struct Array<T, 3> {
	inline static constexpr umm count = 3;
	#include "_array_members_before.inc"
	union {
		T data[count];
		T s[count]; // Compatibility with vectors
		struct { T x, y, z; };
		struct { v2 xy; };
		struct { T _pad0; v2 yz; };
	};
	#include "_array_members_after.inc"
	forceinline constexpr v2 xx() const { return {x,x}; }
	forceinline constexpr v2 xz() const { return {x,z}; }
	forceinline constexpr v2 yx() const { return {y,x}; }
	forceinline constexpr v2 yy() const { return {y,y}; }
	forceinline constexpr v2 zx() const { return {z,x}; }
	forceinline constexpr v2 zy() const { return {z,y}; }
	forceinline constexpr v2 zz() const { return {z,z}; }
	forceinline constexpr v3 xxx() const { return {x,x,x}; }
	forceinline constexpr v3 xxy() const { return {x,x,y}; }
	forceinline constexpr v3 xxz() const { return {x,x,z}; }
	forceinline constexpr v3 xyx() const { return {x,y,x}; }
	forceinline constexpr v3 xyy() const { return {x,y,y}; }
	forceinline constexpr v3 xzx() const { return {x,z,x}; }
	forceinline constexpr v3 xzy() const { return {x,z,y}; }
	forceinline constexpr v3 xzz() const { return {x,z,z}; }
	forceinline constexpr v3 yxx() const { return {y,x,x}; }
	forceinline constexpr v3 yxy() const { return {y,x,y}; }
	forceinline constexpr v3 yxz() const { return {y,x,z}; }
	forceinline constexpr v3 yyx() const { return {y,y,x}; }
	forceinline constexpr v3 yyy() const { return {y,y,y}; }
	forceinline constexpr v3 yyz() const { return {y,y,z}; }
	forceinline constexpr v3 yzx() const { return {y,z,x}; }
	forceinline constexpr v3 yzy() const { return {y,z,y}; }
	forceinline constexpr v3 yzz() const { return {y,z,z}; }
	forceinline constexpr v3 zxx() const { return {z,x,x}; }
	forceinline constexpr v3 zxy() const { return {z,x,y}; }
	forceinline constexpr v3 zxz() const { return {z,x,z}; }
	forceinline constexpr v3 zyx() const { return {z,y,x}; }
	forceinline constexpr v3 zyy() const { return {z,y,y}; }
	forceinline constexpr v3 zyz() const { return {z,y,z}; }
	forceinline constexpr v3 zzx() const { return {z,z,x}; }
	forceinline constexpr v3 zzy() const { return {z,z,y}; }
	forceinline constexpr v3 zzz() const { return {z,z,z}; }
	forceinline constexpr v4 xxxx() const { return {x,x,x,x}; }
	forceinline constexpr v4 xxxy() const { return {x,x,x,y}; }
	forceinline constexpr v4 xxxz() const { return {x,x,x,z}; }
	forceinline constexpr v4 xxyx() const { return {x,x,y,x}; }
	forceinline constexpr v4 xxyy() const { return {x,x,y,y}; }
	forceinline constexpr v4 xxyz() const { return {x,x,y,z}; }
	forceinline constexpr v4 xxzx() const { return {x,x,z,x}; }
	forceinline constexpr v4 xxzy() const { return {x,x,z,y}; }
	forceinline constexpr v4 xxzz() const { return {x,x,z,z}; }
	forceinline constexpr v4 xyxx() const { return {x,y,x,x}; }
	forceinline constexpr v4 xyxy() const { return {x,y,x,y}; }
	forceinline constexpr v4 xyxz() const { return {x,y,x,z}; }
	forceinline constexpr v4 xyyx() const { return {x,y,y,x}; }
	forceinline constexpr v4 xyyy() const { return {x,y,y,y}; }
	forceinline constexpr v4 xyyz() const { return {x,y,y,z}; }
	forceinline constexpr v4 xyzx() const { return {x,y,z,x}; }
	forceinline constexpr v4 xyzy() const { return {x,y,z,y}; }
	forceinline constexpr v4 xyzz() const { return {x,y,z,z}; }
	forceinline constexpr v4 xzxx() const { return {x,z,x,x}; }
	forceinline constexpr v4 xzxy() const { return {x,z,x,y}; }
	forceinline constexpr v4 xzxz() const { return {x,z,x,z}; }
	forceinline constexpr v4 xzyx() const { return {x,z,y,x}; }
	forceinline constexpr v4 xzyy() const { return {x,z,y,y}; }
	forceinline constexpr v4 xzyz() const { return {x,z,y,z}; }
	forceinline constexpr v4 xzzx() const { return {x,z,z,x}; }
	forceinline constexpr v4 xzzy() const { return {x,z,z,y}; }
	forceinline constexpr v4 xzzz() const { return {x,z,z,z}; }
	forceinline constexpr v4 yxxx() const { return {y,x,x,x}; }
	forceinline constexpr v4 yxxy() const { return {y,x,x,y}; }
	forceinline constexpr v4 yxxz() const { return {y,x,x,z}; }
	forceinline constexpr v4 yxyx() const { return {y,x,y,x}; }
	forceinline constexpr v4 yxyy() const { return {y,x,y,y}; }
	forceinline constexpr v4 yxyz() const { return {y,x,y,z}; }
	forceinline constexpr v4 yxzx() const { return {y,x,z,x}; }
	forceinline constexpr v4 yxzy() const { return {y,x,z,y}; }
	forceinline constexpr v4 yxzz() const { return {y,x,z,z}; }
	forceinline constexpr v4 yyxx() const { return {y,y,x,x}; }
	forceinline constexpr v4 yyxy() const { return {y,y,x,y}; }
	forceinline constexpr v4 yyxz() const { return {y,y,x,z}; }
	forceinline constexpr v4 yyyx() const { return {y,y,y,x}; }
	forceinline constexpr v4 yyyy() const { return {y,y,y,y}; }
	forceinline constexpr v4 yyyz() const { return {y,y,y,z}; }
	forceinline constexpr v4 yyzx() const { return {y,y,z,x}; }
	forceinline constexpr v4 yyzy() const { return {y,y,z,y}; }
	forceinline constexpr v4 yyzz() const { return {y,y,z,z}; }
	forceinline constexpr v4 yzxx() const { return {y,z,x,x}; }
	forceinline constexpr v4 yzxy() const { return {y,z,x,y}; }
	forceinline constexpr v4 yzxz() const { return {y,z,x,z}; }
	forceinline constexpr v4 yzyx() const { return {y,z,y,x}; }
	forceinline constexpr v4 yzyy() const { return {y,z,y,y}; }
	forceinline constexpr v4 yzyz() const { return {y,z,y,z}; }
	forceinline constexpr v4 yzzx() const { return {y,z,z,x}; }
	forceinline constexpr v4 yzzy() const { return {y,z,z,y}; }
	forceinline constexpr v4 yzzz() const { return {y,z,z,z}; }
	forceinline constexpr v4 zxxx() const { return {z,x,x,x}; }
	forceinline constexpr v4 zxxy() const { return {z,x,x,y}; }
	forceinline constexpr v4 zxxz() const { return {z,x,x,z}; }
	forceinline constexpr v4 zxyx() const { return {z,x,y,x}; }
	forceinline constexpr v4 zxyy() const { return {z,x,y,y}; }
	forceinline constexpr v4 zxyz() const { return {z,x,y,z}; }
	forceinline constexpr v4 zxzx() const { return {z,x,z,x}; }
	forceinline constexpr v4 zxzy() const { return {z,x,z,y}; }
	forceinline constexpr v4 zxzz() const { return {z,x,z,z}; }
	forceinline constexpr v4 zyxx() const { return {z,y,x,x}; }
	forceinline constexpr v4 zyxy() const { return {z,y,x,y}; }
	forceinline constexpr v4 zyxz() const { return {z,y,x,z}; }
	forceinline constexpr v4 zyyx() const { return {z,y,y,x}; }
	forceinline constexpr v4 zyyy() const { return {z,y,y,y}; }
	forceinline constexpr v4 zyyz() const { return {z,y,y,z}; }
	forceinline constexpr v4 zyzx() const { return {z,y,z,x}; }
	forceinline constexpr v4 zyzy() const { return {z,y,z,y}; }
	forceinline constexpr v4 zyzz() const { return {z,y,z,z}; }
	forceinline constexpr v4 zzxx() const { return {z,z,x,x}; }
	forceinline constexpr v4 zzxy() const { return {z,z,x,y}; }
	forceinline constexpr v4 zzxz() const { return {z,z,x,z}; }
	forceinline constexpr v4 zzyx() const { return {z,z,y,x}; }
	forceinline constexpr v4 zzyy() const { return {z,z,y,y}; }
	forceinline constexpr v4 zzyz() const { return {z,z,y,z}; }
	forceinline constexpr v4 zzzx() const { return {z,z,z,x}; }
	forceinline constexpr v4 zzzy() const { return {z,z,z,y}; }
	forceinline constexpr v4 zzzz() const { return {z,z,z,z}; }
};

template <class T>
struct Array<T, 4> {
	inline static constexpr umm count = 4;
	#include "_array_members_before.inc"
	union {
		T data[count];
		T s[count]; // Compatibility with vectors
		struct { T x, y, z, w; };
		struct { v2 xy, zw; };
		struct { T _pad0; v2 yz; };
		struct { v3 xyz; };
		struct { T _pad1; v3 yzw; };
	};
	#include "_array_members_after.inc"
	forceinline constexpr v2 xx() const { return {x,x}; }
	forceinline constexpr v2 xz() const { return {x,z}; }
	forceinline constexpr v2 xw() const { return {x,w}; }
	forceinline constexpr v2 yx() const { return {y,x}; }
	forceinline constexpr v2 yy() const { return {y,y}; }
	forceinline constexpr v2 yw() const { return {y,w}; }
	forceinline constexpr v2 zx() const { return {z,x}; }
	forceinline constexpr v2 zy() const { return {z,y}; }
	forceinline constexpr v2 zz() const { return {z,z}; }
	forceinline constexpr v2 wx() const { return {w,x}; }
	forceinline constexpr v2 wy() const { return {w,y}; }
	forceinline constexpr v2 wz() const { return {w,z}; }
	forceinline constexpr v2 ww() const { return {w,w}; }
	forceinline constexpr v3 xxx() const { return {x,x,x}; }
	forceinline constexpr v3 xxy() const { return {x,x,y}; }
	forceinline constexpr v3 xxz() const { return {x,x,z}; }
	forceinline constexpr v3 xxw() const { return {x,x,w}; }
	forceinline constexpr v3 xyx() const { return {x,y,x}; }
	forceinline constexpr v3 xyy() const { return {x,y,y}; }
	forceinline constexpr v3 xyw() const { return {x,y,w}; }
	forceinline constexpr v3 xzx() const { return {x,z,x}; }
	forceinline constexpr v3 xzy() const { return {x,z,y}; }
	forceinline constexpr v3 xzz() const { return {x,z,z}; }
	forceinline constexpr v3 xzw() const { return {x,z,w}; }
	forceinline constexpr v3 xwx() const { return {x,w,x}; }
	forceinline constexpr v3 xwy() const { return {x,w,y}; }
	forceinline constexpr v3 xwz() const { return {x,w,z}; }
	forceinline constexpr v3 xww() const { return {x,w,w}; }
	forceinline constexpr v3 yxx() const { return {y,x,x}; }
	forceinline constexpr v3 yxy() const { return {y,x,y}; }
	forceinline constexpr v3 yxz() const { return {y,x,z}; }
	forceinline constexpr v3 yxw() const { return {y,x,w}; }
	forceinline constexpr v3 yyx() const { return {y,y,x}; }
	forceinline constexpr v3 yyy() const { return {y,y,y}; }
	forceinline constexpr v3 yyz() const { return {y,y,z}; }
	forceinline constexpr v3 yyw() const { return {y,y,w}; }
	forceinline constexpr v3 yzx() const { return {y,z,x}; }
	forceinline constexpr v3 yzy() const { return {y,z,y}; }
	forceinline constexpr v3 yzz() const { return {y,z,z}; }
	forceinline constexpr v3 ywx() const { return {y,w,x}; }
	forceinline constexpr v3 ywy() const { return {y,w,y}; }
	forceinline constexpr v3 ywz() const { return {y,w,z}; }
	forceinline constexpr v3 yww() const { return {y,w,w}; }
	forceinline constexpr v3 zxx() const { return {z,x,x}; }
	forceinline constexpr v3 zxy() const { return {z,x,y}; }
	forceinline constexpr v3 zxz() const { return {z,x,z}; }
	forceinline constexpr v3 zxw() const { return {z,x,w}; }
	forceinline constexpr v3 zyx() const { return {z,y,x}; }
	forceinline constexpr v3 zyy() const { return {z,y,y}; }
	forceinline constexpr v3 zyz() const { return {z,y,z}; }
	forceinline constexpr v3 zyw() const { return {z,y,w}; }
	forceinline constexpr v3 zzx() const { return {z,z,x}; }
	forceinline constexpr v3 zzy() const { return {z,z,y}; }
	forceinline constexpr v3 zzz() const { return {z,z,z}; }
	forceinline constexpr v3 zzw() const { return {z,z,w}; }
	forceinline constexpr v3 zwx() const { return {z,w,x}; }
	forceinline constexpr v3 zwy() const { return {z,w,y}; }
	forceinline constexpr v3 zwz() const { return {z,w,z}; }
	forceinline constexpr v3 zww() const { return {z,w,w}; }
	forceinline constexpr v3 wxx() const { return {w,x,x}; }
	forceinline constexpr v3 wxy() const { return {w,x,y}; }
	forceinline constexpr v3 wxz() const { return {w,x,z}; }
	forceinline constexpr v3 wxw() const { return {w,x,w}; }
	forceinline constexpr v3 wyx() const { return {w,y,x}; }
	forceinline constexpr v3 wyy() const { return {w,y,y}; }
	forceinline constexpr v3 wyz() const { return {w,y,z}; }
	forceinline constexpr v3 wyw() const { return {w,y,w}; }
	forceinline constexpr v3 wzx() const { return {w,z,x}; }
	forceinline constexpr v3 wzy() const { return {w,z,y}; }
	forceinline constexpr v3 wzz() const { return {w,z,z}; }
	forceinline constexpr v3 wzw() const { return {w,z,w}; }
	forceinline constexpr v3 wwx() const { return {w,w,x}; }
	forceinline constexpr v3 wwy() const { return {w,w,y}; }
	forceinline constexpr v3 wwz() const { return {w,w,z}; }
	forceinline constexpr v3 www() const { return {w,w,w}; }
	forceinline constexpr v4 xxxx() const { return {x,x,x,x}; }
	forceinline constexpr v4 xxxy() const { return {x,x,x,y}; }
	forceinline constexpr v4 xxxz() const { return {x,x,x,z}; }
	forceinline constexpr v4 xxxw() const { return {x,x,x,w}; }
	forceinline constexpr v4 xxyx() const { return {x,x,y,x}; }
	forceinline constexpr v4 xxyy() const { return {x,x,y,y}; }
	forceinline constexpr v4 xxyz() const { return {x,x,y,z}; }
	forceinline constexpr v4 xxyw() const { return {x,x,y,w}; }
	forceinline constexpr v4 xxzx() const { return {x,x,z,x}; }
	forceinline constexpr v4 xxzy() const { return {x,x,z,y}; }
	forceinline constexpr v4 xxzz() const { return {x,x,z,z}; }
	forceinline constexpr v4 xxzw() const { return {x,x,z,w}; }
	forceinline constexpr v4 xxwx() const { return {x,x,w,x}; }
	forceinline constexpr v4 xxwy() const { return {x,x,w,y}; }
	forceinline constexpr v4 xxwz() const { return {x,x,w,z}; }
	forceinline constexpr v4 xxww() const { return {x,x,w,w}; }
	forceinline constexpr v4 xyxx() const { return {x,y,x,x}; }
	forceinline constexpr v4 xyxy() const { return {x,y,x,y}; }
	forceinline constexpr v4 xyxz() const { return {x,y,x,z}; }
	forceinline constexpr v4 xyxw() const { return {x,y,x,w}; }
	forceinline constexpr v4 xyyx() const { return {x,y,y,x}; }
	forceinline constexpr v4 xyyy() const { return {x,y,y,y}; }
	forceinline constexpr v4 xyyz() const { return {x,y,y,z}; }
	forceinline constexpr v4 xyyw() const { return {x,y,y,w}; }
	forceinline constexpr v4 xyzx() const { return {x,y,z,x}; }
	forceinline constexpr v4 xyzy() const { return {x,y,z,y}; }
	forceinline constexpr v4 xyzz() const { return {x,y,z,z}; }
	forceinline constexpr v4 xywx() const { return {x,y,w,x}; }
	forceinline constexpr v4 xywy() const { return {x,y,w,y}; }
	forceinline constexpr v4 xywz() const { return {x,y,w,z}; }
	forceinline constexpr v4 xyww() const { return {x,y,w,w}; }
	forceinline constexpr v4 xzxx() const { return {x,z,x,x}; }
	forceinline constexpr v4 xzxy() const { return {x,z,x,y}; }
	forceinline constexpr v4 xzxz() const { return {x,z,x,z}; }
	forceinline constexpr v4 xzxw() const { return {x,z,x,w}; }
	forceinline constexpr v4 xzyx() const { return {x,z,y,x}; }
	forceinline constexpr v4 xzyy() const { return {x,z,y,y}; }
	forceinline constexpr v4 xzyz() const { return {x,z,y,z}; }
	forceinline constexpr v4 xzyw() const { return {x,z,y,w}; }
	forceinline constexpr v4 xzzx() const { return {x,z,z,x}; }
	forceinline constexpr v4 xzzy() const { return {x,z,z,y}; }
	forceinline constexpr v4 xzzz() const { return {x,z,z,z}; }
	forceinline constexpr v4 xzzw() const { return {x,z,z,w}; }
	forceinline constexpr v4 xzwx() const { return {x,z,w,x}; }
	forceinline constexpr v4 xzwy() const { return {x,z,w,y}; }
	forceinline constexpr v4 xzwz() const { return {x,z,w,z}; }
	forceinline constexpr v4 xzww() const { return {x,z,w,w}; }
	forceinline constexpr v4 xwxx() const { return {x,w,x,x}; }
	forceinline constexpr v4 xwxy() const { return {x,w,x,y}; }
	forceinline constexpr v4 xwxz() const { return {x,w,x,z}; }
	forceinline constexpr v4 xwxw() const { return {x,w,x,w}; }
	forceinline constexpr v4 xwyx() const { return {x,w,y,x}; }
	forceinline constexpr v4 xwyy() const { return {x,w,y,y}; }
	forceinline constexpr v4 xwyz() const { return {x,w,y,z}; }
	forceinline constexpr v4 xwyw() const { return {x,w,y,w}; }
	forceinline constexpr v4 xwzx() const { return {x,w,z,x}; }
	forceinline constexpr v4 xwzy() const { return {x,w,z,y}; }
	forceinline constexpr v4 xwzz() const { return {x,w,z,z}; }
	forceinline constexpr v4 xwzw() const { return {x,w,z,w}; }
	forceinline constexpr v4 xwwx() const { return {x,w,w,x}; }
	forceinline constexpr v4 xwwy() const { return {x,w,w,y}; }
	forceinline constexpr v4 xwwz() const { return {x,w,w,z}; }
	forceinline constexpr v4 xwww() const { return {x,w,w,w}; }
	forceinline constexpr v4 yxxx() const { return {y,x,x,x}; }
	forceinline constexpr v4 yxxy() const { return {y,x,x,y}; }
	forceinline constexpr v4 yxxz() const { return {y,x,x,z}; }
	forceinline constexpr v4 yxxw() const { return {y,x,x,w}; }
	forceinline constexpr v4 yxyx() const { return {y,x,y,x}; }
	forceinline constexpr v4 yxyy() const { return {y,x,y,y}; }
	forceinline constexpr v4 yxyz() const { return {y,x,y,z}; }
	forceinline constexpr v4 yxyw() const { return {y,x,y,w}; }
	forceinline constexpr v4 yxzx() const { return {y,x,z,x}; }
	forceinline constexpr v4 yxzy() const { return {y,x,z,y}; }
	forceinline constexpr v4 yxzz() const { return {y,x,z,z}; }
	forceinline constexpr v4 yxzw() const { return {y,x,z,w}; }
	forceinline constexpr v4 yxwx() const { return {y,x,w,x}; }
	forceinline constexpr v4 yxwy() const { return {y,x,w,y}; }
	forceinline constexpr v4 yxwz() const { return {y,x,w,z}; }
	forceinline constexpr v4 yxww() const { return {y,x,w,w}; }
	forceinline constexpr v4 yyxx() const { return {y,y,x,x}; }
	forceinline constexpr v4 yyxy() const { return {y,y,x,y}; }
	forceinline constexpr v4 yyxz() const { return {y,y,x,z}; }
	forceinline constexpr v4 yyxw() const { return {y,y,x,w}; }
	forceinline constexpr v4 yyyx() const { return {y,y,y,x}; }
	forceinline constexpr v4 yyyy() const { return {y,y,y,y}; }
	forceinline constexpr v4 yyyz() const { return {y,y,y,z}; }
	forceinline constexpr v4 yyyw() const { return {y,y,y,w}; }
	forceinline constexpr v4 yyzx() const { return {y,y,z,x}; }
	forceinline constexpr v4 yyzy() const { return {y,y,z,y}; }
	forceinline constexpr v4 yyzz() const { return {y,y,z,z}; }
	forceinline constexpr v4 yyzw() const { return {y,y,z,w}; }
	forceinline constexpr v4 yywx() const { return {y,y,w,x}; }
	forceinline constexpr v4 yywy() const { return {y,y,w,y}; }
	forceinline constexpr v4 yywz() const { return {y,y,w,z}; }
	forceinline constexpr v4 yyww() const { return {y,y,w,w}; }
	forceinline constexpr v4 yzxx() const { return {y,z,x,x}; }
	forceinline constexpr v4 yzxy() const { return {y,z,x,y}; }
	forceinline constexpr v4 yzxz() const { return {y,z,x,z}; }
	forceinline constexpr v4 yzxw() const { return {y,z,x,w}; }
	forceinline constexpr v4 yzyx() const { return {y,z,y,x}; }
	forceinline constexpr v4 yzyy() const { return {y,z,y,y}; }
	forceinline constexpr v4 yzyz() const { return {y,z,y,z}; }
	forceinline constexpr v4 yzyw() const { return {y,z,y,w}; }
	forceinline constexpr v4 yzzx() const { return {y,z,z,x}; }
	forceinline constexpr v4 yzzy() const { return {y,z,z,y}; }
	forceinline constexpr v4 yzzz() const { return {y,z,z,z}; }
	forceinline constexpr v4 yzzw() const { return {y,z,z,w}; }
	forceinline constexpr v4 yzwx() const { return {y,z,w,x}; }
	forceinline constexpr v4 yzwy() const { return {y,z,w,y}; }
	forceinline constexpr v4 yzwz() const { return {y,z,w,z}; }
	forceinline constexpr v4 yzww() const { return {y,z,w,w}; }
	forceinline constexpr v4 ywxx() const { return {y,w,x,x}; }
	forceinline constexpr v4 ywxy() const { return {y,w,x,y}; }
	forceinline constexpr v4 ywxz() const { return {y,w,x,z}; }
	forceinline constexpr v4 ywxw() const { return {y,w,x,w}; }
	forceinline constexpr v4 ywyx() const { return {y,w,y,x}; }
	forceinline constexpr v4 ywyy() const { return {y,w,y,y}; }
	forceinline constexpr v4 ywyz() const { return {y,w,y,z}; }
	forceinline constexpr v4 ywyw() const { return {y,w,y,w}; }
	forceinline constexpr v4 ywzx() const { return {y,w,z,x}; }
	forceinline constexpr v4 ywzy() const { return {y,w,z,y}; }
	forceinline constexpr v4 ywzz() const { return {y,w,z,z}; }
	forceinline constexpr v4 ywzw() const { return {y,w,z,w}; }
	forceinline constexpr v4 ywwx() const { return {y,w,w,x}; }
	forceinline constexpr v4 ywwy() const { return {y,w,w,y}; }
	forceinline constexpr v4 ywwz() const { return {y,w,w,z}; }
	forceinline constexpr v4 ywww() const { return {y,w,w,w}; }
	forceinline constexpr v4 zxxx() const { return {z,x,x,x}; }
	forceinline constexpr v4 zxxy() const { return {z,x,x,y}; }
	forceinline constexpr v4 zxxz() const { return {z,x,x,z}; }
	forceinline constexpr v4 zxxw() const { return {z,x,x,w}; }
	forceinline constexpr v4 zxyx() const { return {z,x,y,x}; }
	forceinline constexpr v4 zxyy() const { return {z,x,y,y}; }
	forceinline constexpr v4 zxyz() const { return {z,x,y,z}; }
	forceinline constexpr v4 zxyw() const { return {z,x,y,w}; }
	forceinline constexpr v4 zxzx() const { return {z,x,z,x}; }
	forceinline constexpr v4 zxzy() const { return {z,x,z,y}; }
	forceinline constexpr v4 zxzz() const { return {z,x,z,z}; }
	forceinline constexpr v4 zxzw() const { return {z,x,z,w}; }
	forceinline constexpr v4 zxwx() const { return {z,x,w,x}; }
	forceinline constexpr v4 zxwy() const { return {z,x,w,y}; }
	forceinline constexpr v4 zxwz() const { return {z,x,w,z}; }
	forceinline constexpr v4 zxww() const { return {z,x,w,w}; }
	forceinline constexpr v4 zyxx() const { return {z,y,x,x}; }
	forceinline constexpr v4 zyxy() const { return {z,y,x,y}; }
	forceinline constexpr v4 zyxz() const { return {z,y,x,z}; }
	forceinline constexpr v4 zyxw() const { return {z,y,x,w}; }
	forceinline constexpr v4 zyyx() const { return {z,y,y,x}; }
	forceinline constexpr v4 zyyy() const { return {z,y,y,y}; }
	forceinline constexpr v4 zyyz() const { return {z,y,y,z}; }
	forceinline constexpr v4 zyyw() const { return {z,y,y,w}; }
	forceinline constexpr v4 zyzx() const { return {z,y,z,x}; }
	forceinline constexpr v4 zyzy() const { return {z,y,z,y}; }
	forceinline constexpr v4 zyzz() const { return {z,y,z,z}; }
	forceinline constexpr v4 zyzw() const { return {z,y,z,w}; }
	forceinline constexpr v4 zywx() const { return {z,y,w,x}; }
	forceinline constexpr v4 zywy() const { return {z,y,w,y}; }
	forceinline constexpr v4 zywz() const { return {z,y,w,z}; }
	forceinline constexpr v4 zyww() const { return {z,y,w,w}; }
	forceinline constexpr v4 zzxx() const { return {z,z,x,x}; }
	forceinline constexpr v4 zzxy() const { return {z,z,x,y}; }
	forceinline constexpr v4 zzxz() const { return {z,z,x,z}; }
	forceinline constexpr v4 zzxw() const { return {z,z,x,w}; }
	forceinline constexpr v4 zzyx() const { return {z,z,y,x}; }
	forceinline constexpr v4 zzyy() const { return {z,z,y,y}; }
	forceinline constexpr v4 zzyz() const { return {z,z,y,z}; }
	forceinline constexpr v4 zzyw() const { return {z,z,y,w}; }
	forceinline constexpr v4 zzzx() const { return {z,z,z,x}; }
	forceinline constexpr v4 zzzy() const { return {z,z,z,y}; }
	forceinline constexpr v4 zzzz() const { return {z,z,z,z}; }
	forceinline constexpr v4 zzzw() const { return {z,z,z,w}; }
	forceinline constexpr v4 zzwx() const { return {z,z,w,x}; }
	forceinline constexpr v4 zzwy() const { return {z,z,w,y}; }
	forceinline constexpr v4 zzwz() const { return {z,z,w,z}; }
	forceinline constexpr v4 zzww() const { return {z,z,w,w}; }
	forceinline constexpr v4 zwxx() const { return {z,w,x,x}; }
	forceinline constexpr v4 zwxy() const { return {z,w,x,y}; }
	forceinline constexpr v4 zwxz() const { return {z,w,x,z}; }
	forceinline constexpr v4 zwxw() const { return {z,w,x,w}; }
	forceinline constexpr v4 zwyx() const { return {z,w,y,x}; }
	forceinline constexpr v4 zwyy() const { return {z,w,y,y}; }
	forceinline constexpr v4 zwyz() const { return {z,w,y,z}; }
	forceinline constexpr v4 zwyw() const { return {z,w,y,w}; }
	forceinline constexpr v4 zwzx() const { return {z,w,z,x}; }
	forceinline constexpr v4 zwzy() const { return {z,w,z,y}; }
	forceinline constexpr v4 zwzz() const { return {z,w,z,z}; }
	forceinline constexpr v4 zwzw() const { return {z,w,z,w}; }
	forceinline constexpr v4 zwwx() const { return {z,w,w,x}; }
	forceinline constexpr v4 zwwy() const { return {z,w,w,y}; }
	forceinline constexpr v4 zwwz() const { return {z,w,w,z}; }
	forceinline constexpr v4 zwww() const { return {z,w,w,w}; }
	forceinline constexpr v4 wxxx() const { return {w,x,x,x}; }
	forceinline constexpr v4 wxxy() const { return {w,x,x,y}; }
	forceinline constexpr v4 wxxz() const { return {w,x,x,z}; }
	forceinline constexpr v4 wxxw() const { return {w,x,x,w}; }
	forceinline constexpr v4 wxyx() const { return {w,x,y,x}; }
	forceinline constexpr v4 wxyy() const { return {w,x,y,y}; }
	forceinline constexpr v4 wxyz() const { return {w,x,y,z}; }
	forceinline constexpr v4 wxyw() const { return {w,x,y,w}; }
	forceinline constexpr v4 wxzx() const { return {w,x,z,x}; }
	forceinline constexpr v4 wxzy() const { return {w,x,z,y}; }
	forceinline constexpr v4 wxzz() const { return {w,x,z,z}; }
	forceinline constexpr v4 wxzw() const { return {w,x,z,w}; }
	forceinline constexpr v4 wxwx() const { return {w,x,w,x}; }
	forceinline constexpr v4 wxwy() const { return {w,x,w,y}; }
	forceinline constexpr v4 wxwz() const { return {w,x,w,z}; }
	forceinline constexpr v4 wxww() const { return {w,x,w,w}; }
	forceinline constexpr v4 wyxx() const { return {w,y,x,x}; }
	forceinline constexpr v4 wyxy() const { return {w,y,x,y}; }
	forceinline constexpr v4 wyxz() const { return {w,y,x,z}; }
	forceinline constexpr v4 wyxw() const { return {w,y,x,w}; }
	forceinline constexpr v4 wyyx() const { return {w,y,y,x}; }
	forceinline constexpr v4 wyyy() const { return {w,y,y,y}; }
	forceinline constexpr v4 wyyz() const { return {w,y,y,z}; }
	forceinline constexpr v4 wyyw() const { return {w,y,y,w}; }
	forceinline constexpr v4 wyzx() const { return {w,y,z,x}; }
	forceinline constexpr v4 wyzy() const { return {w,y,z,y}; }
	forceinline constexpr v4 wyzz() const { return {w,y,z,z}; }
	forceinline constexpr v4 wyzw() const { return {w,y,z,w}; }
	forceinline constexpr v4 wywx() const { return {w,y,w,x}; }
	forceinline constexpr v4 wywy() const { return {w,y,w,y}; }
	forceinline constexpr v4 wywz() const { return {w,y,w,z}; }
	forceinline constexpr v4 wyww() const { return {w,y,w,w}; }
	forceinline constexpr v4 wzxx() const { return {w,z,x,x}; }
	forceinline constexpr v4 wzxy() const { return {w,z,x,y}; }
	forceinline constexpr v4 wzxz() const { return {w,z,x,z}; }
	forceinline constexpr v4 wzxw() const { return {w,z,x,w}; }
	forceinline constexpr v4 wzyx() const { return {w,z,y,x}; }
	forceinline constexpr v4 wzyy() const { return {w,z,y,y}; }
	forceinline constexpr v4 wzyz() const { return {w,z,y,z}; }
	forceinline constexpr v4 wzyw() const { return {w,z,y,w}; }
	forceinline constexpr v4 wzzx() const { return {w,z,z,x}; }
	forceinline constexpr v4 wzzy() const { return {w,z,z,y}; }
	forceinline constexpr v4 wzzz() const { return {w,z,z,z}; }
	forceinline constexpr v4 wzzw() const { return {w,z,z,w}; }
	forceinline constexpr v4 wzwx() const { return {w,z,w,x}; }
	forceinline constexpr v4 wzwy() const { return {w,z,w,y}; }
	forceinline constexpr v4 wzwz() const { return {w,z,w,z}; }
	forceinline constexpr v4 wzww() const { return {w,z,w,w}; }
	forceinline constexpr v4 wwxx() const { return {w,w,x,x}; }
	forceinline constexpr v4 wwxy() const { return {w,w,x,y}; }
	forceinline constexpr v4 wwxz() const { return {w,w,x,z}; }
	forceinline constexpr v4 wwxw() const { return {w,w,x,w}; }
	forceinline constexpr v4 wwyx() const { return {w,w,y,x}; }
	forceinline constexpr v4 wwyy() const { return {w,w,y,y}; }
	forceinline constexpr v4 wwyz() const { return {w,w,y,z}; }
	forceinline constexpr v4 wwyw() const { return {w,w,y,w}; }
	forceinline constexpr v4 wwzx() const { return {w,w,z,x}; }
	forceinline constexpr v4 wwzy() const { return {w,w,z,y}; }
	forceinline constexpr v4 wwzz() const { return {w,w,z,z}; }
	forceinline constexpr v4 wwzw() const { return {w,w,z,w}; }
	forceinline constexpr v4 wwwx() const { return {w,w,w,x}; }
	forceinline constexpr v4 wwwy() const { return {w,w,w,y}; }
	forceinline constexpr v4 wwwz() const { return {w,w,w,z}; }
	forceinline constexpr v4 wwww() const { return {w,w,w,w}; }
};

template <class T, class... Rest>
Array(T, Rest...) -> Array<typename RequireAllSame<T, Rest...>::Type, 1 + sizeof...(Rest)>;

template <class T, umm count> inline static constexpr bool is_array<Array<T, count>> = true;

template <class T>
concept AnArray = is_array<T>;

template <class T, umm count> inline static constexpr bool is_unsigned<Array<T, count>> = is_unsigned<T>;

template <class T, umm count> inline static constexpr bool is_integer_like<Array<T, count>> = is_integer_like<T>;

template <class T>            inline static constexpr int array_nestedness = 0;
template <class T, umm count> inline static constexpr int array_nestedness<Array<T, count>> = array_nestedness<T> + 1;

template <class T>
struct ScalarOfT {
	using Type = T;
};
template <class T, umm count> struct ScalarOfT<Array<T, count>> { using Type = typename ScalarOfT<T>::Type; };
template <class T>
using ScalarOf = typename ScalarOfT<T>::Type;

template <class NewScalar, class T>
struct WithScalarT { using Type = NewScalar; };
template <class NewScalar, class T, umm count>
struct WithScalarT<NewScalar, Array<T, count>> { using Type = Array<typename WithScalarT<NewScalar, T>::Type, count>; };
template <class NewScalar, class T>
using WithScalar = typename WithScalarT<NewScalar, T>::Type;

// sub_arrays<2, 1>([[0, 1, 2, 3], [4, 5, 6, 7]]   ) =
// sub_arrays<2   >([[0, 1, 2, 3], [4, 5, 6, 7]], 1) =
// [[1, 2], [5, 6]]

template <umm sub_count, umm sub_start, class T, umm count>
    requires (is_array<T>)
Array<Array<typename T::Element, sub_count>, count> sub_arrays(Array<T, count> self) {
    static_assert(sub_start + sub_count <= T::count);
    Array<Array<typename T::Element, sub_count>, count> r = {};
    for (umm i = 0; i < count; ++i)
        r.data[i] = self.data[i].template sub_array<sub_count, sub_start>();
    return r;
}

template <umm sub_count, class T, umm count>
    requires (is_array<T>)
Array<Array<typename T::Element, sub_count>, count> sub_arrays(Array<T, count> self, umm sub_start = 0) {
    assert(sub_start + sub_count <= T::count);
    Array<Array<typename T::Element, sub_count>, count> r = {};
    for (umm i = 0; i < count; ++i)
        r.data[i] = self.data[i].template sub_array<sub_count>(sub_start);
    return r;
}

template <umm count, class T>
forceinline constexpr Array<T, count> broadcast_to_array(T value) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i)
		result.data[i] = value;
	return result;
}

#define OP(op)                                                                 \
	template <class T, umm count>                                              \
	forceinline constexpr auto operator op(Array<T, count> a)                  \
		requires requires(T t) { op t; }                                       \
	{                                                                          \
		Array<std::remove_cvref_t<decltype(op a.data[0])>, count> result = {}; \
		for (umm i = 0; i < count; ++i)                                        \
			result.data[i] = op a.data[i];                                     \
		return result;                                                         \
	}
OP(+)
OP(-)
OP(~)
OP(!)
OP(*)
#undef OP
	
template <class T>
struct ToBoolUsingAll : T {
	forceinline constexpr explicit operator bool() const { return all(*this); }
};

template <class T>
struct ToBoolUsingAny : T {
	forceinline constexpr explicit operator bool() const { return any(*this); }
};

template <class T, class U> forceinline constexpr auto operator&&(ToBoolUsingAll<T> a, ToBoolUsingAll<U> b) { return (T)a && (U)b; }
template <class T, class U> forceinline constexpr auto operator&&(ToBoolUsingAll<T> a, ToBoolUsingAny<U> b) { return (T)a && (U)b; }
template <class T, class U> forceinline constexpr auto operator&&(ToBoolUsingAny<T> a, ToBoolUsingAll<U> b) { return (T)a && (U)b; }
template <class T, class U> forceinline constexpr auto operator&&(ToBoolUsingAny<T> a, ToBoolUsingAny<U> b) { return (T)a && (U)b; }
template <class T, class U> forceinline constexpr auto operator||(ToBoolUsingAll<T> a, ToBoolUsingAll<U> b) { return (T)a || (U)b; }
template <class T, class U> forceinline constexpr auto operator||(ToBoolUsingAll<T> a, ToBoolUsingAny<U> b) { return (T)a || (U)b; }
template <class T, class U> forceinline constexpr auto operator||(ToBoolUsingAny<T> a, ToBoolUsingAll<U> b) { return (T)a || (U)b; }
template <class T, class U> forceinline constexpr auto operator||(ToBoolUsingAny<T> a, ToBoolUsingAny<U> b) { return (T)a || (U)b; }

#define ASIS(...) __VA_ARGS__
#define WRAP_ToBoolUsingAll(...) ToBoolUsingAll<__VA_ARGS__>
#define WRAP_ToBoolUsingAny(...) ToBoolUsingAny<__VA_ARGS__>

#ifndef TL_ARRAY_STRONG
#define TL_ARRAY_STRONG 1
#endif

// The binary operators for Array allow type promotion.
// This means that Array<int, 3>{} * 1.23f produces Array<float, 3>,
// which is good in most cases as you don't want to explicitly cast.
// This also means that working with small integer types is very annoying:
// Array<u8, 16> * Array<u8, 16> = Array<int, 16>.
// There are three choices:
// 1) Disallow promotion - template code becomes bigger as you need to cast literals to T.
// 2) Allow promotion - working with small integers sucks, need to add casts, and there is wasted cpu time promoting and unpromoting.
// 3) Have both. Make two namespaces and users import whichever they want. Problem is that now every user has to make a decision on what to import
//    and write it. There can't be a default one globally available, as importing another one would introduce ambiguity.

#if TL_ARRAY_STRONG

//////////////////////
// Strong operators //
//////////////////////

#define OP(op)                                                                                                                            \
	template <class T, umm count>                                                                                                         \
	forceinline constexpr auto operator op(Array<T, count> a, Array<T, count> b)                                                          \
		requires requires(T t) { t op t; }                                                                                                \
	{                                                                                                                                     \
		Array<T, count> result = {};                                                                                                      \
		for (umm i = 0; i < count; ++i)                                                                                                   \
			result.data[i] = a.data[i] op b.data[i];                                                                                      \
		return result;                                                                                                                    \
	}                                                                                                                                     \
	template <class U, class T, umm count>                                                                                                \
	forceinline constexpr Array<T, count> operator op(Array<T, count> a, U b)                                                             \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
			requires !is_array<U>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		return a op broadcast_to_array<count>((T)b);                                                                                      \
	}                                                                                                                                     \
	template <class U, class T, umm count>                                                                                                \
	forceinline constexpr Array<T, count> operator op(U a, Array<T, count> b)                                                             \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
			requires !is_array<U>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		return broadcast_to_array<count>((T)a) op b;                                                                                      \
	}                                                                                                                                     \
	template <class U, class T, umm count, umm inner_count>                                                                               \
	forceinline constexpr Array<Array<T, inner_count>, count> operator op(Array<Array<T, inner_count>, count> a, U b)                     \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
			requires !is_array<U>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		return a op broadcast_to_array<count>(broadcast_to_array<inner_count>((T)b));                                                     \
	}                                                                                                                                     \
	template <class U, class T, umm count, umm inner_count>                                                                               \
	forceinline constexpr Array<Array<T, inner_count>, count> operator op(U a, Array<Array<T, inner_count>, count> b)                     \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
			requires !is_array<U>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		return broadcast_to_array<count>(broadcast_to_array<inner_count>((T)a)) op b;                                                     \
	}                                                                                                                                     \
	template <class T, umm count, umm inner_count>                                                                                        \
	forceinline constexpr Array<Array<T, inner_count>, count> operator op(Array<Array<T, inner_count>, count> a, Array<T, inner_count> b) \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		return a op broadcast_to_array<count>(b);                                                                                         \
	}                                                                                                                                     \
	template <class T, umm count, umm inner_count>                                                                                        \
	forceinline constexpr Array<Array<T, inner_count>, count> operator op(Array<T, inner_count> a, Array<Array<T, inner_count>, count> b) \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		return broadcast_to_array<count>(a) op b;                                                                                         \
	}                                                                                                                                     \
	template <class T, umm count, umm inner_count>                                                                                        \
	forceinline constexpr Array<Array<T, inner_count>, count> operator op(Array<Array<T, inner_count>, count> a, Array<T, count> b)       \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		Array<Array<T, inner_count>, count> r;                                                                                            \
		for (umm i = 0; i < count; ++i)                                                                                                   \
			r.data[i] = a.data[i] op b.data[i];                                                                                           \
		return r;                                                                                                                         \
	}                                                                                                                                     \
	template <class T, umm count, umm inner_count>                                                                                        \
	forceinline constexpr Array<Array<T, inner_count>, count> operator op(Array<T, count> a, Array<Array<T, inner_count>, count> b)       \
		requires requires {                                                                                                               \
			requires !is_array<T>;                                                                                                        \
		}                                                                                                                                 \
	{                                                                                                                                     \
		Array<Array<T, inner_count>, count> r;                                                                                            \
		for (umm i = 0; i < count; ++i)                                                                                                   \
			r.data[i] = a.data[i] op b.data[i];                                                                                           \
		return r;                                                                                                                         \
	}                                                                                                                                     \
	template <class T, umm count, class U>                                                                                                \
	forceinline constexpr Array<T, count> &operator op##=(Array<T, count> &a, U b)                                                        \
		requires requires { a op b; }                                                                                                     \
	{                                                                                                                                     \
		return a = a op b;                                                                                                                \
	}
OP(+)
OP(-)
OP(*)
OP(/)
OP(%)
OP(^)
OP(&)
OP(|)
OP(<<)
OP(>>)
#undef OP

#define OP(op, RetTypeMod)                                                                        \
	template <class T, umm count>                                                                 \
	forceinline constexpr auto operator op(Array<T, count> a, Array<T, count> b)                  \
		requires requires(T t) { t op t; }                                                        \
	{                                                                                             \
		RetTypeMod(WithScalar<bool, Array<T, count>>) result = {};                                \
		for (umm i = 0; i < count; ++i)                                                           \
			result.data[i] = a.data[i] op b.data[i];                                              \
		return result;                                                                            \
	}                                                                                             \
	template <class U, class T, umm count>                                                        \
	forceinline constexpr auto operator op(Array<T, count> a, U b)                                \
		requires requires {                                                                       \
			requires array_nestedness<U> <= array_nestedness<T>;                                  \
			requires requires { a op broadcast_to_array<count>((WithScalar<ScalarOf<T>, U>)b); }; \
		}                                                                                         \
	{                                                                                             \
		return a op broadcast_to_array<count>((WithScalar<ScalarOf<T>, U>)b);                     \
	}                                                                                             \
	template <class U, class T, umm count>                                                        \
	forceinline constexpr auto operator op(U a, Array<T, count> b)                                \
		requires requires {                                                                       \
			requires array_nestedness<U> <= array_nestedness<T>;                                  \
			requires requires { broadcast_to_array<count>((WithScalar<ScalarOf<T>, U>)a) op b; }; \
		}                                                                                         \
	{                                                                                             \
		return broadcast_to_array<count>((WithScalar<ScalarOf<T>, U>)a) op b;                     \
	}
OP(==, WRAP_ToBoolUsingAll)
OP(!=, WRAP_ToBoolUsingAny)
OP(<, ASIS)
OP(>, ASIS)
OP(<=, ASIS)
OP(>=, ASIS)
#undef OP

#else

////////////////////
// Weak operators //
////////////////////

#define OP(op, RetTypeMod)                                                                           \
	template <class T, umm count>                                                                    \
	forceinline constexpr auto operator op(Array<T, count> a, Array<T, count> b)                     \
		requires requires(T t) { t op t; }                                                           \
	{                                                                                                \
		RetTypeMod(Array<std::remove_cvref_t<decltype(a.data[0] op b.data[0])>, count>) result = {}; \
		for (umm i = 0; i < count; ++i)                                                              \
			result.data[i] = a.data[i] op b.data[i];                                                 \
		return result;                                                                               \
	}                                                                                                \
	template <class U, class T, umm count>                                                           \
	forceinline constexpr auto operator op(Array<T, count> a, Array<U, count> b)                     \
		requires requires(T t, U u) { t op u; }                                                      \
	{                                                                                                \
		RetTypeMod(Array<std::remove_cvref_t<decltype(a.data[0] op b.data[0])>, count>) result = {}; \
		for (umm i = 0; i < count; ++i)                                                              \
			result.data[i] = a.data[i] op b.data[i];                                                 \
		return result;                                                                               \
	}                                                                                                \
	template <class U, class T, umm count>                                                           \
	forceinline constexpr auto operator op(Array<T, count> a, U b)                                   \
		requires (array_nestedness<U> <= array_nestedness<T>)                                        \
	{                                                                                                \
		return a op broadcast_to_array<count>(b);                                                    \
	}                                                                                                \
	template <class U, class T, umm count>                                                           \
	forceinline constexpr auto operator op(U a, Array<T, count> b)                                   \
		requires (array_nestedness<U> <= array_nestedness<T>)                                        \
	{                                                                                                \
		return broadcast_to_array<count>(a) op b;                                                    \
	}
OP(==, WRAP_ToBoolUsingAll)
OP(!=, WRAP_ToBoolUsingAny)
OP(<, ASIS)
OP(>, ASIS)
OP(<=, ASIS)
OP(>=, ASIS)
OP(+, ASIS)
OP(-, ASIS)
OP(*, ASIS)
OP(/, ASIS)
OP(%, ASIS)
OP(^, ASIS)
OP(&, ASIS)
OP(|, ASIS)
OP(<<, ASIS)
OP(>>, ASIS)
OP(&&, ASIS)
OP(||, ASIS)
#undef OP

#undef ASIS
#undef WRAP_ToBoolUsingAll
#undef WRAP_ToBoolUsingAny

#define OP(op)                                                                        \
	template <class U, class T, umm count>                                            \
	forceinline constexpr auto &operator op##=(Array<T, count> &a, Array<U, count> b) \
		requires requires(T t, U u) { t op##= u; }                                    \
	{                                                                                 \
		for (umm i = 0; i < count; ++i)                                               \
			a.data[i] op##= b.data[i];                                                \
		return a;                                                                     \
	}                                                                                 \
	template <class U, class T, umm count>                              \
	forceinline constexpr auto &operator op##=(Array<T, count> &a, U b) \
		requires requires(T t, U u) { t op##= u; }                      \
	{                                                                   \
		for (umm i = 0; i < count; ++i)                                 \
			a.data[i] op##= b;                                          \
		return a;                                                       \
	}
OP(+)
OP(-)
OP(*)
OP(/)
OP(%)
OP(^)
OP(&)
OP(|)
OP(<<)
OP(>>)
#undef OP

#endif

template <int shift, class T, umm count>
forceinline constexpr Array<T, count> shift_left(Array<T, count> arr) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = arr.data[i] << shift;
	}
	return result;
}

template <int shift, class T, umm count>
forceinline constexpr Array<T, count> shift_right(Array<T, count> arr) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = arr.data[i] >> shift;
	}
	return result;
}

template <umm count, class Callable>
	requires requires { std::declval<Callable>()(); }
forceinline constexpr auto make_array(Callable callable)
	-> Array<std::remove_cvref_t<decltype(callable())>, count>
{
	Array<std::remove_cvref_t<decltype(callable())>, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = callable();
	}
	return result;
}

template <umm count, class Callable>
	requires requires { std::declval<Callable>()((umm)0); }
forceinline constexpr auto make_array(Callable callable)
	-> Array<std::remove_cvref_t<decltype(callable((umm)0))>, count>
{
	Array<std::remove_cvref_t<decltype(callable((umm)0))>, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = callable(i);
	}
	return result;
}

template <class T, umm count>
forceinline constexpr Array<T, count> array_iota(umm start = 0, umm step = 1) {
	Array<T, count> r = {};
	for (umm i = 0, v = start; i < count; ++i, v += step) {
		r.data[i] = convert<T>(v);
	}
	return r;
}

template <class T, umm count>
forceinline constexpr umm count_of(Array<T, count>) {
	return count;
}

template <class T, umm count>
forceinline constexpr Span<T> as_span(Array<T, count> &arr) {
	return {arr.data, arr.count};
}

template <class T, umm count>
forceinline constexpr Array<T, count> to_array(T const (&array)[count]) {
	Array<T, count> result{};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = array[i];
	}
	return result;
}

template <class T, umm count>
forceinline constexpr bool all(Array<T, count> a) {
	for (umm i = 0; i < count; ++i) {
		if (!all(a.data[i])) {
			return false;
		}
	}
	return true;
}

template <class T, umm count>
forceinline constexpr bool any(Array<T, count> a) {
	for (umm i = 0; i < count; ++i) {
		if (any(a.data[i])) {
			return true;
		}
	}
	return false;
}
template <class T, umm count> forceinline constexpr bool all(ToBoolUsingAll<Array<T, count>> a) { return all((Array<T, count> &)a); }
template <class T, umm count> forceinline constexpr bool all(ToBoolUsingAny<Array<T, count>> a) { return all((Array<T, count> &)a); }
template <class T, umm count> forceinline constexpr bool any(ToBoolUsingAll<Array<T, count>> a) { return any((Array<T, count> &)a); }
template <class T, umm count> forceinline constexpr bool any(ToBoolUsingAny<Array<T, count>> a) { return any((Array<T, count> &)a); }

template <class T, umm count>
forceinline constexpr T dot(Array<T, count> a, Array<T, count> b) {
	T result = {};
	for (umm i = 0; i < count; ++i) {
		result += a.data[i] * b.data[i];
	}
	return result;
}

template <class T>
forceinline constexpr Array<T, 3> cross(Array<T, 3> a, Array<T, 3> b) {
	return a.yzx() * b.zxy() - a.zxy() * b.yzx();
}

template <class T, class Index, umm count>
forceinline constexpr Array<T, count> gather(T const *pointer, Array<Index, count> indices) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = pointer[indices.data[i]];
	return r;
}

template <class T, class Index, umm count>
forceinline constexpr void scatter(T *pointer, Array<T, count> values, Array<Index, count> indices) {
	for (umm i = 0; i < count; ++i)
		pointer[indices.data[i]] = values.data[i];
}

#define OP(NAME, OPERATOR)                                                       \
	template <class T>                                                           \
	forceinline constexpr auto mask_##NAME(T a, T b)                             \
		requires requires(T t) { t OPERATOR t; }                                 \
	{                                                                            \
		static_assert(sizeof(UintWithBits<sizeof(T)*8>) == sizeof(T));           \
		return (UintWithBits<sizeof(T)*8>)(-(a OPERATOR b));                     \
	}                                                                            \
	template <class T, umm count>                                                \
	forceinline constexpr auto mask_##NAME(Array<T, count> a, Array<T, count> b) \
		requires requires(T t) { mask_##NAME(t, t); }                            \
	{                                                                            \
		Array<UintWithBits<sizeof(T)*8>, count> r = {};                          \
		static_assert(sizeof(r) == sizeof(Array<T, count>));                     \
		for (umm i = 0; i < count; ++i)                                          \
			r.data[i] = mask_##NAME(a.data[i], b.data[i]);                       \
		return r;                                                                \
	}                                                                            \
	template <class U, class T, umm count>                                       \
	forceinline constexpr auto mask_##NAME(Array<T, count> a, U b)               \
		requires (array_nestedness<U> <= array_nestedness<T>)                    \
	{                                                                            \
		return mask_##NAME(a, broadcast_to_array<count>(b));                     \
	}                                                                            \
	template <class U, class T, umm count>                                       \
	forceinline constexpr auto mask_##NAME(U a, Array<T, count> b)               \
		requires (array_nestedness<U> <= array_nestedness<T>)                    \
	{                                                                            \
		return mask_##NAME(broadcast_to_array<count>(a), b);                     \
	}

OP(eq, ==)
OP(ne, !=)
OP(lt, <)
OP(le, <=)
OP(gt, >)
OP(ge, >=)

#undef OP

// Extract top bits from mask and compress them into a single integer
template <class Mask, umm count>
forceinline constexpr u64 mask_to_int(Array<Mask, count> mask)
	requires requires {
		requires count <= 64;
		requires sizeof(Mask) == 1 || sizeof(Mask) == 2 || sizeof(Mask) == 4 || sizeof(Mask) == 8;
	}
{
	u64 result = 0;
	for (umm i = 0; i < count; ++i)
		result |= ((bit_cast<UintWithBits<sizeof(Mask)*8>>(mask.data[i]) >> (sizeof(Mask)*8 - 1)) & 1) << i;
	return result;
}

// Mask is not an integral, e.g. Array
template <class T, class Mask, umm count>
forceinline constexpr Array<T, count> blend(Array<Mask, count> mask, Array<T, count> a, Array<T, count> b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = blend(mask.data[i], a.data[i], b.data[i]);
	return r;
}

// Mask is an integral
template <class T, std::integral Mask, umm count>
	requires (sizeof(T) == sizeof(Mask))
forceinline constexpr Array<T, count> blend(Array<Mask, count> mask, Array<T, count> a, Array<T, count> b) {
	using MaskInt = UintWithBits<sizeof(Mask) * 8>;
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i) {
		MaskInt mask_int = bit_cast<MaskInt>(mask.data[i]);
		r.data[i] = (mask_int >> (sizeof(Mask)*8-1)) ? a.data[i] : b.data[i];
	}
	return r;
}


template <class Mask, class T, umm count>
forceinline constexpr Array<T, count> select(Array<Mask, count> mask, Array<T, count> a, Array<T, count> b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = select(mask.data[i], a.data[i], b.data[i]);
	return r;
}

template <class U, class T, umm count>
	requires(count * sizeof(T) == count * sizeof(T) / sizeof(U) * sizeof(U) && !std::is_same_v<U, T>)
forceinline constexpr Array<U, count * sizeof(T) / sizeof(U)> reinterpret(Array<T, count> a) {
	return bit_cast<Array<U, count * sizeof(T) / sizeof(U)>>(a);
}

template <class T, umm count, class Index, umm indices_count>
forceinline constexpr Array<T, count> shuffle(Array<T, count> a, Array<Index, indices_count> indices) {
	Array<T, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = a.data[indices.data[i % indices_count] % count];
	return r;
}

template <class T, umm count, class Index>
forceinline constexpr Array<T, count> shuffle(Array<T, count> a, Array<Index, count> indices) {
	Array<T, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = a.data[indices.data[i] % count];
	return r;
}

template <umm dups, class T, umm count>
forceinline constexpr Array<T, count * dups> dup(Array<T, count> a) {
	Array<T, count * dups> r;
	for (umm d = 0; d < dups; ++d)
		for (umm i = 0; i < count; ++i)
			r.data[d * count + i] = a.data[i];
	return r;
}

template <class T, umm count, class From>
	requires std::is_convertible_v<From, T>
forceinline constexpr void convert(Array<T, count> &to, From from) {
	for (umm i = 0; i < count; ++i)
		to.data[i] = (T)from;
}

template <class U, class T, umm count>
	requires is_array<U> && requires(T t) { (ElementOf<U>)t; }
forceinline constexpr U convert(Array<T, count> a) {
	U r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = convert<ElementOf<U>>(a.data[i]);
	return r;
}

template <class U, class T, umm count>
forceinline constexpr Array<U, count> element_cast(Array<T, count> a) {
	return convert<Array<U, count>>(a);
}

template <class T, umm count> forceinline constexpr Array<T, count> muladd(T a, Array<T, count> b, Array<T, count> c) { return muladd(broadcast_to_array<count>(a), b, c); }
template <class T, umm count> forceinline constexpr Array<T, count> muladd(Array<T, count> a, T b, Array<T, count> c) { return muladd(a, broadcast_to_array<count>(b), c); }
template <class T, umm count> forceinline constexpr Array<T, count> muladd(Array<T, count> a, Array<T, count> b, T c) { return muladd(a, b, broadcast_to_array<count>(c)); }
template <class T, umm count> forceinline constexpr Array<T, count> muladd(Array<T, count> a, T b, T c) { return muladd(a, broadcast_to_array<count>(b), broadcast_to_array<count>(c)); }
template <class T, umm count> forceinline constexpr Array<T, count> muladd(T a, Array<T, count> b, T c) { return muladd(broadcast_to_array<count>(a), b, broadcast_to_array<count>(c)); }
template <class T, umm count> forceinline constexpr Array<T, count> muladd(T a, T b, Array<T, count> c) { return muladd(broadcast_to_array<count>(a), broadcast_to_array<count>(b), c); }

template <umm count>
forceinline constexpr Array<u8, count> count_bits(Array<u8, count> a) {
	a = (a & 0x55) + ((a >> 1) & 0x55);
	a = (a & 0x33) + ((a >> 2) & 0x33);
	a = (a & 0x0f) + ((a >> 4) & 0x0f);
	return a;
}
template <umm count>
forceinline constexpr Array<u16, count> count_bits(Array<u16, count> a) {
	a = (a & 0x5555) + ((a >> 1) & 0x5555);
	a = (a & 0x3333) + ((a >> 2) & 0x3333);
	a = (a & 0x0f0f) + ((a >> 4) & 0x0f0f);
	a = (a & 0x00ff) + ((a >> 8) & 0x00ff);
	return a;
}
template <umm count>
forceinline constexpr Array<u32, count> count_bits(Array<u32, count> a) {
	a = (a & 0x55555555) + ((a >>  1) & 0x55555555);
	a = (a & 0x33333333) + ((a >>  2) & 0x33333333);
	a = (a & 0x0f0f0f0f) + ((a >>  4) & 0x0f0f0f0f);
	a = (a & 0x00ff00ff) + ((a >>  8) & 0x00ff00ff);
	a = (a & 0x0000ffff) + ((a >> 16) & 0x0000ffff);
	return a;
}
template <umm count>
forceinline constexpr Array<u64, count> count_bits(Array<u64, count> a) {
	a = (a & 0x5555555555555555) + ((a >>  1) & 0x5555555555555555);
	a = (a & 0x3333333333333333) + ((a >>  2) & 0x3333333333333333);
	a = (a & 0x0f0f0f0f0f0f0f0f) + ((a >>  4) & 0x0f0f0f0f0f0f0f0f);
	a = (a & 0x00ff00ff00ff00ff) + ((a >>  8) & 0x00ff00ff00ff00ff);
	a = (a & 0x0000ffff0000ffff) + ((a >> 16) & 0x0000ffff0000ffff);
	a = (a & 0x00000000ffffffff) + ((a >> 32) & 0x00000000ffffffff);
	return a;
}

using u8x16 = Array<u8, 16>; using u8x32 = Array<u8, 32>; using u8x64 = Array<u8, 64>;
using s8x16 = Array<s8, 16>; using s8x32 = Array<s8, 32>; using s8x64 = Array<s8, 64>;
using u16x8 = Array<u16, 8>; using u16x16 = Array<u16, 16>; using u16x32 = Array<u16, 32>;
using s16x8 = Array<s16, 8>; using s16x16 = Array<s16, 16>; using s16x32 = Array<s16, 32>;
using u32x4 = Array<u32, 4>; using u32x8 = Array<u32, 8>; using u32x16 = Array<u32, 16>;
using s32x4 = Array<s32, 4>; using s32x8 = Array<s32, 8>; using s32x16 = Array<s32, 16>;
using f32x4 = Array<f32, 4>; using f32x8 = Array<f32, 8>; using f32x16 = Array<f32, 16>;
using u64x2 = Array<u64, 2>; using u64x4 = Array<u64, 4>; using u64x8 = Array<u64, 8>;
using s64x2 = Array<s64, 2>; using s64x4 = Array<s64, 4>; using s64x8 = Array<s64, 8>;
using f64x2 = Array<f64, 2>; using f64x4 = Array<f64, 4>; using f64x8 = Array<f64, 8>;

#if TL_USE_SIMD

//#if TL_STRONG_ARRAY
//#define TL_INTR_RET(type) type 
//#else
#define TL_INTR_RET(type) auto
//#endif 

template <class T> struct IntrTypeT {};
template <> struct IntrTypeT<u8x16> { using Type = __m128i; };
template <> struct IntrTypeT<u16x8> { using Type = __m128i; };
template <> struct IntrTypeT<u32x4> { using Type = __m128i; };
template <> struct IntrTypeT<u64x2> { using Type = __m128i; };
template <> struct IntrTypeT<s8x16> { using Type = __m128i; };
template <> struct IntrTypeT<s16x8> { using Type = __m128i; };
template <> struct IntrTypeT<s32x4> { using Type = __m128i; };
template <> struct IntrTypeT<s64x2> { using Type = __m128i; };
template <> struct IntrTypeT<f32x4> { using Type = __m128;  };
template <> struct IntrTypeT<f64x2> { using Type = __m128d; };
template <> struct IntrTypeT<u8x32 > { using Type = __m256i; };
template <> struct IntrTypeT<u16x16> { using Type = __m256i; };
template <> struct IntrTypeT<u32x8 > { using Type = __m256i; };
template <> struct IntrTypeT<u64x4 > { using Type = __m256i; };
template <> struct IntrTypeT<s8x32 > { using Type = __m256i; };
template <> struct IntrTypeT<s16x16> { using Type = __m256i; };
template <> struct IntrTypeT<s32x8 > { using Type = __m256i; };
template <> struct IntrTypeT<s64x4 > { using Type = __m256i; };
template <> struct IntrTypeT<f32x8 > { using Type = __m256;  };
template <> struct IntrTypeT<f64x4 > { using Type = __m256d; };
template <class T> using IntrType = typename IntrTypeT<T>::Type;

// This shit breaks constexpr. Can't call unspecialized version in c++.
// Workaround would be to have two functions (v1 with default implementation,
// v2 with default that calls v1, v2 can be specialized and call v1 if needed)
// ... FOR EACH FUCKING OPERATION.

#define intr1(ret, name, intr, t1)                                                 \
	template <> forceinline constexpr TL_INTR_RET(ret) name(t1 a_) {               \
		auto a = bit_cast<IntrType<t1>>(a_);                                       \
		return bit_cast<ret>(intr);                                                \
	}

#define intr2(ret, name, intr, t1, t2)                                             \
	template <> forceinline constexpr TL_INTR_RET(ret) name(t1 a_, t2 b_) {        \
		auto a = bit_cast<IntrType<t1>>(a_);                                       \
		auto b = bit_cast<IntrType<t2>>(b_);                                       \
		return bit_cast<ret>(intr);                                                \
	}

#define intr3(ret, name, intr, t1, t2, t3)                                         \
	template <> forceinline constexpr TL_INTR_RET(ret) name(t1 a_, t2 b_, t3 c_) { \
		auto a = bit_cast<IntrType<t1>>(a_);                                       \
		auto b = bit_cast<IntrType<t2>>(b_);                                       \
		auto c = bit_cast<IntrType<t3>>(c_);                                       \
		return bit_cast<ret>(intr);                                                \
	}

#define _mm_not_si128(a) _mm_xor_si128(a, _mm_set1_epi8(-1))
#define _mm_topbit_i8  _mm_set1_epi8(0x80)
#define _mm_topbit_i16 _mm_set1_epi16(0x8000)
#define _mm_topbit_i32 _mm_set1_epi32(0x80000000)
#define _mm_topbit_i64 _mm_set1_epi64x(0x8000000000000000)
#define _mm_fliptop_i8(a)  _mm_xor_si128(a, _mm_topbit_i8)
#define _mm_fliptop_i16(a) _mm_xor_si128(a, _mm_topbit_i16)
#define _mm_fliptop_i32(a) _mm_xor_si128(a, _mm_topbit_i32)
#define _mm_fliptop_i64(a) _mm_xor_si128(a, _mm_topbit_i64)

#define _mm256_not_si256(a) _mm256_xor_si256(a, _mm256_set1_epi8(-1))
#define _mm256_topbit_i8  _mm256_set1_epi8(0x80)
#define _mm256_topbit_i16 _mm256_set1_epi16(0x8000)
#define _mm256_topbit_i32 _mm256_set1_epi32(0x80000000)
#define _mm256_topbit_i64 _mm256_set1_epi64x(0x8000000000000000)
#define _mm256_fliptop_i8(a)  _mm256_xor_si256(a, _mm256_topbit_i8)
#define _mm256_fliptop_i16(a) _mm256_xor_si256(a, _mm256_topbit_i16)
#define _mm256_fliptop_i32(a) _mm256_xor_si256(a, _mm256_topbit_i32)
#define _mm256_fliptop_i64(a) _mm256_xor_si256(a, _mm256_topbit_i64)


#ifdef __AVX__
#define IF_AVX(...) __VA_ARGS__
#else
#define IF_AVX(...)
#endif

#ifdef __AVX2__
#define IF_AVX2(...) __VA_ARGS__
#else
#define IF_AVX2(...)
#endif

#ifdef __AVX512F__
#define IF_AVX512F(...) __VA_ARGS__
#else
#define IF_AVX512F(...)
#endif

#ifdef __AVX512BW__
#define IF_AVX512BW(...) __VA_ARGS__
#else
#define IF_AVX512BW(...)
#endif

#ifdef __AVX512DQ__
#define IF_AVX512DQ(...) __VA_ARGS__
#else
#define IF_AVX512DQ(...)
#endif

#ifdef __AVX512VL__
#define IF_AVX512VL(...) __VA_ARGS__
#else
#define IF_AVX512VL(...)
#endif

#if defined(__AVX512VL__) && defined(__AVX512BW__)
#define IF_AVX512VLBW(...) __VA_ARGS__
#else
#define IF_AVX512VLBW(...)
#endif

//u8x16
/*  + */               intr2(u8x16, operator+, _mm_add_epi8 (a, b), u8x16, u8x16)
/*  - */               intr2(u8x16, operator-, _mm_sub_epi8 (a, b), u8x16, u8x16)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(u8x16, operator^, _mm_xor_si128(a, b), u8x16, u8x16)
/*  & */               intr2(u8x16, operator&, _mm_and_si128(a, b), u8x16, u8x16)
/*  | */               intr2(u8x16, operator|, _mm_or_si128 (a, b), u8x16, u8x16)
/* << */ 
/* >> */ 
/* == */               intr2(u8x16, mask_eq, _mm_cmpeq_epi8(a, b), u8x16, u8x16)
/* != */               intr2(u8x16, mask_ne, _mm_not_si128(_mm_cmpeq_epi8(a, b)), u8x16, u8x16)
/*  < */               intr2(u8x16, mask_lt, _mm_cmpgt_epi8(_mm_fliptop_i8(b), _mm_fliptop_i8(a)), u8x16, u8x16)
/* <= */               intr2(u8x16, mask_le, _mm_not_si128(_mm_cmpgt_epi8(_mm_fliptop_i8(a), _mm_fliptop_i8(b))), u8x16, u8x16)
/*  > */               intr2(u8x16, mask_gt, _mm_cmpgt_epi8(_mm_fliptop_i8(a), _mm_fliptop_i8(b)), u8x16, u8x16)
/* >= */               intr2(u8x16, mask_ge, _mm_not_si128(_mm_cmpgt_epi8(_mm_fliptop_i8(b), _mm_fliptop_i8(a))), u8x16, u8x16)
//u16x8
/*  + */               intr2(u16x8, operator+,  _mm_add_epi16  (a, b), u16x8, u16x8)
/*  - */               intr2(u16x8, operator-,  _mm_sub_epi16  (a, b), u16x8, u16x8)
/*  * */               intr2(u16x8, operator*,  _mm_mullo_epi16(a, b), u16x8, u16x8)
/*  / */ 
/*  ^ */               intr2(u16x8, operator^,  _mm_xor_si128  (a, b), u16x8, u16x8)
/*  & */               intr2(u16x8, operator&,  _mm_and_si128  (a, b), u16x8, u16x8)
/*  | */               intr2(u16x8, operator|,  _mm_or_si128   (a, b), u16x8, u16x8)
/* << */ IF_AVX512VLBW(intr2(u16x8, operator<<, _mm_sllv_epi16 (a, b), u16x8, u16x8))
/* >> */ IF_AVX512VLBW(intr2(u16x8, operator>>, _mm_srlv_epi16 (a, b), u16x8, u16x8))
/* == */               intr2(u16x8, mask_eq, _mm_cmpeq_epi16(a, b), u16x8, u16x8)
/* != */               intr2(u16x8, mask_ne, _mm_not_si128(_mm_cmpeq_epi16(a, b)), u16x8, u16x8)
/*  < */               intr2(u16x8, mask_lt, _mm_cmpgt_epi16(_mm_fliptop_i16(b), _mm_fliptop_i16(a)), u16x8, u16x8)
/* <= */               intr2(u16x8, mask_le, _mm_not_si128(_mm_cmpgt_epi16(_mm_fliptop_i16(a), _mm_fliptop_i16(b))), u16x8, u16x8)
/*  > */               intr2(u16x8, mask_gt, _mm_cmpgt_epi16(_mm_fliptop_i16(a), _mm_fliptop_i16(b)), u16x8, u16x8)
/* >= */               intr2(u16x8, mask_ge, _mm_not_si128(_mm_cmpgt_epi16(_mm_fliptop_i16(b), _mm_fliptop_i16(a))), u16x8, u16x8)
//u32x4
/*  + */               intr2(u32x4, operator+, _mm_add_epi32  (a, b), u32x4, u32x4)
/*  - */               intr2(u32x4, operator-, _mm_sub_epi32  (a, b), u32x4, u32x4)
/*  * */               intr2(u32x4, operator*, _mm_mullo_epi32(a, b), u32x4, u32x4)
/*  / */ 
/*  ^ */               intr2(u32x4, operator^, _mm_xor_si128  (a, b), u32x4, u32x4)
/*  & */               intr2(u32x4, operator&, _mm_and_si128  (a, b), u32x4, u32x4)
/*  | */               intr2(u32x4, operator|, _mm_or_si128   (a, b), u32x4, u32x4)
/* << */       IF_AVX2(intr2(u32x4, operator<<, _mm_sllv_epi32(a, b), u32x4, u32x4))
/* >> */       IF_AVX2(intr2(u32x4, operator>>, _mm_srlv_epi32(a, b), u32x4, u32x4))
/* == */               intr2(u32x4, mask_eq, _mm_cmpeq_epi32(a, b), u32x4, u32x4)
/* != */               intr2(u32x4, mask_ne, _mm_not_si128(_mm_cmpeq_epi32(a, b)), u32x4, u32x4)
/*  < */               intr2(u32x4, mask_lt, _mm_cmpgt_epi32(_mm_fliptop_i32(b), _mm_fliptop_i32(a)), u32x4, u32x4)
/* <= */               intr2(u32x4, mask_le, _mm_not_si128(_mm_cmpgt_epi32(_mm_fliptop_i32(a), _mm_fliptop_i32(b))), u32x4, u32x4)
/*  > */               intr2(u32x4, mask_gt, _mm_cmpgt_epi32(_mm_fliptop_i32(a), _mm_fliptop_i32(b)), u32x4, u32x4)
/* >= */               intr2(u32x4, mask_ge, _mm_not_si128(_mm_cmpgt_epi32(_mm_fliptop_i32(b), _mm_fliptop_i32(a))), u32x4, u32x4)
//u64x2
/*  + */               intr2(u64x2, operator+, _mm_add_epi64(a, b), u64x2, u64x2)
/*  - */               intr2(u64x2, operator-, _mm_sub_epi64(a, b), u64x2, u64x2)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(u64x2, operator^, _mm_xor_si128(a, b), u64x2, u64x2)
/*  & */               intr2(u64x2, operator&, _mm_and_si128(a, b), u64x2, u64x2)
/*  | */               intr2(u64x2, operator|, _mm_or_si128 (a, b), u64x2, u64x2)
/* << */       IF_AVX2(intr2(u64x2, operator<<, _mm_sllv_epi64(a, b), u64x2, u64x2))
/* >> */       IF_AVX2(intr2(u64x2, operator>>, _mm_srlv_epi64(a, b), u64x2, u64x2))
/* == */               intr2(u64x2, mask_eq, _mm_cmpeq_epi64(a, b), u64x2, u64x2)
/* != */               intr2(u64x2, mask_ne, _mm_not_si128(_mm_cmpeq_epi64(a, b)), u64x2, u64x2)
/*  < */               intr2(u64x2, mask_lt, _mm_cmpgt_epi64(_mm_fliptop_i64(b), _mm_fliptop_i64(a)), u64x2, u64x2)
/* <= */               intr2(u64x2, mask_le, _mm_not_si128(_mm_cmpgt_epi64(_mm_fliptop_i64(a), _mm_fliptop_i64(b))), u64x2, u64x2)
/*  > */               intr2(u64x2, mask_gt, _mm_cmpgt_epi64(_mm_fliptop_i64(a), _mm_fliptop_i64(b)), u64x2, u64x2)
/* >= */               intr2(u64x2, mask_ge, _mm_not_si128(_mm_cmpgt_epi64(_mm_fliptop_i64(b), _mm_fliptop_i64(a))), u64x2, u64x2)
//s8x16
/*  + */               intr2(s8x16, operator+, _mm_add_epi8 (a, b), s8x16, s8x16)
/*  - */               intr2(s8x16, operator-, _mm_sub_epi8 (a, b), s8x16, s8x16)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(s8x16, operator^, _mm_xor_si128(a, b), s8x16, s8x16)
/*  & */               intr2(s8x16, operator&, _mm_and_si128(a, b), s8x16, s8x16)
/*  | */               intr2(s8x16, operator|, _mm_or_si128 (a, b), s8x16, s8x16)
/* << */
/* >> */
/* == */               intr2(u8x16, mask_eq, _mm_cmpeq_epi8 (a, b), s8x16, s8x16)
/* != */               intr2(u8x16, mask_ne, _mm_not_si128(_mm_cmpeq_epi8 (a, b)), s8x16, s8x16)
/*  < */               intr2(u8x16, mask_lt, _mm_cmpgt_epi8 (b, a), s8x16, s8x16)
/* <= */               intr2(u8x16, mask_le, _mm_not_si128(_mm_cmpgt_epi8 (a, b)), s8x16, s8x16)
/*  > */               intr2(u8x16, mask_gt, _mm_cmpgt_epi8 (a, b), s8x16, s8x16)
/* >= */               intr2(u8x16, mask_ge, _mm_not_si128(_mm_cmpgt_epi8 (b, a)), s8x16, s8x16)
//s16x8
/*  + */               intr2(s16x8, operator+, _mm_add_epi16  (a, b), s16x8, s16x8)
/*  - */               intr2(s16x8, operator-, _mm_sub_epi16  (a, b), s16x8, s16x8)
/*  * */               intr2(s16x8, operator*, _mm_mullo_epi16(a, b), s16x8, s16x8)
/*  / */ 
/*  ^ */               intr2(s16x8, operator^, _mm_xor_si128  (a, b), s16x8, s16x8)
/*  & */               intr2(s16x8, operator&, _mm_and_si128  (a, b), s16x8, s16x8)
/*  | */               intr2(s16x8, operator|, _mm_or_si128   (a, b), s16x8, s16x8)
/* << */ IF_AVX512VLBW(intr2(s16x8, operator<<, _mm_sllv_epi16 (a, b), s16x8, s16x8))
/* >> */ IF_AVX512VLBW(intr2(s16x8, operator>>, _mm_srlv_epi16 (a, b), s16x8, s16x8))
/* == */               intr2(u16x8, mask_eq, _mm_cmpeq_epi16(a, b), s16x8, s16x8)
/* != */               intr2(u16x8, mask_ne, _mm_not_si128(_mm_cmpeq_epi16(a, b)), s16x8, s16x8)
/*  < */               intr2(u16x8, mask_lt, _mm_cmpgt_epi16(b, a), s16x8, s16x8)
/* <= */               intr2(u16x8, mask_le, _mm_not_si128(_mm_cmpgt_epi16(a, b)), s16x8, s16x8)
/*  > */               intr2(u16x8, mask_gt, _mm_cmpgt_epi16(a, b), s16x8, s16x8)
/* >= */               intr2(u16x8, mask_ge, _mm_not_si128(_mm_cmpgt_epi16(b, a)), s16x8, s16x8)
//s32x4
/*  + */               intr2(s32x4, operator+, _mm_add_epi32  (a, b), s32x4, s32x4)
/*  - */               intr2(s32x4, operator-, _mm_sub_epi32  (a, b), s32x4, s32x4)
/*  * */               intr2(s32x4, operator*, _mm_mullo_epi32(a, b), s32x4, s32x4)
/*  / */ 
/*  ^ */               intr2(s32x4, operator^, _mm_xor_si128  (a, b), s32x4, s32x4)
/*  & */               intr2(s32x4, operator&, _mm_and_si128  (a, b), s32x4, s32x4)
/*  | */               intr2(s32x4, operator|, _mm_or_si128   (a, b), s32x4, s32x4)
/* << */       IF_AVX2(intr2(s32x4, operator<<, _mm_sllv_epi32(a, b), s32x4, s32x4))
/* >> */       IF_AVX2(intr2(s32x4, operator>>, _mm_srav_epi32(a, b), s32x4, s32x4))
/* == */               intr2(u32x4, mask_eq, _mm_cmpeq_epi32(a, b), s32x4, s32x4)
/* != */               intr2(u32x4, mask_ne, _mm_not_si128(_mm_cmpeq_epi32(a, b)), s32x4, s32x4)
/*  < */               intr2(u32x4, mask_lt, _mm_cmpgt_epi32(b, a), s32x4, s32x4)
/* <= */               intr2(u32x4, mask_le, _mm_not_si128(_mm_cmpgt_epi32(a, b)), s32x4, s32x4)
/*  > */               intr2(u32x4, mask_gt, _mm_cmpgt_epi32(a, b), s32x4, s32x4)
/* >= */               intr2(u32x4, mask_ge, _mm_not_si128(_mm_cmpgt_epi32(b, a)), s32x4, s32x4)
//s64x2
/*  + */               intr2(s64x2, operator+, _mm_add_epi64(a, b), s64x2, s64x2)
/*  - */               intr2(s64x2, operator-, _mm_sub_epi64(a, b), s64x2, s64x2)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(s64x2, operator^, _mm_xor_si128(a, b), s64x2, s64x2)
/*  & */               intr2(s64x2, operator&, _mm_and_si128(a, b), s64x2, s64x2)
/*  | */               intr2(s64x2, operator|, _mm_or_si128 (a, b), s64x2, s64x2)
/* << */       IF_AVX2(intr2(s64x2, operator<<, _mm_sllv_epi64(a, b), s64x2, s64x2))
/* >> */   IF_AVX512VL(intr2(s64x2, operator>>, _mm_srav_epi64(a, b), s64x2, s64x2))
/* == */               intr2(u64x2, mask_eq, _mm_cmpeq_epi64(a, b), s64x2, s64x2)
/* != */               intr2(u64x2, mask_ne, _mm_not_si128(_mm_cmpeq_epi64(a, b)), s64x2, s64x2)
/*  < */               intr2(u64x2, mask_lt, _mm_cmpgt_epi32(b, a), s64x2, s64x2)
/* <= */               intr2(u64x2, mask_le, _mm_not_si128(_mm_cmpgt_epi64(a, b)), s64x2, s64x2)
/*  > */               intr2(u64x2, mask_gt, _mm_cmpgt_epi64(a, b), s64x2, s64x2)
/* >= */               intr2(u64x2, mask_ge, _mm_not_si128(_mm_cmpgt_epi32(b, a)), s64x2, s64x2)
//f32x4
/*  + */               intr2(f32x4, operator+, _mm_add_ps (a, b), f32x4, f32x4)
/*  - */               intr2(f32x4, operator-, _mm_sub_ps (a, b), f32x4, f32x4)
/*  * */               intr2(f32x4, operator*, _mm_mul_ps (a, b), f32x4, f32x4)
/*  / */               intr2(f32x4, operator/, _mm_div_ps (a, b), f32x4, f32x4)
/* == */               intr2(u32x4, mask_eq, _mm_cmpeq_ps (a, b), f32x4, f32x4)
/* != */               intr2(u32x4, mask_ne, _mm_cmpneq_ps(a, b), f32x4, f32x4)
/*  < */               intr2(u32x4, mask_lt, _mm_cmplt_ps (a, b), f32x4, f32x4)
/* <= */               intr2(u32x4, mask_le, _mm_cmple_ps (a, b), f32x4, f32x4)
/*  > */               intr2(u32x4, mask_gt, _mm_cmpgt_ps (a, b), f32x4, f32x4)
/* >= */               intr2(u32x4, mask_ge, _mm_cmpge_ps (a, b), f32x4, f32x4)
//f64x2
/*  + */               intr2(f64x2, operator+, _mm_add_pd (a, b), f64x2, f64x2)
/*  - */               intr2(f64x2, operator-, _mm_sub_pd (a, b), f64x2, f64x2)
/*  * */               intr2(f64x2, operator*, _mm_mul_pd (a, b), f64x2, f64x2)
/*  / */               intr2(f64x2, operator/, _mm_div_pd (a, b), f64x2, f64x2)
/* == */               intr2(u64x2, mask_eq, _mm_cmpeq_pd (a, b), f64x2, f64x2)
/* != */               intr2(u64x2, mask_ne, _mm_cmpneq_pd(a, b), f64x2, f64x2)
/*  < */               intr2(u64x2, mask_lt, _mm_cmplt_pd (a, b), f64x2, f64x2)
/* <= */               intr2(u64x2, mask_le, _mm_cmple_pd (a, b), f64x2, f64x2)
/*  > */               intr2(u64x2, mask_gt, _mm_cmpgt_pd (a, b), f64x2, f64x2)
/* >= */               intr2(u64x2, mask_ge, _mm_cmpge_pd (a, b), f64x2, f64x2)
#ifdef __AVX__
#ifdef __AVX2__
//u8x32
/*  + */               intr2(u8x32, operator+, _mm256_add_epi8 (a, b), u8x32, u8x32)
/*  - */               intr2(u8x32, operator-, _mm256_sub_epi8 (a, b), u8x32, u8x32)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(u8x32, operator^, _mm256_xor_si256(a, b), u8x32, u8x32)
/*  & */               intr2(u8x32, operator&, _mm256_and_si256(a, b), u8x32, u8x32)
/*  | */               intr2(u8x32, operator|, _mm256_or_si256 (a, b), u8x32, u8x32)
/* << */ 
/* >> */ 
/* == */               intr2(u8x32, mask_eq, _mm256_cmpeq_epi8(a, b), u8x32, u8x32)
/* != */               intr2(u8x32, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi8(a, b)), u8x32, u8x32)
/*  < */               intr2(u8x32, mask_lt, _mm256_cmpgt_epi8(_mm256_fliptop_i8(b), _mm256_fliptop_i8(a)), u8x32, u8x32)
/* <= */               intr2(u8x32, mask_le, _mm256_not_si256(_mm256_cmpgt_epi8(_mm256_fliptop_i8(a), _mm256_fliptop_i8(b))), u8x32, u8x32)
/*  > */               intr2(u8x32, mask_gt, _mm256_cmpgt_epi8(_mm256_fliptop_i8(a), _mm256_fliptop_i8(b)), u8x32, u8x32)
/* >= */               intr2(u8x32, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi8(_mm256_fliptop_i8(b), _mm256_fliptop_i8(a))), u8x32, u8x32)
//u16x16
/*  + */               intr2(u16x16, operator+, _mm256_add_epi16  (a, b), u16x16, u16x16)
/*  - */               intr2(u16x16, operator-, _mm256_sub_epi16  (a, b), u16x16, u16x16)
/*  * */               intr2(u16x16, operator*, _mm256_mullo_epi16(a, b), u16x16, u16x16)
/*  / */ 
/*  ^ */               intr2(u16x16, operator^, _mm256_xor_si256  (a, b), u16x16, u16x16)
/*  & */               intr2(u16x16, operator&, _mm256_and_si256  (a, b), u16x16, u16x16)
/*  | */               intr2(u16x16, operator|, _mm256_or_si256   (a, b), u16x16, u16x16)
/* << */ IF_AVX512VLBW(intr2(u16x16, operator<<, _mm256_sllv_epi16(a, b), u16x16, u16x16))
/* >> */ IF_AVX512VLBW(intr2(u16x16, operator>>, _mm256_srlv_epi16(a, b), u16x16, u16x16))
/* == */               intr2(u16x16, mask_eq, _mm256_cmpeq_epi16(a, b), u16x16, u16x16)
/* != */               intr2(u16x16, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi16(a, b)), u16x16, u16x16)
/*  < */               intr2(u16x16, mask_lt, _mm256_cmpgt_epi16(_mm256_fliptop_i16(b), _mm256_fliptop_i16(a)), u16x16, u16x16)
/* <= */               intr2(u16x16, mask_le, _mm256_not_si256(_mm256_cmpgt_epi16(_mm256_fliptop_i16(a), _mm256_fliptop_i16(b))), u16x16, u16x16)
/*  > */               intr2(u16x16, mask_gt, _mm256_cmpgt_epi16(_mm256_fliptop_i16(a), _mm256_fliptop_i16(b)), u16x16, u16x16)
/* >= */               intr2(u16x16, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi16(_mm256_fliptop_i16(b), _mm256_fliptop_i16(a))), u16x16, u16x16)
//u32x8
/*  + */               intr2(u32x8, operator+, _mm256_add_epi32  (a, b), u32x8, u32x8)
/*  - */               intr2(u32x8, operator-, _mm256_sub_epi32  (a, b), u32x8, u32x8)
/*  * */               intr2(u32x8, operator*, _mm256_mullo_epi32(a, b), u32x8, u32x8)
/*  / */ 
/*  ^ */               intr2(u32x8, operator^, _mm256_xor_si256  (a, b), u32x8, u32x8)
/*  & */               intr2(u32x8, operator&, _mm256_and_si256  (a, b), u32x8, u32x8)
/*  | */               intr2(u32x8, operator|, _mm256_or_si256   (a, b), u32x8, u32x8)
/* << */               intr2(u32x8, operator<<, _mm256_sllv_epi32(a, b), u32x8, u32x8)
/* >> */               intr2(u32x8, operator>>, _mm256_srlv_epi32(a, b), u32x8, u32x8)
/* == */               intr2(u32x8, mask_eq, _mm256_cmpeq_epi32(a, b), u32x8, u32x8)
/* != */               intr2(u32x8, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi32(a, b)), u32x8, u32x8)
/*  < */               intr2(u32x8, mask_lt, _mm256_cmpgt_epi32(_mm256_fliptop_i32(b), _mm256_fliptop_i32(a)), u32x8, u32x8)
/* <= */               intr2(u32x8, mask_le, _mm256_not_si256(_mm256_cmpgt_epi32(_mm256_fliptop_i32(a), _mm256_fliptop_i32(b))), u32x8, u32x8)
/*  > */               intr2(u32x8, mask_gt, _mm256_cmpgt_epi32(_mm256_fliptop_i32(a), _mm256_fliptop_i32(b)), u32x8, u32x8)
/* >= */               intr2(u32x8, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi32(_mm256_fliptop_i32(b), _mm256_fliptop_i32(a))), u32x8, u32x8)
//u64x4
/*  + */               intr2(u64x4, operator+, _mm256_add_epi64(a, b), u64x4, u64x4)
/*  - */               intr2(u64x4, operator-, _mm256_sub_epi64(a, b), u64x4, u64x4)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(u64x4, operator^, _mm256_xor_si256(a, b), u64x4, u64x4)
/*  & */               intr2(u64x4, operator&, _mm256_and_si256(a, b), u64x4, u64x4)
/*  | */               intr2(u64x4, operator|, _mm256_or_si256 (a, b), u64x4, u64x4)
/* << */               intr2(u64x4, operator<<, _mm256_sllv_epi64(a, b), u64x4, u64x4)
/* >> */               intr2(u64x4, operator>>, _mm256_srlv_epi64(a, b), u64x4, u64x4)
/* == */               intr2(u64x4, mask_eq, _mm256_cmpeq_epi64(a, b), u64x4, u64x4)
/* != */               intr2(u64x4, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi64(a, b)), u64x4, u64x4)
/*  < */               intr2(u64x4, mask_lt, _mm256_cmpgt_epi64(_mm256_fliptop_i64(b), _mm256_fliptop_i64(a)), u64x4, u64x4)
/* <= */               intr2(u64x4, mask_le, _mm256_not_si256(_mm256_cmpgt_epi64(_mm256_fliptop_i64(a), _mm256_fliptop_i64(b))), u64x4, u64x4)
/*  > */               intr2(u64x4, mask_gt, _mm256_cmpgt_epi64(_mm256_fliptop_i64(a), _mm256_fliptop_i64(b)), u64x4, u64x4)
/* >= */               intr2(u64x4, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi64(_mm256_fliptop_i64(b), _mm256_fliptop_i64(a))), u64x4, u64x4)
//s8x32
/*  + */               intr2(s8x32, operator+, _mm256_add_epi8 (a, b), s8x32, s8x32)
/*  - */               intr2(s8x32, operator-, _mm256_sub_epi8 (a, b), s8x32, s8x32)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(s8x32, operator^, _mm256_xor_si256(a, b), s8x32, s8x32)
/*  & */               intr2(s8x32, operator&, _mm256_and_si256(a, b), s8x32, s8x32)
/*  | */               intr2(s8x32, operator|, _mm256_or_si256 (a, b), s8x32, s8x32)
/* << */
/* >> */
/* == */               intr2(u8x32, mask_eq, _mm256_cmpeq_epi8 (a, b), s8x32, s8x32)
/* != */               intr2(u8x32, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi8 (a, b)), s8x32, s8x32)
/*  < */               intr2(u8x32, mask_lt, _mm256_cmpgt_epi8 (b, a), s8x32, s8x32)
/* <= */               intr2(u8x32, mask_le, _mm256_not_si256(_mm256_cmpgt_epi8 (a, b)), s8x32, s8x32)
/*  > */               intr2(u8x32, mask_gt, _mm256_cmpgt_epi8 (a, b), s8x32, s8x32)
/* >= */               intr2(u8x32, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi8 (b, a)), s8x32, s8x32)
//s16x16
/*  + */               intr2(s16x16, operator+, _mm256_add_epi16  (a, b), s16x16, s16x16)
/*  - */               intr2(s16x16, operator-, _mm256_sub_epi16  (a, b), s16x16, s16x16)
/*  * */               intr2(s16x16, operator*, _mm256_mullo_epi16(a, b), s16x16, s16x16)
/*  / */ 
/*  ^ */               intr2(s16x16, operator^, _mm256_xor_si256  (a, b), s16x16, s16x16)
/*  & */               intr2(s16x16, operator&, _mm256_and_si256  (a, b), s16x16, s16x16)
/*  | */               intr2(s16x16, operator|, _mm256_or_si256   (a, b), s16x16, s16x16)
/* << */ IF_AVX512VLBW(intr2(s16x16, operator<<, _mm256_sllv_epi16(a, b), s16x16, s16x16))
/* >> */ IF_AVX512VLBW(intr2(s16x16, operator>>, _mm256_srav_epi16(a, b), s16x16, s16x16))
/* == */               intr2(u16x16, mask_eq, _mm256_cmpeq_epi16(a, b), s16x16, s16x16)
/* != */               intr2(u16x16, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi16(a, b)), s16x16, s16x16)
/*  < */               intr2(u16x16, mask_lt, _mm256_cmpgt_epi16(b, a), s16x16, s16x16)
/* <= */               intr2(u16x16, mask_le, _mm256_not_si256(_mm256_cmpgt_epi16(a, b)), s16x16, s16x16)
/*  > */               intr2(u16x16, mask_gt, _mm256_cmpgt_epi16(a, b), s16x16, s16x16)
/* >= */               intr2(u16x16, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi16(b, a)), s16x16, s16x16)
//s32x8
/*  + */               intr2(s32x8, operator+, _mm256_add_epi32  (a, b), s32x8, s32x8)
/*  - */               intr2(s32x8, operator-, _mm256_sub_epi32  (a, b), s32x8, s32x8)
/*  * */               intr2(s32x8, operator*, _mm256_mullo_epi32(a, b), s32x8, s32x8)
/*  / */ 
/*  ^ */               intr2(s32x8, operator^, _mm256_xor_si256  (a, b), s32x8, s32x8)
/*  & */               intr2(s32x8, operator&, _mm256_and_si256  (a, b), s32x8, s32x8)
/*  | */               intr2(s32x8, operator|, _mm256_or_si256   (a, b), s32x8, s32x8)
/* << */               intr2(s32x8, operator<<, _mm256_sllv_epi32(a, b), s32x8, s32x8)
/* >> */               intr2(s32x8, operator>>, _mm256_srav_epi32(a, b), s32x8, s32x8)
/* == */               intr2(u32x8, mask_eq, _mm256_cmpeq_epi32(a, b), s32x8, s32x8)
/* != */               intr2(u32x8, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi32(a, b)), s32x8, s32x8)
/*  < */               intr2(u32x8, mask_lt, _mm256_cmpgt_epi32(b, a), s32x8, s32x8)
/* <= */               intr2(u32x8, mask_le, _mm256_not_si256(_mm256_cmpgt_epi32(a, b)), s32x8, s32x8)
/*  > */               intr2(u32x8, mask_gt, _mm256_cmpgt_epi32(a, b), s32x8, s32x8)
/* >= */               intr2(u32x8, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi32(b, a)), s32x8, s32x8)
//s64x4
/*  + */               intr2(s64x4, operator+, _mm256_add_epi64(a, b), s64x4, s64x4)
/*  - */               intr2(s64x4, operator-, _mm256_sub_epi64(a, b), s64x4, s64x4)
/*  * */ 
/*  / */ 
/*  ^ */               intr2(s64x4, operator^, _mm256_xor_si256(a, b), s64x4, s64x4)
/*  & */               intr2(s64x4, operator&, _mm256_and_si256(a, b), s64x4, s64x4)
/*  | */               intr2(s64x4, operator|, _mm256_or_si256 (a, b), s64x4, s64x4)
/* << */               intr2(s64x4, operator<<, _mm256_sllv_epi64(a, b), s64x4, s64x4)
/* >> */   IF_AVX512VL(intr2(s64x4, operator>>, _mm256_srav_epi64(a, b), s64x4, s64x4))
/* == */               intr2(u64x4, mask_eq, _mm256_cmpeq_epi64(a, b), s64x4, s64x4)
/* != */               intr2(u64x4, mask_ne, _mm256_not_si256(_mm256_cmpeq_epi64(a, b)), s64x4, s64x4)
/*  < */               intr2(u64x4, mask_lt, _mm256_cmpgt_epi32(b, a), s64x4, s64x4)
/* <= */               intr2(u64x4, mask_le, _mm256_not_si256(_mm256_cmpgt_epi64(a, b)), s64x4, s64x4)
/*  > */               intr2(u64x4, mask_gt, _mm256_cmpgt_epi64(a, b), s64x4, s64x4)
/* >= */               intr2(u64x4, mask_ge, _mm256_not_si256(_mm256_cmpgt_epi32(b, a)), s64x4, s64x4)
#endif // def __AVX2__
//f32x8
/*  + */               intr2(f32x8, operator+, _mm256_add_ps (a, b), f32x8, f32x8)
/*  - */               intr2(f32x8, operator-, _mm256_sub_ps (a, b), f32x8, f32x8)
/*  * */               intr2(f32x8, operator*, _mm256_mul_ps (a, b), f32x8, f32x8)
/*  / */               intr2(f32x8, operator/, _mm256_div_ps (a, b), f32x8, f32x8)
/* == */               intr2(u32x8, mask_eq, _mm256_cmp_ps(a, b, _CMP_EQ_OQ), f32x8, f32x8)
/* != */               intr2(u32x8, mask_ne, _mm256_cmp_ps(a, b, _CMP_NEQ_OQ), f32x8, f32x8)
/*  < */               intr2(u32x8, mask_lt, _mm256_cmp_ps(a, b, _CMP_LT_OQ), f32x8, f32x8)
/* <= */               intr2(u32x8, mask_le, _mm256_cmp_ps(a, b, _CMP_LE_OQ), f32x8, f32x8)
/*  > */               intr2(u32x8, mask_gt, _mm256_cmp_ps(a, b, _CMP_GT_OQ), f32x8, f32x8)
/* >= */               intr2(u32x8, mask_ge, _mm256_cmp_ps(a, b, _CMP_GE_OQ), f32x8, f32x8)
//f64x4
/*  + */               intr2(f64x4, operator+, _mm256_add_pd (a, b), f64x4, f64x4)
/*  - */               intr2(f64x4, operator-, _mm256_sub_pd (a, b), f64x4, f64x4)
/*  * */               intr2(f64x4, operator*, _mm256_mul_pd (a, b), f64x4, f64x4)
/*  / */               intr2(f64x4, operator/, _mm256_div_pd (a, b), f64x4, f64x4)
/* == */               intr2(u64x4, mask_eq, _mm256_cmp_pd(a, b, _CMP_EQ_OQ), f64x4, f64x4)
/* != */               intr2(u64x4, mask_ne, _mm256_cmp_pd(a, b, _CMP_NEQ_OQ), f64x4, f64x4)
/*  < */               intr2(u64x4, mask_lt, _mm256_cmp_pd(a, b, _CMP_LT_OQ), f64x4, f64x4)
/* <= */               intr2(u64x4, mask_le, _mm256_cmp_pd(a, b, _CMP_LE_OQ), f64x4, f64x4)
/*  > */               intr2(u64x4, mask_gt, _mm256_cmp_pd(a, b, _CMP_GT_OQ), f64x4, f64x4)
/* >= */               intr2(u64x4, mask_ge, _mm256_cmp_pd(a, b, _CMP_GE_OQ), f64x4, f64x4)
#endif

template <> forceinline constexpr f32x4 element_cast(s32x4 a) { return bit_cast<f32x4>(_mm_cvtepi32_ps(bit_cast<__m128i>(a))); }
template <> forceinline constexpr s32x4 element_cast(f32x4 a) { return bit_cast<s32x4>(_mm_cvtps_epi32(bit_cast<__m128>(a))); }

template <std::integral Shift> forceinline constexpr u16x8 operator<<(u16x8 arr, Shift shift) { return bit_cast<u16x8>(_mm_slli_epi16(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s16x8 operator<<(s16x8 arr, Shift shift) { return bit_cast<s16x8>(_mm_slli_epi16(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u16x8 operator>>(u16x8 arr, Shift shift) { return bit_cast<u16x8>(_mm_srli_epi16(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s16x8 operator>>(s16x8 arr, Shift shift) { return bit_cast<s16x8>(_mm_srai_epi16(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u32x4 operator<<(u32x4 arr, Shift shift) { return bit_cast<u32x4>(_mm_slli_epi32(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s32x4 operator<<(s32x4 arr, Shift shift) { return bit_cast<s32x4>(_mm_slli_epi32(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u32x4 operator>>(u32x4 arr, Shift shift) { return bit_cast<u32x4>(_mm_srli_epi32(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s32x4 operator>>(s32x4 arr, Shift shift) { return bit_cast<s32x4>(_mm_srai_epi32(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u64x2 operator<<(u64x2 arr, Shift shift) { return bit_cast<u64x2>(_mm_slli_epi64(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s64x2 operator<<(s64x2 arr, Shift shift) { return bit_cast<s64x2>(_mm_slli_epi64(bit_cast<__m128i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u64x2 operator>>(u64x2 arr, Shift shift) { return bit_cast<u64x2>(_mm_srli_epi64(bit_cast<__m128i>(arr), shift)); }
IF_AVX512F(template <std::integral Shift> forceinline constexpr s64x2 operator>>(s64x2 arr, Shift shift) { return bit_cast<s64x2>(_mm_srai_epi64(bit_cast<__m128i>(arr), shift)); })
#ifdef __AVX2__
template <std::integral Shift> forceinline constexpr u16x16 operator<<(u16x16 arr, Shift shift) { return bit_cast<u16x16>(_mm256_slli_epi16(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s16x16 operator<<(s16x16 arr, Shift shift) { return bit_cast<s16x16>(_mm256_slli_epi16(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u16x16 operator>>(u16x16 arr, Shift shift) { return bit_cast<u16x16>(_mm256_srli_epi16(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s16x16 operator>>(s16x16 arr, Shift shift) { return bit_cast<s16x16>(_mm256_srai_epi16(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u32x8 operator<<(u32x8 arr, Shift shift) { return bit_cast<u32x8>(_mm256_slli_epi32(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s32x8 operator<<(s32x8 arr, Shift shift) { return bit_cast<s32x8>(_mm256_slli_epi32(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u32x8 operator>>(u32x8 arr, Shift shift) { return bit_cast<u32x8>(_mm256_srli_epi32(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s32x8 operator>>(s32x8 arr, Shift shift) { return bit_cast<s32x8>(_mm256_srai_epi32(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u64x4 operator<<(u64x4 arr, Shift shift) { return bit_cast<u64x4>(_mm256_slli_epi64(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr s64x4 operator<<(s64x4 arr, Shift shift) { return bit_cast<s64x4>(_mm256_slli_epi64(bit_cast<__m256i>(arr), shift)); }
template <std::integral Shift> forceinline constexpr u64x4 operator>>(u64x4 arr, Shift shift) { return bit_cast<u64x4>(_mm256_srli_epi64(bit_cast<__m256i>(arr), shift)); }
IF_AVX512F(template <std::integral Shift> forceinline constexpr s64x4 operator>>(s64x4 arr, Shift shift) { return bit_cast<s64x4>(_mm256_srai_epi64(bit_cast<__m256i>(arr), shift)); })
#endif

#if defined(__AVX2__) && !TL_STRICT_MATH
	intr3(f32x4, muladd, _mm_fmadd_ps(a, b, c), f32x4, f32x4, f32x4)
#else
	intr3(f32x4, muladd, _mm_add_ps(_mm_mul_ps(a, b), c), f32x4, f32x4, f32x4)
#endif
#ifdef __AVX2__
	#if !TL_STRICT_MATH
		intr3(f32x8, muladd, _mm256_fmadd_ps(a, b, c), f32x8, f32x8, f32x8)
	#else
		intr3(f32x8, muladd, _mm256_add_ps(_mm256_mul_ps(a, b), c), f32x8, f32x8, f32x8)
	#endif
#endif


       forceinline f32x4 min(f32x4 a, f32x4 b) { return bit_cast<f32x4>(_mm_min_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
       forceinline f32x4 max(f32x4 a, f32x4 b) { return bit_cast<f32x4>(_mm_max_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
	   forceinline f64x2 min(f64x2 a, f64x2 b) { return bit_cast<f64x2>(_mm_min_pd(bit_cast<__m128d>(a), bit_cast<__m128d>(b))); }
	   forceinline f64x2 max(f64x2 a, f64x2 b) { return bit_cast<f64x2>(_mm_max_pd(bit_cast<__m128d>(a), bit_cast<__m128d>(b))); }
IF_AVX(forceinline f32x8 min(f32x8 a, f32x8 b) { return bit_cast<f32x8>(_mm256_min_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); })
IF_AVX(forceinline f32x8 max(f32x8 a, f32x8 b) { return bit_cast<f32x8>(_mm256_max_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); })
IF_AVX(forceinline f64x4 min(f64x4 a, f64x4 b) { return bit_cast<f64x4>(_mm256_min_pd(bit_cast<__m256d>(a), bit_cast<__m256d>(b))); })
IF_AVX(forceinline f64x4 max(f64x4 a, f64x4 b) { return bit_cast<f64x4>(_mm256_max_pd(bit_cast<__m256d>(a), bit_cast<__m256d>(b))); })

#define blenddef(elem_bits, count, intr, __m)                                                                  \
	template<class T>                                                                                          \
		requires (sizeof(T) == elem_bits/8)                                                                    \
	forceinline Array<T, count> blend(Array<u##elem_bits, count> mask, Array<T, count> a, Array<T, count> b) { \
		return bit_cast<Array<T, count>>(intr(bit_cast<__m>(b), bit_cast<__m>(a), bit_cast<__m>(mask)));       \
	}
        blenddef(8,  16, _mm_blendv_epi8, __m128i)
        blenddef(32,  4, _mm_blendv_ps,   __m128 )
        blenddef(64,  2, _mm_blendv_pd,   __m128d)
IF_AVX (blenddef(8,  32, _mm256_blendv_epi8, __m256i))
IF_AVX (blenddef(32,  8, _mm256_blendv_ps,   __m256 ))
IF_AVX2(blenddef(64,  4, _mm256_blendv_pd,   __m256d))
#undef blenddef

template <class T, umm count>
	requires (count * sizeof(T) == 16)
forceinline constexpr Array<T, count> pshufb(Array<T, count> a, s8x16 indices) {
	return bit_cast<Array<T, count>>(_mm_shuffle_epi8(bit_cast<__m128i>(a), bit_cast<__m128i>(indices)));
}

        template <> forceinline constexpr u64 mask_to_int(u8x16 mask) { return _mm_movemask_epi8 (bit_cast<__m128i>(mask)); }
        template <> forceinline constexpr u64 mask_to_int(u32x4 mask) { return _mm_movemask_ps(bit_cast<__m128 >(mask)); }
        template <> forceinline constexpr u64 mask_to_int(u64x2 mask) { return _mm_movemask_pd(bit_cast<__m128d>(mask)); }
IF_AVX2(template <> forceinline constexpr u64 mask_to_int(u8x32 mask) { return _mm256_movemask_epi8 (bit_cast<__m256i>(mask)); })
IF_AVX2(template <> forceinline constexpr u64 mask_to_int(u32x8 mask) { return _mm256_movemask_ps(bit_cast<__m256 >(mask)); })
IF_AVX2(template <> forceinline constexpr u64 mask_to_int(u64x4 mask) { return _mm256_movemask_pd(bit_cast<__m256d>(mask)); })

#ifdef __AVX2__

template <class T, umm count>
	requires (count * sizeof(T) == 32)
forceinline constexpr Array<T, count> pshufb(Array<T, count> a, s8x32 indices) {
	return bit_cast<Array<T, count>>(_mm256_shuffle_epi8(bit_cast<__m256i>(a), bit_cast<__m256i>(indices)));
}

#endif

#undef IF_AVX
#undef IF_AVX2
#undef IF_AVX512F
#undef IF_AVX512BW
#undef IF_AVX512DQ
#undef IF_AVX512VL

#endif

}

#define TL_ARRAY_H_INCLUDED

#ifdef TL_HASH_H_INCLUDED
#include "array-hash.h"
#endif