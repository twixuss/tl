forceinline constexpr auto begin(this auto &&self) { return self.data; }
forceinline constexpr auto end(this auto &&self) { return self.data + self.count; }

forceinline constexpr operator Span<T>() const { return {(T *)data, count}; }
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

template <umm sub_count, umm sub_start>
forceinline constexpr Array<T, sub_count> &sub_array() {
    static_assert(sub_start + sub_count <= count);
    return *(Array<T, sub_count> *)(data + sub_start);
}

template <umm sub_count>
forceinline constexpr Array<T, sub_count> &sub_array(umm sub_start = 0) {
    assert(sub_start + sub_count <= count);
    return *(Array<T, sub_count> *)(data + sub_start);
}

// SFINAE required, otherwise does not compile when T is not Array.
template <umm sub_count, umm sub_start, class U=T>
forceinline constexpr Array<Array<typename std::enable_if_t<is_array<U>, U>::Element, sub_count>, count> sub_arrays() {
    static_assert(sub_start + sub_count <= T::count);
    Array<Array<typename T::Element, sub_count>, count> r = {};
    for (umm i = 0; i < count; ++i)
        r.data[i] = data[i].template sub_array<sub_count, sub_start>();
    return r;
}

// SFINAE required, otherwise does not compile when T is not Array.
template <umm sub_count, class U=T>
forceinline constexpr Array<Array<typename std::enable_if_t<is_array<U>, U>::Element, sub_count>, count> sub_arrays(umm sub_start = 0) {
    assert(sub_start + sub_count <= T::count);
    Array<Array<typename T::Element, sub_count>, count> r = {};
    for (umm i = 0; i < count; ++i)
        r.data[i] = data[i].template sub_array<sub_count>(sub_start);
    return r;
}

forceinline constexpr auto map(auto &&mapper) const
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