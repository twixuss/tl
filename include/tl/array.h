#pragma once
#include "common.h"

namespace tl {

template <class T>
inline static constexpr bool is_array = false;

template <class T, umm count_>
struct Array {
	inline static constexpr umm count = count_;
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

template <class T, class... Rest>
Array(T, Rest...) -> Array<typename RequireAllSame<T, Rest...>::Type, 1 + sizeof...(Rest)>;

template <class T, umm count>
inline static constexpr bool is_array<Array<T, count>> = true;

template <class T, umm count>
inline static constexpr bool is_unsigned<Array<T, count>> = is_unsigned<T>;

template <class T, umm count>
inline static constexpr bool is_integer_like<Array<T, count>> = is_integer_like<T>;

template <class T>            inline static constexpr int array_nestedness = 0;
template <class T, umm count> inline static constexpr int array_nestedness<Array<T, count>> = array_nestedness<T> + 1;

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

#define OP(op)                                                                 \
	template <class T, umm count>                                              \
	forceinline constexpr auto operator op(Array<T, count> const &a)           \
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

#define ASIS(...) __VA_ARGS__
#define WRAP_ToBoolUsingAll(...) ToBoolUsingAll<__VA_ARGS__>
#define WRAP_ToBoolUsingAny(...) ToBoolUsingAny<__VA_ARGS__>

#define OP(op, RetTypeMod)                                                                           \
	template <class T, umm count>                                                                    \
	forceinline constexpr auto operator op(Array<T, count> const &a, Array<T, count> const &b)       \
		requires requires(T t) { t op t; }                                                           \
	{                                                                                                \
		RetTypeMod(Array<std::remove_cvref_t<decltype(a.data[0] op b.data[0])>, count>) result = {}; \
		for (umm i = 0; i < count; ++i)                                                              \
			result.data[i] = a.data[i] op b.data[i];                                                 \
		return result;                                                                               \
	}                                                                                                \
	template <class U, class T, umm count>                                                           \
	forceinline constexpr auto operator op(Array<T, count> const &a, Array<U, count> const &b)       \
		requires requires(T t, U u) { t op u; }                                                      \
	{                                                                                                \
		RetTypeMod(Array<std::remove_cvref_t<decltype(a.data[0] op b.data[0])>, count>) result = {}; \
		for (umm i = 0; i < count; ++i)                                                              \
			result.data[i] = a.data[i] op b.data[i];                                                 \
		return result;                                                                               \
	}                                                                                                \
	template <class U, class T, umm count>                                                           \
	forceinline constexpr auto operator op(Array<T, count> const &a, U const &b)                     \
		requires requires(T t, U u) { t op u; } && (array_nestedness<U> <= array_nestedness<T>)      \
	{                                                                                                \
		RetTypeMod(Array<std::remove_cvref_t<decltype(a.data[0] op b)>, count>) result = {};         \
		for (umm i = 0; i < count; ++i)                                                              \
			result.data[i] = a.data[i] op b;                                                         \
		return result;                                                                               \
	}                                                                                                \
	template <class U, class T, umm count>                                                           \
	forceinline constexpr auto operator op(U const &a, Array<T, count> const &b)                     \
		requires requires(U u, T t) { u op t; } && (array_nestedness<U> <= array_nestedness<T>)      \
	{                                                                                                \
		RetTypeMod(Array<std::remove_cvref_t<decltype(a op b.data[0])>, count>) result = {};         \
		for (umm i = 0; i < count; ++i)                                                              \
			result.data[i] = a op b.data[i];                                                         \
		return result;                                                                               \
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

#define OP(op)                                                                               \
	template <class U, class T, umm count>                                                   \
	forceinline constexpr auto &operator op##=(Array<T, count> &a, Array<U, count> const &b) \
		requires requires(T t, U u) { t op##= u; }                                           \
	{                                                                                        \
		for (umm i = 0; i < count; ++i)                                                      \
			a.data[i] op##= b.data[i];                                                       \
		return a;                                                                            \
	}                                                                                        \
	template <class U, class T, umm count>                                     \
	forceinline constexpr auto &operator op##=(Array<T, count> &a, U const &b) \
		requires requires(T t, U u) { t op##= u; }                             \
	{                                                                          \
		for (umm i = 0; i < count; ++i)                                        \
			a.data[i] op##= b;                                                 \
		return a;                                                              \
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

template <umm count, class T>
forceinline constexpr Array<T, count> broadcast_to_array(T value) {
	Array<T, count> result = {};
	for (umm i = 0; i < count; ++i) {
		result.data[i] = value;
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
forceinline constexpr umm count_of(Array<T, count> const &) {
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
forceinline constexpr bool all(Array<T, count> const &a) {
	for (umm i = 0; i < count; ++i) {
		if (!all(a.data[i])) {
			return false;
		}
	}
	return true;
}

template <class T, umm count>
forceinline constexpr bool any(Array<T, count> const &a) {
	for (umm i = 0; i < count; ++i) {
		if (any(a.data[i])) {
			return true;
		}
	}
	return false;
}
template <class T, umm count> forceinline constexpr bool all(ToBoolUsingAll<Array<T, count>> const &a) { return all((Array<T, count> &)a); }
template <class T, umm count> forceinline constexpr bool all(ToBoolUsingAny<Array<T, count>> const &a) { return all((Array<T, count> &)a); }
template <class T, umm count> forceinline constexpr bool any(ToBoolUsingAll<Array<T, count>> const &a) { return any((Array<T, count> &)a); }
template <class T, umm count> forceinline constexpr bool any(ToBoolUsingAny<Array<T, count>> const &a) { return any((Array<T, count> &)a); }

template <class T, umm count>
forceinline constexpr T dot(Array<T, count> const &a, Array<T, count> const &b) {
	T result = {};
	for (umm i = 0; i < count; ++i) {
		result += a.data[i] * b.data[i];
	}
	return result;
}

template <class T>
forceinline constexpr Array<T, 3> cross(Array<T, 3> const &a, Array<T, 3> const &b) {
	return a.yzx() * b.zxy() - a.zxy() * b.yzx();
}

template <class T, class Index, umm count>
forceinline constexpr Array<T, count> gather(T const *pointer, Array<Index, count> const &indices) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = pointer[indices.data[i]];
	return r;
}

#define OP(NAME, OPERATOR)                                                         \
	template <class T>                                                             \
	forceinline constexpr auto mask_##NAME(T a, T b) {                             \
		static_assert(sizeof(UintWithBits<sizeof(T)*8>) == sizeof(T));             \
		return (UintWithBits<sizeof(T)*8>)(a OPERATOR b ? -1 : 0);                 \
	}                                                                              \
	template <class T, umm count>                                                  \
	forceinline constexpr auto mask_##NAME(Array<T, count> a, Array<T, count> b) { \
		Array<UintWithBits<sizeof(T)*8>, count> r = {};                            \
		static_assert(sizeof(r) == sizeof(Array<T, count>));                       \
		for (umm i = 0; i < count; ++i)                                            \
			r.data[i] = mask_##NAME(a.data[i], b.data[i]);                         \
		return r;                                                                  \
	}

OP(equal, ==)
OP(not_equal, !=)
OP(less, <)
OP(less_equal, <=)
OP(greater, >)
OP(greater_equal, >=)

#undef OP

// Mask is not an integral, e.g. Array
template <class T, class Mask, umm count>
forceinline constexpr Array<T, count> blend(Array<Mask, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = blend(mask.data[i], a.data[i], b.data[i]);
	return r;
}

// Mask is an integral
template <class T, std::integral Mask, umm count>
	requires (sizeof(T) == sizeof(Mask))
forceinline constexpr Array<T, count> blend(Array<Mask, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	using MaskInt = UintWithBits<sizeof(Mask) * 8>;
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i) {
		MaskInt mask_int = bit_cast<MaskInt>(mask.data[i]);
		r.data[i] = (mask_int >> (sizeof(Mask)*8-1)) ? a.data[i] : b.data[i];
	}
	return r;
}


template <class Mask, class T, umm count>
forceinline constexpr Array<T, count> select(Array<Mask, count> const &mask, Array<T, count> const &a, Array<T, count> const &b) {
	Array<T, count> r = {};
	for (umm i = 0; i < count; ++i)
		r.data[i] = select(mask.data[i], a.data[i], b.data[i]);
	return r;
}

template <class U, class T, umm count>
	requires(count * sizeof(T) == count * sizeof(T) / sizeof(U) * sizeof(U) && !std::is_same_v<U, T>)
forceinline constexpr Array<U, count * sizeof(T) / sizeof(U)> reinterpret(Array<T, count> const &a) {
	return bit_cast<Array<U, count * sizeof(T) / sizeof(U)>>(a);
}

template <class T, umm count, class Index, umm indices_count>
forceinline constexpr Array<T, count> shuffle(Array<T, count> const &a, Array<Index, indices_count> const &indices) {
	Array<T, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = a.data[indices.data[i % indices_count] % count];
	return r;
}

template <class T, umm count, class Index>
forceinline constexpr Array<T, count> shuffle(Array<T, count> const &a, Array<Index, count> const &indices) {
	Array<T, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = a.data[indices.data[i] % count];
	return r;
}

template <umm dups, class T, umm count>
forceinline constexpr Array<T, count * dups> dup(Array<T, count> const &a) {
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
forceinline constexpr Array<U, count> element_cast(Array<T, count> a) {
	Array<U, count> r;
	for (umm i = 0; i < count; ++i)
		r.data[i] = convert<U>(a.data[i]);
	return r;
}


#if TL_USE_SIMD

forceinline constexpr Array<u32, 4> operator+(Array<u32, 4> a, Array<u32, 4> b) { return bit_cast<Array<u32, 4>>(_mm_add_epi32  (bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<u32, 4> operator-(Array<u32, 4> a, Array<u32, 4> b) { return bit_cast<Array<u32, 4>>(_mm_sub_epi32  (bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<u32, 4> operator*(Array<u32, 4> a, Array<u32, 4> b) { return bit_cast<Array<u32, 4>>(_mm_mullo_epi32(bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<u32, 4> operator^(Array<u32, 4> a, Array<u32, 4> b) { return bit_cast<Array<u32, 4>>(_mm_xor_si128(bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<u32, 4> operator&(Array<u32, 4> a, Array<u32, 4> b) { return bit_cast<Array<u32, 4>>(_mm_and_si128(bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<u32, 4> operator|(Array<u32, 4> a, Array<u32, 4> b) { return bit_cast<Array<u32, 4>>(_mm_or_si128 (bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }

forceinline constexpr Array<s32, 4> operator+(Array<s32, 4> a, Array<s32, 4> b) { return bit_cast<Array<s32, 4>>(_mm_add_epi32  (bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<s32, 4> operator-(Array<s32, 4> a, Array<s32, 4> b) { return bit_cast<Array<s32, 4>>(_mm_sub_epi32  (bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<s32, 4> operator*(Array<s32, 4> a, Array<s32, 4> b) { return bit_cast<Array<s32, 4>>(_mm_mullo_epi32(bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<s32, 4> operator^(Array<s32, 4> a, Array<s32, 4> b) { return bit_cast<Array<s32, 4>>(_mm_xor_si128(bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<s32, 4> operator&(Array<s32, 4> a, Array<s32, 4> b) { return bit_cast<Array<s32, 4>>(_mm_and_si128(bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }
forceinline constexpr Array<s32, 4> operator|(Array<s32, 4> a, Array<s32, 4> b) { return bit_cast<Array<s32, 4>>(_mm_or_si128 (bit_cast<__m128i>(a), bit_cast<__m128i>(b))); }

forceinline constexpr Array<f32, 4> operator+(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_add_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline constexpr Array<f32, 4> operator-(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_sub_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline constexpr Array<f32, 4> operator*(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_mul_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline constexpr Array<f32, 4> operator/(Array<f32, 4> a, Array<f32, 4> b) { return bit_cast<Array<f32, 4>>(_mm_div_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }

template <> forceinline constexpr Array<f32, 4> element_cast(Array<s32, 4> a) { return bit_cast<Array<f32, 4>>(_mm_cvtepi32_ps(bit_cast<__m128i>(a))); }
template <> forceinline constexpr Array<s32, 4> element_cast(Array<f32, 4> a) { return bit_cast<Array<s32, 4>>(_mm_cvtps_epi32(bit_cast<__m128>(a))); }

template <int shift> forceinline constexpr Array<u32, 4> shift_left(Array<u32, 4> arr) { return bit_cast<Array<u32, 4>>(_mm_slli_epi32(bit_cast<__m128i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 4> shift_left(Array<s32, 4> arr) { return bit_cast<Array<s32, 4>>(_mm_slli_epi32(bit_cast<__m128i>(arr), shift)); }
template <int shift> forceinline constexpr Array<u32, 4> shift_right(Array<u32, 4> arr) { return bit_cast<Array<u32, 4>>(_mm_srli_epi32(bit_cast<__m128i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 4> shift_right(Array<s32, 4> arr) { return bit_cast<Array<s32, 4>>(_mm_srai_epi32(bit_cast<__m128i>(arr), shift)); }

template <>
forceinline auto lerp(Array<f32, 4> a_, Array<f32, 4> b_, Array<f32, 4> t_) {
	__m128 a = bit_cast<__m128>(a_);
	__m128 b = bit_cast<__m128>(b_);
	__m128 t = bit_cast<__m128>(t_);

	#ifdef __AVX__
	__m128 r = _mm_fmadd_ps(_mm_sub_ps(b, a), t, a);
	#else
	__m128 r = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(b, a), t), a);
	#endif

	return bit_cast<Array<f32, 4>>(r);
}

forceinline Array<f32, 4> min(Array<f32, 4> const &a, Array<f32, 4> const &b) { return bit_cast<Array<f32, 4>>(_mm_min_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }
forceinline Array<f32, 4> max(Array<f32, 4> const &a, Array<f32, 4> const &b) { return bit_cast<Array<f32, 4>>(_mm_max_ps(bit_cast<__m128>(a), bit_cast<__m128>(b))); }

template<class T>
	requires (sizeof(T) == 1)
forceinline Array<T, 16> blend(Array<u8, 16> mask, Array<T, 16> a, Array<T, 16> b) {
	return bit_cast<Array<T, 16>>(_mm_blendv_epi8(bit_cast<__m128i>(b), bit_cast<__m128i>(a), bit_cast<__m128i>(mask)));
}

template<class T>
	requires (sizeof(T) == 4)
forceinline Array<T, 4> blend(Array<u32, 4> mask, Array<T, 4> a, Array<T, 4> b) {
	return bit_cast<Array<T, 4>>(_mm_blendv_ps(bit_cast<__m128>(b), bit_cast<__m128>(a), bit_cast<__m128>(mask)));
}

template<class T>
	requires (sizeof(T) == 8)
forceinline Array<T, 2> blend(Array<u64, 2> mask, Array<T, 2> a, Array<T, 2> b) {
	return bit_cast<Array<T, 2>>(_mm_blendv_pd(bit_cast<__m128d>(b), bit_cast<__m128d>(a), bit_cast<__m128d>(mask)));
}

template <class T, umm count>
	requires (count * sizeof(T) == 16)
forceinline constexpr Array<T, count> pshufb(Array<T, count> const &a, Array<s8, 16> const &indices) {
	return bit_cast<Array<T, count>>(_mm_shuffle_epi8(bit_cast<__m128i>(a), bit_cast<__m128i>(indices)));
}

#ifdef __AVX__

forceinline constexpr Array<f32, 8> operator+(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_add_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline constexpr Array<f32, 8> operator-(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_sub_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline constexpr Array<f32, 8> operator*(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_mul_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline constexpr Array<f32, 8> operator/(Array<f32, 8> a, Array<f32, 8> b) { return bit_cast<Array<f32, 8>>(_mm256_div_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }

template <>
forceinline auto lerp(Array<f32, 8> a_, Array<f32, 8> b_, Array<f32, 8> t_) {
	__m256 a = bit_cast<__m256>(a_);
	__m256 b = bit_cast<__m256>(b_);
	__m256 t = bit_cast<__m256>(t_);

	__m256 r = _mm256_fmadd_ps(_mm256_sub_ps(b, a), t, a);

	return bit_cast<Array<f32, 8>>(r);
}

forceinline Array<f32, 8> min(Array<f32, 8> const &a, Array<f32, 8> const &b) { return bit_cast<Array<f32, 8>>(_mm256_min_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }
forceinline Array<f32, 8> max(Array<f32, 8> const &a, Array<f32, 8> const &b) { return bit_cast<Array<f32, 8>>(_mm256_max_ps(bit_cast<__m256>(a), bit_cast<__m256>(b))); }

#endif

#ifdef __AVX2__

forceinline constexpr Array<u32, 8> operator+(Array<u32, 8> a, Array<u32, 8> b) { return bit_cast<Array<u32, 8>>(_mm256_add_epi32  (bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<u32, 8> operator-(Array<u32, 8> a, Array<u32, 8> b) { return bit_cast<Array<u32, 8>>(_mm256_sub_epi32  (bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<u32, 8> operator*(Array<u32, 8> a, Array<u32, 8> b) { return bit_cast<Array<u32, 8>>(_mm256_mullo_epi32(bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<u32, 8> operator^(Array<u32, 8> a, Array<u32, 8> b) { return bit_cast<Array<u32, 8>>(_mm256_xor_si256(bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<u32, 8> operator&(Array<u32, 8> a, Array<u32, 8> b) { return bit_cast<Array<u32, 8>>(_mm256_and_si256(bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<u32, 8> operator|(Array<u32, 8> a, Array<u32, 8> b) { return bit_cast<Array<u32, 8>>(_mm256_or_si256 (bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }

forceinline constexpr Array<s32, 8> operator+(Array<s32, 8> a, Array<s32, 8> b) { return bit_cast<Array<s32, 8>>(_mm256_add_epi32  (bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<s32, 8> operator-(Array<s32, 8> a, Array<s32, 8> b) { return bit_cast<Array<s32, 8>>(_mm256_sub_epi32  (bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<s32, 8> operator*(Array<s32, 8> a, Array<s32, 8> b) { return bit_cast<Array<s32, 8>>(_mm256_mullo_epi32(bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<s32, 8> operator^(Array<s32, 8> a, Array<s32, 8> b) { return bit_cast<Array<s32, 8>>(_mm256_xor_si256(bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<s32, 8> operator&(Array<s32, 8> a, Array<s32, 8> b) { return bit_cast<Array<s32, 8>>(_mm256_and_si256(bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }
forceinline constexpr Array<s32, 8> operator|(Array<s32, 8> a, Array<s32, 8> b) { return bit_cast<Array<s32, 8>>(_mm256_or_si256 (bit_cast<__m256i>(a), bit_cast<__m256i>(b))); }

template <int shift> forceinline constexpr Array<u32, 8> shift_left(Array<u32, 8> arr) { return bit_cast<Array<u32, 8>>(_mm256_slli_epi32(bit_cast<__m256i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 8> shift_left(Array<s32, 8> arr) { return bit_cast<Array<s32, 8>>(_mm256_slli_epi32(bit_cast<__m256i>(arr), shift)); }
template <int shift> forceinline constexpr Array<u32, 8> shift_right(Array<u32, 8> arr) { return bit_cast<Array<u32, 8>>(_mm256_srli_epi32(bit_cast<__m256i>(arr), shift)); }
template <int shift> forceinline constexpr Array<s32, 8> shift_right(Array<s32, 8> arr) { return bit_cast<Array<s32, 8>>(_mm256_srai_epi32(bit_cast<__m256i>(arr), shift)); }

forceinline constexpr Array<u32, 4> shift_left(Array<u32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<u32, 4>>(_mm_sllv_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }
forceinline constexpr Array<s32, 4> shift_left(Array<s32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<s32, 4>>(_mm_sllv_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }
forceinline constexpr Array<u32, 4> shift_right(Array<u32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<u32, 4>>(_mm_srlv_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }
forceinline constexpr Array<s32, 4> shift_right(Array<s32, 4> arr, Array<u32, 4> shift) { return bit_cast<Array<s32, 4>>(_mm_srav_epi32(bit_cast<__m128i>(arr), bit_cast<__m128i>(shift))); }

forceinline constexpr Array<u32, 8> shift_left(Array<u32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<u32, 8>>(_mm256_sllv_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }
forceinline constexpr Array<s32, 8> shift_left(Array<s32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<s32, 8>>(_mm256_sllv_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }
forceinline constexpr Array<u32, 8> shift_right(Array<u32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<u32, 8>>(_mm256_srlv_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }
forceinline constexpr Array<s32, 8> shift_right(Array<s32, 8> arr, Array<u32, 8> shift) { return bit_cast<Array<s32, 8>>(_mm256_srav_epi32(bit_cast<__m256i>(arr), bit_cast<__m256i>(shift))); }

template<class T>
	requires (sizeof(T) == 1)
forceinline Array<T, 32> blend(Array<u8, 32> mask, Array<T, 32> a, Array<T, 32> b) {
	return bit_cast<Array<T, 32>>(_mm256_blendv_epi8(bit_cast<__m256i>(b), bit_cast<__m256i>(a), bit_cast<__m256i>(mask)));
}

template<class T>
	requires (sizeof(T) == 4)
forceinline Array<T, 8> blend(Array<u32, 8> mask, Array<T, 8> a, Array<T, 8> b) {
	return bit_cast<Array<T, 8>>(_mm256_blendv_ps(bit_cast<__m256>(b), bit_cast<__m256>(a), bit_cast<__m256>(mask)));
}

template<class T>
	requires (sizeof(T) == 8)
forceinline Array<T, 4> blend(Array<u64, 4> mask, Array<T, 4> a, Array<T, 4> b) {
	return bit_cast<Array<T, 4>>(_mm256_blendv_pd(bit_cast<__m256d>(b), bit_cast<__m256d>(a), bit_cast<__m256d>(mask)));
}

template <class T, umm count>
	requires (count * sizeof(T) == 32)
forceinline constexpr Array<T, count> pshufb(Array<T, count> const &a, Array<s8, 32> const &indices) {
	return bit_cast<Array<T, count>>(_mm256_shuffle_epi8(bit_cast<__m256i>(a), bit_cast<__m256i>(indices)));
}

#endif

#endif

}

#define TL_ARRAY_H_INCLUDED

#ifdef TL_HASH_H_INCLUDED
#include "array-hash.h"
#endif