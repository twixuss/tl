#pragma once
#include "common.h"
#include "macros.h"

namespace tl {

#define CLASS(I) , class T##I 
#define CASE(I) case I: return visitor(*(T##I *)storage);
#define CONST_CASE(I) case I: return visitor(*(T##I const *)storage);
#define VISIT_IMPL(I)                                                                     \
	template <class Dummy TL_IREPEAT##I(CLASS)>                                           \
	decltype(auto) visit_impl(umm index, u8 *storage, auto &&visitor) {                   \
		switch (index) {                                                                  \
			TL_IREPEAT##I(CASE);                                                          \
		}                                                                                 \
		invalid_code_path("Attempt to visit variant of size {} with index {}", I, index); \
	}                                                                                     \
	template <class Dummy TL_IREPEAT##I(CLASS)>                                           \
	decltype(auto) visit_impl(umm index, u8 const *storage, auto &&visitor) {             \
		switch (index) {                                                                  \
			TL_IREPEAT##I(CONST_CASE);                                                    \
		}                                                                                 \
		invalid_code_path("Attempt to visit variant of size {} with index {}", I, index); \
	}

TL_IREPEAT64(VISIT_IMPL)

#undef CLASS
#undef CASE
#undef CONST_CASE
#undef VISIT_IMPL

template <class ...T>
struct Variant {
	template <class U>
	static constexpr umm index_of = type_index_of<U, T...>;

	static constexpr umm count = sizeof...(T);
	
	static constexpr umm sizes[] = {sizeof(T)...};
	
	using Size = UintForCount<count>;

private:
	alignas(max(alignof(T)...)) u8 storage[max(sizeof(T)...)];
	Size _index;

public:
	Variant() { 
		using First = TypeAt<0, T...>;

		new (storage) First();
		_index = 0;
	}

	template <OneOf<T...> U>
	Variant(U value) {
		new (storage) U(value);
		_index = index_of<U>;
	}

	template <class Visitor>
	forceinline decltype(auto) visit(this auto &&self, Visitor &&visitor) requires requires {
		{ (visitor(*(T *)0), ...) };
	} {
		if constexpr (sizeof...(T) <= 64) {
			return visit_impl<void, T...>(self._index, self.storage, std::forward<Visitor>(visitor));
		} else {
			Array table = {
				+[](u8 *storage, Visitor &&visitor) -> decltype(auto) { return visitor(*(T *)storage); }...
			};

			return table.data[self._index](self.storage, std::forward<Visitor>(visitor));
		}
	}

	template <OneOf<T...> U>
	bool is() const {
		return _index == index_of<U>;
	}

	template <OneOf<T...> U>
	U *as_ptr() {
		return (U *)((umm)storage & (umm)-(smm)(_index == index_of<U>));
	}

	template <OneOf<T...> U>
	U const *as_ptr() const {
		return (U const *)((umm)storage & (umm)-(smm)(_index == index_of<U>));
	}

	template <OneOf<T...> U>
	Optional<U> as() const {
		if (auto p = as_ptr<U>()) {
			return *p;
		}
		return {};
	}

	umm index() const { return _index; }

	umm current_value_size() const { return sizes[_index]; }

	bool operator==(Variant const &that) const {
		if (_index != that._index)
			return false;

		return visit([&](auto &&this_inner) {
			return that.visit([&](auto &&that_inner) {
				if constexpr (std::is_same_v<decltype(this_inner), decltype(that_inner)>) {
					return this_inner == that_inner;
				} else {
					return false;
				}
			});
		});
	}
};

template <class ...T>
inline void append(StringBuilder &builder, Variant<T...> const &v) {
	v.visit([&](auto &inner) {
		append(builder, inner);
	});
}

}

#ifdef TL_ENABLE_TESTS

namespace tl::test {

template <int i>
struct S {};

}
TL_TEST {
	using namespace tl;
	using namespace tl::test;
	{
		Variant<int, float, bool> v;
		assert(v.index() == 0);
		assert(v.is<int>());
		assert(v.as<int>().value() == 0);
		v.visit(Combine{
			[](int) {},
			[](auto) { invalid_code_path(); },
		});

		v = 1.0f;
		assert(v.index() == 1);
		assert(v.is<float>());
		assert(v.as<float>().value() == 1.0f);
		v.visit(Combine{
			[](float) {},
			[](auto) { invalid_code_path(); },
		});

		v = true;
		assert(v.index() == 2);
		assert(v.is<bool>());
		assert(v.as<bool>().value() == true);
		v.visit(Combine{
			[](bool) {},
			[](auto) { invalid_code_path(); },
		});
	}
	{
		#define x(i) S<i>, 
		Variant<TL_IREPEAT64(x) S<64>> v;

		assert(v.index() == 0);
		assert(v.is<S<0>>());
		v.visit(Combine{
			[](S<0>) {},
			[](auto) { invalid_code_path(); },
		});

		v = S<64>{};
		assert(v.index() == 64);
		assert(v.is<S<64>>());
		v.visit(Combine{
			[](S<64>) {},
			[](auto) { invalid_code_path(); },
		});
	}
};

#endif
