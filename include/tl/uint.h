#pragma once
#include "common.h"
#include "array.h"

namespace tl {

using UnsignedWord = u64;
using SignedWord   = s64;

template <umm bit_count/*, bool is_signed TODO. Remove s128.h after implementing */>
	requires (bit_count % (sizeof(UnsignedWord)*8) == 0)
struct uint {
	inline static constexpr umm bits_in_word = sizeof(UnsignedWord) * 8;
	inline static constexpr umm bits_in_word_log2 = log2(bits_in_word);
	inline static constexpr umm word_count = bit_count / bits_in_word;
	UnsignedWord words[word_count];

	// Everything should be constexpr.

	forceinline constexpr uint() { for (umm i = 0; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr uint(bool value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr uint(u8   value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr uint(u16  value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr uint(u32  value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr uint(u64  value) { words[0] = value; for (umm i = 1; i < word_count; ++i) words[i] = 0; }
	forceinline constexpr uint(s8   value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >>  7); for (umm i = 1; i < word_count; ++i) words[i] = fill; }
	forceinline constexpr uint(s16  value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >> 15); for (umm i = 1; i < word_count; ++i) words[i] = fill; }
	forceinline constexpr uint(s32  value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >> 31); for (umm i = 1; i < word_count; ++i) words[i] = fill; }
	forceinline constexpr uint(s64  value) { words[0] = (UnsignedWord)(SignedWord)value; auto fill = (UnsignedWord)(value >> 63); for (umm i = 1; i < word_count; ++i) words[i] = fill; }

	// NOTE: Lowest word comes last!
	template <class ...T>
		requires requires {
			requires sizeof...(T) >= 2;
			requires sizeof...(T) <= word_count;
			requires (std::is_same_v<T, UnsignedWord> && ...);
		}
	forceinline constexpr uint(T ...values) {
		UnsignedWord arr[] = {values...};
		umm i;
		for (i = 0; i < sizeof...(T); ++i)
			words[i] = arr[sizeof...(T) - i - 1];
		for (; i < word_count; ++i)
			words[i] = 0;
	}

	forceinline constexpr uint &operator++() { return *this += uint(1ull); }
	forceinline constexpr uint operator++(int) { uint copy = *this; ++*this; return copy; }

	forceinline constexpr uint operator~() const {
		uint r = *this;
		for (auto &v : r)
			v = ~v;
		return r;
	}
	forceinline constexpr uint operator-() const {
		uint result = ~*this;
		return ++result;
	}
	forceinline constexpr uint operator^(uint b) const { uint r; for (umm i = 0; i < word_count; ++i) r.words[i] = words[i] ^ b.words[i]; return r; }
	forceinline constexpr uint operator&(uint b) const { uint r; for (umm i = 0; i < word_count; ++i) r.words[i] = words[i] & b.words[i]; return r; }
	forceinline constexpr uint operator|(uint b) const { uint r; for (umm i = 0; i < word_count; ++i) r.words[i] = words[i] | b.words[i]; return r; }
	forceinline constexpr uint operator<<(u64 b) const {
		b &= bit_count - 1;
		UnsignedWord word_shift = b / bits_in_word;
		UnsignedWord bit_shift  = b % bits_in_word;

		word_shift = min(word_shift, word_count);

		uint r;
		umm i;
		for (i = 0; i < word_shift; ++i)
			r.words[i] = 0;

		for (; i < word_count; ++i)
			r.words[i] = words[i - word_shift];

		for (i = word_count - 1; i != 0; --i)
			r.words[i] = (r.words[i] << bit_shift) | (r.words[i - 1] >> (bits_in_word - bit_shift));
		r.words[0] <<= bit_shift;
		
		return r;
	}
	forceinline constexpr uint operator<<(u8  b) const { return *this << (u64)b; }
	forceinline constexpr uint operator<<(u16 b) const { return *this << (u64)b; }
	forceinline constexpr uint operator<<(u32 b) const { return *this << (u64)b; }
	forceinline constexpr uint operator<<(s8  b) const { return *this << (u64)(s64)b; }
	forceinline constexpr uint operator<<(s16 b) const { return *this << (u64)(s64)b; }
	forceinline constexpr uint operator<<(s32 b) const { return *this << (u64)(s64)b; }
	forceinline constexpr uint operator<<(s64 b) const { return *this << (u64)(s64)b; }
	forceinline constexpr uint operator<<(uint b) const { return *this << b.words[0]; }

	forceinline constexpr uint operator>>(u64 b) const {
		b &= bit_count - 1;
		UnsignedWord word_shift = b / bits_in_word;
		UnsignedWord bit_shift  = b % bits_in_word;

		word_shift = min(word_shift, word_count);

		uint r;
		umm i;
		for (i = 0; i < word_count - word_shift; ++i)
			r.words[i] = words[i + word_shift];

		for (; i < word_shift; ++i)
			r.words[i] = 0;

		for (i = 0; i < word_count - 1; ++i)
			r.words[i] = (r.words[i] >> bit_shift) | (r.words[i + 1] << (bits_in_word - bit_shift));
		r.words[word_count - 1] >>= bit_shift;
		
		return r;
	}
	forceinline constexpr uint operator>>(u8  b) const { return *this >> (u64)b; }
	forceinline constexpr uint operator>>(u16 b) const { return *this >> (u64)b; }
	forceinline constexpr uint operator>>(u32 b) const { return *this >> (u64)b; }
	forceinline constexpr uint operator>>(s8  b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr uint operator>>(s16 b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr uint operator>>(s32 b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr uint operator>>(s64 b) const { return *this >> (u64)(s64)b; }
	forceinline constexpr uint operator>>(uint b) const { return *this >> b.words[0]; }

	forceinline constexpr uint operator+(uint b) const {
		uint result;
		bool carry = false;
		for (umm i = 0; i < word_count; ++i)
			add_carry(words[i], b.words[i], carry, &result.words[i], &carry);
		return result;
	}
	forceinline constexpr uint operator-(uint b) const {
		return *this + -b;
	}
	forceinline constexpr uint operator*(bool b) const {
		return *this & (-b);
	}
	forceinline constexpr uint operator*(u8 b) const {
		uint result = {};
		for (u32 bit_index = 0; bit_index < 8; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr uint operator*(u16 b) const {
		uint result = {};
		for (u32 bit_index = 0; bit_index < 16; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr uint operator*(u32 b) const {
		uint result = {};
		for (u32 bit_index = 0; bit_index < 32; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr uint operator*(u64 b) const {
		uint result = {};
		for (u32 bit_index = 0; bit_index < 64; ++bit_index)
			result += (*this << bit_index) & (-((b >> bit_index) & 1));
		return result;
	}
	forceinline constexpr uint operator*(uint b) const {
		uint result = {};
		for (u32 bit_index = 0; bit_index < bit_count; ++bit_index)
			result += (*this << bit_index) & uint(-b.get_bit(bit_index));
		return result;
	}
	forceinline constexpr uint operator*(s8  b) const { return *this * (u64)(s64)b; }
	forceinline constexpr uint operator*(s16 b) const { return *this * (u64)(s64)b; }
	forceinline constexpr uint operator*(s32 b) const { return *this * (u64)(s64)b; }
	forceinline constexpr uint operator*(s64 b) const { return *this * (u64)(s64)b; }

	forceinline constexpr Array<uint, 2> divmod(uint b) const {
		uint quotient = {};
		uint remainder = {};
		for (umm i = bit_count - 1; i != (umm)-1; --i) {
			remainder <<= 1;
			remainder.set_bit(0, get_bit(i));
			if (remainder >= b) {
				remainder -= b;
				quotient.set_bit(i, 1);
			}
		}
		return {quotient, remainder};
	}
	forceinline constexpr uint operator/(uint b) const { return divmod(b)[0]; }
	forceinline constexpr uint operator%(uint b) const { return divmod(b)[1]; }

	forceinline constexpr bool operator==(uint b) const { bool result = true;  for (umm i = 0; i < word_count; ++i) result &= words[i] == b.words[i]; return result; }
	forceinline constexpr bool operator!=(uint b) const { bool result = false; for (umm i = 0; i < word_count; ++i) result |= words[i] != b.words[i]; return result; }

	forceinline constexpr bool operator>(uint b) const {
		for (umm i = word_count - 1; i != 0; ++i)
			if (words[i] != b.words[i])
			 return words[i] > b.words[i];
		return words[0] > b.words[0];
	}
	forceinline constexpr bool operator>=(uint b) const {
		for (umm i = word_count - 1; i != 0; ++i)
			if (words[i] != b.words[i])
			 return words[i] >= b.words[i];
		return words[0] >= b.words[0];
	}
	forceinline constexpr bool operator<(uint b) const {
		for (umm i = word_count - 1; i != 0; ++i)
			if (words[i] != b.words[i])
			 return words[i] < b.words[i];
		return words[0] < b.words[0];
	}
	forceinline constexpr bool operator<=(uint b) const {
		for (umm i = word_count - 1; i != 0; ++i)
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

	forceinline constexpr uint &operator+= (auto b) requires requires { *this +  b; } { return *this = *this +  b; }
	forceinline constexpr uint &operator-= (auto b) requires requires { *this -  b; } { return *this = *this -  b; }
	forceinline constexpr uint &operator*= (auto b) requires requires { *this *  b; } { return *this = *this *  b; }
	forceinline constexpr uint &operator/= (auto b) requires requires { *this /  b; } { return *this = *this /  b; }
	forceinline constexpr uint &operator^= (auto b) requires requires { *this ^  b; } { return *this = *this ^  b; }
	forceinline constexpr uint &operator&= (auto b) requires requires { *this &  b; } { return *this = *this &  b; }
	forceinline constexpr uint &operator|= (auto b) requires requires { *this |  b; } { return *this = *this |  b; }
	forceinline constexpr uint &operator<<=(auto b) requires requires { *this << b; } { return *this = *this << b; }
	forceinline constexpr uint &operator>>=(auto b) requires requires { *this >> b; } { return *this = *this >> b; }

	forceinline constexpr explicit operator bool() const { return *this != uint(); };
	forceinline constexpr explicit operator u8  () const { return words[0]; };
	forceinline constexpr explicit operator u16 () const { return words[0]; };
	forceinline constexpr explicit operator u32 () const { return words[0]; };
	forceinline constexpr explicit operator u64 () const { return words[0]; };
	forceinline constexpr explicit operator s8  () const { return words[0]; };
	forceinline constexpr explicit operator s16 () const { return words[0]; };
	forceinline constexpr explicit operator s32 () const { return words[0]; };
	forceinline constexpr explicit operator s64 () const { return words[0]; };
};

using u128 = uint<128>;
using u256 = uint<256>;

template <umm bit_count> inline constexpr bool is_integer<uint<bit_count>> = true;
template <umm bit_count> inline constexpr bool is_integer_like<uint<bit_count>> = true;
template <umm bit_count> inline constexpr bool is_signed<uint<bit_count>> = false;
template <umm bit_count> inline constexpr bool is_unsigned<uint<bit_count>> = true;

}
