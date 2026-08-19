#pragma once
#include "common.h"
#include "array.h"

namespace tl {

using UnsignedWord = u64;
using SignedWord   = s64;

template <umm bit_count, bool is_signed /* TODO: Remove s128.h after implementing */>
	requires (bit_count % (sizeof(UnsignedWord)*8) == 0)
struct Int {
	using SignedInt = Int<bit_count, true>;
	using UnsignedInt = Int<bit_count, false>;
	inline static constexpr umm bits_in_word = sizeof(UnsignedWord) * 8;
	inline static constexpr umm bits_in_word_log2 = log2(bits_in_word);
	inline static constexpr umm word_count = bit_count / bits_in_word;
	UnsignedWord words[word_count];

	forceinline constexpr Int() { for (umm i = 0; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr Int(bool value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr Int(u8   value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr Int(u16  value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr Int(u32  value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr Int(u64  value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr Int(s8   value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >>  7); for (umm i = 1; i < word_count; ++i) words[i] = fill; }
	forceinline constexpr Int(s16  value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >> 15); for (umm i = 1; i < word_count; ++i) words[i] = fill; }
	forceinline constexpr Int(s32  value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >> 31); for (umm i = 1; i < word_count; ++i) words[i] = fill; }
	forceinline constexpr Int(s64  value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >> 63); for (umm i = 1; i < word_count; ++i) words[i] = fill; }

	// NOTE: Lowest word comes last!
	template <class ...T>
		requires requires {
			requires sizeof...(T) >= 2;
			requires sizeof...(T) <= word_count;
			requires (std::is_same_v<T, UnsignedWord> && ...);
		}
	forceinline constexpr Int(T ...values) {
		UnsignedWord arr[] = {values...};
		umm i;
		for (i = 0; i < sizeof...(T); ++i)
			words[i] = arr[sizeof...(T) - i - 1];

		UnsignedWord fill = is_signed ?
			(UnsignedWord)((SignedWord)arr[0] >> (bits_in_word -1)) :
			(UnsignedWord)0;
		
		for (; i < word_count; ++i)
			words[i] = fill;
	}

	forceinline constexpr Int &operator++() { return *this += Int(1ull); }
	forceinline constexpr Int operator++(int) { Int copy = *this; ++*this; return copy; }
	forceinline constexpr Int &operator--() { return *this -= Int(1ull); }
	forceinline constexpr Int operator--(int) { Int copy = *this; --*this; return copy; }

	forceinline constexpr Int operator~() const {
		Int r = *this;
		for (auto &v : r.words)
			v = ~v;
		return r;
	}
	forceinline constexpr Int operator-() const {
		Int result = ~*this;
		return ++result;
	}
	forceinline constexpr Int negated_if(bool condition) const {
		Int result = *this;
		result ^= -condition;
		result += condition;
		return result;
	}
	forceinline constexpr Int operator^(Int b) const { Int r; for (umm i = 0; i < word_count; ++i) r.words[i] = words[i] ^ b.words[i]; return r; }
	forceinline constexpr Int operator&(Int b) const { Int r; for (umm i = 0; i < word_count; ++i) r.words[i] = words[i] & b.words[i]; return r; }
	forceinline constexpr Int operator|(Int b) const { Int r; for (umm i = 0; i < word_count; ++i) r.words[i] = words[i] | b.words[i]; return r; }
	forceinline constexpr Int operator<<(u64 b) const {
		b &= bit_count - 1;
		UnsignedWord word_shift = b / bits_in_word;
		UnsignedWord bit_shift  = b % bits_in_word;

		word_shift = min(word_shift, word_count);

		Int r;
		umm i;
		for (i = 0; i < word_shift; ++i)
			r.words[i] = 0;

		for (; i < word_count; ++i)
			r.words[i] = words[i - word_shift];

		if (bit_shift) {
			for (i = word_count - 1; i != 0; --i)
				r.words[i] = (r.words[i] << bit_shift) | (r.words[i - 1] >> (bits_in_word - bit_shift));
			r.words[0] <<= bit_shift;
		}
		
		return r;
	}
	forceinline constexpr Int operator<<(u8  b) const { return *this << (u64)b; }
	forceinline constexpr Int operator<<(u16 b) const { return *this << (u64)b; }
	forceinline constexpr Int operator<<(u32 b) const { return *this << (u64)b; }
	forceinline constexpr Int operator<<(s8  b) const { return *this << (u64)(s64)b; }
	forceinline constexpr Int operator<<(s16 b) const { return *this << (u64)(s64)b; }
	forceinline constexpr Int operator<<(s32 b) const { return *this << (u64)(s64)b; }
	forceinline constexpr Int operator<<(s64 b) const { return *this << (u64)(s64)b; }
	forceinline constexpr Int operator<<(Int b) const { return *this << b.words[0]; }

	forceinline constexpr Int operator>>(u64 b) const {
		b &= bit_count - 1;
		UnsignedWord word_shift = b / bits_in_word;
		UnsignedWord bit_shift  = b % bits_in_word;

		word_shift = min(word_shift, word_count);

		Int r;
		umm i;
		for (i = 0; i < word_count - word_shift; ++i)
			r.words[i] = words[i + word_shift];

		UnsignedWord fill =
			is_signed ?
			(UnsignedWord)((SignedWord)words[word_count - 1] >> (bits_in_word - 1)) :
			(UnsignedWord)0;

		for (; i < word_count; ++i)
			r.words[i] = fill;

		if (bit_shift) {
			for (i = 0; i < word_count - 1; ++i)
				r.words[i] = (r.words[i] >> bit_shift) | (r.words[i + 1] << (bits_in_word - bit_shift));

			r.words[word_count - 1] = is_signed ?
				(UnsignedWord)((SignedWord)r.words[word_count - 1] >> bit_shift) :
				                           r.words[word_count - 1] >> bit_shift;
		}
		
		return r;
	}
	forceinline constexpr Int operator>>(u8  b) const { return *this >> (u64)b; }
	forceinline constexpr Int operator>>(u16 b) const { return *this >> (u64)b; }
	forceinline constexpr Int operator>>(u32 b) const { return *this >> (u64)b; }
	forceinline constexpr Int operator>>(s8  b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr Int operator>>(s16 b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr Int operator>>(s32 b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr Int operator>>(s64 b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr Int operator>>(Int b) const { return *this >> b.words[0]; }

	forceinline constexpr Int operator+(Int b) const {
		Int result;
		bool carry = false;
		for (umm i = 0; i < word_count; ++i)
			add_carry(words[i], b.words[i], carry, &result.words[i], &carry);
		return result;
	}
	forceinline constexpr Int operator-(Int b) const {
		return *this + -b;
	}
	forceinline constexpr Int operator*(bool b) const {
		return *this & (-b);
	}
	forceinline constexpr Int operator*(u8 b) const {
		Int result = {};
		for (u32 bit_index = 0; bit_index < 8; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr Int operator*(u16 b) const {
		Int result = {};
		for (u32 bit_index = 0; bit_index < 16; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr Int operator*(u32 b) const {
		Int result = {};
		for (u32 bit_index = 0; bit_index < 32; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr Int operator*(u64 b) const {
		Int result = {};
		for (u32 bit_index = 0; bit_index < 64; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr Int operator*(Int b) const {
		Int result = {};
		for (u32 bit_index = 0; bit_index < bit_count; ++bit_index)
			result += (*this << bit_index) & Int(-b.get_bit(bit_index));
		return result;
	}
	forceinline constexpr Int operator*(s8  b) const { return *this * (u64)(s64)b; }
	forceinline constexpr Int operator*(s16 b) const { return *this * (u64)(s64)b; }
	forceinline constexpr Int operator*(s32 b) const { return *this * (u64)(s64)b; }
	forceinline constexpr Int operator*(s64 b) const { return *this * (u64)(s64)b; }

	forceinline static constexpr Array<Int, 2> divmod(Int a, Int b) {
		bool a_neg, b_neg, q_neg, r_neg;

		if constexpr (is_signed) {
			a_neg = a.get_bit(bit_count - 1);
			b_neg = b.get_bit(bit_count - 1);
			q_neg = (a_neg ^ b_neg);
			r_neg = a_neg;
			
			a = a.negated_if(a_neg);
			b = b.negated_if(b_neg);
		}

		Int quotient = {};
		Int remainder = {};
		for (umm i = bit_count - 1; i != (umm)-1; --i) {
			remainder <<= 1;
			remainder.set_bit(0, a.get_bit(i));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit(i, 1);
			}
		}

		if constexpr (is_signed) {
			quotient  = quotient .negated_if(q_neg);
			remainder = remainder.negated_if(r_neg);
		}

		return {quotient, remainder};
	}
	forceinline constexpr Int operator/(Int b) const { return divmod(*this, b)[0]; }
	forceinline constexpr Int operator%(Int b) const { return divmod(*this, b)[1]; }

	forceinline constexpr bool operator==(Int b) const { bool result = true;  for (umm i = 0; i < word_count; ++i) result &= words[i] == b.words[i]; return result; }
	forceinline constexpr bool operator!=(Int b) const { bool result = false; for (umm i = 0; i < word_count; ++i) result |= words[i] != b.words[i]; return result; }

	forceinline constexpr Int with_top_bit_flipped() const {
		Int r = *this;
		r.words[word_count - 1] ^= (UnsignedWord)1 << (bits_in_word - 1);
		return r;
	}

	forceinline constexpr bool operator>(Int b) const {
		if constexpr (is_signed)
			return (UnsignedInt)this->with_top_bit_flipped() > (UnsignedInt)b.with_top_bit_flipped();
		for (umm i = word_count - 1; i != 0; --i)
			if (words[i] != b.words[i])
				return words[i] > b.words[i];
		return words[0] > b.words[0];
	}
	forceinline constexpr bool operator>=(Int b) const {
		if constexpr (is_signed)
			return (UnsignedInt)this->with_top_bit_flipped() >= (UnsignedInt)b.with_top_bit_flipped();
		for (umm i = word_count - 1; i != 0; --i)
			if (words[i] != b.words[i])
				return words[i] >= b.words[i];
		return words[0] >= b.words[0];
	}
	forceinline constexpr bool operator<(Int b) const {
		if constexpr (is_signed)
			return (UnsignedInt)this->with_top_bit_flipped() < (UnsignedInt)b.with_top_bit_flipped();
		for (umm i = word_count - 1; i != 0; --i)
			if (words[i] != b.words[i])
				return words[i] < b.words[i];
		return words[0] < b.words[0];
	}
	forceinline constexpr bool operator<=(Int b) const {
		if constexpr (is_signed)
			return (UnsignedInt)this->with_top_bit_flipped() <= (UnsignedInt)b.with_top_bit_flipped();
		for (umm i = word_count - 1; i != 0; --i)
			if (words[i] != b.words[i])
				return words[i] <= b.words[i];
		return words[0] <= b.words[0];
	}

	forceinline bool get_bit(u64 index) const {
		return words[index / bits_in_word] & ((UnsignedWord)1 << (index % bits_in_word));
	}
	forceinline void set_bit(u64 index, bool value) {
		auto &word = words[index / bits_in_word];
		auto bit = (UnsignedWord)1 << (index % bits_in_word);
		auto set = word |  bit;
		auto clr = word & ~bit;
		word = value ? set : clr;
	}

	forceinline constexpr Int &operator+= (auto b) requires requires { *this +  b; } { return *this = *this +  b; }
	forceinline constexpr Int &operator-= (auto b) requires requires { *this -  b; } { return *this = *this -  b; }
	forceinline constexpr Int &operator*= (auto b) requires requires { *this *  b; } { return *this = *this *  b; }
	forceinline constexpr Int &operator/= (auto b) requires requires { *this /  b; } { return *this = *this /  b; }
	forceinline constexpr Int &operator^= (auto b) requires requires { *this ^  b; } { return *this = *this ^  b; }
	forceinline constexpr Int &operator&= (auto b) requires requires { *this &  b; } { return *this = *this &  b; }
	forceinline constexpr Int &operator|= (auto b) requires requires { *this |  b; } { return *this = *this |  b; }
	forceinline constexpr Int &operator<<=(auto b) requires requires { *this << b; } { return *this = *this << b; }
	forceinline constexpr Int &operator>>=(auto b) requires requires { *this >> b; } { return *this = *this >> b; }

	forceinline constexpr explicit operator bool() const { return *this != Int(); }
	forceinline constexpr explicit operator u8  () const { return words[0]; }
	forceinline constexpr explicit operator u16 () const { return words[0]; }
	forceinline constexpr explicit operator u32 () const { return words[0]; }
	forceinline constexpr explicit operator u64 () const { return words[0]; }
	forceinline constexpr explicit operator s8  () const { return words[0]; }
	forceinline constexpr explicit operator s16 () const { return words[0]; }
	forceinline constexpr explicit operator s32 () const { return words[0]; }
	forceinline constexpr explicit operator s64 () const { return words[0]; }
	forceinline constexpr explicit operator Int<bit_count, !is_signed>() const { return bit_cast<Int<bit_count, !is_signed>>(*this); }

	template <class Float>
	forceinline constexpr Float to_float() const {
		Int n;
		bool sign;
		if constexpr (is_signed) {
			sign = get_bit(bit_count - 1);
			n = negated_if(sign);
		} else {
			n = *this;
		}
		
		Float r = (Float)0.0;
		for (umm i = 0; i < word_count; ++i) {
			r *= (Float)18446744073709551616.0;
			r += n.words[i];
		}
		
		if constexpr (is_signed) {
			r = sign ? -r : r;
		}
		return r;
	}

	forceinline constexpr explicit operator f32() const { return to_float<f32>(); }
	forceinline constexpr explicit operator f64() const { return to_float<f64>(); }
};

using u128 = Int<128, false>;
using u256 = Int<256, false>;
using s128 = Int<128, true>;
using s256 = Int<256, true>;

template <umm bit_count, bool is_signed_> inline constexpr bool is_integer<Int<bit_count, is_signed_>> = true;
template <umm bit_count, bool is_signed_> inline constexpr bool is_integer_like<Int<bit_count, is_signed_>> = true;
template <umm bit_count, bool is_signed_> inline constexpr bool is_signed<Int<bit_count, is_signed_>> = is_signed_;
template <umm bit_count, bool is_signed_> inline constexpr bool is_unsigned<Int<bit_count, is_signed_>> = !is_signed_;

template <umm bit_count, bool is_signed> struct MakeUnsignedT<Int<bit_count, is_signed>> { using Type = Int<bit_count, false>; };
template <umm bit_count, bool is_signed> struct MakeSignedT  <Int<bit_count, is_signed>> { using Type = Int<bit_count, true >; };

forceinline s128 mulfull(s64 a, s64 b) {
	#ifdef _MSC_VER
	s128 result;
	result.words[0] = _mul128(a, b, (long long *)&result.words[1]);
	return result;
	#else
	return bit_cast<s128>((__int128_t)a * (__int128_t)b);
	#endif
}
forceinline s64 div(s128 a, s64 b) {
	#ifdef _MSC_VER
	s64 remainder;
	return _div128(a.high, a.low, b, &remainder);
	#else
	return bit_cast<__int128_t>(a) / b;
	#endif
}

#ifdef TL_ENABLE_TESTS

TL_TEST(int) {
	{
		u128 a = u128(0x0123012301230123ull, 0x4567456745674567ull);
		u128 b = u128(0x89ab89ab89ab89abull, 0xcdefcdefcdefcdefull);

		

		#define check(a, b) check_(a, b, __FILE__, __LINE__, #a, #b)
		auto check_ = [&](auto a, auto b, char const *file, int line, char const *astr, char const *bstr) {
			if (a != b) {
				println("{}:{}: {} == {} is false", file, line, astr, bstr);
				println("a = 0x{}", format_hex(a));
				println("b = 0x{}", format_hex(b));
				invalid_code_path();
			}
		};

		check(~a   , u128(0xfedcfedcfedcfedcull, 0xba98ba98ba98ba98ull));
		check(-a   , u128(0xfedcfedcfedcfedcull, 0xba98ba98ba98ba99ull));
		check(a + b, u128(0x8ace8ace8ace8acfull, 0x1357135713571356ull));
		check(b - a, u128(0x8888888888888888ull, 0x8888888888888888ull));
		check(a * b, u128(0xf072b6b97d004346ull, 0xc0214223c4264629ull)); // full is 9c7e99fc977a95 1d96574f9108cac1 f072b6b97d004346 c0214223c4264629
		check(b / a, 0x79);
		check(b % a, u128(  0x20002000200000ull, 0x0020002000200040ull));
		assert(!(a == b));
		assert(a != b);
		assert(a < b);
		assert(a <= b);
		assert(b > a);
		assert(b >= a);
		check(a ^ b, u128(0x8888888888888888ull, 0x8888888888888888ull));
		check(a & b, u128( 0x123012301230123ull, 0x4567456745674567ull));
		check(a | b, u128(0x89ab89ab89ab89abull, 0xcdefcdefcdefcdefull));

		u128 c = a;
		check(c++, a);
		check(c, a + 1);
		check(++c, a + 2);
		check(c--, a + 2);
		check(c, a + 1);
		check(--c, a);

		s128 r = (s128)a;
		s128 s = (s128)b;

		check(b >>  3, u128(0x1135713571357135ull, 0x79bdf9bdf9bdf9bdull));
		check(s >>  3, s128(0xf135713571357135ull, 0x79bdf9bdf9bdf9bdull));
		check(b >> 67, u128(0x0000000000000000ull, 0x1135713571357135ull));
		check(s >> 67, s128(0xffffffffffffffffull, 0xf135713571357135ull));
		check(b <<  3, u128(0x4d5c4d5c4d5c4d5eull, 0x6f7e6f7e6f7e6f78ull));
		check(s <<  3, s128(0x4d5c4d5c4d5c4d5eull, 0x6f7e6f7e6f7e6f78ull));
		check(b << 67, u128(0x6f7e6f7e6f7e6f78ull, 0x0000000000000000ull));
		check(s << 67, s128(0x6f7e6f7e6f7e6f78ull, 0x0000000000000000ull));

		assert(!(r == s));
		assert(r != s);
		assert(r > s);
		assert(r >= s);
		assert(s < r);
		assert(s <= r);
		check(r ^ s, s128(0x8888888888888888ull, 0x8888888888888888ull));
		check(r & s, s128( 0x123012301230123ull, 0x4567456745674567ull));
		check(r | s, s128(0x89ab89ab89ab89abull, 0xcdefcdefcdefcdefull));
		check(s128((u64)-64, 0ull) / 128, s128(-1ull, 0x8000000000000000ull));
		check(s128(-234) % 100, s128(-34));
		#undef check
	}
};

#endif

}

#define TL_INT_H_INCLUDED

#ifdef TL_STRING_H_INCLUDED
#include "int-string.h"
#endif
