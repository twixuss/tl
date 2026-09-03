#pragma once
#include "common.h"

namespace tl {

template <class T>
inline static constexpr bool is_array = false;

#define ENUMERATE_SWIZZLES_NOCONSEC_2(x2, x3, x4) \
	x2(x,x)         \
	x2(y,x) x2(y,y) \
	x3(x,x,x) x3(x,x,y) x3(x,y,x) x3(x,y,y) \
	x3(y,x,x) x3(y,x,y) x3(y,y,x) x3(y,y,y) \
	x4(x,x,x,x) x4(x,x,x,y) x4(x,x,y,x) x4(x,x,y,y) x4(x,y,x,x) x4(x,y,x,y) x4(x,y,y,x) x4(x,y,y,y) \
	x4(y,x,x,x) x4(y,x,x,y) x4(y,x,y,x) x4(y,x,y,y) x4(y,y,x,x) x4(y,y,x,y) x4(y,y,y,x) x4(y,y,y,y) \

#define ENUMERATE_SWIZZLES_NOCONSEC_3(x2, x3, x4) \
	x2(x,x)         x2(x,z) \
	x2(y,x) x2(y,y)         \
	x2(z,x) x2(z,y) x2(z,z) \
	x3(x,x,x) x3(x,x,y) x3(x,x,z) x3(x,y,x) x3(x,y,y)           x3(x,z,x) x3(x,z,y) x3(x,z,z) \
	x3(y,x,x) x3(y,x,y) x3(y,x,z) x3(y,y,x) x3(y,y,y) x3(y,y,z) x3(y,z,x) x3(y,z,y) x3(y,z,z) \
	x3(z,x,x) x3(z,x,y) x3(z,x,z) x3(z,y,x) x3(z,y,y) x3(z,y,z) x3(z,z,x) x3(z,z,y) x3(z,z,z) \
	x4(x,x,x,x) x4(x,x,x,y) x4(x,x,x,z) x4(x,x,y,x) x4(x,x,y,y) x4(x,x,y,z) x4(x,x,z,x) x4(x,x,z,y) x4(x,x,z,z) \
	x4(x,y,x,x) x4(x,y,x,y) x4(x,y,x,z) x4(x,y,y,x) x4(x,y,y,y) x4(x,y,y,z) x4(x,y,z,x) x4(x,y,z,y) x4(x,y,z,z) \
	x4(x,z,x,x) x4(x,z,x,y) x4(x,z,x,z) x4(x,z,y,x) x4(x,z,y,y) x4(x,z,y,z) x4(x,z,z,x) x4(x,z,z,y) x4(x,z,z,z) \
	x4(y,x,x,x) x4(y,x,x,y) x4(y,x,x,z) x4(y,x,y,x) x4(y,x,y,y) x4(y,x,y,z) x4(y,x,z,x) x4(y,x,z,y) x4(y,x,z,z) \
	x4(y,y,x,x) x4(y,y,x,y) x4(y,y,x,z) x4(y,y,y,x) x4(y,y,y,y) x4(y,y,y,z) x4(y,y,z,x) x4(y,y,z,y) x4(y,y,z,z) \
	x4(y,z,x,x) x4(y,z,x,y) x4(y,z,x,z) x4(y,z,y,x) x4(y,z,y,y) x4(y,z,y,z) x4(y,z,z,x) x4(y,z,z,y) x4(y,z,z,z) \
	x4(z,x,x,x) x4(z,x,x,y) x4(z,x,x,z) x4(z,x,y,x) x4(z,x,y,y) x4(z,x,y,z) x4(z,x,z,x) x4(z,x,z,y) x4(z,x,z,z) \
	x4(z,y,x,x) x4(z,y,x,y) x4(z,y,x,z) x4(z,y,y,x) x4(z,y,y,y) x4(z,y,y,z) x4(z,y,z,x) x4(z,y,z,y) x4(z,y,z,z) \
	x4(z,z,x,x) x4(z,z,x,y) x4(z,z,x,z) x4(z,z,y,x) x4(z,z,y,y) x4(z,z,y,z) x4(z,z,z,x) x4(z,z,z,y) x4(z,z,z,z) \

	
#define ENUMERATE_SWIZZLES_NOCONSEC_4(x2, x3, x4) \
	x2(x,x)         x2(x,z) x2(x,w) \
	x2(y,x) x2(y,y)         x2(y,w) \
	x2(z,x) x2(z,y) x2(z,z)         \
	x2(w,x) x2(w,y) x2(w,z) x2(w,w) \
	x3(x,x,x) x3(x,x,y) x3(x,x,z) x3(x,x,w) x3(x,y,x) x3(x,y,y)           x3(x,y,w) x3(x,z,x) x3(x,z,y) x3(x,z,z) x3(x,z,w) x3(x,w,x) x3(x,w,y) x3(x,w,z) x3(x,w,w) \
	x3(y,x,x) x3(y,x,y) x3(y,x,z) x3(y,x,w) x3(y,y,x) x3(y,y,y) x3(y,y,z) x3(y,y,w) x3(y,z,x) x3(y,z,y) x3(y,z,z)           x3(y,w,x) x3(y,w,y) x3(y,w,z) x3(y,w,w) \
	x3(z,x,x) x3(z,x,y) x3(z,x,z) x3(z,x,w) x3(z,y,x) x3(z,y,y) x3(z,y,z) x3(z,y,w) x3(z,z,x) x3(z,z,y) x3(z,z,z) x3(z,z,w) x3(z,w,x) x3(z,w,y) x3(z,w,z) x3(z,w,w) \
	x3(w,x,x) x3(w,x,y) x3(w,x,z) x3(w,x,w) x3(w,y,x) x3(w,y,y) x3(w,y,z) x3(w,y,w) x3(w,z,x) x3(w,z,y) x3(w,z,z) x3(w,z,w) x3(w,w,x) x3(w,w,y) x3(w,w,z) x3(w,w,w) \
	x4(x,x,x,x) x4(x,x,x,y) x4(x,x,x,z) x4(x,x,x,w) x4(x,x,y,x) x4(x,x,y,y) x4(x,x,y,z) x4(x,x,y,w) x4(x,x,z,x) x4(x,x,z,y) x4(x,x,z,z) x4(x,x,z,w) x4(x,x,w,x) x4(x,x,w,y) x4(x,x,w,z) x4(x,x,w,w) \
	x4(x,y,x,x) x4(x,y,x,y) x4(x,y,x,z) x4(x,y,x,w) x4(x,y,y,x) x4(x,y,y,y) x4(x,y,y,z) x4(x,y,y,w) x4(x,y,z,x) x4(x,y,z,y) x4(x,y,z,z)             x4(x,y,w,x) x4(x,y,w,y) x4(x,y,w,z) x4(x,y,w,w) \
	x4(x,z,x,x) x4(x,z,x,y) x4(x,z,x,z) x4(x,z,x,w) x4(x,z,y,x) x4(x,z,y,y) x4(x,z,y,z) x4(x,z,y,w) x4(x,z,z,x) x4(x,z,z,y) x4(x,z,z,z) x4(x,z,z,w) x4(x,z,w,x) x4(x,z,w,y) x4(x,z,w,z) x4(x,z,w,w) \
	x4(x,w,x,x) x4(x,w,x,y) x4(x,w,x,z) x4(x,w,x,w) x4(x,w,y,x) x4(x,w,y,y) x4(x,w,y,z) x4(x,w,y,w) x4(x,w,z,x) x4(x,w,z,y) x4(x,w,z,z) x4(x,w,z,w) x4(x,w,w,x) x4(x,w,w,y) x4(x,w,w,z) x4(x,w,w,w) \
	x4(y,x,x,x) x4(y,x,x,y) x4(y,x,x,z) x4(y,x,x,w) x4(y,x,y,x) x4(y,x,y,y) x4(y,x,y,z) x4(y,x,y,w) x4(y,x,z,x) x4(y,x,z,y) x4(y,x,z,z) x4(y,x,z,w) x4(y,x,w,x) x4(y,x,w,y) x4(y,x,w,z) x4(y,x,w,w) \
	x4(y,y,x,x) x4(y,y,x,y) x4(y,y,x,z) x4(y,y,x,w) x4(y,y,y,x) x4(y,y,y,y) x4(y,y,y,z) x4(y,y,y,w) x4(y,y,z,x) x4(y,y,z,y) x4(y,y,z,z) x4(y,y,z,w) x4(y,y,w,x) x4(y,y,w,y) x4(y,y,w,z) x4(y,y,w,w) \
	x4(y,z,x,x) x4(y,z,x,y) x4(y,z,x,z) x4(y,z,x,w) x4(y,z,y,x) x4(y,z,y,y) x4(y,z,y,z) x4(y,z,y,w) x4(y,z,z,x) x4(y,z,z,y) x4(y,z,z,z) x4(y,z,z,w) x4(y,z,w,x) x4(y,z,w,y) x4(y,z,w,z) x4(y,z,w,w) \
	x4(y,w,x,x) x4(y,w,x,y) x4(y,w,x,z) x4(y,w,x,w) x4(y,w,y,x) x4(y,w,y,y) x4(y,w,y,z) x4(y,w,y,w) x4(y,w,z,x) x4(y,w,z,y) x4(y,w,z,z) x4(y,w,z,w) x4(y,w,w,x) x4(y,w,w,y) x4(y,w,w,z) x4(y,w,w,w) \
	x4(z,x,x,x) x4(z,x,x,y) x4(z,x,x,z) x4(z,x,x,w) x4(z,x,y,x) x4(z,x,y,y) x4(z,x,y,z) x4(z,x,y,w) x4(z,x,z,x) x4(z,x,z,y) x4(z,x,z,z) x4(z,x,z,w) x4(z,x,w,x) x4(z,x,w,y) x4(z,x,w,z) x4(z,x,w,w) \
	x4(z,y,x,x) x4(z,y,x,y) x4(z,y,x,z) x4(z,y,x,w) x4(z,y,y,x) x4(z,y,y,y) x4(z,y,y,z) x4(z,y,y,w) x4(z,y,z,x) x4(z,y,z,y) x4(z,y,z,z) x4(z,y,z,w) x4(z,y,w,x) x4(z,y,w,y) x4(z,y,w,z) x4(z,y,w,w) \
	x4(z,z,x,x) x4(z,z,x,y) x4(z,z,x,z) x4(z,z,x,w) x4(z,z,y,x) x4(z,z,y,y) x4(z,z,y,z) x4(z,z,y,w) x4(z,z,z,x) x4(z,z,z,y) x4(z,z,z,z) x4(z,z,z,w) x4(z,z,w,x) x4(z,z,w,y) x4(z,z,w,z) x4(z,z,w,w) \
	x4(z,w,x,x) x4(z,w,x,y) x4(z,w,x,z) x4(z,w,x,w) x4(z,w,y,x) x4(z,w,y,y) x4(z,w,y,z) x4(z,w,y,w) x4(z,w,z,x) x4(z,w,z,y) x4(z,w,z,z) x4(z,w,z,w) x4(z,w,w,x) x4(z,w,w,y) x4(z,w,w,z) x4(z,w,w,w) \
	x4(w,x,x,x) x4(w,x,x,y) x4(w,x,x,z) x4(w,x,x,w) x4(w,x,y,x) x4(w,x,y,y) x4(w,x,y,z) x4(w,x,y,w) x4(w,x,z,x) x4(w,x,z,y) x4(w,x,z,z) x4(w,x,z,w) x4(w,x,w,x) x4(w,x,w,y) x4(w,x,w,z) x4(w,x,w,w) \
	x4(w,y,x,x) x4(w,y,x,y) x4(w,y,x,z) x4(w,y,x,w) x4(w,y,y,x) x4(w,y,y,y) x4(w,y,y,z) x4(w,y,y,w) x4(w,y,z,x) x4(w,y,z,y) x4(w,y,z,z) x4(w,y,z,w) x4(w,y,w,x) x4(w,y,w,y) x4(w,y,w,z) x4(w,y,w,w) \
	x4(w,z,x,x) x4(w,z,x,y) x4(w,z,x,z) x4(w,z,x,w) x4(w,z,y,x) x4(w,z,y,y) x4(w,z,y,z) x4(w,z,y,w) x4(w,z,z,x) x4(w,z,z,y) x4(w,z,z,z) x4(w,z,z,w) x4(w,z,w,x) x4(w,z,w,y) x4(w,z,w,z) x4(w,z,w,w) \
	x4(w,w,x,x) x4(w,w,x,y) x4(w,w,x,z) x4(w,w,x,w) x4(w,w,y,x) x4(w,w,y,y) x4(w,w,y,z) x4(w,w,y,w) x4(w,w,z,x) x4(w,w,z,y) x4(w,w,z,z) x4(w,w,z,w) x4(w,w,w,x) x4(w,w,w,y) x4(w,w,w,z) x4(w,w,w,w) \

template <class T, umm count_>
struct Array;

template <class T, umm storage_count, umm ...indices>
struct ArraySwizzle {
	inline static constexpr umm swizzle_count = sizeof...(indices);
	T data[storage_count];

	forceinline constexpr operator Array<T, swizzle_count>() const;
	forceinline constexpr ArraySwizzle &operator=(Array<T, swizzle_count> array);
	forceinline constexpr Array<T, swizzle_count> operator()() const {
		return (Array<T, swizzle_count>)*this;
	}

	forceinline constexpr operator T() const requires (swizzle_count == 1) {
		return data[indices...];
	}
	forceinline constexpr ArraySwizzle &operator=(T value) requires (swizzle_count == 1) {
		data[indices...] = value;
	}
	forceinline constexpr T operator()() const requires (swizzle_count == 1) {
		return data[indices...];
	}
};

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

	inline static constexpr umm _ix = 0;
	inline static constexpr umm _iy = 1;
	
	union {
		T data[count];
		T s[count]; // Compatibility with vectors
		struct { T x, y; };

		#define x2(a,b)     ArraySwizzle<T, 2, _i##a, _i##b>               a##b##_;
		#define x3(a,b,c)   ArraySwizzle<T, 2, _i##a, _i##b, _i##c>        a##b##c##_;
		#define x4(a,b,c,d) ArraySwizzle<T, 2, _i##a, _i##b, _i##c, _i##d> a##b##c##d##_;
		ENUMERATE_SWIZZLES_NOCONSEC_2(x2,x3,x4)
		#undef x2
		#undef x3
		#undef x4
	};
	#include "_array_members_after.inc"

	#define x2(a,b)     forceinline constexpr v2 a##b()       const { return {a,b    }; }
	#define x3(a,b,c)   forceinline constexpr v3 a##b##c()    const { return {a,b,c  }; }
	#define x4(a,b,c,d) forceinline constexpr v4 a##b##c##d() const { return {a,b,c,d}; }
	ENUMERATE_SWIZZLES_NOCONSEC_2(x2,x3,x4)
	#undef x2
	#undef x3
	#undef x4
};

template <class T>
struct Array<T, 3> {
	inline static constexpr umm count = 3;
	#include "_array_members_before.inc"

	inline static constexpr umm _ix = 0;
	inline static constexpr umm _iy = 1;
	inline static constexpr umm _iz = 2;

	union {
		T data[count];
		T s[count]; // Compatibility with vectors
		struct { T x, y, z; };
		struct { v2 xy; };
		struct { T _pad0; v2 yz; };
		
		#define x2(a,b)     ArraySwizzle<T, 3, _i##a, _i##b>               a##b##_;
		#define x3(a,b,c)   ArraySwizzle<T, 3, _i##a, _i##b, _i##c>        a##b##c##_;
		#define x4(a,b,c,d) ArraySwizzle<T, 3, _i##a, _i##b, _i##c, _i##d> a##b##c##d##_;
		ENUMERATE_SWIZZLES_NOCONSEC_3(x2,x3,x4)
		#undef x2
		#undef x3
		#undef x4
	};
	#include "_array_members_after.inc"

	#define x2(a,b)     forceinline constexpr v2 a##b()       const { return {a,b    }; }
	#define x3(a,b,c)   forceinline constexpr v3 a##b##c()    const { return {a,b,c  }; }
	#define x4(a,b,c,d) forceinline constexpr v4 a##b##c##d() const { return {a,b,c,d}; }
	ENUMERATE_SWIZZLES_NOCONSEC_3(x2,x3,x4)
	#undef x2
	#undef x3
	#undef x4
};

template <class T>
struct Array<T, 4> {
	inline static constexpr umm count = 4;
	#include "_array_members_before.inc"

	inline static constexpr umm _ix = 0;
	inline static constexpr umm _iy = 1;
	inline static constexpr umm _iz = 2;
	inline static constexpr umm _iw = 3;
	
	union {
		T data[count];
		T s[count]; // Compatibility with vectors
		struct { T x, y, z, w; };
		struct { v2 xy, zw; };
		struct { T _pad0; v2 yz; };
		struct { v3 xyz; };
		struct { T _pad1; v3 yzw; };

		#define x2(a,b)     ArraySwizzle<T, 4, _i##a, _i##b>               a##b##_;
		#define x3(a,b,c)   ArraySwizzle<T, 4, _i##a, _i##b, _i##c>        a##b##c##_;
		#define x4(a,b,c,d) ArraySwizzle<T, 4, _i##a, _i##b, _i##c, _i##d> a##b##c##d##_;
		ENUMERATE_SWIZZLES_NOCONSEC_4(x2,x3,x4)
		#undef x2
		#undef x3
		#undef x4
	};
	#include "_array_members_after.inc"

	#define x2(a,b)     forceinline constexpr v2 a##b()       const { return {a,b    }; }
	#define x3(a,b,c)   forceinline constexpr v3 a##b##c()    const { return {a,b,c  }; }
	#define x4(a,b,c,d) forceinline constexpr v4 a##b##c##d() const { return {a,b,c,d}; }
	ENUMERATE_SWIZZLES_NOCONSEC_4(x2,x3,x4)
	#undef x2
	#undef x3
	#undef x4
};

template <class T, umm storage_count, umm ...indices>
forceinline constexpr ArraySwizzle<T, storage_count, indices...>::operator Array<T, ArraySwizzle<T, storage_count, indices...>::swizzle_count>() const {
	return { .data = { data[indices]... } };
}
template <class T, umm storage_count, umm ...indices>
forceinline constexpr ArraySwizzle<T, storage_count, indices...> &ArraySwizzle<T, storage_count, indices...>::operator=(Array<T, ArraySwizzle<T, storage_count, indices...>::swizzle_count> array) {
	[&]<umm ...src_indices>(std::index_sequence<src_indices...>){
		((data[indices] = array[src_indices]), ...);
	}(std::make_index_sequence<sizeof...(indices)>{});
	return *this;
}

template <class T, class... Rest>
Array(T, Rest...) -> Array<typename RequireAllSame<T, Rest...>::Type, 1 + sizeof...(Rest)>;

template <class T, umm count> inline static constexpr bool is_array<Array<T, count>> = true;

template <class T>
concept AnArray = is_array<T>;

template <class T, umm count> inline static constexpr bool is_unsigned<Array<T, count>> = is_unsigned<T>;

template <class T, umm count> inline static constexpr bool is_integer_like<Array<T, count>> = is_integer_like<T>;

template <class T>            inline static constexpr int array_nestedness = 0;
template <class T, umm count> inline static constexpr int array_nestedness<Array<T, count>> = array_nestedness<T> + 1;

template <class T, umm x>                      inline Span<T> flatten(Array<T, x>                               &array) { return {(T *)&array, x      }; }
template <class T, umm x, umm y>               inline Span<T> flatten(Array<Array<T, y>, x>                     &array) { return {(T *)&array, x*y    }; }
template <class T, umm x, umm y, umm z>        inline Span<T> flatten(Array<Array<Array<T, z>, y>, x>           &array) { return {(T *)&array, x*y*z  }; }
template <class T, umm x, umm y, umm z, umm w> inline Span<T> flatten(Array<Array<Array<Array<T, w>, z>, y>, x> &array) { return {(T *)&array, x*y*z*w}; }

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
OP(&&, ASIS)
OP(||, ASIS)
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

// at_deepest(
//     [[0, 1, 2], [3, 4, 5]],
//     [0, 2]
// )
// =
// [[0, 2], [3, 5]]
template <class T, umm count, class Index, umm index_count>
	requires (!is_array<T>)
forceinline constexpr Array<T, index_count> at_deepest(Array<T, count> array, Array<Index, index_count> indices) {
	Array<T, index_count> result = {};
	for (umm i = 0; i < index_count; ++i)
		result.data[i] = array[indices.data[i]]; // bounds check is intended
	return result;
}

template <class T, umm count, umm inner_count, class Index, umm index_count>
	requires (!is_array<T>)
forceinline constexpr Array<Array<T, index_count>, count> at_deepest(Array<Array<T, inner_count>, count> array, Array<Index, index_count> indices) {
	Array<Array<T, index_count>, count> result = {};
	for (umm j = 0; j < count; ++j)
		for (umm i = 0; i < index_count; ++i)
			result.data[j].data[i] = array.data[j][indices.data[i]]; // bounds check is intended
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
forceinline constexpr Array<T, 2> perp(Array<T, 2> a) { return {-a.y, a.x}; }

template <class T>
forceinline constexpr T cross(Array<T, 2> a, Array<T, 2> b) {
	return a.x * b.y - a.y * b.x;
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

template <class Mask, class T, umm count, umm outer_count>
forceinline constexpr Array<Array<T, count>, outer_count> select(Array<Mask, count> mask, Array<Array<T, count>, outer_count> a, Array<Array<T, count>, outer_count> b) {
	Array<Array<T, count>, outer_count> r = {};
	for (umm i = 0; i < outer_count; ++i)
		r.data[i] = select(mask, a.data[i], b.data[i]);
	return r;
}

template <class T, umm count>
void sort_values(Array<T, count> &a, Array<T, count> &b) {
	auto alessb = mask_lt(a, b);
	auto c = a;
	a = select(alessb, a, b);
	b = select(alessb, b, c);
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
	T t = (T)from;
	for (umm i = 0; i < count; ++i)
		to.data[i] = t;
}

template <class T, umm count, umm inner_count, class From>
	requires std::is_convertible_v<From, T>
forceinline constexpr void convert(Array<Array<T, inner_count>, count> &to, From from) {
	T t = (T)from;
	for (umm i = 0; i < count; ++i)
		for (umm j = 0; j < inner_count; ++j)
			to.data[i].data[j] = t;
}

template <class U, class T, umm count>
	requires std::is_convertible_v<T, U>
forceinline constexpr void convert(Array<U, count> &r, Array<T, count> a) {
	for (umm i = 0; i < count; ++i)
		r.data[i] = convert<U>(a.data[i]);
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

template <class T, umm count>
forceinline auto sum(Array<T, count> v) {
	T r = {};
	for (umm i = 0; i < count; ++i)
		r += v.data[i];
	return r;
}

template <class T, umm count>
forceinline auto average(Array<T, count> v) {
	return sum(v) / count;
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