// This file is intended to be included in tl::Array template specializations
// Why?
// I just decided to have one struct for all - arrays, vectors, simd.
// And vectors require xyzw members, which in turn require different specializations for sizes 2, 3 and 4+.
// So to not duplicate code, put in an h file.

using Element = T;
using v2 = Array<T, 2>;
using v3 = Array<T, 3>;
using v4 = Array<T, 4>;

T data[count];

forceinline constexpr auto begin(this auto &&self) { return self.data; }
forceinline constexpr auto end(this auto &&self) { return self.data + self.count; }

inline operator Span<T>() const { return {(T *)data, count}; }
forceinline constexpr Span<T> span() const { return {begin(), end()}; }

forceinline constexpr auto &at(this auto &&self, umm i) {
    bounds_check(assert_less(i, self.count));
    return self.data[i];
}
forceinline constexpr auto &operator[](this auto &&self, umm i) { return self.at(i); }

forceinline constexpr auto &front(this auto &&self) { return self.data[0]; }
forceinline constexpr auto &back(this auto &&self) { return self.data[self.count - 1]; }

forceinline constexpr umm index_of(T const *pointer) const { return pointer - data; }
forceinline constexpr bool owns(T *pointer) { return (umm)(pointer - data) < count; }

using IterOptions = ReverseIterOption;

forceinline constexpr auto iter(this auto &&self, IterOptions options = {}) {
    return span_iter(self.data, self.data + count, options);
}

template <umm sub_count>
Array<T, sub_count> &sub_array(umm sub_start = 0) {
    return *(Array<T, sub_count> *)(data + sub_start);
}

auto map(auto &&mapper) const
    requires requires { mapper(data[0]); }
{
    Array<std::remove_cvref_t<decltype(mapper(data[0]))>, count> result = {};
    for (umm i = 0; i < count; ++i) {
        result.data[i] = mapper(data[i]);
    }
    return result;
}

template <class U>
forceinline constexpr explicit operator Array<U, count>() const
    requires requires(T t) { (U)t; }
{
    Array<U, count> result = {};
    for (umm i = 0; i < count; ++i)
        result.data[i] = (U)data[i];
    return result;
};