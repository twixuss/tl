#pragma once
#include "big_int.h"

namespace tl {

template <class Allocator>
struct BigRational {
	using BigInt = BigInt<Allocator>;
	using List = typename BigInt::List;
	using Part = typename BigInt::Part;
	using SignedPart = typename BigInt::SignedPart;
	using Size = typename BigInt::Size;
	
	inline static constexpr umm bytes_in_part = BigInt::bytes_in_part;
	inline static constexpr umm bits_in_part = BigInt::bits_in_part;

	BigInt numer;
	BigInt denom;
	
	void set(SignedPart p) {
		numer.set(p);
		denom.set(1);
	}
	void set(Part p) {
		numer.set(p);
		denom.set(1);
	}
	void set(BigRational p) {
		numer.set(p.numer);
		denom.set(p.denom);
	}

	BigRational &operator+=(BigRational const &that) {
		auto that_numer = that.numer.copy();
		defer { that_numer.free(); };

		that_numer *= denom;
		numer *= that.denom;
		denom *= that.denom;

		numer += that_numer;

		normalize();

		return *this;
	}
	BigRational &operator-=(BigRational const &that) {
		auto that_numer = that.numer.copy();
		defer { that_numer.free(); };

		that_numer *= denom;
		numer *= that.denom;
		denom *= that.denom;

		numer -= that_numer;

		normalize();

		return *this;
	}
	BigRational &operator*=(BigRational const &that) {
		numer *= that.numer;
		denom *= that.denom;

		normalize();

		return *this;
	}
	BigRational &operator/=(BigRational const &that) {
		numer *= that.denom;
		denom *= that.numer;

		normalize();

		return *this;
	}
	BigRational &operator%=(BigRational const &that) {
		// this = this - floor(this / that) * that;

		auto c = copy();

		c /= that;
		c.floor();
		c *= that;

		*this -= c;

		c.free();

		normalize();
		return *this;
	}

	BigRational &operator+=(SignedPart that) {
		auto that_numer = denom.copy();
		defer { that_numer.free(); };

		that_numer *= that;

		numer += that_numer;

		normalize();

		return *this;
	}
	BigRational &operator-=(SignedPart that) {
		auto that_numer = denom.copy();
		defer { that_numer.free(); };

		that_numer *= that;

		numer -= that_numer;

		normalize();

		return *this;
	}
	BigRational &operator*=(SignedPart that) {
		numer *= that;

		normalize();

		return *this;
	}
	BigRational &operator/=(SignedPart that) {
		denom *= that;

		normalize();

		return *this;
	}
	
	explicit operator f64() const {
		return (f64)numer / (f64)denom;
	}

	void free() {
		numer.free();
		denom.free();
	}

	BigRational copy() const {
		BigRational result;
		result.numer = numer.copy();
		result.denom = denom.copy();
		return result;
	}

	void normalize() {
		if (numer == 0) {
			denom.parts.set(1);
			denom.msb = 0;
			return;
		}

		big_int_scratch_scoped(d);
		d.set(numer);

		gcd(d, denom);

		numer /= d;
		denom /= d;
	}

	void floor() {
		auto n = numer.copy();
		n %= denom;
		numer -= n;
		n.free();
	}
};

template <class Allocator = Allocator>
BigRational<Allocator> make_big_rational(s64 numerator, s64 denominator = 1) {
	BigRational<Allocator> result;
	result.numer = make_big_int<Allocator>(numerator);
	result.denom = make_big_int<Allocator>(denominator);
	return result;
}

template <class Allocator>
umm append(StringBuilder &builder, BigRational<Allocator> f) {
	if (f.denom.parts.count == 1) {
		auto denom = f.denom.parts.data[0];
		//umm dec = 10000000000000000000;
		//if (dec % denom == 0) {
		//
		//}
		u32 dc = 0;
		switch (denom) {
			case 1: return append(builder, f.numer);
			case 10: 				   dc = 1; break;
			case 100: 				   dc = 2; break;
			case 1000: 				   dc = 3; break;
			case 10000: 			   dc = 4; break;
			case 100000: 			   dc = 5; break;
			case 1000000: 			   dc = 6; break;
			case 10000000: 			   dc = 7; break;
			case 100000000: 		   dc = 8; break;
			case 1000000000: 		   dc = 9; break;
			case 10000000000: 		   dc = 10; break;
			case 100000000000: 		   dc = 11; break;
			case 1000000000000: 	   dc = 12; break;
			case 10000000000000: 	   dc = 13; break;
			case 100000000000000: 	   dc = 14; break;
			case 1000000000000000: 	   dc = 15; break;
			case 10000000000000000:    dc = 16; break;
			case 100000000000000000:   dc = 17; break;
			case 1000000000000000000:  dc = 18; break;
			case 10000000000000000000: dc = 19; break;
		}

		if (dc) {
			big_int_scratch_scoped(quotient);
			big_int_scratch_scoped(remainder);

			f.numer.divmod(denom, quotient, remainder);

			return append_format(builder, "{}.{}", quotient, FormatInt{.value = remainder, .leading_zero_count = dc});
		}
	}
	return append(builder, FormatFloat{.value = (f64)f, .precision = 99, .trailing_zeros = false});
}

}
