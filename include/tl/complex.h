#pragma once
#include "common.h"

namespace TL {

template <class T>
struct Complex {
	T real;
	T imaginary;
	Complex operator+(Complex b) { return {real + b.real, imaginary + b.imaginary}; }
	Complex operator-(Complex b) { return {real - b.real, imaginary - b.imaginary}; }
	Complex operator*(Complex b) {
		return {
			real * b.real      - imaginary * b.imaginary,
			real * b.imaginary + imaginary * b.real,
		};
	}
	Complex &operator*=(Complex b) {
		T old_real = real;
		real = real * b.real - imaginary * b.imaginary;
		imaginary = old_real * b.imaginary + imaginary * b.real;
		return *this;
	}
};

template <class T>
T length(Complex<T> c) {
	return sqrt(c.real * c.real + c.imaginary * c.imaginary);
}

}
